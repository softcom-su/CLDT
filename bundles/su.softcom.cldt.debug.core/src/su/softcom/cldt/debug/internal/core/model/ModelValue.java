package su.softcom.cldt.debug.internal.core.model;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

import org.eclipse.core.runtime.Platform;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IValue;
import org.eclipse.debug.core.model.IVariable;

import su.softcom.cldt.debug.core.model.events.FetchVariablesEvent;
import su.softcom.cldt.debug.internal.core.DebugTarget;
import su.softcom.cldt.debug.internal.core.model.ModelVariableType.MVType;

public class ModelValue extends DebugElement implements IValue {

	byte[] data;
	Object value;
	List<ModelVariable> children;
	ModelVariable variable;

	public ModelValue(IDebugTarget target, ModelVariable variable, Object value) {
		super(target);
		this.variable = variable;
		this.value = value;
		this.children = new ArrayList<>();
		if (variable.isHasChild()) {
			children.add(new ModelVariable(target));
		}
	}

	@Override
	public String getModelIdentifier() {
		return getDebugTarget().getModelIdentifier();
	}

	@Override
	public String getReferenceTypeName() throws DebugException {
		// TODO Auto-generated method stub
		return null;
	}

	public void setData(Object value) {
		if (value != null) {
			this.value = value;
		}
	}

	@Override
	public String getValueString() throws DebugException {
		String result = ""; //$NON-NLS-1$
		if (variable.isDirty() && getDebugTarget() instanceof DebugTarget debugTarget) {
			List<Long> path = getPathId(variable);
			if (path.size() > 1) {
				path.remove(path.size() - 1);
				debugTarget.addEvent(new FetchVariablesEvent(getStackFrameId(variable), path));
			}
		}

		if (hasVariables() || value != null) {
			result = getStringRepresentation();
		} else {
			result = (variable.getType() != null ? variable.getType().getName() : "") + "<0x"
					+ Long.toHexString(variable.getAddress()) + ">";
		}
		return result;
	}

	private String getStringRepresentation() throws DebugException {
		StringBuilder sb = getStringRepresentation(new StringBuilder(), variable.getType());
		addAddressStringRepresentation(sb);

		return sb.toString();
	}

	private StringBuilder addAddressStringRepresentation(StringBuilder sb) {
		switch (variable.getType().getType()) {
		case POINTER, ARRAY:
			sb.append(" <0x" + Long.toHexString(BigInteger.valueOf(variable.getAddress()).longValue()) + ">");
			break;
		default:
			break;
		}
		return sb;
	}

	private StringBuilder getStringRepresentation(StringBuilder sb, ModelVariableType type) {
		if (type == null) {
			return sb;
		}
		switch (type.getType()) {
		case FLOAT:
			return sb.append(value != null ? String.valueOf(value) : "float ");
		case INTEGER:
			return sb.append(value != null ? String.valueOf(value) : "int ");
		case CHARACTER:
			if (value != null) {
				if (value instanceof Byte byteValue) {
					return sb.append(charToStringRepresentation(byteValue.byteValue() & 0xFF));
				} else if (value instanceof Long longValue) {
					return sb.append(charToStringRepresentation((int) (longValue & 0xFF)));
				}
			}

			return sb.append("char ");
		case ARRAY:
			return getStringRepresentation(sb, type.getChild()).append("[]");
		case POINTER:
			if (type.getChild() != null) {
				return getStringRepresentation(sb, type.getChild()).append("*");
			} else {
				return sb.append("void *");
			}
		case STRUCTURE, UNION, CLASS, TEMPLATE:
			return sb.append(type.getName() != null ? type.getName() : "");
		case ENUMERATION:
			return sb.append("enum");
		case REFERENCE:
			return getStringRepresentation(sb, type.getChild()).append("&");
		case BOOLEAN:
			if (value != null) {
				String b = (Byte) value == 0 ? "false" : "true";
				return sb.append(b);
			} else {
				return sb.append("bool");
			}
		case EMPTY:
			return sb.append("void");
		default:
			return sb.append("");
		}
	}

	private String charToStringRepresentation(int code) {
		String charRepr = (code == 0) ? "NUL" : String.valueOf((char) code);
		return "%s (0x%s)".formatted(charRepr, Integer.toHexString(code));
	}

	@Override
	public boolean isAllocated() throws DebugException {
		// TODO Auto-generated method stub
		return false;
	}

	/**
	 * Добавить потомка переменной
	 * 
	 * @param variable
	 */
	public void addChildren(ModelVariable variable) {
		children.add(variable);
	}

	@Override
	public IVariable[] getVariables() throws DebugException {
		if (variable.isHasChild()) {
			if (children.size() == 1 && children.get(0).isPhantom()) {
				Platform.getLog(getClass()).info("Variable: " + variable.getName());
				if (getDebugTarget() instanceof DebugTarget debugTarget) {
					debugTarget.addEvent(new FetchVariablesEvent(getStackFrameId(variable), getPathId(variable)));
				}
			}

			if (children.size() > 1) {
				int i = 0;
				for (ModelVariable v : children) {
					if (v.isPhantom()) {
						children.remove(i);
						break;
					} else {
						i++;
					}
				}
			}

			return children.toArray(new IVariable[children.size()]);
		}
		return new IVariable[0];
	}

	private List<Long> getPathId(ModelVariable v) {
		LinkedList<Long> pathId = new LinkedList<>();
		for (ModelVariable modelVariable = v; modelVariable != null; modelVariable = (modelVariable
				.getParent() instanceof ModelVariable mVariable) ? mVariable : null) {
			pathId.addFirst(modelVariable.getId());
		}
		return pathId;
	}

	private long getStackFrameId(ModelVariable v) {
		for (ModelVariable modelVariable = v; modelVariable != null; modelVariable = (modelVariable
				.getParent() instanceof ModelVariable mVariable) ? mVariable : null) {
			if (modelVariable.getParent() == null) {
				return modelVariable.getStackFrameId();
			}
		}
		return 0L;
	}

	@Override
	public boolean hasVariables() throws DebugException {
		return variable.isHasChild();
	}

	// TODO: Может пригодиться позже
	String getChildName(int i) {
//		if (type instanceof ArrayType arrType) {
//			return "[%d]".formatted(Integer.valueOf(i)); //$NON-NLS-1$
//		} else {
//			return type.getChild().get(i).getName();
//		}
		return "Child [" + i + "]";
	}

	public List<ModelVariable> getChildrens() {
		return children;
	}

}
