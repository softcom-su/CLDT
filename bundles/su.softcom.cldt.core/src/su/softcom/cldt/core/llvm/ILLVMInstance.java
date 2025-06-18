package su.softcom.cldt.core.llvm;

import java.io.File;
import java.util.List;

import su.softcom.cldt.core.Version;

/**
 * Interface representing an instance of LLVM tools. Provides access to
 * available tools, supported architectures, and version information.
 */
public interface ILLVMInstance {

	/**
	 * Represents a target architecture supported by the LLVM instance.
	 */
	class Architecture {

		final String name;
		final String description;

		/**
		 * Constructs a new Architecture instance.
		 *
		 * @param name        the identifier (short name) of the architecture (e.g.,
		 *                    "x86_64")
		 * @param description a human-readable description of the architecture (e.g.,
		 *                    "Intel x86-64 architecture")
		 */
		public Architecture(String name, String description) {
			this.name = name;
			this.description = description;
		}

		/**
		 * Returns the name of the architecture.
		 *
		 * @return the architecture name
		 */
		public String getName() {
			return name;
		}

		/**
		 * Returns the human-readable description of the architecture.
		 *
		 * @return the architecture description
		 */
		public String getDescription() {
			return description;
		}
	}

	/**
	 * Checks if the instance contains a tool with the specified name.
	 *
	 * @param toolName the name of the tool (without extension), e.g., "clang" or
	 *                 "clang++"
	 * @return {@code true} if the tool is present; {@code false} otherwise
	 */
	boolean hasTool(String toolName);

	/**
	 * Returns a list of target architectures supported by this LLVM instance.
	 *
	 * @return a list of {@link Architecture} objects representing supported targets
	 */
	List<Architecture> getTargets();

	/**
	 * Returns the file corresponding to the specified tool name, if present.
	 *
	 * @param name the name of the tool (without extension), e.g., "clang"
	 * @return the {@link File} of the tool, or {@code null} if not found
	 */
	File getTool(String name);

	/**
	 * Returns the version of this LLVM instance.
	 *
	 * @return the {@link Version} object representing the LLVM version
	 */
	Version getVersion();
}
