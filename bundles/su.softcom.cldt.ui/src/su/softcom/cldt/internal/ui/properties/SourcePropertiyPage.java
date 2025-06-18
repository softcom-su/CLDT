package su.softcom.cldt.internal.ui.properties;

import java.util.List;
import java.util.function.Function;

import org.eclipse.jface.resource.FontDescriptor;
import org.eclipse.jface.resource.JFaceResources;
import org.eclipse.jface.viewers.IBaseLabelProvider;
import org.eclipse.jface.viewers.IFontProvider;
import org.eclipse.jface.viewers.ILabelProvider;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Tree;
import org.eclipse.swt.widgets.Widget;
import org.eclipse.ui.IWorkbenchPropertyPage;
import org.eclipse.ui.dialogs.PropertyPage;

import su.softcom.cldt.core.cmake.Target;
import su.softcom.cldt.core.source.ISourceFile;

public class SourcePropertiyPage extends PropertyPage implements IWorkbenchPropertyPage {

	private class ContentProvider implements ITreeContentProvider {

		final Function<Target, List<? extends Object>> converter;

		public ContentProvider(Function<Target, List<? extends Object>> f) {
			converter = f;
		}

		@Override
		public Object[] getElements(Object inputElement) {
			if (inputElement instanceof Target target) {
				return converter.apply(target).toArray();
			}
			return new Object[] { inputElement };
		}

		@Override
		public Object[] getChildren(Object parentElement) {
			return null;
		}

		@Override
		public Object getParent(Object element) {
			return null;
		}

		@Override
		public boolean hasChildren(Object element) {
			return false;
		}
	}

	private class LabelProvider implements ILabelProvider, IFontProvider {

		final Font generatedFont;

		public LabelProvider(Widget w) {
			generatedFont = FontDescriptor.createFrom(JFaceResources.getDefaultFont()).setStyle(SWT.ITALIC)
					.createFont(w.getDisplay());
		}

		@Override
		public Image getImage(Object element) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public String getText(Object element) {
			if (element instanceof ISourceFile src) {
				String result = src.getName();
				if (src.isGenerated()) {
					result.concat(" (Генерированный)");
				}
				return result;
			}
			return null;
		}

		@Override
		public void addListener(ILabelProviderListener listener) {
			// TODO Auto-generated method stub

		}

		@Override
		public void dispose() {
			generatedFont.dispose();
		}

		@Override
		public boolean isLabelProperty(Object element, String property) {
			// TODO Auto-generated method stub
			return false;
		}

		@Override
		public void removeListener(ILabelProviderListener listener) {
			// TODO Auto-generated method stub

		}

		@Override
		public Font getFont(Object element) {
			if (element instanceof ISourceFile src) {
				if (src.isGenerated()) {
					return generatedFont;
				}
			}
			return null;
		}
	}

	private Composite pageRoot;

	public SourcePropertiyPage() {
	}

	@Override
	protected Control createContents(Composite parent) {
		pageRoot = new Composite(parent, SWT.NONE);
		pageRoot.setLayout(new GridLayout(1, false));
		Group src = createAddRemoveTreeGroup(pageRoot, "Исходный код", new ContentProvider(e -> e.getSources()),
				new LabelProvider(pageRoot));
		Group inc = createAddRemoveTreeGroup(pageRoot, "Заголовочные файлы", new ContentProvider(e -> e.getSources()),
				new LabelProvider(pageRoot));
		Group lib = createAddRemoveTreeGroup(pageRoot, "Библиотеки", new ContentProvider(e -> e.getSources()),
				new LabelProvider(pageRoot));
		return pageRoot;
	}

	private Group createAddRemoveTreeGroup(Composite root, String label, ITreeContentProvider contentProvider,
			IBaseLabelProvider labelProvider) {

		Group group = new Group(root, SWT.NONE);
		group.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		group.setText(label);
		group.setLayout(new GridLayout(2, false));

		TreeViewer treeViewer = new TreeViewer(group, SWT.BORDER);
		treeViewer.setContentProvider(contentProvider);
		treeViewer.setLabelProvider(labelProvider);
		treeViewer.setInput(getElement());
		Tree tree = treeViewer.getTree();
		tree.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 2));

		Button addButton = new Button(group, SWT.NONE);
		addButton.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));
		addButton.setText("Добавить");

		Button removeButton = new Button(group, SWT.NONE);
		removeButton.setLayoutData(new GridData(SWT.FILL, SWT.TOP, false, false, 1, 1));
		removeButton.setText("Удалить");
		return group;
	}
}
