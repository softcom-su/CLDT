package su.softcom.cldt.analysis.core;

import java.util.List;

import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.IProgressMonitor;

public interface IAnalysisTool {

	public static final String I_ANALYZIS_TOOL_ID = "su.softcom.cldt.core.analyzisTool";

	public String getID();

	public String getUIName();

	public void execute(List<IResource> resources, IProgressMonitor monitor);

}
