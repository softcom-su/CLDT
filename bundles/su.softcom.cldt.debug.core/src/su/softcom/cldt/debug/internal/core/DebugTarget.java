package su.softcom.cldt.debug.internal.core;

import java.io.IOException;
import java.math.BigInteger;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import org.eclipse.core.resources.IMarkerDelta;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.IBreakpointManager;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.IBreakpoint;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IMemoryBlock;
import org.eclipse.debug.core.model.IProcess;
import org.eclipse.debug.core.model.IRegister;
import org.eclipse.debug.core.model.IRegisterGroup;
import org.eclipse.debug.core.model.IStep;
import org.eclipse.debug.core.model.IThread;

import su.softcom.cldt.debug.core.IDebugEventBus;
import su.softcom.cldt.debug.core.IEvent;
import su.softcom.cldt.debug.core.IEventListener;
import su.softcom.cldt.debug.core.IStackFrameDescription;
import su.softcom.cldt.debug.core.debugger.events.ConsoleOutputEvent;
import su.softcom.cldt.debug.core.debugger.events.LaunchedProcessEvent;
import su.softcom.cldt.debug.core.debugger.events.SuspendedEvent;
import su.softcom.cldt.debug.core.debugger.events.SuspendedEvent.StopReason;
import su.softcom.cldt.debug.core.debugger.events.TerminatedEvent;
import su.softcom.cldt.debug.core.debugger.events.ThreadsInfoEvent;
import su.softcom.cldt.debug.core.debugger.events.VariablesEvent;
import su.softcom.cldt.debug.core.model.events.TerminateEvent;
import su.softcom.cldt.debug.internal.core.dwarf.RegisterDTO;
import su.softcom.cldt.debug.internal.core.dwarf.VariableDTO;
import su.softcom.cldt.debug.internal.core.gdb.DebugProcess;
import su.softcom.cldt.debug.internal.core.gdb.DebugStreamProxy;
import su.softcom.cldt.debug.internal.core.model.RegisterGroup;
import su.softcom.cldt.debug.internal.core.model.arch32bit.IntRegValue;
import su.softcom.cldt.debug.internal.core.model.arch32bit.IntRegister;
import su.softcom.cldt.debug.internal.core.model.arch64bit.GeneralizedRegValue;
import su.softcom.cldt.debug.internal.core.model.arch64bit.LongRegister;

/**
 * @see <a href=
 *      "https://lldb.llvm.org/resources/lldbgdbremote.html">lldbgdbremote.html</a>
 * @see <a href=
 *      "https://sourceware.org/gdb/current/onlinedocs/gdb.html/General-Query-Packets.html#General-Query-Packets">https://sourceware.org/gdb/current/onlinedocs/gdb.html/General-Query-Packets.html#General-Query-Packets</a>
 * @see <a href=
 *      "https://www.embecosm.com/appnotes/ean4/embecosm-howto-rsp-server-ean4-issue-2.pdf">https://www.embecosm.com/appnotes/ean4/embecosm-howto-rsp-server-ean4-issue-2.pdf</a>
 */
public class DebugTarget extends CLDTDebugElement implements IDebugTarget, IEventListener, IStep {

	static final Pattern replyPattern = Pattern.compile("\\$(.*?)\\#([0-9a-fA-F]{2})");

	final ILaunch launch;
	final IDebugEventBus eventBus;

	private final IBreakpointManager breakpointManager = DebugPlugin.getDefault().getBreakpointManager();

	private List<IRegisterGroup> fRegisterGroups = new ArrayList<>();
	private List<DebugThread> debugThreads = new ArrayList<>();
	private Map<Integer, IRegister> registers = new HashMap<>();
	// TODO: Временное решение сохранения всех стековых кадров из всех потоков
	private Map<Long, StackFrame> stackFrames = new HashMap<>();

	private State targetState = State.NOT_STARTED;
	private DebugProcess process;

	public DebugTarget(ILaunch launch, IDebugEventBus eventBus) throws UnknownHostException {
		super(null);
		this.eventBus = eventBus;
		this.launch = launch;
		breakpointManager.addBreakpointListener(this);
		launch.addDebugTarget(this);
		process = new DebugProcess(this);
		// process.
		launch.addProcess(process);
		DebugPlugin.getDefault().fireDebugEventSet(new DebugEvent[] { new DebugEvent(this, DebugEvent.CREATE) });
	}

