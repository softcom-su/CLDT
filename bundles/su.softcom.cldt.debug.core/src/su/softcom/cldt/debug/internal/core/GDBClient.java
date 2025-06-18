package su.softcom.cldt.debug.internal.core;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.math.BigInteger;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.Charset;
import java.nio.charset.IllegalCharsetNameException;
import java.nio.charset.UnsupportedCharsetException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.HexFormat;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Objects;
import java.util.Set;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IMarker;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.ILog;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.IBreakpoint;
import org.eclipse.debug.core.model.IMemoryBlock;
import su.softcom.cldt.debug.core.AuxVector;
import su.softcom.cldt.debug.core.AuxVector.AUXV_TYPE;
import su.softcom.cldt.debug.core.CommandLineBreakpoint;
import su.softcom.cldt.debug.core.ELF;
import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IDebugEndpoint;
import su.softcom.cldt.debug.core.IDebugEventBus;
import su.softcom.cldt.debug.core.IDebugVariable;
import su.softcom.cldt.debug.core.IDeclaration;
import su.softcom.cldt.debug.core.IEvent;
import su.softcom.cldt.debug.core.IEventListener;
import su.softcom.cldt.debug.core.IFunction;
import su.softcom.cldt.debug.core.IGDBReply;
import su.softcom.cldt.debug.core.IGdbCommand;
import su.softcom.cldt.debug.core.IProcMapsFile;
import su.softcom.cldt.debug.core.IProcMapsFile.IProcMapsEntry;
import su.softcom.cldt.debug.core.IStackFrameDescription;
import su.softcom.cldt.debug.core.IStackInfo;
import su.softcom.cldt.debug.core.ISymbol;
import su.softcom.cldt.debug.core.IType;
import su.softcom.cldt.debug.core.Utils;
import su.softcom.cldt.debug.core.debugger.events.ConsoleOutputEvent;
import su.softcom.cldt.debug.core.debugger.events.ErrorEvent;
import su.softcom.cldt.debug.core.debugger.events.LaunchedProcessEvent;
import su.softcom.cldt.debug.core.debugger.events.SuspendedEvent;
import su.softcom.cldt.debug.core.debugger.events.SuspendedEvent.StopReason;
import su.softcom.cldt.debug.core.info.IDebugInfo;
import su.softcom.cldt.debug.core.debugger.events.TerminatedEvent;
import su.softcom.cldt.debug.core.debugger.events.ThreadsInfoEvent;
import su.softcom.cldt.debug.core.debugger.events.VariablesEvent;
import su.softcom.cldt.debug.core.model.events.BreakpointEvent;
import su.softcom.cldt.debug.core.model.events.FetchVariablesEvent;
import su.softcom.cldt.debug.core.model.events.ResumeEvent;
import su.softcom.cldt.debug.core.model.events.ResumeEvent.ResumeState;
import su.softcom.cldt.debug.core.model.events.TerminateEvent;
import su.softcom.cldt.debug.internal.core.dwarf.BreakpointInfo;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.TableRow;
import su.softcom.cldt.debug.internal.core.dwarf.VariableDTO;
import su.softcom.cldt.debug.internal.core.dwarf.cfa.reg.IRegRule;
import su.softcom.cldt.debug.internal.core.dwarf.tags.BaseType;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Inheritance;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Member;
import su.softcom.cldt.debug.internal.core.dwarf.tags.PointerType;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Subprogram;
import su.softcom.cldt.debug.internal.core.gdb.commands.AuxvReadCommand;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Tag;
import su.softcom.cldt.debug.internal.core.dwarf.tags.Typedef;
import su.softcom.cldt.debug.internal.core.gdb.commands.BreakpointCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.BreakpointCommand.BreakpointAction;
import su.softcom.cldt.debug.internal.core.gdb.commands.BreakpointCommand.BreakpointType;
import su.softcom.cldt.debug.internal.core.gdb.commands.DylibReadCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.DylibReadCommand.LibraryXML;
import su.softcom.cldt.debug.internal.core.gdb.commands.FeaturesReadCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.FeaturesReadCommand.Target;
import su.softcom.cldt.debug.internal.core.gdb.commands.FeaturesReadCommand.Target.Reg;
import su.softcom.cldt.debug.internal.core.gdb.commands.GetProcessInfoCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.GetThreadsCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.KillCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.QListThreadsInStopReply;
import su.softcom.cldt.debug.internal.core.gdb.commands.QSupported;
import su.softcom.cldt.debug.internal.core.gdb.commands.VContCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.VContCommand.VContAction;
import su.softcom.cldt.debug.internal.core.gdb.commands.VFileCloseCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.VFileOpenCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.VFilePreadCommand;
import su.softcom.cldt.debug.internal.core.gdb.commands.VFilePreadCommand.FileDescriptor;
import su.softcom.cldt.debug.internal.core.gdb.commands.XCommand;
import su.softcom.cldt.debug.internal.core.gdb.replies.EReply;
import su.softcom.cldt.debug.internal.core.gdb.replies.TReply;
import su.softcom.cldt.debug.internal.core.gdb.replies.TReply.StopInfo;
import su.softcom.cldt.debug.internal.core.lldb.IDebugServerListener;
import su.softcom.cldt.debug.internal.core.model.Executable;
import su.softcom.cldt.debug.internal.core.model.Library;
import su.softcom.cldt.debug.internal.core.model.Module;

/**
 * @author maxim.drumov
 * @author rinat.nizamov
 * @author petr.babanov
 */
public class GDBClient implements IEventListener, IDebugServerListener {

	private static final int TIMEOUT_TIME = 1000; // milliseconds
	private final ILog clientLOG = Platform.getLog(this.getClass());
	private final IDebugEndpoint endpoint;
	private final IDebugEventBus eventBus;

	private boolean mainBreak;
	private Socket socket;
	private DataOutputStream outputStream;
	private DataInputStream inputStream;
	private long packetSize;
	private Set<Long> breakpointAddresses = new HashSet<>();
	private long dlopenAddress = -1;
	// private long exitAddress = -1;
	private List<IBreakpoint> unresolvedBPs = new ArrayList<>();
	/**
	 * //TODO: Перенести в модель (скорее всего уже) и убрать отсюда (дублирование
	 * данных)
	 */
	private long pid;
	/**
	 * //TODO: Перенести в "Таргет", это свойство целевой архитектуры
	 */
	private ByteOrder byteOrder;
	/**
	 * //TODO: Поднять в модель, это не свойство клиента
	 */
	private Map<Long, Long> threadIdPcMap = new HashMap<>();
	/**
	 * //TODO: Перенести в класс "Таргет", это свойство целевой архитектуры а не
	 * клиента.
	 */
	private Map<Integer, BigInteger> registers = new HashMap<>();
	/**
	 * Массив номеров регистров соответсвующих dwarfRegNum, при обращении по
	 * индексу, получаем обычный номер регистра Если по индексу лежит -1, значит нет
	 * маппинга номеров.
	 */
	private int[] dwarfRegNum;
	// Поддержать чтение всех мемори блоков и вынести в DebugTarget
	private IMemoryBlock stackMemoryBlock;

	/**
	 * Временное решение хранение переменных ключ --- айди стекфрейма значение ---
	 * переменные на данный момент времени
	 */
	private Map<Long, List<IDebugVariable>> variables = new HashMap<>();

	private LinkedList<Module> modules = new LinkedList<>();
	private Executable executable;
	private Charset charset = null;

	// TODO: Подумать куда убрать
	public enum Action {
		STEPPING, RESUME
	}

