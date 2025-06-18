package su.softcom.cldt.debug.internal.core.dwarf;

import java.math.BigInteger;

import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

/**
 * DTO для переменной
 */
public class VariableDTO {

	private long id;
	private String name;
	private BigInteger address = BigInteger.ZERO;
	private Object value;
	private boolean hasChild;
	private ModelVariableType type;

	/**
	 * Конструктор для создания объекта переменной с именем, адресом и типом.
	 * 
	 * @param id       идентификатор переменной
	 * @param name     имя переменной
	 * @param address  адрес переменной в памяти
	 * @param type     тип переменной для модели
	 * @param hasChild есть ли потомки
	 * @param value    значение переменной
	 */
	public VariableDTO(long id, String name, BigInteger address, ModelVariableType type, boolean hasChild,
			Object value) {
		this.id = id;
		this.name = name;
		this.address = address;
		this.type = type;
		this.hasChild = hasChild;
		this.value = value;
	}

	/**
	 * Конструктор для создания объекта переменной с именем, адресом и типом.
	 *
	 * @param id       идентификатор переменной
	 * @param name     имя переменной
	 * @param address  адрес переменной в памяти
	 * @param type     тип переменной для модели
	 * @param hasChild есть ли потомки
	 */
	public VariableDTO(long id, String name, BigInteger address, ModelVariableType type, boolean hasChild) {
		this.id = id;
		this.name = name;
		this.address = address;
		this.type = type;
		this.hasChild = hasChild;
		this.value = null;
	}

	/**
	 * Возвращает адрес переменной в памяти.
	 *
	 * @return адрес переменной
	 */
	public BigInteger getAddress() {
		return address;
	}

	/**
	 * Возвращает имя переменной.
	 *
	 * @return имя переменной
	 */
	public String getName() {
		return name;
	}

	/**
	 * Возвращает значение переменной.
	 *
	 * @return значение переменной
	 */
	public Object getValue() {
		return value;
	}

	/**
	 * @return true если есть потомки, иначе false
	 */
	public boolean isHasChild() {
		return hasChild;
	}

	/**
	 * @return тип переменной для модели
	 */
	public ModelVariableType getType() {
		return type;
	}

	/**
	 * @return идентификатор переменной
	 */
	public long getId() {
		return id;
	}

}
