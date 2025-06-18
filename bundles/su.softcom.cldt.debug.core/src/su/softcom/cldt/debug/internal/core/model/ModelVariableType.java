package su.softcom.cldt.debug.internal.core.model;

public class ModelVariableType {

	private final ModelVariableType child;
	private final String name;
	private final MVType type;

	public enum MVType {
		INTEGER, FLOAT, ARRAY, STRUCTURE, UNION, ENUMERATION, POINTER, REFERENCE, TEMPLATE, // ?
		BOOLEAN, CHARACTER, CLASS, EMPTY
	}

	/**
	 * Конструктор типа переменной в модели
	 * 
	 * @param type
	 * @param name
	 * @param child
	 */
	public ModelVariableType(MVType type, String name, ModelVariableType child) {
		this.type = type;
		this.name = name;
		this.child = child;
	}

	/**
	 * @return потомок типа
	 */
	public ModelVariableType getChild() {
		return child;
	}

	/**
	 * @return тип
	 */
	public MVType getType() {
		return type;
	}

	/**
	 * @return имя типа если есть
	 */
	public String getName() {
		return name;
	}

}