	/**
	 * Создает сущность клиента
	 * 
	 * @param endpoint
	 * @param eventBus  для разговора с таргетом
	 * @param mainBreak ставить ли breakpoint на main?
	 * @param launch    пока лишь для чарсета
	 */
	public GDBClient(IDebugEndpoint endpoint, IDebugEventBus eventBus, boolean mainBreak, ILaunch launch) {
		this.endpoint = endpoint;
		this.eventBus = eventBus;
		this.mainBreak = mainBreak;
		String encoding = launch.getAttribute(DebugPlugin.ATTR_CONSOLE_ENCODING);
		if (encoding != null) {
			try {
				charset = Charset.forName(encoding);
			} catch (UnsupportedCharsetException | IllegalCharsetNameException e) {
				DebugPlugin.log(e);
			}
		}
		if (charset == null) {
			charset = Platform.getSystemCharset();
		}
		clientLOG.info("Using charset: %s".formatted(charset.toString()));
		socket = new Socket();
		try {
			socket.setSoTimeout(TIMEOUT_TIME);
		} catch (SocketException e) {
			eventBus.addEvent(new TerminatedEvent(-1));
			e.printStackTrace();
		}
	}

	private void handleCommand(IGdbCommand command) throws IOException {
		byte[] commandRaw = generateByteRequest(command.getCommandString());
		String requestStr = new String(commandRaw);
		Character firstChar = '-';
		while (firstChar != '+') {
			// read until get +
			try {
				clientLOG.info("Client request sent to lldb-server: %s".formatted(requestStr));
				outputStream.write(commandRaw);
				outputStream.flush();
				firstChar = (char) inputStream.read();
			} catch (SocketTimeoutException e) {
				clientLOG.info("SOCKET stuck, SENDing AGAIN");
				clientLOG.info("Client request RESEND to lldb-server: %s".formatted(requestStr));
				outputStream.write('+');
				outputStream.flush();
				firstChar = (char) inputStream.read();
			}
			if (firstChar == '+') {
				clientLOG.info("lldb-server successfully received the command (sent +)");
				clientLOG.info("Waiting for server response...");
				byte[] singlePacket = getSinglePacketBytes();
				clientLOG.info("Got server response, start to read it...");
				String hexString = "";
				// Check whether O packet is accepted, if not do handleSimplePackets()
				if (command instanceof XCommand) {
					clientLOG.info("Got raw bytes, not printed");
				} else {
					hexString = new String(singlePacket);
					clientLOG.info("Got: %s ".formatted(hexString));
				}
				if (!hexString.isEmpty() && hexString.charAt(1) == 'O' && hexString.charAt(2) != 'K'
						&& command instanceof VContCommand) {
					handleOPackets(command, singlePacket);
				} else {
					handleSimplePackets(command, singlePacket);
				}
				break;
			}
		}
	}

	private byte[] getSinglePacketBytes() {
		ByteArrayOutputStream responseStream = new ByteArrayOutputStream();
		int readByte = 0;
		while (readByte != '#') {
			readByte = readInputStream();
			if (readByte < 0) {
				return responseStream.toByteArray();
			}
			responseStream.write(readByte);
		}
		// write checksum
		responseStream.write(readInputStream());
		responseStream.write(readInputStream());
		return responseStream.toByteArray();
	}

	private void handleSimplePackets(IGdbCommand command, byte[] result) throws IOException {
		if (validate(result)) {
			outputStream.write('+');
			outputStream.flush();
			byte[] packet = Arrays.copyOfRange(result, 1, result.length - 3);
			if (packet.length > 0) {
				command.handleResult(packet);
			}
		}
	}

	/**
	 * Обрабатывает O пакет.
	 * <li>Цикл работает до тех пор, пока последний ответ окажется НЕ О пакетом
	 * (может быть Т, F, E, W пакеты)
	 * 
	 * @param command
	 * @param initOPacket
	 * @throws IOException
	 */
	private void handleOPackets(IGdbCommand command, byte[] initOPacket) throws IOException {
		byte[] currentReply = initOPacket;
		byte[] currentPacket = Arrays.copyOfRange(currentReply, 1, currentReply.length - 3);
		if (currentPacket.length > 0) {
			command.handleResult(currentPacket);
			sendOPacketToTarget(currentPacket);
			outputStream.write('+');
			outputStream.flush();
		}
		long startTime = System.currentTimeMillis();
		long passedTime = 0;
		while ((currentReply.length > 0 && currentReply[1] == 'O')) {
			passedTime = System.currentTimeMillis() - startTime;
			int availableReply = inputStream.available();
			if (availableReply > 0) {
				startTime = System.currentTimeMillis();
				currentReply = getSinglePacketBytes();
				if (currentPacket.length > 0) {
					currentPacket = Arrays.copyOfRange(currentReply, 1, currentReply.length - 3);
					command.handleResult(currentPacket);
					if (currentReply[0] == '$' && currentReply[1] == 'O') {
						sendOPacketToTarget(currentPacket);
					}
					outputStream.write('+');
					outputStream.flush();
				}
			} else if (availableReply <= 0 && passedTime > 1000) {
				// проверяем, не упал ли inputStream:
				// при этом, если это просто большая пауза перед новым О пакетом, то макерами мы
				// не считаем лишнего
				inputStream.mark(4);
				readInputStream();
				if (inputStream.markSupported()) {
					inputStream.reset();
				} else {
					break;
				}
			}
		}
	}

	/**
	 * Если IOException упадет сервер, то отправляем TerminatedEvent
	 * 
	 * @return прочитанный байт
	 */
	private int readInputStream() {
		int result = 0;
		try {
			result = inputStream.read();
		} catch (IOException e) {
			// TODO уточнять тут у сервера, действительно ли он умерчик
			eventBus.addEvent(new TerminatedEvent(-1));
			e.printStackTrace();
			return -1;
		}
		return result;
	}

	/**
	 * Парсит О пакет и отправялет в DebugTarget
	 * {@link su.softcom.cldt.debug.core.debugger.events.ConsoleOutputEvent
	 * ConsoleOutputEvent}
	 * 
	 * 
	 * @param packet
	 */
	private void sendOPacketToTarget(byte[] packet) {
		String resultStr = new String(packet);
		String hexStr = resultStr.substring(1);
		String result = Utils.hexToCharset(hexStr, charset);
		eventBus.addEvent(new ConsoleOutputEvent(result, false));
	}

	/**
	 * Validates given input data by checking it's checksum and packet format
	 * 
	 * @param inputData byte array from server. Being decoded from prohibited chars.
	 * @return true, if checksum from server matches with calculated one.
	 * 
	 */
	private boolean validate(byte[] inputData) {
		if (inputData.length < 3) {
			return false;
		}
		// Check for "$" at start and "#" at the end of the packet
		if ((inputData[0] != 36) && (inputData[inputData.length - 3] != 35)) {
			return false;
		}
		byte[] packetData = Arrays.copyOfRange(inputData, 1, inputData.length - 3);
		byte[] resivedChecksum = Arrays.copyOfRange(inputData, inputData.length - 2, inputData.length);

		String checksumStrResult = new String(resivedChecksum);
		byte recievedChecksum = Integer.valueOf(checksumStrResult, 16).byteValue();
		byte calcChecksum = Utils.getChecksum(packetData);
		if (packetData.length > 1000) {
			String tmpStr = new String(packetData, 0, 1000);
			clientLOG.info("Client successfully received the packet\nPacket is too big to be fully printed"
					+ "%nPrinting starting part of it: $%s...#%s".formatted(tmpStr, checksumStrResult));
		} else {
			String hexStrResult = HexFormat.of().formatHex(packetData);
			clientLOG.info("Client successfully received the packet %n Resived hex bytes: $%s#%s"
					.formatted(hexStrResult, checksumStrResult));
			clientLOG.info(new String(packetData));
		}
		return recievedChecksum == calcChecksum;
	}

	/**
	 * Method to generate request to lldb-server from command string.
	 */
	private byte[] generateByteRequest(String commandStr) {
		byte checksum = Utils.getChecksum(commandStr.getBytes());
		return "$%s#%s".formatted(commandStr, HexFormat.of().toHexDigits(checksum)).getBytes();
	}

