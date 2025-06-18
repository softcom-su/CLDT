package su.softcom.cldt.debug.internal.core.dwarf.op;

import java.io.IOException;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Map;

import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.ICompileUnit;
import su.softcom.cldt.debug.core.IFunction;
import su.softcom.cldt.debug.internal.core.GDBClient;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfConstants;
import su.softcom.cldt.debug.internal.core.dwarf.DwarfElement;

public class Operation extends DwarfElement {

	public static long evaluateExpression(GDBClient client, Map<Integer, BigInteger> regs, IFunction function,
			ICompileUnit cu, byte[] expression) {
		Deque<BigInteger> stack = new ArrayDeque<>();
		ByteBuffer buffer = ByteBuffer.wrap(expression);
		
		while (buffer.hasRemaining()) {
			BigInteger a, b;
			int opcode = buffer.get() & 0xFF;

			switch (opcode) {
			case DwarfConstants.DW_OP_addrx:
				stack.push(BigInteger.valueOf(cu.getAddress((int) readUleb128(buffer))));
				break;
			case DwarfConstants.DW_OP_bregx:
				long regNumber = readUleb128(buffer);
				long offset = readLeb128(buffer);
				int index = client.getDwarfReg((int) regNumber);
				BigInteger regValue = regs.get(index);
				stack.push(regValue.add(BigInteger.valueOf(offset)));
				break;
			case DwarfConstants.DW_OP_plus:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.add(b));
				break;
			case DwarfConstants.DW_OP_and:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.and(b));
				break;
			case DwarfConstants.DW_OP_shl:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.shiftLeft(b.intValue()));
				break;
			case DwarfConstants.DW_OP_ge:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.compareTo(b) >= 0 ? BigInteger.ONE : BigInteger.ZERO);
				break;
			case DwarfConstants.DW_OP_minus:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.subtract(b));
				break;
			case DwarfConstants.DW_OP_mul:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.multiply(b));
				break;
			case DwarfConstants.DW_OP_div:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.divide(b));
				break;
			case DwarfConstants.DW_OP_mod:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.mod(b));
				break;
			case DwarfConstants.DW_OP_neg:
				a = stack.pop();
				stack.push(a.negate());
				break;
			case DwarfConstants.DW_OP_not:
				a = stack.pop();
				stack.push(a.not());
				break;
			case DwarfConstants.DW_OP_dup:
				stack.push(stack.peek());
				break;
			case DwarfConstants.DW_OP_drop:
				stack.pop();
				break;
			case DwarfConstants.DW_OP_over:
				b = stack.pop();
				a = stack.peek();
				stack.push(b);
				stack.push(a);
				break;
			case DwarfConstants.DW_OP_swap:
				b = stack.pop();
				a = stack.pop();
				stack.push(b);
				stack.push(a);
				break;
			case DwarfConstants.DW_OP_rot:
				BigInteger third = stack.removeLast();
				BigInteger second = stack.removeLast();
				BigInteger first = stack.removeLast();
				stack.push(second);
				stack.push(third);
				stack.push(first);
				break;
			case DwarfConstants.DW_OP_deref:
				BigInteger top = stack.pop();
				try {
					long derefAddress = client.deref(top.longValue(), cu.getAddressSize());
					stack.push(BigInteger.valueOf(derefAddress));
				} catch (IOException e) {
					e.printStackTrace();
				}
				break;
			case DwarfConstants.DW_OP_regx:
				int i = client.getDwarfReg((int) readUleb128(buffer));
				stack.push(regs.get(i));
				break;
			case DwarfConstants.DW_OP_piece:
//				long size = readUleb128(buffer);
				// TODO: Реализовать поддержку сборки адреса по кусочкам
				break;
			case DwarfConstants.DW_OP_deref_size:
//				int size = buffer.get() & 0xFF;
				// TODO: Разобраться как это сделать
				break;
			case DwarfConstants.DW_OP_xderef_size:
