package su.softcom.cldt.ui;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.eclipse.jface.action.MenuManager;
import org.eclipse.swt.SWT;
import org.eclipse.ui.menus.CommandContributionItem;
import org.eclipse.ui.menus.CommandContributionItemParameter;
import org.eclipse.ui.menus.ExtensionContributionFactory;
import org.eclipse.ui.menus.IContributionRoot;
import org.eclipse.ui.services.IServiceLocator;

import su.softcom.cldt.analysis.core.IAnalysisTool;
import su.softcom.cldt.core.CMakeCorePlugin;

public class AnalyzisSubmenu extends ExtensionContributionFactory {

	public AnalyzisSubmenu() {
		// TODO Auto-generated constructor stub
	}

	public static void inintMenuManager(MenuManager manager, IServiceLocator sl) {
		Set<IAnalysisTool> analyzisTools = CMakeCorePlugin.eINSTANCE.getAnalyzisProvider().getAnalyzisTools();
		for (IAnalysisTool tool : analyzisTools) {
			CommandContributionItemParameter p = new CommandContributionItemParameter(sl, "", tool.getID(), SWT.PUSH);
			p.label = tool.getUIName();

			CommandContributionItem tci = new CommandContributionItem(p);

			tci.fill(null);
			tci.setVisible(true);

			manager.add(tci);
		}
	}

	public static List<CommandContributionItem> createContributionItemsList(IServiceLocator serviceLocator) {
		List<CommandContributionItem> ret = new ArrayList<CommandContributionItem>();
		Set<IAnalysisTool> analyzisTools = CMakeCorePlugin.eINSTANCE.getAnalyzisProvider().getAnalyzisTools();

		for (IAnalysisTool tool : analyzisTools) {
			CommandContributionItemParameter p = new CommandContributionItemParameter(serviceLocator, "", tool.getID(),
					SWT.PUSH);
			p.label = tool.getUIName();

			CommandContributionItem tci = new CommandContributionItem(p);

			tci.fill(null);
			tci.setVisible(true);

			ret.add(tci);
		}

		return ret;
	}

	@Override
	public void createContributionItems(IServiceLocator serviceLocator, IContributionRoot additions) {

		Set<IAnalysisTool> analyzisTools = CMakeCorePlugin.eINSTANCE.getAnalyzisProvider().getAnalyzisTools();

		for (IAnalysisTool tool : analyzisTools) {
			CommandContributionItemParameter p = new CommandContributionItemParameter(serviceLocator, "", tool.getID(),
					SWT.PUSH);
			p.label = tool.getUIName();

			CommandContributionItem tci = new CommandContributionItem(p);

			tci.fill(null);
			tci.setVisible(true);

			additions.addContributionItem(tci, null);
		}

	}
}