	@Override
	public void handleEvent(IEvent event) {
		if (event instanceof ThreadsInfoEvent threadEvent) {
			updateThreads(threadEvent);
		} else if (event instanceof LaunchedProcessEvent lpEvent) {
			if (lpEvent.getRegisters() != null) {
				initfRegisterGroups(lpEvent.getRegisters());
			}
		} else if (event instanceof TerminatedEvent termEvent) {
			handleTermEvent(termEvent);
		} else if (event instanceof SuspendedEvent suspEvent) {
			Platform.getLog(getClass()).info("Received suspended event ");
			handleSuspendedEvent(suspEvent);
		} else if (event instanceof ConsoleOutputEvent outEvent) {
			try {
				handleOutputEvent(outEvent);
			} catch (IOException e) {
				e.printStackTrace();
			}
		} else if (event instanceof VariablesEvent variablesEvent) {
			updateVariables(variablesEvent.getPathId(), variablesEvent.getVariables(),
					variablesEvent.getStackFrameId());

		}
	}

	private void handleTermEvent(TerminatedEvent termEvent) {
		try {
			process.terminate();
			for (DebugThread thread : debugThreads) {
				thread.terminate();
			}
		} catch (DebugException e) {
			e.printStackTrace();
		}
		targetState = State.TERMINATED;
		eventBus.stop();
		fireTerminateEvent();
	}

	private void handleOutputEvent(ConsoleOutputEvent outEvent) throws IOException {
		if (outEvent.isErrorStream()) {
			DebugStreamProxy debugProxy = (DebugStreamProxy) process.getStreamsProxy();
			debugProxy.writeToErrorStream(outEvent.getContent());
		} else {
			process.getStreamsProxy().write(outEvent.getContent());
		}

	}

	private void handleSuspendedEvent(StopReason stopReason) {
		switch (stopReason) {
		case BREAKPOINT -> this.fireSuspendEvent(DebugEvent.BREAKPOINT);
		case STEP -> this.fireSuspendEvent(DebugEvent.STEP_INTO);
		case UNKNOWN -> this.fireSuspendEvent(DebugEvent.UNSPECIFIED);
		case EXCEPTION -> this.fireSuspendEvent(DebugEvent.UNSPECIFIED);// FIXME
		default -> Platform.getLog(getClass()).info("This type of stop reason is not yet supported");
		}
	}

	private void handleSuspendedEvent(SuspendedEvent event) {
		handleSuspendedEvent(event.getStopReason());
		try {
			DebugThread thread = (DebugThread) Arrays.stream(getThreads())
					.filter(t -> ((DebugThread) t).getId() == event.getThreadID()).findFirst().orElse(null);
			if (thread != null) {
				updateRegisters(event.getRegs());
				updateStackFrames(event, thread);

				targetState = State.SUSPENDED;

				// inform eclipse of suspended state
				thread.fireSuspendEvent(DebugEvent.CLIENT_REQUEST);
			} else {
				Platform.getLog(getClass())
						.info("Current thread is null. Created: " + getThreads().length + " threads");
			}
		} catch (DebugException e) {
			e.printStackTrace();
		}
	}

	private void updateStackFrames(SuspendedEvent event, DebugThread thread) {
		List<StackFrame> frames = new ArrayList<>();
		for (IStackFrameDescription stackFrameDesc : event.getStackFrames()) {
			StackFrame stackFrame = null;
			long key = stackFrameDesc.getCFA();
			if (stackFrames.containsKey(key) && key > 0) {
				stackFrame = stackFrames.get(key);
				stackFrame.setRegisters(stackFrameDesc.getRegisters());
				stackFrame.setLineNumber(stackFrameDesc.getLineNumber() == -1 ? stackFrame.getLineNumber()
						: stackFrameDesc.getLineNumber());
				stackFrame.setCurrentAddress(stackFrameDesc.getIP());
				if (stackFrameDesc.getPath() != null && !stackFrameDesc.getPath().toString()
						.contains(stackFrame.getFile().getFullPath().toFile().getAbsoluteFile().getAbsolutePath())) {
					stackFrame.setFile(stackFrameDesc.getPath());
				}
			} else {
				stackFrame = StackFrame.fromDescription(stackFrameDesc, this, thread);
				stackFrames.put(key, stackFrame);
			}
			stackFrame.fireChangeEvent(DebugEvent.CONTENT);
			frames.add(stackFrame);
		}
		thread.setStackFrames(frames);
	}