//				int size = buffer.get() & 0xFF;
				// TODO: Разобраться как это сделать
				break;
			case DwarfConstants.DW_OP_nop:
				// No operation; продолжаем
				break;
			case DwarfConstants.DW_OP_gt:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.compareTo(b) > 0 ? BigInteger.ONE : BigInteger.ZERO);
				break;
			case DwarfConstants.DW_OP_le:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.compareTo(b) <= 0 ? BigInteger.ONE : BigInteger.ZERO);
				break;
			case DwarfConstants.DW_OP_lt:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.compareTo(b) < 0 ? BigInteger.ONE : BigInteger.ZERO);
				break;
			case DwarfConstants.DW_OP_ne:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.compareTo(b) != 0 ? BigInteger.ONE : BigInteger.ZERO);
				break;
			case DwarfConstants.DW_OP_skip:
				int skipBytes = (int) readLeb128(buffer);
				buffer.position(buffer.position() + skipBytes);
				break;
			case DwarfConstants.DW_OP_shr:
			case DwarfConstants.DW_OP_shra:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.shiftRight(b.intValue()));
				break;
			case DwarfConstants.DW_OP_xor:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.xor(b));
				break;
			case DwarfConstants.DW_OP_bra:
				buffer.position(buffer.position() + (int) readLeb128(buffer));
				break;
			case DwarfConstants.DW_OP_eq:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.compareTo(b) == 0 ? BigInteger.ONE : BigInteger.ZERO);
				break;
			case DwarfConstants.DW_OP_addr:
				stack.push(BigInteger.valueOf(readUleb128(buffer)));
				break;
			case DwarfConstants.DW_OP_const1u:
				stack.push(BigInteger.valueOf(buffer.get() & 0xFF));
				break;
			case DwarfConstants.DW_OP_const1s:
				stack.push(BigInteger.valueOf(buffer.get()));
				break;
			case DwarfConstants.DW_OP_const2u:
				stack.push(BigInteger.valueOf(buffer.getShort() & 0xFFFF));
				break;
			case DwarfConstants.DW_OP_const2s:
				stack.push(BigInteger.valueOf(buffer.getShort()));
				break;
			case DwarfConstants.DW_OP_const4u:
				stack.push(BigInteger.valueOf(buffer.getInt() & 0xFFFFFFFF));
				break;
			case DwarfConstants.DW_OP_const4s:
				stack.push(BigInteger.valueOf(buffer.getInt()));
				break;
			case DwarfConstants.DW_OP_const8u:
			case DwarfConstants.DW_OP_const8s:
				stack.push(BigInteger.valueOf(buffer.getLong()));
				break;
			case DwarfConstants.DW_OP_constu:
				stack.push(BigInteger.valueOf(readUleb128(buffer)));
				break;
			case DwarfConstants.DW_OP_consts:
				stack.push(BigInteger.valueOf(readLeb128(buffer)));
				break;
			case DwarfConstants.DW_OP_pick:
				stack.push(stack.toArray(new BigInteger[0])[stack.size() - 1 - buffer.get() & 0xFF]);
				break;
			case DwarfConstants.DW_OP_abs:
				stack.push(stack.pop().abs());
				break;
			case DwarfConstants.DW_OP_or:
				b = stack.pop();
				a = stack.pop();
				stack.push(a.or(b));
				break;
			case DwarfConstants.DW_OP_plus_uconst:
				long uconst = readUleb128(buffer);
				stack.push(stack.pop().add(BigInteger.valueOf(uconst)));
				break;
			case DwarfConstants.DW_OP_push_object_address:
				BigInteger address = BigInteger.valueOf(readUleb128(buffer));
				stack.push(address);
				break;
			case DwarfConstants.DW_OP_implicit_value:
				long size = readUleb128(buffer);
				byte[] value = new byte[(int) size];
				buffer.get(value);
				BigInteger implicitValue = new BigInteger(value);
				stack.push(implicitValue);
				break;
			case DwarfConstants.DW_OP_stack_value:
				return stack.pop().longValue();
			case DwarfConstants.DW_OP_implicit_pointer:
				break;
			case DwarfConstants.DW_OP_constx:
//				long index = readUleb128(buffer);
				break;
			case DwarfConstants.DW_OP_entry_value:
//				long entry = readUleb128(buffer);
				break;
			case DwarfConstants.DW_OP_const_type:
//				long typeIndex = readUleb128(buffer);
				break;
			case DwarfConstants.DW_OP_regval_type:
//				long regIndex = readUleb128(buffer);
//				long typeIndex = readUleb128(buffer);
				break;
			case DwarfConstants.DW_OP_deref_type:
//				long typeIndex = readUleb128(buffer);
//				BigInteger address = stack.pop();
				break;
			case DwarfConstants.DW_OP_xderef_type:
//				long typeIndex = readUleb128(buffer);
//				BigInteger address = stack.pop();
				break;
			case DwarfConstants.DW_OP_convert:
//				long typeIndex = readUleb128(buffer);
//				BigInteger value = stack.pop();
				break;
			case DwarfConstants.DW_OP_reinterpret:
//				long typeIndex = readUleb128(buffer);
//				BigInteger value = stack.pop();
				break;
			default:
				int regNum;
				long off;
				if (opcode >= DwarfConstants.DW_OP_breg0 && opcode <= DwarfConstants.DW_OP_breg31) {
					regNum = opcode - DwarfConstants.DW_OP_breg0;
					off = readLeb128(buffer);
					BigInteger val = regs.get(client.getDwarfReg(regNum));
					stack.push(val.add(BigInteger.valueOf(off)));
					break;

				} else if (opcode >= DwarfConstants.DW_OP_reg0 && opcode <= DwarfConstants.DW_OP_reg31) {
					regNum = opcode - DwarfConstants.DW_OP_reg0;
					stack.push(regs.get(client.getDwarfReg(regNum)));
					break;

				} else if (opcode >= DwarfConstants.DW_OP_lit0 && opcode <= DwarfConstants.DW_OP_lit31) {
					int literal = opcode - DwarfConstants.DW_OP_lit0;
					stack.push(BigInteger.valueOf(literal));
					break;
				}

				if (function != null) {
					if (opcode == DwarfConstants.DW_OP_fbreg) {
						off = readLeb128(buffer);
						long frameBase = function.resolveFrameBase(client, regs);
						stack.push(BigInteger.valueOf(frameBase).add(BigInteger.valueOf(off)));
						break;
					} else {
						Platform.getLog(Operation.class).warn("Неизвестный опкод: " + opcode);
					}
				}
				break;
			}
			
		}

		if (stack.isEmpty()) {
			throw new IllegalStateException("Stack is empty after evaluating expression");
		}

		return stack.pop().longValue();
	}

}
