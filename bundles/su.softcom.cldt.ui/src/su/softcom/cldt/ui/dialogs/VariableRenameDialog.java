package su.softcom.cldt.ui.dialogs;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Pattern;

import org.eclipse.jface.dialogs.InputDialog;
import org.eclipse.swt.widgets.Shell;

/**
 * Dialog box for renaming a variable.
 */
public class VariableRenameDialog extends InputDialog {

	private static final Pattern VALID_IDENTIFIER_PATTERN = Pattern.compile("^[a-zA-Z_]\\w*$"); //$NON-NLS-1$

	private static final Set<String> C_KEYWORDS = new HashSet<>(Arrays.asList("auto", "break", "case", "char", "const", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$
			"continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$ //$NON-NLS-9$ //$NON-NLS-10$ //$NON-NLS-11$ //$NON-NLS-12$
			"long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$ //$NON-NLS-9$ //$NON-NLS-10$ //$NON-NLS-11$
			"unsigned", "void", "volatile", "while")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$

	private static final Set<String> CPP_KEYWORDS = new HashSet<>(Arrays.asList("alignas", "alignof", "and", "and_eq", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
			"asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "bitand", "bitor", "bool", "catch", "class", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$ //$NON-NLS-9$
			"compl", "concept", "const_cast", "constexpr", "decltype", "delete", "dynamic_cast", "explicit", "export", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$ //$NON-NLS-9$
			"false", "friend", "inline", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$ //$NON-NLS-9$ //$NON-NLS-10$
			"operator", "or", "or_eq", "private", "protected", "public", "reinterpret_cast", "static_assert", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$
			"static_cast", "template", "this", "thread_local", "throw", "true", "try", "typeid", "typename", "using", //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$ //$NON-NLS-5$ //$NON-NLS-6$ //$NON-NLS-7$ //$NON-NLS-8$ //$NON-NLS-9$ //$NON-NLS-10$
			"virtual", "xor", "xor_eq")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$

	/**
	 * Creates an instance for VariableRenameDialog.
	 * 
	 * @param parent        parent shell.
	 * @param dialogTitle   title of the dialog box.
	 * @param dialogMessage message of the dialog box.
	 * @param initialValue  initial value in the input field.
	 */
	public VariableRenameDialog(Shell parent, String dialogTitle, String dialogMessage, String initialValue) {
		super(parent, dialogTitle, dialogMessage, initialValue, VariableRenameDialog::validateName);
	}

	/**
	 * Gets a new variable name.
	 * 
	 * @return new variable name.
	 */
	public String getNewName() {
		return super.getValue();
	}

	private static String validateName(String newName) {
		if (newName == null || newName.trim().isEmpty()) {
			return "Имя не может быть пустым";
		}

		if (!VALID_IDENTIFIER_PATTERN.matcher(newName).matches()) {
			return """
					Некорректное имя переменной. Имя должно:
					- Начинаться с буквы или символа '_'
					- Содержать только буквы, цифры и символ '_'
					""";
		}

		String lowerCaseName = newName.toLowerCase();
		if (C_KEYWORDS.contains(lowerCaseName) || CPP_KEYWORDS.contains(lowerCaseName)) {
			return "Имя не может быть ключевым словом C/C++";
		}

		if (newName.matches("^_[A-Z].*")) { //$NON-NLS-1$
			return "Имена, начинающиеся с символа '_' и заглавной буквы, зарезервированы для реализации";
		}

		if (newName.contains("__")) { //$NON-NLS-1$
			return "Имена, содержащие двойное подчеркивание '__', зарезервированы для компилятора";
		}

		return null;
	}
}