	/**
	 * Sends initial set of commands in order:
	 * <ol>
	 * <li>qProcessInfo - получает информацию о запущенном процессе. Узнает pid и
	 * endian и др.</li>
	 * <li>qSupported - запрашивает у GDB поддерживаемые им функции</li>
	 * <li>QListThreadsInStopReply - Enable the threads: and thread-pcs: data in the
	 * question-mark packet (“T packet”) responses when the stub reports that a
	 * program has stopped executing.</li>
	 * <li>vFile:open: /proc/%s/maps - открывает файл /proc/%s/maps. Сохраняет его
	 * fileDescriptor1</li>
	 * <li>vFile:pread: -- по полученному fileDescriptor1 читает содержимое
	 * файла</li>
	 * <li>ELFRequest -- Отправляет запрос на получение ELF файла отлаживаемой
	 * программы</li>
	 * <li>Disassemble -- считывает disassemble секции отлаживаемое программы</li>
	 * <li>InitCreationBreakpoint -- отправляет запрос на отправку брейкпоинтов</li>
	 * <li>BreakpointCommand -- устанавливаем breakpoint на main(), если передан
	 * соответствующий флаг
	 * {@link su.softcom.cldt.debug.internal.core.GDBClient#isMainBreakpoint()
	 * isMainBreak}
	 * <li>ContinueCommand -- запускает исполнение программы. После неё программа
	 * останавливается на breapkpoint на main() или начинает свое исполнение, если
	 * не передан соответствующий флаг</li>
	 * </ol>
	 * 
	 */
	private void initRequest() throws CoreException, IOException {
		AuxVector auxVector;
		socket.connect(new InetSocketAddress(endpoint.getHost(), endpoint.getPort()));
		outputStream = new DataOutputStream(socket.getOutputStream());
		inputStream = new DataInputStream(socket.getInputStream());

		// 0) getThreads
		GetThreadsCommand getThreadsCmd = new GetThreadsCommand();
		handleCommand(getThreadsCmd);
		eventBus.addEvent(new ThreadsInfoEvent(getThreadsCmd.getFirstActiveThreadIds(), new ArrayList<>()));
		// 1) QSupported -> packetSize
		QSupported qsupp = new QSupported();
		handleCommand(qsupp);
		packetSize = qsupp.getPacketSize();

		FeaturesReadCommand featReadCmd = new FeaturesReadCommand("target.xml", 0, packetSize);
		handleCommand(featReadCmd);
		Target target = featReadCmd.getTarget();
		eventBus.addEvent(new LaunchedProcessEvent(target));
		initDwarfRegNum(target);
		// 2) get PROCESS INFO
		GetProcessInfoCommand procInfo = new GetProcessInfoCommand();
		handleCommand(procInfo);
		setProcessInfo(procInfo.getProcInfo());
		// 2.5) auxV
		AuxvReadCommand auxvReadCmd = new AuxvReadCommand(0, packetSize, byteOrder);
		handleCommand(auxvReadCmd);
		auxVector = auxvReadCmd.getAuxVector();
		// 3) extend replies
		handleCommand(new QListThreadsInStopReply());
		// 4) read /proc/%s/maps
		String mapsPath = "/proc/%s/maps".formatted(pid);
		VFileOpenCommand vOpenMAPS = new VFileOpenCommand(mapsPath, 0, 0400);
		handleCommand(vOpenMAPS);
		VFilePreadCommand vPreadMAPS = new VFilePreadCommand(
				new FileDescriptor(vOpenMAPS.getFileDescriptor(), mapsPath), packetSize, 0);
		handleCommand(vPreadMAPS);
		stackMemoryBlock = vPreadMAPS.getStackMemoryBlock();

		// 5) get MAIN executable
		String debugProgramPath = endpoint.getDebugProgram().getPath().formatted(pid);
		File file = new File(debugProgramPath);
		ELF elfFile = ELFReader.readELFFile(file);
		long initAddress = elfFile.isElfTypeDYN() ? vPreadMAPS.getInitAdress() : 0;
		executable = new Executable(debugProgramPath, initAddress);
		executable.setElfFile(elfFile);
		addNewModule(executable);
		long entryAddress = auxVector.getAuxvEntry(AUXV_TYPE.AT_ENTRY);
		doVContBreakpoint(Action.STEPPING, VContAction.CONTINUE, -1, entryAddress);
		if (isMainBreakpoint()) {
			ISymbol mainSymbol = elfFile.getSymbol("main");
			long mainAddress = mainSymbol.getValue() + initAddress;
			handleBreakpointCommand(BreakpointAction.ADD, mainAddress);
		}
		// 7) Set breakpoints from BreakpointManager
		createInitialBreakpoints();
		// 8) send ContinueCommand
		postInitRequest(mapsPath, vOpenMAPS);
		vContAction(Action.RESUME, VContAction.CONTINUE, -1);
	}

	private void postInitRequest(String mapsPath, VFileOpenCommand vOpenMAPS) throws IOException {
		// 9)Initialize modules + read vPreadMAPS again
		DylibReadCommand dyLibReadCmd = new DylibReadCommand();
		handleCommand(dyLibReadCmd);
		for (LibraryXML lib : dyLibReadCmd.getDTO().getLibraries()) {
			String libName = lib.getName();
			long libAddr = HexFormat.fromHexDigitsToLong(lib.getAddr().substring(2));
			addNewModule(new Library(libName, libAddr));
		}
		// 10) Read ProcMaps again for getting libc.so.6 base address
		VFilePreadCommand vPreadMaps2 = new VFilePreadCommand(
				new FileDescriptor(vOpenMAPS.getFileDescriptor(), mapsPath), packetSize, 0);
		handleCommand(vPreadMaps2);
		// FIXME later
		IProcMapsFile mapsFile = vPreadMaps2.getProcMapsFile();
		String libToSearch = "libc.so.6";
		List<IProcMapsEntry> libcEntries = mapsFile.getProcMapsEntries("libc.so.6");
		long libcInitAddress = -1;
		if (!libcEntries.isEmpty()) {
			libcInitAddress = libcEntries.get(0).getAddressStart();
		} else {
			libcEntries = mapsFile.getProcMapsEntries("libdl-");
			if (!libcEntries.isEmpty()) {
				libcInitAddress = libcEntries.get(0).getAddressStart();
			}
			libToSearch = "libdl.so.2";
		}
		handleCommand(new VFileCloseCommand(vOpenMAPS.getFileDescriptor()));
		// 11) set dlopen BP
		if (libcInitAddress != -1) {
			for (Module module : modules) {
				if (module.getName().contains(libToSearch)) {
					Module libc = module;
					File libcFile = new File(libc.getName());
					ELF libcELF = ELFReader.readELFFile(libcFile);
					libc.setElfFile(libcELF);
					ISymbol dlopenSymbol = libcELF.getSymbol("dlopen");
					dlopenAddress = dlopenSymbol.getValue() + libcInitAddress;
					handleBreakpointCommand(BreakpointAction.ADD, dlopenAddress);
					break;
				}
			}
		}
	}

	/**
	 * 
	 * @param newModule
	 */
	private void addNewModule(Module newModule) {
		if (!modules.isEmpty()) {
			Module oldLastModule = modules.getLast();
			modules.add(newModule);
			if (oldLastModule.getBaseAddress() < newModule.getBaseAddress()) {
				Collections.sort(modules,
						(module1, module2) -> (int) (module1.getBaseAddress() - module2.getBaseAddress()));
			}
		} else {
			modules.add(newModule);
		}
	}

	@Override
	public void handleEvent(IEvent event) {
		if (event instanceof ResumeEvent resumeEvent) {
			if (resumeEvent.getState().equals(ResumeState.RUN)) {
				resume(resumeEvent);
			} else if (resumeEvent.getState().equals(ResumeState.STEP_RETURN)) {
				try {
					stepReturn(resumeEvent.getThreadId());
				} catch (IOException e) {
					e.printStackTrace();
				}
			} else {
				step(resumeEvent.getThreadId(), resumeEvent.getState());
			}

		} else if (event instanceof TerminateEvent) {
			terminate();
		} else if (event instanceof BreakpointEvent bpEvent) {
			try {
				sendBreakpoint(bpEvent);
			} catch (IOException | CoreException e) {
				e.printStackTrace();
			}
		} else if (event instanceof FetchVariablesEvent variablesEvent) {
			try {
				List<VariableDTO> localVariables;
				if (variablesEvent.getPathId() == null) {
					localVariables = getVariables(variablesEvent.getIp(), variablesEvent.getStackFrameId());
				} else {
					localVariables = getVariablesChildren(variablesEvent.getStackFrameId(), variablesEvent.getPathId());
				}
				if (!localVariables.isEmpty()) {
					eventBus.addEvent(new VariablesEvent(localVariables, variablesEvent.getPathId(),
							variablesEvent.getStackFrameId()));
				}
			} catch (IOException e) {
				clientLOG.error("Прочитать значение переменной из памяти не вышло: " + e.getMessage());
			}
		}
	}

