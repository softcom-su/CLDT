package su.softcom.cldt.core.source;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ProjectScope;
import org.eclipse.core.runtime.Assert;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.lsp4j.DocumentSymbol;
import org.osgi.service.prefs.Preferences;

import su.softcom.cldt.core.CMakeCorePlugin;
import su.softcom.cldt.core.cmake.ICMakeProject;
import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.internal.core.lsp.DocumentSymbolHandler;

/**
 * Cpp реализация ISourceFile.
 */
public class SourceFile extends AbstractSourcePart implements ISourceFile {

	private static final String SRC_IS_GENERATED = "src_gen"; //$NON-NLS-1$
	private static final String SRC_PATH = "src_path"; //$NON-NLS-1$

	/**
	 * Типы исходных файлов, поддерживаемые в CMake-проектах.
	 * <p>
	 * Используется для классификации файлов в системе сборки.
	 * </p>
	 */
	public enum SourceType {

		/**
		 * Исходный файл на языке C.
		 * <p>
		 * Примеры расширений: .c
		 * </p>
		 * <p>
		 * Идентификатор: 1
		 * </p>
		 */
		C_FILE(1),

		/**
		 * Исходный файл на языке C++.
		 * <p>
		 * Примеры расширений: .c, .cpp, .cxx
		 * </p>
		 * <p>
		 * Идентификатор: 2
		 * </p>
		 */
		CPP_FILE(2),

		/**
		 * Файл неопределенного или неподдерживаемого типа.
		 * <p>
		 * Идентификатор: 0
		 * </p>
		 */
		UNDEFINED(0);

		private final int fileTypeCode;

		/**
		 * Конструктор типа файла.
		 * 
		 * @param code числовой идентификатор типа
		 */
		SourceType(int code) {
			this.fileTypeCode = code;
		}

		/**
		 * Возвращает числовой идентификатор типа файла.
		 * 
		 * @return уникальный код типа
		 */
		public int getFileTypeCode() {
			return fileTypeCode;
		}
	}

	int backtrace;
	int compileGroupIndex;
	int sourceGroupIndex;
	boolean isGenerated;

	private final ICMakeProject cmakeProject;
	private final List<Target> targets = new ArrayList<>();

	private String name;
	private IFile file;
	private SourceType type;
	private boolean isDirty;
	private Preferences preferences;
	private final List<ISourceElement> elements = new LinkedList<>();

	/**
	 * Явное создание исходного файла CMake
	 * 
	 * @param path   IPath к eclipse файлу
	 * @param target цель сборки к которому относиться этот файл
	 */
	public SourceFile(IPath path, Target target) {
		this(path.lastSegment(), target.getProject());
		this.isGenerated = true;
		this.file = (IFile) project.findMember(path);
	}

	/**
	 * Cоздание CppSource используя {@code Preferences}
	 * 
	 * @param name   имя исходного файла
	 * @param target цель сборки
	 */
	public SourceFile(String name, Target target) {
		this(name, target.getProject());
		IResource foundRes = project.findMember(preferences.get(SRC_PATH, "")); //$NON-NLS-1$
		if (foundRes instanceof IFile f) {
			this.file = f;
		} else {
			this.file = null;
		}
		this.isGenerated = preferences.getBoolean(SRC_IS_GENERATED, false);
	}

	private SourceFile(String name, ICMakeProject project) {
		super(project.getProject());
		this.cmakeProject = project;
		this.type = getSourceType(name);
		this.name = name;
		this.preferences = new ProjectScope(project.getProject()).getNode(CMakeCorePlugin.PLUGIN_ID)
				.node(ISourcePart.SOURCES_CONTAINER).node(name);
		this.isDirty = true;
	}

	@Override
	public String getName() {
		if (name == null) {
			return ""; //$NON-NLS-1$
		}
		return name;
	}

	@Override
	public boolean isGenerated() {
		return isGenerated;
	}

	@Override
	public IFile getFile() {
		return file;
	}

	/**
	 * @return type of this CppSource
	 */
	public SourceType getSourceType() {
		return type;
	}

	/**
	 * 
	 */
	public void save() {
		if (file == null) {
			return;
		}
		if (preferences == null) {
			preferences = new ProjectScope(project).getNode(CMakeCorePlugin.PLUGIN_ID)
					.node(ISourcePart.SOURCES_CONTAINER).node(name);
		}
		preferences.putBoolean(SRC_IS_GENERATED, isGenerated);
		preferences.put(SRC_PATH, file.getProjectRelativePath().toPortableString());
	}

	/**
	 * @param target
	 */
	@Override
	public void addTarget(Target target) {
		if (targets.contains(target)) {
			return;
		}
		targets.add(target);
	}

	/**
	 * @return targets
	 */
	@Override
	public List<Target> getTargets() {
		return targets;
	}

	private SourceType getSourceType(String fileStr) {
		if (fileStr == null) {
			return SourceType.UNDEFINED;
		}
		IPath filePath = new Path(fileStr);
		String extension = filePath.getFileExtension();
		if (extension == null) {
			return SourceType.UNDEFINED;
		}

		return switch (extension.toLowerCase()) {
		case "c", "h" -> SourceType.C_FILE; //$NON-NLS-1$ //$NON-NLS-2$
		case "cpp", "cc", "cxx", "hpp", "hxx" -> SourceType.CPP_FILE; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$
		default -> SourceType.UNDEFINED;
		};
	}

	@Override
	public String toString() {
		return " %s in %s".formatted(this.getName(), this.cmakeProject.toString()); //$NON-NLS-1$
	}

	@Override
	@SuppressWarnings("unchecked")
	public <T> T getAdapter(Class<T> adapter) {
		Assert.isNotNull(adapter);
		if (IFile.class.equals(adapter)) {
			return (T) file;
		}
		if (IResource.class.equals(adapter)) {
			return (T) file;
		}
		return null;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null || getClass() != obj.getClass()) {
			return false;
		}
		SourceFile other = (SourceFile) obj;
		if (!other.getName().equals(this.getName())) {
			return false;
		}
		if (file == null || other.file == null) {
			return false;
		}
		String othersPathStr = other.getFile().getLocation().toOSString();
		return file.getLocation().toOSString().equals(othersPathStr);
	}

	@Override
	public int hashCode() {
		int result = 17;
		result = 31 * result + (name != null ? name.hashCode() : 0);
		result = 31 * result + (cmakeProject != null ? cmakeProject.hashCode() : 0);
		return result;
	}

	/**
	 * Sets Dirty status
	 * 
	 * @param isDirty value
	 */
	public void setDirty(boolean isDirty) {
		this.isDirty = isDirty;
	}

	@Override
	public List<ISourcePart> getElements() {
		if (isDirty) {
			refreshElements();
		}
		return Collections.unmodifiableList(elements);
	}

	private void refreshElements() {
		List<DocumentSymbol> symbols = DocumentSymbolHandler.getSymbols(this);
		elements.clear();
		var tmp = symbols.stream().map(symbol -> SourcePartFactory.getDefault().getSourceElement(symbol, this))
				.toList();
		elements.addAll(tmp);
		// TODO подумать как тут лучше сделать
//		for (DocumentSymbol sym : symbols) {
//			ISourceElement elem = SourcePartFactory.getDefault().getSourceElement(sym, this);
//			if (!elements.contains(elem)) {
//				elements.add(elem);
//			}
//
//		}
	}

	@Override
	public String getIdentifierName() {
		return (file != null) ? file.getFullPath().toString() : name;
	}

}