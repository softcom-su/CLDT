package su.softcom.cldt.debug.core;


public interface IGdbCommand {
	
	/**
	 * 
	 * @param result Принимает байты ответа от сервера для дальнейшей их обработки
	 */
	void handleResult(byte[] result);
	
	/**
	 * 
	 * @return Строковое представление команды
	 */
	String getCommandString();
	
}
