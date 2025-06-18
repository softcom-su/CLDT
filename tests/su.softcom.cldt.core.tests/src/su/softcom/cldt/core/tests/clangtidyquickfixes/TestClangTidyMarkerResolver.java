package su.softcom.cldt.core.tests.clangtidyquickfixes;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.when;

import org.eclipse.core.resources.IMarker;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.ui.IMarkerResolution;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import su.softcom.cldt.ui.resolvers.ClangTidyMarkerResolver;
import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyAutoQuickFix;
import su.softcom.cldt.ui.resolvers.quickfixes.ClangTidyRenameQuickFix;

public class TestClangTidyMarkerResolver {
    
	private AutoCloseable closeable;
	private String clangTidyMarker = "su.softcom.cldt.core.marker.clangTidyMarker"; //$NON-NLS-1$
	private String checkIdAttribute = "checkId"; //$NON-NLS-1$
	
    @Mock
    private IMarker marker;
    
    @BeforeEach
    public void setUp() {
        closeable = MockitoAnnotations.openMocks(this);
    }
    
    @AfterEach
    public void tearDown() throws Exception {
        closeable.close();
    }
    
    @Test
    public void testAutoQuickFixGeneration() throws CoreException {
        when(marker.getType()).thenReturn(clangTidyMarker);
        when(marker.getAttribute(checkIdAttribute, "")).thenReturn("modernize-use-auto"); //$NON-NLS-1$ //$NON-NLS-2$
        
        IMarkerResolution[] resolutions = new ClangTidyMarkerResolver().getResolutions(marker);
        
        assertEquals(1, resolutions.length);
        assertTrue(resolutions[0].getClass() == ClangTidyAutoQuickFix.class);
    }
    
    @Test
    public void testRenameQuickFixGeneration() throws CoreException {
        when(marker.getType()).thenReturn(clangTidyMarker);
        when(marker.getAttribute(checkIdAttribute, "")).thenReturn("misc-confusable-identifiers"); //$NON-NLS-1$ //$NON-NLS-2$
        
        IMarkerResolution[] resolutions = new ClangTidyMarkerResolver().getResolutions(marker);
        
        assertEquals(1, resolutions.length);
        assertTrue(resolutions[0].getClass() == ClangTidyRenameQuickFix.class);
    }
    
    @Test
    public void testDoesNotGenerateQuickFixForUnsuitableCheck() throws CoreException {
        when(marker.getType()).thenReturn(clangTidyMarker);
        when(marker.getAttribute(checkIdAttribute, "")).thenReturn("abseil-no-namespace"); //$NON-NLS-1$ //$NON-NLS-2$
        
        IMarkerResolution[] resolutions = new ClangTidyMarkerResolver().getResolutions(marker);
        
        assertEquals(0, resolutions.length);
    }
    
    @Test
    public void testDoesNotGenerateQuickFixForUnsuitableMarker() throws CoreException {
        when(marker.getType()).thenReturn("notClangTidyMarkerId"); //$NON-NLS-1$
        
        IMarkerResolution[] resolutions = new ClangTidyMarkerResolver().getResolutions(marker);
        
        assertEquals(0, resolutions.length);
    }
}