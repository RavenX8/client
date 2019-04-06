/** 
 * @file zz_max_importer_mesh.h
 * @brief 3ds max importer base class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    05-apr-2003
 *
 * $Header: /ENGINE/maximp/zz_max_importer_mesh.h 1     03-11-30 8:21p Zho $
 */

#ifndef __ZZ_MAX_IMPORTER_MESH_H__
#define __ZZ_MAX_IMPORTER_MESH_H__

#include "zz_max_importer.h"

class zz_max_importer_mesh : public zz_max_importer {
	Mesh * max_mesh;
	zz_mesh * engine_mesh;

	bool mesh_file_load (const TCHAR* filename, Mesh * mesh);

public:
	zz_max_importer_mesh();
	~zz_max_importer_mesh();

	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")

	int DoImport (const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	
};

#endif // __ZZ_MAX_IMPORTER_MESH_H__