	private void addBreakpoint(Long address) {
		breakpointAddresses.add(address);
	}

	private void removeBreakpoint(Long address) {
		breakpointAddresses.removeIf(bp -> Objects.equals(bp, address));
	}

	private void handleBreakpointAction(BreakpointAction action, Long address) {
		if (action.equals(BreakpointAction.ADD)) {
			addBreakpoint(address);
		} else if (action.equals(BreakpointAction.REMOVE)) {
			removeBreakpoint(address);
		}
	}

	/**
	 * Добавляет брейкпоинты, которые были установленны до начала работы программы в
	 * IDE.
	 * 
	 * Работает только с MainExecutable, т.к. изначально нет никаких других модулей.
	 * 
	 * @throws CoreException
	 */
	private void createInitialBreakpoints() throws CoreException {
		List<Long> resolvedAddresses = new ArrayList<>();
		List<Integer> resolvedLines = new ArrayList<>();
		// refactor it TODO
		for (IBreakpoint breakpoint : DebugPlugin.getDefault().getBreakpointManager().getBreakpoints()) {
			if (breakpoint instanceof CommandLineBreakpoint bp && breakpoint.isEnabled()
					&& bp.getResource() instanceof IFile file) {
				BreakpointInfo bpInfo = getMainExecutable().getElfFile().getDebugInfo().getBreakpointAddress(file,
						bp.getLineNumber());
				boolean contains = resolvedLines.contains(bpInfo.getCurrentLine());
				if (!contains) {
					resolvedLines.add(bpInfo.getCurrentLine());
				}
				if (bpInfo.getCurrentLine() != -1) {
					// правильно работает с дебаг инфой
					if (bpInfo.getCurrentLine() == bp.getLineNumber()) {
						if (bpInfo.getAddress() != -1 && !contains) {
							resolvedAddresses.add(bpInfo.getAddress());
						}
					} else { // тут если не совпала инфа с БП и с отладочной инфы
						if (!contains) {
							bp.getMarker().setAttribute(IMarker.LINE_NUMBER, bpInfo.getCurrentLine());
							bp.getMarker().setAttribute(IMarker.MESSAGE,
									"Line Breakpoint: " + file.getName() + " [line: " + bpInfo.getCurrentLine() + "]");
							resolvedAddresses.add(bpInfo.getAddress());
						}
					}
				} else {
					clientLOG.info("Unable to resolve breakpoint %s:%d to any actual locations. (pending)"
							.formatted(bp.getResource().toString(), bp.getLineNumber()));
					unresolvedBPs.add(breakpoint);
				}
			}
		}
		try {
			handleInitBreakpoints(resolvedAddresses);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void handleInitBreakpoints(List<Long> addresses) throws IOException {
		for (long address : addresses) {
			handleBreakpointCommand(BreakpointAction.ADD, address + getMainExecutable().getBaseAddress());
		}
	}

	private void sendBreakpoint(BreakpointEvent bpEvent) throws IOException, CoreException {
		// fix it,когда будут не только CommandLine БП
		CommandLineBreakpoint clBreakpoint = (CommandLineBreakpoint) bpEvent.getBreakpoint();
		BreakpointInfo bpInfo = resolveBreakpoint(clBreakpoint, bpEvent.getLineNumber());

		if (bpInfo != null) {
			handleBreakpointCommand(bpEvent.getAction(), bpInfo.getAddress() + bpInfo.getModulesBaseAddress());
		} else {
			if (bpEvent.getAction().equals(BreakpointAction.ADD)) {
				clientLOG.info("Unable to resolve breakpoint %s:%d to any actual locations. (pending)"
						.formatted(clBreakpoint.getResource().toString(), clBreakpoint.getLineNumber()));
				unresolvedBPs.add(bpEvent.getBreakpoint());
			} else {
				unresolvedBPs.removeIf(bp -> bp.equals(clBreakpoint));
			}
		}
	}

	private BreakpointInfo resolveBreakpoint(CommandLineBreakpoint bp, int lineNumber) {
		BreakpointInfo result = null;
		for (Module module : modules) {
			if (!module.getName().contains("libc.so.6") && module.getElfFile() != null
					&& module.getElfFile().getDebugInfo() != null) {
				IDebugInfo dwarfInfo = module.getElfFile().getDebugInfo();
				IFile eclFile = (IFile) bp.getResource();
				BreakpointInfo bpInfo = dwarfInfo.getBreakpointAddress(eclFile, lineNumber);
				if (bpInfo.getAddress() != -1 && bpInfo.getCurrentLine() != -1) {
					bpInfo.setModulesBaseAddress(module.getBaseAddress());
					return bpInfo;
				}
			}
		}
		return result;
	}

	private void handleBreakpointCommand(BreakpointAction action, long address) throws IOException {
		boolean contains = breakpointAddresses.contains(address);
		if (!contains && action.equals(BreakpointAction.ADD) || contains && action.equals(BreakpointAction.REMOVE)) {
			BreakpointCommand bp = new BreakpointCommand(action, BreakpointType.SOFTWARE, 1, address, "", "");
			handleCommand(bp);
			if (bp.getStatus()) {
				handleBreakpointAction(action, address);
			}
		}
	}

	private void resume(ResumeEvent event) {
		try {
			long threadId = event.getThreadId();
			if (threadIdPcMap.containsKey(threadId)) {
				if (!threadIdPcMap.isEmpty() && breakpointAddresses.contains(threadIdPcMap.get(threadId))) {
					doVContBreakpoint(Action.RESUME, VContAction.STEP, threadId, threadIdPcMap.get(threadId));
				}
				vContAction(Action.RESUME, VContAction.CONTINUE, threadId);
			} else {
				eventBus.addEvent(
						new ErrorEvent("Error: %s thread not suspended".formatted(Long.toHexString(threadId))));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void step(long threadId, ResumeState state) {
		try {
			long pc = threadIdPcMap.get(threadId);
			Module currentModule = getModule(pc);
			IDebugInfo currentDwarfInfo = currentModule.getElfFile().getDebugInfo();
			long currentBaseAddress = currentModule.getBaseAddress();
			long currentAddress = threadIdPcMap.get(threadId) - currentBaseAddress;
			ICompileUnit cu = currentDwarfInfo.getCompileUnit(currentAddress);
			IFunction function = null;
			if (cu != null) {
				function = cu.getFunction(currentAddress);
			}
			long currentStackPointer = registers.get(7).longValue();
			long previousStackPointer;
			do {
				long checkAddress = currentAddress + currentBaseAddress;
				if (!threadIdPcMap.isEmpty() && breakpointAddresses.contains(checkAddress)) {
					doVContBreakpoint(Action.RESUME, VContAction.STEP, threadId, checkAddress);
				} else {
					IGDBReply reply = vContAction(Action.STEPPING, VContAction.STEP, threadId);
					if (reply instanceof EReply) {
						return;
					}
				}

				currentModule = getModule(threadIdPcMap.get(threadId));
				currentBaseAddress = currentModule.getBaseAddress();
				currentAddress = threadIdPcMap.get(threadId) - currentBaseAddress;
				previousStackPointer = currentStackPointer;
				currentStackPointer = registers.get(7).longValue();
				currentDwarfInfo = currentModule.getElfFile().getDebugInfo();

				if (function != null && !function.contents(currentAddress)) {
					if (previousStackPointer > currentStackPointer && function != null || function == null) {
						if (state.equals(ResumeState.STEP_INTO)) {
							// TODO: Hot fix
							do {
								checkAddress = currentAddress + currentBaseAddress;
								if (!threadIdPcMap.isEmpty() && breakpointAddresses.contains(checkAddress)) {
									doVContBreakpoint(Action.RESUME, VContAction.STEP, threadId, checkAddress);
								} else {
									IGDBReply reply = vContAction(Action.STEPPING, VContAction.STEP, threadId);
									if (reply instanceof EReply) {
										return;
									}
								}
								currentModule = getModule(threadIdPcMap.get(threadId));
								currentDwarfInfo = currentModule.getElfFile().getDebugInfo();
								currentBaseAddress = currentModule.getBaseAddress();

								currentAddress = threadIdPcMap.get(threadId) - currentBaseAddress;
								cu = currentDwarfInfo.getCompileUnit(currentAddress);
							} while (cu == null);
						} else {
							cu = currentDwarfInfo.getCompileUnit(currentAddress);
						}

						function = cu != null ? cu.getFunction(currentAddress) : null;
						if ((cu == null || function == null) || state.equals(ResumeState.STEP_OVER)) {
							doStepReturn(threadId);
							currentAddress = threadIdPcMap.get(threadId) - currentBaseAddress;
							cu = currentDwarfInfo.getCompileUnit(currentAddress);
						}
					} else {
						break;
					}
				}
				if (cu != null && function == null
						|| function != null && !function.contents(currentAddress) && cu != null) {
					function = cu.getFunction(currentAddress);
				}
			} while (cu != null && !(cu.isStatementAddress(currentAddress) && function.contents(currentAddress)
					&& !function.isProlog(currentAddress)));

			sendSuspendedEvent(currentAddress + currentBaseAddress, registers, threadId, Action.STEPPING, true);

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Находит в стеке адрес возврата
	 * 
	 * @param threadId
	 * @throws IOException
	 */
	private long findReturnAddress(long threadId) throws IOException {
		long address = threadIdPcMap.get(threadId);
		ELF elfFile = getModule(address).getElfFile();
		long initAddress = getModule(address).getBaseAddress();
		IStackInfo stackInfo = elfFile.getStackInfo();
		Map<Integer, BigInteger> currentRegs = registers;
		byte[] stack = getStack(currentRegs.get(7).longValue());
		long a = threadIdPcMap.get(threadId) - initAddress;
		TableRow row = stackInfo.getRow(a);
		// nullable
		ICompileUnit cu = elfFile.getDebugInfo().getCompileUnit(a);
		IFunction function = null;
		if (cu != null) {
			function = cu.getFunction(a);
		}
		long cfa = row.getCFAAddress(this, currentRegs, function, cu);
		Map<Integer, IRegRule> regRuleChanges = row.getChangedRegs(currentRegs);
		currentRegs = readRegisterValue(regRuleChanges, currentRegs, cfa, stack);
		int returnAddressReg = stackInfo.getReturnAddressRegister(a);
		return currentRegs.get(returnAddressReg).longValue();
	}

	/**
	 * Находит в стеке адрес возврата
	 * 
	 * @param thread id
	 * @throws IOException
	 */
	private void stepReturn(long threadId) throws IOException {
		doStepReturn(threadId);
		long address = threadIdPcMap.get(threadId);
		ELF elf = getModule(address).getElfFile();
		IDebugInfo debugInfo = elf.getDebugInfo();
		long initAddress = getModule(address).getBaseAddress();
		ICompileUnit cu = debugInfo.getCompileUnit(address - initAddress);

		if (cu != null && cu.isStatementAddress(address - initAddress)) {
			sendSuspendedEvent(address, registers, threadId, Action.STEPPING, true);
		} else {
			step(threadId, ResumeState.STEP_OVER);
		}
	}

	private void doStepReturn(long threadId) throws IOException {
		long returnAddress = findReturnAddress(threadId);
		doVContBreakpoint(Action.STEPPING, VContAction.CONTINUE, threadId, returnAddress);
	}

	/**
	 * Делает vCont action с последующей обработкой Reply
	 * 
	 * @param action      описывает контекст исполнения метода
	 * @param vContAction либо STEP, либо CONTINUE.
	 * @param threadId    может быть -1, чтобы указать continue на все потоки
	 * @return
	 * @throws IOException
	 */
	private IGDBReply vContAction(Action action, VContAction vContAction, long threadId) throws IOException {
		VContCommand command = new VContCommand(vContAction, threadId, byteOrder);
		handleCommand(command);
		if (command.getReply() != null && command.getReply() instanceof EReply eReply) {
			eventBus.addEvent(new TerminatedEvent(eReply.getErrorCode()));
			return eReply;
		} else if (command.getReply() != null && command.getReply() instanceof TReply tReply) {
			if (action.equals(Action.RESUME) && vContAction.equals(VContAction.STEP)) {
				handleTReply(tReply, Action.STEPPING);
			} else {
				handleTReply(tReply, action);
			}
		}
		return null;

	}

	/**
	 * Выполнить vCont команду с командами брейкпоинта. <br>
	 * Если action == RESUME, то сначала убираем БП -> делаем vCont -> ставим тот же
	 * БП. <br>
	 * Если action == STEPPING, то сначала ставим БП -> делаем vCont -> убираем тот
	 * же БП.
	 * 
	 * @param action      определяет с какой целью будут ставиться брейкпоинты, для
	 *                    прохода брейкпоинта или чтобы дойти до брейкпоинта.
	 * @param vContAction действие команды vCont
	 * @param threadId
	 * @param address     целевой адрес для брейкпоинта
	 * @throws IOException
	 */
	private void doVContBreakpoint(Action action, VContAction vContAction, long threadId, long address)
			throws IOException {
		BreakpointAction[] actions = action.equals(Action.RESUME)
				? new BreakpointAction[] { BreakpointAction.REMOVE, BreakpointAction.ADD }
				: new BreakpointAction[] { BreakpointAction.ADD, BreakpointAction.REMOVE };
		BreakpointAction fstAction = actions[0];
		BreakpointAction scdAction = actions[1];

		handleCommand(new BreakpointCommand(fstAction, BreakpointType.SOFTWARE, 1, address, "", ""));
		vContAction(action, vContAction, threadId);
		handleCommand(new BreakpointCommand(scdAction, BreakpointType.SOFTWARE, 1, address, "", ""));
	}

	/**
	 * Метод переводит опкоды смещения, написанные в обратном порядке и пропускает
	 * опкоды мнемоники
	 * 
	 * @param arr
	 * @param lengthOpcode
	 * @return hex строку смещения
	 */
	public static String opcodesToHexString(int[] arr, int lengthOpcode) {
		StringBuilder sb = new StringBuilder();
		for (int i = arr.length - lengthOpcode; i >= lengthOpcode; i--) {
			String hexString = String.format("%02x", arr[i]);
			sb.append(hexString);
		}
		return sb.toString();
	}

	private void terminate() {
		try {
			KillCommand kill = new KillCommand();
			handleCommand(new KillCommand());
			if (kill.getExitCode() >= 0) {
				serverFinished(kill.getExitCode());
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void serverStarted(String host, long port) {
		clientLOG.info("GDBClinet started; Sending initial commands...");
		try {
			initRequest();
		} catch (CoreException | IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void serverFinished(int exitCode) {
		clientLOG.info("GDBClinet finished it's job; Sending terminate event");
		eventBus.addEvent(new TerminatedEvent(exitCode));
	}

	private void setProcessInfo(String infoStr) {
		for (String info : infoStr.split(";")) {
			String[] keyValueStr = info.split(":");

			// Add more cases and field, if needed
			if (keyValueStr[0].equals("pid")) {
				pid = (Long.parseLong(keyValueStr[1], 16));
			} else if (keyValueStr[0].equals("endian")) {
				if (keyValueStr[1].equals("little")) {
					byteOrder = (ByteOrder.LITTLE_ENDIAN);
				} else {
					byteOrder = (ByteOrder.BIG_ENDIAN);
				}
			}
		}
	}

	/**
	 * Возвращает значение поля mainBreak. <br>
	 * Если <b>true</b>, то при запуске отлаживаемой программы клиент установит
	 * breakpoint на main().
	 * 
	 * @return mainBreak
	 */
	private boolean isMainBreakpoint() {
		return mainBreak;
	}

	/**
	 * Handles T packet.
	 */
	private void handleTReply(TReply tReply, Action action) {
		threadIdPcMap.clear();
		List<Long> newThreadIdList = tReply.getAllThreadIds();
		registers = tReply.getRegisters();
		long stoppedId = tReply.getStopedThreadId();
		for (int i = 0; i < newThreadIdList.size(); i++) {
			long address = tReply.getThreadPcs().get(i);
			long newId = newThreadIdList.get(i);
			threadIdPcMap.put(newId, address);
		}

		if (!tReply.getStopInfos().isEmpty()) {
			List<Long> stoppedThreadIds = new ArrayList<>();
			for (StopInfo stopInfo : tReply.getStopInfos()) {
				// check for dlopen bp
				long stoppedThId = stopInfo.getTid();
				stoppedThreadIds.add(stoppedThId);
				if (threadIdPcMap.get(stoppedThId) == dlopenAddress) {
					perfomDylibCheck(stoppedThId);
					return;
				}
			}
			eventBus.addEvent(new ThreadsInfoEvent(newThreadIdList, stoppedThreadIds));
			if (action.equals(Action.RESUME)) {
				long stoppedAddress = threadIdPcMap.get(stoppedId);
				sendSuspendedEvent(stoppedAddress, tReply.getRegisters(), stoppedId, action, false);
			}
		} else if (tReply.getStopInfos().isEmpty()) {
			// check for dlopen bp
			if (threadIdPcMap.get(stoppedId) == dlopenAddress) {
				perfomDylibCheck(stoppedId);
				return;
			}
			List<Long> stoppedThreadIds = new ArrayList<>();
			stoppedThreadIds.add(stoppedId);
			eventBus.addEvent(new ThreadsInfoEvent(newThreadIdList, stoppedThreadIds));
			if (action.equals(Action.RESUME)) {
				long stoppedAddress = threadIdPcMap.get(stoppedId);
				sendSuspendedEvent(stoppedAddress, tReply.getRegisters(), stoppedId, action, false);
			}
		} else if (tReply.getReason().equals("signal")) {
			eventBus.addEvent(new ConsoleOutputEvent(tReply.getErrorDescriprion(), true));
		}

	}

	/**
	 * Вызывается, если остановились в методе dlopen() модуля libc.so.6
	 * 
	 * @param newId id остановившегося потока
	 */
	private void perfomDylibCheck(long newId) {
		try {
			handleCommand(
					new BreakpointCommand(BreakpointAction.REMOVE, BreakpointType.SOFTWARE, 1, dlopenAddress, "", ""));
			doStepReturn(newId);
			readDylibModules();
			handleCommand(
					new BreakpointCommand(BreakpointAction.ADD, BreakpointType.SOFTWARE, 1, dlopenAddress, "", ""));
			vContAction(Action.RESUME, VContAction.CONTINUE, newId);
		} catch (IOException | CoreException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Запрашиваем все модули у таргета. Если есть новые, то считываем и
	 * обрабатываем их ELF файл.
	 * 
	 * @throws CoreException
	 */
	private void readDylibModules() throws CoreException {
		try {
			DylibReadCommand command = new DylibReadCommand();
			handleCommand(command);
			for (LibraryXML lib : command.getDTO().getLibraries()) {
				String libName = lib.getName();
				long libAddr = HexFormat.fromHexDigitsToLong(lib.getAddr().substring(2));
				Library newLib = new Library(libName, libAddr);
				if (!modules.contains(newLib)) {
					// если мы тут, значит пришла динамическая либа
					long libDynAddr = HexFormat.fromHexDigitsToLong(lib.getLd().substring(2));
					newLib.setDynamicAddress(libDynAddr);
					File javaFile = new File(libName);
					ELF elfFile = ELFReader.readELFFile(javaFile);
					newLib.setElfFile(elfFile);
					IDebugInfo dwarfInfo = elfFile.getDebugInfo();
					for (IBreakpoint bp : unresolvedBPs) {
						CommandLineBreakpoint clBreakpoint = (CommandLineBreakpoint) bp;
						IFile eclipseResource = (IFile) clBreakpoint.getResource();
						BreakpointInfo bpInfo = dwarfInfo.getBreakpointAddress(eclipseResource,
								clBreakpoint.getLineNumber());
						if (bpInfo.getAddress() != -1) {
							clBreakpoint.setModule(newLib);
							handleBreakpointCommand(BreakpointAction.ADD, bpInfo.getAddress() + libAddr);
						}
					}
					addNewModule(newLib);
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void sendSuspendedEvent(long address, Map<Integer, BigInteger> registers, long stopedThreadId,
			Action action, boolean send) {
		Module module = getModule(address);
		ELF elfFile = module.getElfFile();
		IDebugInfo dwarfInfo = null;
		if (elfFile != null) {
			dwarfInfo = elfFile.getDebugInfo();
		}
		long initAddress = module.getBaseAddress();
		ICompileUnit cu = null;

		if (dwarfInfo != null) {
			cu = dwarfInfo.getCompileUnit(address - initAddress);
		}
		if (cu != null && cu.isStatementAddress(address - initAddress) || send) {
			clientLOG.info("Send SuspendedEvent. Compile unit at address: " + HexFormat.of().toHexDigits(address)
					+ " not null");
			StopReason reason = StopReason.UNKNOWN;

			if (action.equals(Action.RESUME)) {
				reason = StopReason.BREAKPOINT;
			} else if (action.equals(Action.STEPPING)) {
				reason = StopReason.STEP;
			}
			List<IStackFrameDescription> stackFrames = new ArrayList<>();
			try {
				stackFrames = unwindStack(address, registers);
			} catch (IOException e) {
				e.printStackTrace();
			}
			eventBus.addEvent(
					new SuspendedEvent(address - initAddress, stackFrames, registers, reason, stopedThreadId));
		}
	}

	// Вынести в Таргет
	/**
	 * Возвращает регистры
	 * 
	 * @return номера регистров с адресами
	 */
	public Map<Integer, BigInteger> getRegisters() {
		return registers;
	}

	private List<VariableDTO> getVariablesChildren(long stackFrameId, List<Long> pathId) throws IOException {
		List<VariableDTO> variablesDTO = new ArrayList<>();
		IDebugVariable rootVariable = findRootVariable(stackFrameId, pathId);

		if (rootVariable == null) {
			return variablesDTO;
		}

		TraversalState state = initializeTraversalState(rootVariable, pathId);
		traversePath(state, pathId, rootVariable);
		populateChildren(state, variablesDTO);

		return variablesDTO;
	}

	/**
	 * Находит корневую переменную по адресу из первого элемента пути.
	 */
	private IDebugVariable findRootVariable(long stackFrameId, List<Long> pathId) {
		List<IDebugVariable> stackFrameVariables = variables.get(stackFrameId);
		long rootAddress = pathId.get(0);

		for (IDebugVariable variable : stackFrameVariables) {
			if (variable.getAddress() == rootAddress) {
				return variable;
			}
		}
		return null;
	}

	/**
	 * Состояние обхода структуры переменной.
	 */
	private static class TraversalState {
		long currentAddress;
		long offset;
		IType currentType;
		long currentId;
		int pathIndex;

		TraversalState(long currentAddress, long offset, IType currentType, long currentId, int pathIndex) {
			this.currentAddress = currentAddress;
			this.offset = offset;
			this.currentType = currentType;
			this.currentId = currentId;
			this.pathIndex = pathIndex;
		}
	}

	/**
	 * Инициализирует начальное состояние обхода на основе корневой переменной.
	 */
	private TraversalState initializeTraversalState(IDebugVariable rootVariable, List<Long> pathId) throws IOException {
		long currentAddress = rootVariable.getAddress();
		long offset = rootVariable.getSize();
		IType currentType = rootVariable.getType();

		if (currentType instanceof PointerType pointer) {
			currentAddress = checkPointer(currentAddress, rootVariable, pointer);
			offset = 0;
		}

		int pathIndex = 1;
		long currentId = pathIndex >= pathId.size() ? -1 : pathId.get(pathIndex);
		return new TraversalState(currentAddress, offset, currentType, currentId, pathIndex);
	}

	/**
	 * Обходит путь по идентификаторам, обновляя состояние.
	 */
	private void traversePath(TraversalState state, List<Long> pathId, IDebugVariable rootVariable) throws IOException {
		while (state.currentId != -1 && state.pathIndex < pathId.size()) {
			List<Tag> typeChildren = state.currentType.getTypeChildren();
			if (typeChildren.isEmpty() || !processTypeChildren(typeChildren, pathId, state, rootVariable)) {
				break;
			}
		}
	}

	/**
	 * Обрабатывает дочерние теги текущего типа, обновляя состояние.
	 *
	 * @return true, если обработка завершена (найден нужный тег), иначе false
	 */
	private boolean processTypeChildren(List<Tag> typeChildren, List<Long> pathId, TraversalState state,
			IDebugVariable rootVariable) throws IOException {
		long localOffset = 0;

		for (Tag childTag : typeChildren) {
			if (childTag instanceof Member member) {
				if (processMember(member, state, pathId)) {
					return true;
				}
			} else if (childTag instanceof IType childType) {
				if (processChildType(childType, state, pathId, rootVariable, localOffset)) {
					return true;
				}
				localOffset += childType.getSize();
			} else if (childTag instanceof Inheritance inheritance) {
				if (processInheritance(inheritance, state, pathId)) {
					state.offset += inheritance.getMemberLocation();
					return true;
				}
				state.offset += inheritance.getMemberLocation();
			}
		}
		state.offset += localOffset;
		return false;
	}

	/**
	 * Обрабатывает член структуры {@link Member}
	 *
	 * @return true, если член соответствует текущему id
	 */
	private boolean processMember(Member member, TraversalState state, List<Long> pathId) {
		if (member.getId() != state.currentId) {
			return false;
		}
		long memberOffset = member.getMemberLocation();

		state.currentAddress += state.offset + memberOffset;
		state.offset += memberOffset;
		state.currentType = member;
		updatePathIndex(state, pathId);

		return true;
	}

	/**
	 * Обрабатывает дочерний тип {@link IType}
	 *
	 * @return true, если тип соответствует текущему id
	 */
	private boolean processChildType(IType childType, TraversalState state, List<Long> pathId,
			IDebugVariable rootVariable, long localOffset) throws IOException {
		if (childType.getId() != state.currentId) {
			return false;
		}

		if (childType instanceof PointerType pointer) {
			state.currentAddress = checkPointer(state.currentAddress, rootVariable, pointer);
			state.offset = 0;
		} else {
			state.offset += localOffset;
		}
		state.currentType = childType;
		updatePathIndex(state, pathId);

		return true;
	}

	/**
	 * Обрабатывает наследование {@link Inheritance}
	 *
	 * @return true, если найден подходящий член в унаследованном типе
	 */
	private boolean processInheritance(Inheritance inheritance, TraversalState state, List<Long> pathId) {
		Tag resolvedTag = resolveTypedef(inheritance.getType());

		for (Tag tag : ((IType) resolvedTag).getTypeChildren()) {
			if (tag instanceof Member member && member.getId() == state.currentId) {
				state.currentType = member.getType();
				updatePathIndex(state, pathId);
				return true;
			}
		}
		return false;
	}

	/**
	 * Обновляет текущий идентификатор и индекс пути.
	 */
	private void updatePathIndex(TraversalState state, List<Long> pathId) {
		state.pathIndex++;
		state.currentId = state.pathIndex >= pathId.size() ? -1 : pathId.get(state.pathIndex);
	}

	/**
	 * Обрабатывает потомков текущей переменной и добавляет о них информацию в виде
	 * {@link VariableDTO}
	 */
	private void populateChildren(TraversalState state, List<VariableDTO> variablesDTO) throws IOException {
		long childOffset = 0;
		for (Tag tag : state.currentType.getTypeChildren()) {
			if (tag instanceof Member member) {
				memberToVariableDTO(variablesDTO, member, state);
				childOffset = member.getMemberLocation() + state.currentType.getSize();
			} else if (tag instanceof IType type) {
				typeToVariableDTO(variablesDTO, type, state, childOffset);
				childOffset += type.getSize() * getIterationCount(state.currentType);
			} else if (tag instanceof Inheritance inheritance) {
				inheritedMemberToVariableDTO(variablesDTO, inheritance, state, childOffset);
			}
		}
	}

	/**
	 * Генерирует VariableDTO из члена {@link Member}
	 */
	private void memberToVariableDTO(List<VariableDTO> variablesDTO, Member member, TraversalState state)
			throws IOException {
		BigInteger varAddress = BigInteger.valueOf(state.currentAddress + state.offset + member.getMemberLocation());
		variablesDTO.add(generateVariableDTO(member, member.getName(), member.getId(), varAddress));
	}

	/**
	 * Генерирует VariableDTO из типа {@link IType}
	 */
	private void typeToVariableDTO(List<VariableDTO> variablesDTO, IType type, TraversalState state, long childOffset)
			throws IOException {
		int iterationCount = getIterationCount(state.currentType);
		for (int i = 0; i < iterationCount; i++) {
			BigInteger varAddress = BigInteger
					.valueOf(state.currentAddress + state.offset + childOffset + i * type.getSize());
			variablesDTO.add(generateVariableDTO(type, type.getName(), type.getId(), varAddress));
		}
	}

	/**
	 * Определяет количество итераций для указателей на базовые типы.
	 */
	private int getIterationCount(IType type) {
		if (type instanceof PointerType pointer && !pointer.getTypeChildren().isEmpty()
				&& pointer.getTypeChildren().get(0) instanceof BaseType) {
			return 10;
		}
		return 1;
	}

	/**
	 * Заполняет DTO для унаследованных членов.
	 */
	private void inheritedMemberToVariableDTO(List<VariableDTO> variablesDTO, Inheritance inheritance,
			TraversalState state, long childOffset) throws IOException {
		Tag resolvedTag = resolveTypedef(inheritance.getType());
		long localOffset = childOffset;

		for (Tag tag : ((IType) resolvedTag).getTypeChildren()) {
			if (tag instanceof Member member) {
				BigInteger varAddress = BigInteger.valueOf(state.currentAddress + state.offset + localOffset);
				variablesDTO.add(generateVariableDTO(member, member.getName(), member.getId(), varAddress));
				localOffset += member.getType().getSize();
			}
		}
	}

	/**
	 * Разрешает typedef до базового типа.
	 */
	private Tag resolveTypedef(IType type) {
		Tag resolvedTag = (Tag) type;
		while (resolvedTag instanceof Typedef typedef) {
			resolvedTag = (Tag) typedef.getType();
		}
		return resolvedTag;
	}

	/**
	 * Проверяет и обновляет адрес указателя, если требуется.
	 */
	private long checkPointer(long address, IDebugVariable root, PointerType pointer) throws IOException {
		if (pointer.getAddress() < 0) {
			XCommand command = new XCommand(address, ((Tag) root).getCompileUnit().getAddressSize());
			handleCommand(command);
			ByteBuffer buffer = ByteBuffer.wrap(command.getValue()).order(byteOrder);
			long newAddress = buffer.getLong();
			if (newAddress > 0) {
				pointer.setAddress(newAddress);
			}
			return newAddress;
		}
		return pointer.getAddress();
	}

	/**
	 * Генерирует DTO для переменной на основе её типа и адреса.
	 */
	private VariableDTO generateVariableDTO(IType type, String name, long id, BigInteger varAddress)
			throws IOException {
		long size = type.getSize();
		if (!type.hasChild()) {
			XCommand command = new XCommand(varAddress.longValue(), size);
			if (!(type instanceof IDeclaration) || !((IDeclaration) type).isDeclaration()) {
				handleCommand(command);
			}
			if (command.getValue() == null || command.getValue()[0] == 'E') {
				return new VariableDTO(id, name, varAddress, type.getModelVariableType(), false);
			}
			Object value = type.decodeValue(command.getValue());
			return new VariableDTO(id, name, varAddress, type.getModelVariableType(), false, value);
		}
		return new VariableDTO(id, name, varAddress, type.getModelVariableType(), true);
	}

	private List<VariableDTO> getVariables(long absAddress, long stackFrameId) throws IOException {
		List<VariableDTO> variablesDTO = new ArrayList<>();
		Module module = getModule(absAddress);
		IDebugInfo debugInfo = module.getElfFile().getDebugInfo();
		long address = absAddress - module.getBaseAddress();

		ICompileUnit cu = debugInfo.getCompileUnit(address);
		IFunction function = cu.getFunction(address);
		int currentLine = cu.getLine(address);
		int currentFileIndex = -1;
		if (function instanceof Subprogram subprogram) {
			currentFileIndex = subprogram.getFileIndex();
		}
		List<IDebugVariable> vars = cu.getVariables(function, currentLine);
		variables.put(stackFrameId, vars);
		for (IDebugVariable v : vars) {
			IType type = null;
			String name = "";
			BigInteger varAddress = null;
			// TODO: По-хорошему оставить условие v.getLine() <= currentLine
			// И решить проблему с пропуском пролога, так как образование а
			// переменной происходит зачастую через frameBaseReg, который может быть
			// rbp. А в случае если мы не пропустили пролог, то тогда значение регистра
			// не то что нужно нам.

			// Проверка на пролог отсюда уйдет в дальнейшем, когда решится вопрос с
			// обработкой точек останова в модулях внешних
			if (currentFileIndex == v.getFileIndex() && v.getLine() <= currentLine && !function.isProlog(address)) {
				type = v.getType();
				name = v.getName();
				varAddress = BigInteger.valueOf(v.getAddress(this, function, cu));
				variablesDTO.add(generateVariableDTO(type, name, varAddress.longValue(), varAddress));
			}
		}
		return variablesDTO;
	}

	private Map<Integer, BigInteger> readRegisterValue(Map<Integer, IRegRule> rules, Map<Integer, BigInteger> regs,
			long cfa, byte[] stack) throws IOException {
		Map<Integer, BigInteger> resultRegs = new HashMap<>();
		for (Entry<Integer, BigInteger> entry : regs.entrySet()) {
			IRegRule rule = rules.get(entry.getKey());
			long rspValue = regs.get(7).longValue();
			if (rule != null) {
				// long regValue = cfa - regs.get(7).longValue() + rule.getOffset();
				long regValue;
				// читаем со стека 8 байт значение по адресу regValue
				byte[] array;
				if (stack.length != 0) {
					regValue = rule.calculateValue(cfa) - rspValue;
					array = Arrays.copyOfRange(stack, (int) regValue, 8 + (int) regValue);
				} else {
					regValue = rule.calculateValue(cfa);
					XCommand command = new XCommand(regValue, 8);
					handleCommand(command);
					array = command.getValue();
				}

				ByteBuffer buffer = ByteBuffer.wrap(array);
				buffer.order(byteOrder);
				resultRegs.put(entry.getKey(), BigInteger.valueOf(buffer.getLong()));
			} else {
				resultRegs.put(entry.getKey(), entry.getValue());
			}
		}
		return resultRegs;
	}

	private List<IStackFrameDescription> unwindStack(long address, Map<Integer, BigInteger> registers)
			throws IOException {
		Module module = getModule(address);
		ELF elfFile = module.getElfFile();
		IDebugInfo debugInfo = elfFile.getDebugInfo();
		long initAddress = module.getBaseAddress();
		IStackInfo stackInfo = elfFile.getStackInfo();
		List<IStackFrameDescription> result = new ArrayList<>();
		Map<Integer, BigInteger> currentRegs = registers;
		byte[] stack = getStack(currentRegs.get(7).longValue());
		long a = address - initAddress;
		while (a != -1) {
			TableRow row = stackInfo.getRow(a);
			ICompileUnit cu = debugInfo.getCompileUnit(a);
			IFunction function = null;
			Path path = null;
			int lineNumber = -1;
			if (cu != null) {
				path = cu.getFilePath(a);
				lineNumber = cu.getLine(a);
				function = cu.getFunction(a);
			}
			long cfa = row.getCFAAddress(this, currentRegs, function, cu);
			result.add(new StackFrameDescriptor(currentRegs, cfa, a + initAddress, path, lineNumber));
			Map<Integer, IRegRule> regRuleChanges = row.getChangedRegs(currentRegs);
			currentRegs = readRegisterValue(regRuleChanges, currentRegs, cfa, stack);
			long rip = currentRegs.get(16).longValue();
			a = getCallPlace(module, rip);
		}
		return result;

	}

	/**
	 * 
	 * @param module
	 * @param returnAddress
	 * @return long value
	 */
	public long getCallPlace(Module module, long returnAddress) {
		IDebugInfo debugInfo = module.getElfFile().getDebugInfo();
		long baseAddress = module.getBaseAddress();
		if (debugInfo == null) {
			throw new IllegalStateException("Expected not null dwarfInfo");
		}
		long ip = returnAddress - baseAddress;
		ICompileUnit cu = debugInfo.getCompileUnit(ip);
		if (cu == null) {
			return -1;
		}
		ip -= 1;
		while ((cu.getLine(ip) == -1)) {
			ip = cu.getPreviousAddress(ip);
			cu = debugInfo.getCompileUnit(ip);
			if (cu == null) {
				return -1;
			}
		}
		return ip;
	}

	// TODO: Вынести чтение стека
	private byte[] getStack(long sp) throws IOException {
		long a = stackMemoryBlock.getStartAddress() + stackMemoryBlock.getLength();
		BigInteger stackBase = BigInteger.valueOf(a);
		BigInteger spValue = BigInteger.valueOf(sp);
		// Если размер стека слишком большой, то проще вычитывать определенные части
		// памяти для быстроты работы
		int diff = stackBase.subtract(spValue).intValue();
		if (diff > packetSize * 10) {
			return new byte[0];
		}
		byte[] result = new byte[stackBase.subtract(spValue).intValue()];
		BigInteger tmp = BigInteger.valueOf(spValue.longValue());
		int index = 0;
		while (stackBase.subtract(tmp).longValue() > packetSize) {
			XCommand command = new XCommand(tmp.longValue(), packetSize);
			handleCommand(command);
			if (command.getValue() != null && command.getValue()[0] == 'E') {
				return result;
			}

			System.arraycopy(command.getValue(), 0, result, tmp.subtract(spValue).intValue(), (int) packetSize);
			tmp = tmp.add(BigInteger.valueOf(packetSize));
			index += packetSize;
		}
		XCommand command = new XCommand(tmp.longValue(), stackBase.subtract(tmp).intValue());
		handleCommand(command);
		System.arraycopy(command.getValue(), 0, result, index, stackBase.subtract(tmp).intValue());
		return result;
	}

	private void initDwarfRegNum(Target target) {
		List<Reg> regs = target.getFeature();
		dwarfRegNum = new int[regs.size() + 1];
		Arrays.fill(dwarfRegNum, -1);
		regs.stream().filter(reg -> reg.getDwarfRegNum() != null)
				.forEach(reg -> dwarfRegNum[reg.getDwarfRegNum()] = reg.getRegnum());
	}

	/**
	 * Сопоставляет dwarf номер регистра с обычным номером
	 * 
	 * @param dRegNum
	 * @return номер регистра
	 */
	public int getDwarfReg(int dRegNum) {
		return dwarfRegNum[dRegNum];
	}

	/**
	 * Вспомогательный метод для получения другого адреса по адресу в памяти
	 * 
	 * @param address
	 * @param size
	 * @return адрес из памяти
	 * @throws IOException
	 */
	public long deref(long address, int size) throws IOException {
		XCommand command = new XCommand(address, size);
		handleCommand(command);
		ByteBuffer buffer = ByteBuffer.wrap(command.getValue());
		buffer.order(byteOrder);
		return buffer.getLong();
	}

	private Executable getMainExecutable() {
		return executable;
	}

	/**
	 * 
	 * @param address
	 * @return
	 */
	private Module getModule(long address) {
		Module lastModule = modules.getLast();
		for (Module module : modules) {
			if (module.getBaseAddress() > address) {
				return lastModule;
			}
			lastModule = module;
		}
		return modules.getLast();
	}
}
