package su.softcom.cldt.core.builders;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import su.softcom.cldt.core.cmake.ICMakeProject;

public class LanguageStandartManager {

	public interface ILanguageStandart {
		public String getUIName();

		public String getCMakeName();

		public String getPreferenceString();
	}

	private static class LanguageStandart implements ILanguageStandart {
		String uiName;
		String cmakeName;
		String prefernce;

		public LanguageStandart(String uiName, String cmakeName) {
			this.prefernce = cmakeName;
			this.cmakeName = cmakeName;
			this.uiName = uiName;
		}

		@Override
		public String getUIName() {
			return uiName;
		}

		@Override
		public String getCMakeName() {
			return cmakeName;
		}

		@Override
		public String getPreferenceString() {
			return prefernce;
		}
	}

	private static final String CXX_STANDART_98 = "C++ 98";
//	private static final String CXX_STANDART_03 = "C++ 03";
	private static final String CXX_STANDART_11 = "C++ 11";
	private static final String CXX_STANDART_14 = "C++ 14";
	private static final String CXX_STANDART_17 = "C++ 17";
	private static final String CXX_STANDART_20 = "C++ 20";
	private static final String CXX_STANDART_23 = "C++ 23";

	private static final String C_STANDART_90 = "C 90";
	private static final String C_STANDART_99 = "C 99";
	private static final String C_STANDART_11 = "C 11";
	private static final String C_STANDART_17 = "C 17";
	private static final String C_STANDART_23 = "C 23";

	private static final Map<String, List<ILanguageStandart>> STANDARD = Map.of(ICMakeProject.LANGUAGE_CXX, List.of(
			new LanguageStandart(CXX_STANDART_98, "cxx_std_98"), new LanguageStandart(CXX_STANDART_11, "cxx_std_11"),
			new LanguageStandart(CXX_STANDART_14, "cxx_std_14"), new LanguageStandart(CXX_STANDART_17, "cxx_std_17"),
			new LanguageStandart(CXX_STANDART_20, "cxx_std_20"), new LanguageStandart(CXX_STANDART_23, "cxx_std_23")),
			ICMakeProject.LANGUAGE_C,
			List.of(new LanguageStandart(C_STANDART_90, "c_std_90"), new LanguageStandart(C_STANDART_99, "c_std_99"),
					new LanguageStandart(C_STANDART_11, "c_std_11"), new LanguageStandart(C_STANDART_17, "c_std_17"),
					new LanguageStandart(C_STANDART_23, "c_std_23")));

	static public List<ILanguageStandart> getAvailableLanguageStandarts(String language) {
		return STANDARD.containsKey(language) ? STANDARD.get(language) : new ArrayList<ILanguageStandart>();
	}

	static public ILanguageStandart getLanguageStandartFromPreferenceString(String preferenceString) {
		List<ILanguageStandart> suitableLSs = new ArrayList<ILanguageStandart>();

		for (List<ILanguageStandart> standartList : STANDARD.values()) {
			for (ILanguageStandart standard : standartList) {
				if (standard.getPreferenceString().equals(preferenceString)) {
					suitableLSs.add(standard);
				}
			}
		}

		return suitableLSs.isEmpty()
				? new LanguageStandart("Unknown Standart( %s)".formatted(preferenceString), preferenceString)
				: suitableLSs.get(0);
	}

	static public ILanguageStandart getLanguageStandartFromUIName(String uiName) {
		List<ILanguageStandart> suitableLSs = new ArrayList<ILanguageStandart>();

		for (List<ILanguageStandart> standartList : STANDARD.values()) {
			for (ILanguageStandart standard : standartList) {
				if (standard.getUIName().equals(uiName)) {
					suitableLSs.add(standard);
				}
			}
		}
		return suitableLSs.isEmpty()
				? new LanguageStandart("Unknown Standart( %s)".formatted(uiName), uiName.strip().replace(' ', '_'))
				: suitableLSs.get(0);
	}

	static public boolean standardIsC(String standard) {
		boolean found = false;
		for (ILanguageStandart std : STANDARD.get(ICMakeProject.LANGUAGE_C)) {
			if (std.getPreferenceString().equals(standard) || std.getCMakeName().equals(standard)
					|| std.getUIName().equals(standard)) {
				found = true;
				break;
			}
		}
		return found;
	}

	static public String getLanguageStandartUIName(String cmakeName) {
		for (String language : STANDARD.keySet()) {
			for (ILanguageStandart languageStandert : LanguageStandartManager.getAvailableLanguageStandarts(language)) {
				if (languageStandert.getCMakeName().equals(cmakeName)) {
					return languageStandert.getUIName();
				}
			}
		}

		return new String();
	}
}
