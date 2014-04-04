//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class governs all the XLib usage and interactions
//  and provides simpler Qt-based functions for use elsewhere
//===========================================
#ifndef _LUMINA_LIBRARY_X11_H
#define _LUMINA_LIBRARY_X11_H

//Qt includes
#include <QList>
#include <QString>
#include <QPixmap>
#include <QX11Info>
#include <QDebug>
// Addition includes for compilations (cause issues with X11 libs later)
#include <QDir>
#include <QEvent>


//X includes (these need to be last due to Qt compile issues)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrender.h>

//SYSTEM TRAY STANDARD DEFINITIONS
#define _NET_SYSTEM_TRAY_ORIENTATION_HORZ 0
#define _NET_SYSTEM_TRAY_ORIENTATION_VERT 1
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2

class LX11{
public:
	enum WINDOWSTATE {VISIBLE, INVISIBLE, ACTIVE, ATTENTION, IGNORE};
	
	//General Info Functions
	static QList<WId> WindowList(); //List all current windows
	static QList<WId> findChildren(Window, int); //Recursive utility for getting all children of a window
	static WId ActiveWindow();				//List the currently-active window
	
	//Single Window Management
	static void CloseWindow(WId);			// Close the Window
	static void IconifyWindow(WId);			// Iconify (minimize) the Window
	static void RestoreWindow(WId);		//  Restore (non-iconify) the Window 
	
	//Single Window Information
	static QString WindowName(WId); 		// long name (untranslated)
	static QString WindowVisibleName(WId); 	// long name (translated)
	static QString WindowIconName(WId); 	// short name (untranslated)
	static QString WindowVisibleIconName(WId); // short name (translated)
	static QPixmap WindowPixmap(WId);		// Pixmap/icon for the window
	static WINDOWSTATE GetWindowState(WId win, bool forDisplay=true); //State of activity
	
	//System Tray Management
	static WId startSystemTray(); //Startup the system tray
	static void closeSystemTray(WId); //Close the system tray
	
	
	//EWMH Convenience functions
	static QString getNetWMProp(WId win, QString prop); //Returns a _NET_WM_* string value
};

#endif