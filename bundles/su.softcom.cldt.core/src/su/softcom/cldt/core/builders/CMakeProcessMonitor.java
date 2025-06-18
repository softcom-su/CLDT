package su.softcom.cldt.core.builders;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;

import org.eclipse.core.runtime.Assert;

import su.softcom.cldt.core.IProcessListener;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.internal.core.cmake.CMakeProjectDocument;
import su.softcom.cldt.internal.core.cmake.CMakeProjectDocument.DocumentLine;

/**
 * Монитор для связи UI и core. Управляет слушателями и содержимым out и error
 * потока процесса сборки CMake.
 */
public class CMakeProcessMonitor {

	private static CMakeProcessMonitor instance = new CMakeProcessMonitor();

	/**
	 * Типы текстовых строк процесса сборки CMake
	 */
	public enum OUTPUT_TYPE {
		/**
		 * for out text of process
		 */
		OUT,
		/**
		 * for system text
		 */
		SYSTEM,
		/**
		 * for error text of process
		 */
		ERROR;
	}

	private class ListenerState {

		private final IProjectOutputListener listener;
		private int readIndex = 0;

		public ListenerState(IProjectOutputListener listener) {
			this.listener = listener;
		}

		public int getReadIndex() {
			return readIndex;
		}

		public IProjectOutputListener getListener() {
			return listener;
		}

		public void incrementReadIndex() {
			readIndex++;
		}
	}

	private Map<ICMakeProject, List<ListenerState>> projectListenersMap = new HashMap<>();
	private ConcurrentLinkedQueue<IProcessListener> processListeners = new ConcurrentLinkedQueue<>();
	private List<CMakeProjectDocument> documents = new ArrayList<>();

	/**
	 * @return instance
	 */
	public static synchronized CMakeProcessMonitor getInstance() {
		if (instance == null) {
			instance = new CMakeProcessMonitor();
		}
		return instance;
	}

	/**
	 * Добавляет слушателя вывода к соответствующему ему проекту.
	 * 
	 * @param listener слушатель вывода. Не может быть {@code null}
	 * @param prj      соответствующий проект. Не может быть {@code null}
	 * @return лист типизированных строк, которые уже были записаны в документ для
	 *         этого проекта в момент добавления слушателя. Может быть пустым
	 *         списком.
	 */
	public List<DocumentLine> addListener(IProjectOutputListener listener, ICMakeProject prj) {
		Assert.isNotNull(listener);
		Assert.isNotNull(prj);
		ListenerState newLstnrState = new ListenerState(listener);
		CMakeProjectDocument document = getDocumentByProject(prj);
		List<ListenerState> tmpListenerList = new ArrayList<>();
		if (projectListenersMap.containsKey(prj)) {
			tmpListenerList = projectListenersMap.get(prj);
			tmpListenerList.add(newLstnrState);
		} else {
			// если по этому проекту еще НЕ был создан Entry, то создаем
			tmpListenerList.add(newLstnrState);
			projectListenersMap.put(prj, tmpListenerList);
		}
		return document.readLines(newLstnrState.getReadIndex());
	}

	/**
	 * Удаляет слушаетеля вывода.
	 * 
	 * @param listener слушатель вывода. Не может быть {@code null}.
	 * @param prj      соответствующий проект. Не может быть {@code null}.
	 */
	public void removeListener(IProjectOutputListener listener, ICMakeProject prj) {
		Assert.isNotNull(listener);
		Assert.isNotNull(prj);
		if (projectListenersMap.containsKey(prj)) {
			List<ListenerState> lstnrStates = projectListenersMap.get(prj);
			for (Iterator<ListenerState> iterator = lstnrStates.iterator(); iterator.hasNext();) {
				ListenerState state = iterator.next();
				if (state.getListener().equals(listener)) {
					iterator.remove();
				}
			}
		}
	}

	/**
	 * Вернет существующий документ {@link CMakeProjectDocument} или создаст новый.
	 * 
	 * @param prj
	 * @return документ
	 */
	private CMakeProjectDocument getDocumentByProject(ICMakeProject prj) {
		for (CMakeProjectDocument doc : documents) {
			if (doc.getProject().equals(prj)) {
				return doc;
			}
		}
		CMakeProjectDocument newDocument = new CMakeProjectDocument(prj);
		documents.add(newDocument);
		return newDocument;
	}

	/**
	 * Добавляет строку определенного типа в соответсвующий этому проекту
	 * {@link CMakeProjectDocument} и рассылает полученную строку всем
	 * зарегистрированным слушателям
	 * 
	 * @param project проект, слушателям которого будет отправлена строка. Не может
	 *                быть {@code null}
	 * @param line    строка
	 * @param type    тип строки. See: {@link OUTPUT_TYPE}
	 */
	public void addLine(ICMakeProject project, final String line, OUTPUT_TYPE type) {
		Assert.isNotNull(project);
		CMakeProjectDocument document = getDocumentByProject(project);
		synchronized (document) {
			document.appendConsoleLine(type, line);
			if (!projectListenersMap.containsKey(document.getProject())) {
				return;
			}
			for (ListenerState listenerState : projectListenersMap.get(document.getProject())) {
				IProjectOutputListener listener = listenerState.getListener();
				switch (type) {
				case OUT:
					listener.outLineReceived(line);
					break;
				case SYSTEM:
					listener.systemLineReceived(line);
					break;
				case ERROR:
					listener.errorLineReceived(line);
					break;
				default:
					listener.outLineReceived(line);
				}
				listenerState.incrementReadIndex();
			}
		}
	}

	/**
	 * add process Listener
	 * 
	 * @param listener слушатель процесса CMake. Не может быть {@code null}
	 */
	public void addProcesListener(IProcessListener listener) {
		Assert.isNotNull(listener);
		if (!processListeners.contains(listener)) {
			processListeners.add(listener);
		}
	}

	/**
	 * Сообщить {@link IProcessListener} о том, что процесс сборки CMake запущен
	 * Очищает документ (TODO: делать это, если выставлен соответсвувующий флаг в
	 * настройках).
	 * 
	 * @param project проект, на который была запущена сборка. Не может быть
	 *                {@code null}.
	 */
	public void notifyProcessListeners(ICMakeProject project) {
		Assert.isNotNull(project);
		boolean clearDocument = true; // TODO read it from prefs
		if (clearDocument) {
			getDocumentByProject(project).clear();
		}
		for (IProcessListener listener : processListeners) {
			listener.processStarted(project);
		}
	}

}
