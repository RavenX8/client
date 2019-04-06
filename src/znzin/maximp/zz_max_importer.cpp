/** 
 * @file zz_max_importer.h
 * @brief 3ds max importer base class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    05-apr-2003
 *
 * $Header: /engine/maximp/zz_max_importer.cpp 2     04-06-29 5:54p Zho $
 */

#include "zz_max_importer.h"
#include "zz_max_importer_mesh.h"

#define ZZ_MAX_IMPORTER_VERSION 102

// by /maxsdk/Help/gencid.exe
#define ZZ_CLASS_ID_MESH  Class_ID(0x12cd6411, 0x1c6a5d1e)

HINSTANCE hInstance;

TCHAR *GetString(int id)
{
	static TCHAR buf[256];
	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
	hInstance = hinstDLL;
	
	setWindowInstance( hinstDLL );

	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return(TRUE);
}

//------------------------------------------------------
class zz_class_desc_mesh : public ClassDesc {
public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading = FALSE) { return new zz_max_importer_mesh; }
	const TCHAR *	ClassName() { return GetString(IDS_CLASSNAME_MESH); }
	SClass_ID		SuperClassID() { return SCENE_IMPORT_CLASS_ID; }
	Class_ID		ClassID() { return Class_ID(1,0); }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY);  }
};

static zz_class_desc_mesh mesh_desc;

//------------------------------------------------------
// This is the interface to Jaguar:
//------------------------------------------------------

__declspec( dllexport ) const TCHAR *
LibDescription() { return GetString(IDS_LIBDESCRIPTION); }

__declspec( dllexport ) int
LibNumberClasses() { return 1; }

__declspec( dllexport ) ClassDesc *
LibClassDesc(int i) {
	switch(i) {
		case 0: return &mesh_desc; break;
		default: return 0; break;
	}
}

// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG 
LibVersion() { return VERSION_3DSMAX; }

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}

zz_max_importer::zz_max_importer()
{
	initZnzin();
}

zz_max_importer::~zz_max_importer()
{
	destZnzin();
}

int
zz_max_importer::ExtCount() {
	return 1;
}

const TCHAR *
zz_max_importer::Ext(int n) {		// Extensions supported for import/export modules
	switch(n) {
		case 0:
			return GetString(IDS_EXT_MESH);;
	}
	return _T("");
}


const TCHAR *
zz_max_importer::AuthorName() {			// ASCII Author name
	return GetString(IDS_AUTHORNAME);
}

const TCHAR *
zz_max_importer::CopyrightMessage() {	// ASCII Copyright message
	return GetString(IDS_COPYRIGHT);
}

const TCHAR *
zz_max_importer::OtherMessage1() {		// Other message #1
	return _T("");
}

const TCHAR *
zz_max_importer::OtherMessage2() {		// Other message #2
	return _T("");
}

unsigned int
zz_max_importer::Version() {				// Version number * 100 (i.e. v3.01 = 301)
	return ZZ_MAX_IMPORTER_VERSION;
}

void
zz_max_importer::ShowAbout(HWND hWnd) {			// Optional
}


