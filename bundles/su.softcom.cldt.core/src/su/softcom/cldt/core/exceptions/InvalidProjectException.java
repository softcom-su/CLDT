package su.softcom.cldt.core.exceptions;

import java.io.IOException;

/**
 * Specialized exception
 */
public class InvalidProjectException extends IOException {

	/**
	 * @param info exception info
	 */
	public InvalidProjectException(String info) {
		super(info);
	}

}
