package su.softcom.cldt.debug.ui.filters;

import org.eclipse.jface.viewers.Viewer;
import org.eclipse.jface.viewers.ViewerFilter;
import su.softcom.cldt.core.cmake.Target;

public class ExecutableFilter extends ViewerFilter {

	  @Override
	  public boolean select(Viewer viewer, Object parentElement, Object element) {
		  if(element instanceof Target target) {
			  return target.getType().equals(Target.Type.EXECUTABLE);
		  }
		  return false;
	} 
}