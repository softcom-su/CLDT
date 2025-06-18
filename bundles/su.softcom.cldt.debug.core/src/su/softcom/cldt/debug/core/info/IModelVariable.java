package su.softcom.cldt.debug.core.info;

import java.util.List;

import org.eclipse.debug.core.model.IVariable;

import su.softcom.cldt.debug.internal.core.model.ModelVariable;

public interface IModelVariable extends IVariable, IHasParent {

	public long getId();

	/**
	 * Получить детей переменной
	 * 
	 * @return дети переменной
	 */
	public List<IModelVariable> getChildrens();

	/**
	 * Присвоить значение переменной
	 * 
	 * @param newValue
	 */
	public void setValue(Object newValue);

	/**
	 * Получить адрес переменной
	 * 
	 * @return адрес переменной
	 */
	public long getAddress();

	/**
	 * Изменить состояние переменной. {@code true} если переменная гразная и нужно
	 * ее обновить, иначе {@code false} если переменная с актуальным значением
	 * 
	 * @param isDirty
	 */
	public void setDirty(boolean isDirty);
}
