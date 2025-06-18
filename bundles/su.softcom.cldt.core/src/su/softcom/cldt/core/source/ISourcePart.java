package su.softcom.cldt.core.source;

import java.util.List;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.runtime.IAdaptable;

/**
 * Интерфейс, описывающий составную часть кода проекта.
 */
public interface ISourcePart extends IAdaptable {

	/**
	 * Id for request node from project scope with exists sources.
	 */
	final String SOURCES_CONTAINER = "sources-container"; //$NON-NLS-1$

	/**
	 * Возвращает список дочерних элементов если они существуют.
	 * 
	 * @return {@link List}, никогда не {@code null}
	 */
	public List<ISourcePart> getElements();

	/**
	 * @return project
	 */
	public IProject getProject();

	/**
	 * @return name уникальное имя объекта
	 */
	public String getIdentifierName();

}