	private void updateVariables(List<Long> pathId, List<VariableDTO> variables, long stackFrameId) {
		try {

			StackFrame currentStackFrame = stackFrames.get(stackFrameId);
			if (currentStackFrame != null) {
				currentStackFrame.setVariables(pathId, variables);
			}
		} catch (DebugException e) {
			e.printStackTrace();
		}
	}

	@Override
	public boolean canTerminate() {
		return !isTerminated();
	}

	@Override
	public boolean isTerminated() {
		return targetState == State.TERMINATED;
	}

	@Override
	public void terminate() throws DebugException {
		eventBus.addEvent(new TerminateEvent());
	}

	@Override
	public boolean canResume() {
		if (targetState == State.TERMINATED) {
			return false;
		}
		return !isResumed();
	}

	public boolean isResumed() {
		return targetState == State.RESUMED;
	}

	@Override
	public boolean canSuspend() {
		return false;
	}

	@Override
	public boolean isSuspended() {
		return targetState == State.SUSPENDED;
	}

	@Override
	public void resume() throws DebugException {
		for (DebugThread thread : debugThreads) {
			if (thread.isSuspended()) {
				thread.resume();
			}
		}
		targetState = State.RESUMED;
	}

	@Override
	public void suspend() throws DebugException {
	}

	@Override
	public void breakpointAdded(IBreakpoint breakpoint) {
		try {
			breakpoint.setRegistered(true);
			breakpoint.setEnabled(true);
			breakpointManager.addBreakpoint(breakpoint);
			breakpointManager.fireBreakpointChanged(breakpoint);
		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void breakpointRemoved(IBreakpoint breakpoint, IMarkerDelta delta) {
		try {
			breakpoint.setRegistered(false);
			breakpoint.setEnabled(false);
			breakpoint.delete();
			breakpointManager.removeBreakpoint(breakpoint, true);
			breakpointManager.fireBreakpointChanged(breakpoint);
		} catch (CoreException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void breakpointChanged(IBreakpoint breakpoint, IMarkerDelta delta) {

	}

	@Override
	public boolean canDisconnect() {
		return false;
	}

	@Override
	public void disconnect() throws DebugException {

	}

	@Override
	public boolean isDisconnected() {
		return false;
	}

	@Override
	public boolean supportsStorageRetrieval() {
		return false;
	}

	@Override
	public IMemoryBlock getMemoryBlock(long startAddress, long length) throws DebugException {
		MemoryBlock memoryBlock = new MemoryBlock(this, startAddress, length);
		return memoryBlock;
	}

	@Override
	public String getName() {
		return "%s [Debug configuration]".formatted(launch.getLaunchConfiguration().getName());
	}

	@Override
	public boolean supportsBreakpoint(final IBreakpoint breakpoint) {
		return true;
	}

	/**
	 * Method to get a string from the list of conditions or commands for
	 * breakpoints TODO: finish it
	 */
	public String parameterListToString(String inputString) {
		StringBuilder strBuilder = new StringBuilder("");
		for (String cond : inputString.split("\n")) {
			if (!cond.isEmpty()) {
				byte[] bytecode = cond.getBytes();
				int lenBytecode = bytecode.length;
				String parameter = String.format("X%d,%s", lenBytecode, bytecode.toString());
				strBuilder.append(parameter);
			}
		}
		return strBuilder.toString();
	}

	@Override
	public IDebugTarget getDebugTarget() {
		return this;
	}

	@Override
	public ILaunch getLaunch() {
		return launch;
	}

	@Override
	public <T> T getAdapter(Class<T> adapter) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public IProcess getProcess() {
		return launch.getProcesses()[0];
	}

	@Override
	public IThread[] getThreads() throws DebugException {
		return debugThreads.toArray(new IThread[debugThreads.size()]);
	}

	@Override
	public boolean hasThreads() throws DebugException {
		return !debugThreads.isEmpty();
	}

	private void addThread(DebugThread thread) {
		debugThreads.add(thread);
		thread.fireCreationEvent();
	}

	private void addThreads(List<DebugThread> threads) {
		if (!threads.isEmpty()) {
			for (DebugThread thread : threads) {
				addThread(thread);
			}
			this.fireChangeEvent(DebugEvent.CONTENT);
		}
	}

	private void deleteThread(DebugThread thread) {
		try {
			thread.fireTerminateEvent();
			debugThreads.removeIf((DebugThread th) -> th.equals(thread));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public IBreakpointManager getBreakPointManager() {
		return breakpointManager;
	}

	public void addEvent(IEvent event) {
		eventBus.addEvent(event);
	}

	/**
	 * Update threads:
	 * <ol>
	 * <li>Обновить потоки: удалить те, которые отработали; Добавить новые.</li>
	 * <li>Установить состояние остановившегося потока на SUSPENDED</li>
	 * </ol>
	 * 
	 * @author rinat.nizamov@softcom.su
	 */
	private void updateThreads(ThreadsInfoEvent event) {

		List<Long> newThreadIdList = event.getThreadIds();
		List<Long> stoppedThreadIdList = event.getStoppedThreads();
		List<DebugThread> oldThreads = new ArrayList<>();

		for (DebugThread oldThread : debugThreads) {
			if (!newThreadIdList.contains(oldThread.getId())) {
				oldThreads.add(oldThread);
			} else {
				if (stoppedThreadIdList.contains(oldThread.getId())) {
					oldThread.setState(State.SUSPENDED);
					stoppedThreadIdList.remove(oldThread.getId());
				}
				newThreadIdList.remove(oldThread.getId());
			}
		}

		for (DebugThread thread : oldThreads) {
			deleteThread(thread);
		}

		for (long threadId : stoppedThreadIdList) {
			DebugThread thread = new DebugThread(this, threadId);
			thread.setState(State.SUSPENDED);
			addThread(thread);
			newThreadIdList.removeIf(thId -> thId == threadId);
		}

		addThreads(newThreadIdList.stream().map(newId -> {
			DebugThread newThread = new DebugThread(this, newId);
			newThread.fireCreationEvent();
			return newThread;
		}).collect(Collectors.toList()));

	}

	@Override
	public boolean canStepInto() {
		return isSuspended();
	}

	@Override
	public boolean canStepOver() {
		return isSuspended();
	}

	@Override
	public boolean canStepReturn() {
		return isSuspended();
	}

	@Override
	public boolean isStepping() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void stepInto() throws DebugException {
		for (DebugThread thread : debugThreads) {
			if (thread.isSuspended()) {
				thread.stepInto();
			}
		}
		targetState = State.RESUMED;
	}

	@Override
	public void stepOver() throws DebugException {
		for (DebugThread thread : debugThreads) {
			if (thread.isSuspended()) {
				thread.stepOver();
			}
		}
		targetState = State.RESUMED;
	}

	@Override
	public void stepReturn() throws DebugException {
		for (DebugThread thread : debugThreads) {
			if (thread.isSuspended()) {
				thread.stepReturn();
			}
		}
		targetState = State.RESUMED;
	}

	public List<IRegisterGroup> getfRegisterGroups() {
		return fRegisterGroups;
	}

	public void updateRegisters(Map<Integer, BigInteger> regs) throws NumberFormatException, DebugException {
		for (Entry<Integer, BigInteger> entry : regs.entrySet()) {
			IRegister reg = registers.get(entry.getKey());
			if (reg instanceof IntRegister) {
				reg.setValue(new IntRegValue(this, entry.getValue().intValue()));
			} else if (reg instanceof LongRegister) {
				reg.setValue(new GeneralizedRegValue(this, entry.getValue().longValue()));
			}
		}
	}

	public void initfRegisterGroups(Map<String, List<RegisterDTO>> registers) {
		Map<Integer, IRegister> tmpList = new HashMap<>();
		for (Entry<String, List<RegisterDTO>> entry : registers.entrySet()) {
			List<IRegister> regs = new ArrayList<>();
			for (RegisterDTO register : entry.getValue()) {
				IRegister reg;
				if (register.getBitsize() == 32) {
					reg = new IntRegister(this, register.getName(), 0);
				} else {
					reg = new LongRegister(this, register.getName(), 0);
				}
				regs.add(reg);
				tmpList.put(register.getRegNum(), reg);
			}
			fRegisterGroups.add(new RegisterGroup(this, entry.getKey(), regs));
		}
		this.registers = tmpList;
	}

}
