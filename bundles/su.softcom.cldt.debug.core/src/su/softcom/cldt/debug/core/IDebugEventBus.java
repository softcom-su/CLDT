package su.softcom.cldt.debug.core;

public interface IDebugEventBus {

	public void setDebugger(IEventListener listener);
	
	public void setModel(IEventListener model);
	
	public void addEvent(IEvent event);
	
	public void start();
	
	public void stop();
}
