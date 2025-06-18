package su.softcom.cldt.debug.core;

public interface DebugConstants {

	//TODO: перенести константы дебага в другое место. Исп их в отдельном интерфейсе плохая практика (c) linter
	String HOST_LAUNCH_ID = "su.softcom.cldt.debug.core.hostLaunch";
	String PROJECT_NAME = "project";
	String TARGET_NAME = "target";
	String LAUNCH_ARGUMENTS = "args";
	String LIST_SEPARATOR = ";";
	String MAIN_BREAK = "break_main";
	String BREAKPOINT = "breakpoint";
	String MODEL_IDENTIFIER = "su.softcom.cldt.debug.model.id";
	String LINE_BREAKPOINT_MARKER = "su.softcom.cldt.debug.core.CommandLineBreakpointMarker";
	String WRONG_BUILD_TYPE_FOR_DEBUG = "wrong_build_type";
	

}
