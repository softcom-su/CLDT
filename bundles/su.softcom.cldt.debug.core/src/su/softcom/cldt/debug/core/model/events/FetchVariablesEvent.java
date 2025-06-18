package su.softcom.cldt.debug.core.model.events;

import java.util.List;

import su.softcom.cldt.debug.core.IModelEvent;

public class FetchVariablesEvent implements IModelEvent {

	private final long stackFrameId;
	private final List<Long> pathId;
	private long ip;

	/**
	 * Конструктор для event'a, чтобы получить переменные из отладчика по адресу.
	 * pathId в этом случае null
	 * 
	 * @param stackFrameId идентификатор стекового кадра в котором лежат переменные
	 * @param pathId       список идентификаторов, представляющий собой путь по
	 *                     потомкам переменной, чтобы восстановить
	 *                     последовательность
	 * @param ip           адрес на котором находится стекфрейм
	 */
	public FetchVariablesEvent(long stackFrameId, List<Long> pathId, long ip) {
		this.stackFrameId = stackFrameId;
		this.pathId = pathId;
		this.ip = ip;
	}

	/**
	 * Конструктор для event'a, чтобы получить переменные из отладчика по пути айди
	 * переменных
	 * 
	 * @param stackFrameId идентификатор стекового кадра в котором лежат переменные
	 * @param pathId       список идентификаторов, представляющий собой путь по
	 *                     потомкам переменной, чтобы восстановить
	 *                     последовательность
	 */
	public FetchVariablesEvent(long stackFrameId, List<Long> pathId) {
		this.stackFrameId = stackFrameId;
		this.pathId = pathId;
	}

	/**
	 * @return идентификатор стекового кадра в котором лежат переменные
	 */
	public long getStackFrameId() {
		return stackFrameId;
	}

	/**
	 * @return список идентификаторов, представляющий собой путь по потомкам
	 *         переменной, чтобы восстановить последовательность
	 */
	public List<Long> getPathId() {
		return pathId;
	}

	/**
	 * Адрес на котором остановилось исполнение стекфрейма
	 * 
	 * @return instruction pointer
	 */
	public long getIp() {
		return ip;
	}

}
