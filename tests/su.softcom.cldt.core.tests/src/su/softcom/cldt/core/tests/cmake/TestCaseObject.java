package su.softcom.cldt.core.tests.cmake;

import java.util.List;

public class TestCaseObject {
	
	public static class TokenDescription{
		public String type;
		public String value;
	}

	public String name;
	public String content;
	public List<TokenDescription> tokens;
	
	@Override
	public String toString() {
		return name;
	}
}
