/*=============================================================================
	C++ class definitions exported from script.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .uc files instead!
=============================================================================*/


#ifndef __EDITORCLASSES_H__
#define __EDITORCLASSES_H__

/*-----------------------------------------------------------------------------
	Forward declarations
-----------------------------------------------------------------------------*/

class CAppSettings;


/*-----------------------------------------------------------------------------
	CAppSettings class (declared in AppSettings.uc)
-----------------------------------------------------------------------------*/


/**
 * Window positions
 */
struct CWndPos
{
	int							x;
	int							y;
	int							w;
	int							h;
	bool						maximized;
};


class CAppSettings
{
	DECLARE_BASE_CLASS(CAppSettings)
public:
	/**
	 * Frame splitter positions
	 */
	int							Splitter1;
	int							Splitter2;
	CWndPos						MainFramePos;
	CWndPos						LogFramePos;
	CWndPos						SettingsFramePos;
	/**
	 * PropEditor's splitter positions
	 */
	int							MeshPropSplitter;
	int							AnimSetPropSplitter;
	int							AnimSeqPropSplitter;
	int							PrefsPropSplitter;
	/**
	 * Base directory with game resources.
	 * Should be set for correct editor functionality.
	 * Changes in this value will require to reload mesh.
	 */
	TString<256>				ResourceRoot;
	/**
	 * Directory, containing exported from modeling program PSK and PSA files.
	 */
	TString<256>				ImportDirectory;
	/**
	 * Directory, containing engine animation resources.
	 * Used by 'open mesh' etc dialog.
	 */
	TString<256>				AnimDataDirectory;
	/**
	 * Background colour of mesh viewer/editor
	 */
	CColor3f					MeshBackground;
	/**
	 * Gradient color
	 */
	CColor3f					MeshBackground2;
	/**
	 * Enable gradient for background
	 */
	bool						EnableGradient;
};


/*-----------------------------------------------------------------------------
	Class registration
-----------------------------------------------------------------------------*/

#define REGISTER_EDITOR_CLASSES \
	REGISTER_CLASS(CAppSettings)


#endif // __EDITORCLASSES_H__
