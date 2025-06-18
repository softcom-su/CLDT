package su.softcom.cldt.debug.core;

import java.nio.charset.Charset;
import java.util.HexFormat;

import org.eclipse.core.runtime.Platform;

public class Utils {

	/**
	 * Removes leading zeros in a string
	 * 
	 * @param str
	 * @return str with removed leading zeros in a input str
	 */
	public static String removeLeadZero(String str) {
		int i = 0;
		while (i < str.length() && str.charAt(i) == '0') {
			i++;
		}
		StringBuffer sb = new StringBuffer(str);
		sb.replace(0, i, "");
		return sb.toString();
	}

	/**
	 * Encode String to Hex String
	 * 
	 * @param text
	 * @return hexStr
	 */
	public static String strToHex(String text) {
		return HexFormat.of().formatHex(text.getBytes());
	}

	/**
	 * Method to calculate checksum from bytes
	 * 
	 * @param bytes
	 * @return checksum
	 */
	public static byte getChecksum(byte[] bytes) {
		int tmp = 0;
		for (byte c : bytes) {
			tmp += (char) c;
		}
		return (byte) (((byte) tmp) & 255);
	}

	/**
	 * Конвертирует Hex числа в ASCII
	 * 
	 * @param arg
	 * @return asciiStr
	 */
	public static String hexToAscii(String arg) {
		StringBuilder str = new StringBuilder();
		for (int i = 0; i < arg.length(); i += 2) {
			String s = arg.substring(i, (i + 2));
			int decimal = Integer.parseInt(s, 16);
			str.append((char) decimal);
		}
		return str.toString();
	}

	/**
	 * Конвертирует Hex числа в UTF_8
	 * 
	 * @param hex     строка для конвертации
	 * @param charset используемый
	 * @return UTFstr
	 */
	public static String hexToCharset(String hex, Charset charset) {
		byte[] b = new byte[hex.length() / 2];
		for (int i = 0; i < b.length; i++) {
			try {
				b[i] = (byte) Integer.parseInt(hex, i * 2, i * 2 + 2, 16);
			} catch (NumberFormatException e) {
				Platform.getLog(Utils.class).error("При конвертации hex строки произошла ошибка");
			}
		}
		return new String(b, charset);
	}

}
