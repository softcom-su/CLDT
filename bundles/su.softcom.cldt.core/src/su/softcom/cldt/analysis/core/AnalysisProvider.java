package su.softcom.cldt.analysis.core;

import java.util.HashSet;
import java.util.Set;

public class AnalysisProvider {
	static AnalysisProvider instance;

	Set<IAnalysisTool> availableTools;

	public AnalysisProvider() {
		availableTools = new HashSet<IAnalysisTool>();
	}

	public void reportAnalyzisTool(IAnalysisTool tool) {
		availableTools.add(tool);
	}

	public Set<IAnalysisTool> getAnalyzisTools() {
		return availableTools;
	}
}
