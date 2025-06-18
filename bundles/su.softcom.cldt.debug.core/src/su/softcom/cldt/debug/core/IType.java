package su.softcom.cldt.debug.core;

import java.util.List;

import su.softcom.cldt.debug.internal.core.dwarf.tags.Tag;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType;

/**
 * Interface providing base functionality for programming languages types at
 * debugging
 * 
 * @author petr.babanov@softcom.su
 *
 */
public interface IType {

	/**
	 * Получить индентификатор типа
	 * 
	 * @return индентификатор типа
	 */
	public long getId();

	/**
	 * Get human readable name of type
	 * 
	 * @return non-empty String
	 */
	public String getName();

	/**
	 * Get size of type in bytes
	 * 
	 * @return return long bigger 0
	 */
	public long getSize();

	/**
	 * @return тип переменной для модели
	 */
	public ModelVariableType getModelVariableType();

	/**
	 * Декодировать массив байт относительно типа в значение
	 * 
	 * @param data
	 * @return Object с декодированным значением or {@code null}
	 */
	public Object decodeValue(byte[] data);

	/**
	 * Decode byte array according type to human readable String
	 * 
	 * @param data
	 * @return String representation if may be decoded or {@code null}
	 */
	public String getStringRepresentation(byte[] data);

	/**
	 * Check has type child or not
	 * 
	 * @return {@code true} if has or {@code false} if not
	 */
	public boolean hasChild();

	/**
	 * return childs if has, or null or empty array owervise
	 * 
	 * @return {@link java.util.List List<IType>} or null or zero sized if
	 *         {@link IType#hasChild()} returns {@code false}
	 */
	public List<Tag> getTypeChildren();

	/**
	 * returns a copy of range source data array for setting to passed children
	 * 
	 * @param name - name of child
	 * @param data full data array
	 * @return new array with subsequence source array
	 */
	public byte[] getChildData(String name, byte[] data);

	/**
	 * check is methods {@link #getHexRepresentation(byte[] data)},
	 * {@link #getOctRepresentation(byte[] data)},
	 * {@link #getBinRepresentation(byte[] data)} supported
	 * 
	 * @return {@code true} if supported, {@code false} otherwise
	 */
	public boolean supportNumericalRepresentations();

	/**
	 * Get octal string representation if supported
	 * 
	 * @param data raw data
	 * @return octal string representation
	 */
	String getOctRepresentation(byte[] data);

	/**
	 * Get hexadecimal string representation if supported
	 * 
	 * @param data raw data
	 * @return hexadecimal string representation
	 */
	String getHexRepresentation(byte[] data);

	/**
	 * Get binary string representation if supported
	 * 
	 * @param data raw data
	 * @return binary string representation
	 */
	String getBinRepresentation(byte[] data);

}
