package su.softcom.cldt.debug.core;

/**
 * @author Petr Babanov (petr.babanov@softcom.su)
 *
 */
public interface IModelProvider {
	
	/**
	 * @return {@link su.softcom.cldt.debug.core.IStackModel IStackModel} for this emulator
	 */
	public IStackModel getStackModel();
	/** 
	 * @return {@link su.softcom.cldt.debug.core.IRegisterModel IRegisterModel} for this emulator
	 */
	public IRegisterModel getRegisterModel();

}
