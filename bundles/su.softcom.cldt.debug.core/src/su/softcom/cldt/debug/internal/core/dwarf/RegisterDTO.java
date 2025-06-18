package su.softcom.cldt.debug.internal.core.dwarf;

public class RegisterDTO {
	
	private String name;
	private int bitsize;
	private int regNum;

	public RegisterDTO(String name, int bitsize, int regNum) {
		this.name = name;
		this.bitsize = bitsize;
		this.regNum = regNum;
	}

	public int getBitsize() {
		return bitsize;
	}

	public String getName() {
		return name;
	}

	public int getRegNum() {
		return regNum;
	}

}
