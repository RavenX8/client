/** 
 * @file zz_max_exporter_camera.cpp
 * @brief 3ds max exporter camera class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    26-jun-2002
 *
 * $Id: zz_max_exporter_camera.cpp,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#include "zz_max_exporter_camera.h"

zz_max_exporter_camera::zz_max_exporter_camera()
{
	get_string(IDS_LONGDESC_CAMERA, str_long_desc);
	get_string(IDS_SHORTDESC_CAMERA, str_short_desc);
}

zz_max_exporter_camera::~zz_max_exporter_camera()
{
}

const TCHAR * zz_max_exporter_camera::Ext(int n)
{
	return get_string(IDS_EXT_CAMERA);
}

// Dialog proc
static BOOL CALLBACK camera_export_dlg_proc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	zz_max_exporter_camera * exp = reinterpret_cast<zz_max_exporter_camera *>((LPARAM)GetWindowLong(hWnd, GWL_USERDATA));

	switch (msg) {
		case WM_INITDIALOG:
			exp = (zz_max_exporter_camera *)lParam;
			SetWindowLong(hWnd, GWL_USERDATA, LONG(lParam)); 
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					exp->set_asciioutput(IsDlgButtonChecked(hWnd, IDC_ASCIIOUTPUT) == TRUE);
					EndDialog(hWnd, 1);
					break;

				case IDCANCEL:
					EndDialog(hWnd, 0);
					break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}       

int zz_max_exporter_camera::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	// Grab the interface pointer.
	ip = i;

	// Popup Dialog Box
	// Prompt the user with our dialogbox, and get all the options.
	if (suppressPrompts == FALSE) {
		if (!DialogBoxParam(h_instance, MAKEINTRESOURCE(IDD_CAMERA_EXPORT_OPTION), ip->GetMAXHWnd(), camera_export_dlg_proc, (LPARAM)this)) {
			return 1;
		}
	}

	// Open the stream
	pstream = _tfopen(name,_T("wb"));
	if (!pstream) {
		popup_message("camera: Cannot open file!");
		return 0;
	}

	// Startup the progress bar.
	ip->ProgressStart(get_string(IDS_PROGRESS_MSG), TRUE, fn, NULL);

	int current_phase = 1;
	int total_phase = 6;
	export_camera_header(name, MAGIC_NUMBER_CAMERA);
	{
		float projTM[4][4], viewTM[4][4]; // You can use these in OpenGL later.
		float znear, zfar, zfovy, zaspect;
		bool persp;
		float eye[3], up[3], center[3];
		persp = get_camera_direct3d(projTM, viewTM, &zfovy, &zaspect, &znear, &zfar, eye, center, up);
		if (persp) dump_uint(1, "perspective");
		else dump_uint(0, "orthogonal");
		dump_comment("viewTM\r\n");
		dump_matrix(viewTM);
		dump_comment("projTM\r\n");
		dump_matrix(projTM);
		dump_float(zfovy, "fovy");
		dump_float(zaspect, "aspect_ratio");
		dump_float(znear, "near_plane");
		dump_float(zfar, "far_plane");
		dump_float3(eye[0], eye[1], eye[2], "eye");
		dump_float3(center[0], center[1], center[2], "center");
		dump_float3(up[0], up[1], up[2], "up");
	}
	ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

	// We're done. Finish the progress bar.
	ip->ProgressEnd();

	// Close the stream
	fclose(pstream);

	return 1;
}

// export header section
void zz_max_exporter_camera::export_camera_header(const TCHAR *name, const TCHAR * magic_number)
{
	dump_string(magic_number, (int)strlen(magic_number), "magic_number");
	//dump_nullstring(get_filename(name), "file_id");
}
