package su.softcom.cldt.debug.core;

import java.io.IOException;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.EnumMap;
import java.util.Map;

import org.eclipse.core.runtime.Platform;

/**
 * Representation for Linux Auxiliary Vector 
 * 
 * @author rinat.nizamov@softcom.su
 */
public class AuxVector extends BinaryReadedObject {

	public enum AUXV_TYPE{

		/**  End of vector   */
		AT_NULL (0),
		/** entry should be ignored */
		AT_IGNORE (1),
		/** file descriptor of program */
		AT_EXECFD (2),
		/* program headers for program */
		AT_PHDR   (3),	
		/* size of program header entry */
		AT_PHENT  (4),	
		/* number of program headers */
		AT_PHNUM  (5),
		/** system page size */
		AT_PAGESZ (6),	
		/** base address of interpreter */
		AT_BASE   (7),	
		/** flags */
		AT_FLAGS  (8),	
		/** entry point of program */
		AT_ENTRY  (9),	
		/** program is not ELF */
		AT_NOTELF (10),	
		/** real uid */
		AT_UID    (11),	
		/** effective uid */
		AT_EUID   (12),	
		/** real gid */
		AT_GID    (13),	
		/** effective gid */
		AT_EGID   (14),	
		/** string identifying CPU for optimizations */
		AT_PLATFORM (15),  
		/** arch dependent hints at CPU capabilities */
		 AT_HWCAP  (16),    
		 /** frequency at which times() increments */
		 AT_CLKTCK (17),
		 /** secure mode boolean */
		 AT_SECURE (23),  
		 /** string identifying real platform, may differ from AT_PLATFORM. */
		 AT_BASE_PLATFORM (24),	
		 /** address of 16 random bytes */
		 AT_RANDOM (25),	
		 /** extension of AT_HWCAP */
		 AT_HWCAP2 (26),	
		 /** rseq supported feature size */
		 AT_RSEQ_FEATURE_SIZE (27),
		 /** rseq allocation alignment */
		 AT_RSEQ_ALIGN (28),	
		 /** extension of AT_HWCAP */
		 AT_HWCAP3 (29),	
		 /** extension of AT_HWCAP */
		 AT_HWCAP4 (30),	
		 /** filename of program */
		 AT_EXECFN  (31),	
		 /** Pointer to the global system page used for system calls and other nice things.  */
		 AT_SYSINFO (32),
		 AT_SYSINFO_EHDR (33),
		 /** minimal stack size for signal delivery */
		 AT_MINSIGSTKSZ	(51);
		
		private long typeNumber;
		
		AUXV_TYPE(long number) {
			this.typeNumber = number;
		}

		public long getNumber() {
			return typeNumber;
		}
		
		public static AUXV_TYPE getType(long type) {
			for (AUXV_TYPE auxType: AUXV_TYPE.values()){
				if (type == auxType.getNumber()) {
					return auxType;
				}
			}
			Platform.getLog(AUXV_TYPE.class).warn("Unexpected AUXV type %d".formatted(type));
			return null;
		}

	}
	
	private Map<AUXV_TYPE, Long> auxvEntries = new EnumMap<>(AUXV_TYPE.class);
	
	public AuxVector(byte[] result, ByteOrder endian) throws IOException {
		byte[] array = Arrays.copyOfRange(result, 1, result.length);
		this.setEndian(endian);
		for(int i = 0; i < (result.length-1)/16; i++) {
			AUXV_TYPE type = AUXV_TYPE.getType(readLong(array));
			auxvEntries.put(type, readLong(array));
		}
		setAuxvEntryList(auxvEntries);
	}


	private void setAuxvEntryList(Map<AUXV_TYPE, Long> auxvEntryList) {
		this.auxvEntries = auxvEntryList;
	}
	
	public long getAuxvEntry(AUXV_TYPE type) {
		return auxvEntries.get(type);
		
	}
}
