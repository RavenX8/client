/** 
 * @file zz_max_exporter_camera.h
 * @brief 3ds max exporter camera class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    26-jun-2002
 *
 * $Id: zz_max_exporter_camera.h,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#ifndef __ZZ_MAX_EXPORTER_CAMERA_H__
#define __ZZ_MAX_EXPORTER_CAMERA_H__

#include "zz_max_exporter.h"

#define MAGIC_NUMBER_CAMERA "ZCA0001"

class zz_max_exporter_camera : public zz_max_exporter {
public:
	zz_max_exporter_camera();
	~zz_max_exporter_camera();

	// from SceneExport class
	const TCHAR * Ext(int n);
	int DoExport (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
	void export_camera_header (const TCHAR *name, const TCHAR * magic_number);
};

#endif // __ZZ_MAX_EXPORTER_CAMERA_H__