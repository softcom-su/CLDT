package su.softcom.cldt.debug.internal.core.dwarf.cfa;

import java.nio.ByteBuffer;
import org.eclipse.core.runtime.Platform;
import su.softcom.cldt.debug.internal.core.dwarf.CallFrameInfo.CommonInfoEntry;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfElement;

public final class CFAFactory extends DwarfElement {
	
	
	
	public static ICallFrameInstruction getCFA(ByteBuffer data, CommonInfoEntry cie) {
		byte code = data.get();
		byte highBits = (byte)(((code) & 192) >> 6);
		byte lowBits = (byte)(code & ((byte)63));
		if (highBits == 1) {
			return new AdvanceLoc(lowBits, 0);
		} else if (highBits == 2) {
			return new CFAOffset(lowBits, readUleb128(data), cie);
		} else if (highBits == 3) {
			return null; //TODO: Not yet implemented
		} else {
			Long value = 0L;  //TODO: Not yet implemented
			switch(lowBits) {
			case DwarfConstants.DW_CFA_nop:
				return null;
			case DwarfConstants.DW_CFA_remember_state:
				return new RememberStateCFA();
			case DwarfConstants.DW_CFA_restore_state:
				return new RestoreStateCFA();
			case DwarfConstants.DW_CFA_def_cfa_register:
				return new DefCFARegister(readUleb128(data));
			case DwarfConstants.DW_CFA_def_cfa_offset:
				return new DefCFAOffset(readUleb128(data));
			case DwarfConstants.DW_CFA_def_cfa:
				return new DefCFA(readUleb128(data), readUleb128(data));
			case DwarfConstants.DW_CFA_undefined:
				return new UndefinedCFA(readUleb128(data));
			case DwarfConstants.DW_CFA_advance_loc1:
				return new AdvanceLoc(Byte.toUnsignedInt(data.get()), 1);
			case DwarfConstants.DW_CFA_advance_loc2:
				return new AdvanceLoc(data.getShort(), 2);
			case DwarfConstants.DW_CFA_advance_loc4:
				return new AdvanceLoc(data.getInt(), 4);
			case DwarfConstants.DW_CFA_register:
				return new CFARegister(readUleb128(data),readUleb128(data));
			case DwarfConstants.DW_CFA_GNU_args_size:
				// TODO: DW_CFA_GNU_args_size
				readUleb128(data);
				Platform.getLog(CFAFactory.class).info("Нереализованная инструкция DW_CFA_GNU_args_size");
				return null;
			case DwarfConstants.DW_CFA_expression:
				//TODO: create expression rule, return here
				value = readUleb128(data);
				long size1 = readUleb128(data);
				byte[] arr1 = new byte[(int) size1];
				data.get(arr1);
				Platform.getLog(CFAFactory.class).info("Нереализованная инструкция DW_CFA_expression");
				return null;
			case DwarfConstants.DW_CFA_offset_extended_sf:
				return new OffsetExtendedCFA(readUleb128(data), readSleb128(data), cie);
			case DwarfConstants.DW_CFA_offset_extended:
				return new OffsetExtendedCFA(readUleb128(data), readUleb128(data), cie);
			case DwarfConstants.DW_CFA_same_value:
				value = readUleb128(data); //TODO: Not yet implemented
				Platform.getLog(CFAFactory.class).info("Нереализованная инструкция DW_CFA_same_value");
				return null;
			case DwarfConstants.DW_CFA_def_cfa_expression:
				long size = readUleb128(data);
				byte[] arr = new byte[(int) size];
				data.get(arr);
				return new DefCFAExpression(arr);
			default:
				Platform.getLog(CFAFactory.class).info("Неизвестная инструкция с номером: " + lowBits);
				return null;
			} 

		}
	}

}

