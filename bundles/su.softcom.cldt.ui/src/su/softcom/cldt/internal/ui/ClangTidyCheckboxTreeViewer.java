package su.softcom.cldt.internal.ui;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.eclipse.jface.viewers.CheckStateChangedEvent;
import org.eclipse.jface.viewers.CheckboxTreeViewer;
import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerComparator;
import org.eclipse.swt.widgets.Composite;

import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyChecks;

/**
 * A class representing check box tree viewer for clang-tidy checks.
 */
public class ClangTidyCheckboxTreeViewer extends CheckboxTreeViewer {

	private final Map<String, CheckCategory> categories = new HashMap<>();

	/**
	 * Creates a new instance of ClangTidyCheckboxTreeViewer.
	 * 
	 * @param parent parent composite.
	 * @param style  style.
	 */
	public ClangTidyCheckboxTreeViewer(Composite parent, int style) {
		super(parent, style);
		this.setChecks(ClangTidyChecks.getAll());
	}

	/**
	 * Sets up checks tree. Sets content provider, label provider and input source.
	 */
	public void setUp() {
		this.setContentProvider(new ITreeContentProvider() {
			@Override
			public Object[] getElements(Object input) {
				return categories.values().toArray();
			}

			@Override
			public Object[] getChildren(Object parent) {
				if (parent instanceof String) {
					return categories.get(parent).getChecks().toArray();
				}
				if (parent instanceof CheckCategory checkCategory) {
					return checkCategory.getChecks().toArray();
				}

				return new Object[0];
			}

			@Override
			public boolean hasChildren(Object element) {
				return element instanceof CheckCategory;
			}

			@Override
			public Object getParent(Object element) {
				if (element instanceof CheckItem checkItem) {
					return categories.get((checkItem).getCategory());
				}

				return null;
			}
		});

		this.setLabelProvider(new LabelProvider() {
			@Override
			public String getText(Object element) {
				return element.toString();
			}
		});

		this.addCheckStateListener(this::checkStateListener);

		this.setComparator(new ViewerComparator() {
			@Override
			public int compare(Viewer viewer, Object e1, Object e2) {
				return e1.toString().compareToIgnoreCase(e2.toString());
			}
		});

		this.setInput(categories);
	}

	/**
	 * Sets all checks to the given state.
	 * 
	 * @param state state.
	 */
	public void setAllChecks(boolean state) {
		for (CheckCategory category : categories.values()) {
			this.setChecked(category, state);
			this.setGrayed(category, false);

			for (CheckItem check : category.getChecks()) {
				check.setChecked(state);
				this.setChecked(check, state);
			}
		}

		this.refresh();
	}

	/**
	 * Restores default checks.
	 */
	public void restoreDefaultChecks() {
		setAllChecks(false);

		Set<String> defaultCategories = new HashSet<>(Arrays.asList("readability", "modernize", "performance")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
		Set<String> defaultChecks = new HashSet<>(Arrays.asList("misc-confusable-identifiers")); //$NON-NLS-1$

		for (CheckCategory category : categories.values()) {
			boolean isDefaultCategory = defaultCategories.contains(category.getName());

			for (CheckItem check : category.getChecks()) {
				String fullCheckName = category.getName() + "-" + check.getName(); //$NON-NLS-1$
				if (isDefaultCategory || defaultChecks.contains(fullCheckName)) {
					check.setChecked(true);
					setChecked(check, true);
				}
			}

			updateCategoryState(category);
		}
		this.refresh();
	}

	/**
	 * Returns the enabled checks as a string in the format for the operation being
	 * performed.
	 * 
	 * @return string with enabled checks.
	 */
	public String getSelectedChecksAsString() {
		List<String> selected = new ArrayList<>();
		selected.add("-*"); //$NON-NLS-1$

		for (CheckCategory category : categories.values()) {
			boolean allChecked = true;
			List<String> categoryChecks = new ArrayList<>();

			for (CheckItem check : category.getChecks()) {
				if (check.isChecked()) {
					categoryChecks.add(check.getName());
				} else {
					allChecked = false;
				}
			}

			if (allChecked && !category.getChecks().isEmpty()) {
				selected.add(category.getName() + "-*"); //$NON-NLS-1$
			} else {
				for (String check : categoryChecks) {
					selected.add(category.getName() + "-" + check); //$NON-NLS-1$
				}
			}
		}

		return String.join(",", selected); //$NON-NLS-1$
	}

	/**
	 * Enables checks from a string in the format of the checks parameter for
	 * clang-tidy.
	 */
	public void applyChecksFromString(String checksString) {
		setAllChecks(false);

		if (checksString == null || checksString.isEmpty())
			return;

		Set<String> selectedChecks = new HashSet<>(Arrays.asList(checksString.split(","))); //$NON-NLS-1$

		for (CheckCategory category : categories.values()) {
			String categoryPrefix = category.getName() + "-"; //$NON-NLS-1$
			boolean categorySelected = selectedChecks.contains(category.getName() + "-*"); //$NON-NLS-1$

			if (categorySelected) {
				for (CheckItem check : category.getChecks()) {
					check.setChecked(true);
					setChecked(check, true);
				}

				updateCategoryState(category);
				continue;
			}

			for (CheckItem check : category.getChecks()) {
				String fullCheckName = categoryPrefix + check.getName();
				if (selectedChecks.contains(fullCheckName)) {
					check.setChecked(true);
					setChecked(check, true);
				}
			}

			updateCategoryState(category);
		}
		this.refresh();
	}

	private void checkStateListener(CheckStateChangedEvent event) {
		Object element = event.getElement();
		boolean checked = event.getChecked();

		if (element instanceof CheckCategory category) {
			for (CheckItem check : category.getChecks()) {
				check.setChecked(checked);
				setChecked(check, checked);
			}
		} else if (element instanceof CheckItem check) {
			check.setChecked(checked);
			CheckCategory category = categories.get(check.getCategory());

			updateCategoryState(category);
		}

		this.refresh();
	}

	private void updateCategoryState(CheckCategory category) {
		boolean allChecked = true;
		boolean anyChecked = false;

		for (CheckItem check : category.getChecks()) {
			if (check.isChecked()) {
				anyChecked = true;
			} else {
				allChecked = false;
			}
		}

		this.setChecked(category, anyChecked);
		this.setGrayed(category, anyChecked && !allChecked);
	}

	private void setChecks(Set<String> checks) {
		for (String check : checks) {
			String[] parts = check.split("-", 2); //$NON-NLS-1$
			if (parts.length == 2) {
				String categoryName = parts[0];
				String checkName = parts[1];

				CheckCategory category = categories.computeIfAbsent(categoryName, k -> new CheckCategory(categoryName));
				category.addCheck(new CheckItem(checkName, categoryName));
			}
		}
	}

	private class CheckCategory {
		private final String name;
		private final List<CheckItem> checks = new ArrayList<>();

		public CheckCategory(String name) {
			this.name = name;
		}

		public void addCheck(CheckItem item) {
			checks.add(item);
		}

		public List<CheckItem> getChecks() {
			return Collections.unmodifiableList(checks);
		}

		public String getName() {
			return name;
		}

		@Override
		public String toString() {
			return name;
		}
	}

	private class CheckItem {
		private final String name;
		private final String category;
		private boolean checked = false;

		public CheckItem(String name, String category) {
			this.name = name;
			this.category = category;
		}

		public String getName() {
			return name;
		}

		public String getCategory() {
			return category;
		}

		public boolean isChecked() {
			return checked;
		}

		public void setChecked(boolean checked) {
			this.checked = checked;
		}

		@Override
		public String toString() {
			return name;
		}
	}
}