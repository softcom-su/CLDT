package su.softcom.cldt.core.source;

import org.eclipse.core.resources.IProject;

/**
 * Абстрактная базовая реализация интерфейса {@link ISourcePart}
 * <p>
 * Хранит ссылку на {@link IProject}, к которому относится данный элемент.
 */
public abstract class AbstractSourcePart implements ISourcePart {

	protected final IProject project;

	protected AbstractSourcePart(IProject project) {
		this.project = project;
	}

	@Override
	public IProject getProject() {
		return project;
	}

}
