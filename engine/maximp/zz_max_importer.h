/** 
 * @file zz_max_import.h
 * @brief 3ds max importer base class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    05-apr-2003
 *
 * $Header: /ENGINE/maximp/zz_max_importer.h 1     03-11-30 8:21p Zho $
 */

#ifndef __ZZ_MAX_IMPORTER_H__
#define __ZZ_MAX_IMPORTER_H__

#include "Max.h"

#include "resource.h"

#include "zz_system.h"
#include "zz_mesh_tool.h"
#include "zz_interface.h"

#pragma comment (lib, "core.lib")
#pragma comment (lib, "CustDlg.lib")
#pragma comment (lib, "mesh.lib")
#pragma comment (lib, "maxutil.lib")
#pragma comment (lib, "geom.lib")

#ifdef _DEBUG
#pragma comment (lib, "znzin_static_d.lib")
#else
#pragma comment (lib, "znzin_static.lib")
#endif

class zz_max_importer : public SceneImport {
public:
	zz_max_importer();
	~zz_max_importer();
	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	//const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	//const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	//int				DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	// Import file
};

#endif // __ZZ_MAX_IMPORTER_H__