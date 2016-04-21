/** 
* @file zz_max_exporter.cpp
* @brief 3ds max exporter base class for znzin.
* @author Jiho Choi (zho@korea.com)
* @version 1.0
* @date    26-jun-2002
*
* $Id: zz_max_exporter.cpp,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
*/

#include "zz_max_exporter.h"
#include "zz_max_exporter_model.h"
#include "zz_max_exporter_motion.h"
#include "zz_max_exporter_camera.h"
#include "zz_max_exporter_mesh.h"

// for resettm
#include "simpmod.h"
// <from mod.h>
#define CLUSTOSM_CLASS_ID			0x25215824
// </from mod.h>

#include <list>
#include <map>
#include <string>

using namespace std;

#define ZZ_MAX_EXPORTER_VERSION 100

HINSTANCE h_instance;
static BOOL export_selected;
Interface * zz_max_exporter::ip = NULL;
FILE * zz_max_exporter::pstream = NULL;
bool zz_max_exporter::is_asciioutput = false;

#define ZZ_CLASS_ID_MESH  Class_ID(0x66bd6c40, 0x315029c9)
#define ZZ_CLASS_ID_MODEL  Class_ID(0x4b8e4a71, 0x6025e52)
#define ZZ_CLASS_ID_MOTION Class_ID(0x16c007cc, 0x43614f22)
#define ZZ_CLASS_ID_CAMERA Class_ID(0x77111af3, 0x2e701dfe)

#define ZZ_CLASS_ID_EDIT_NORMAL Class_ID(0x4aa52ae3, 0x35ca1cde) // is it true on 3dsmax6

TCHAR * get_string (int id, TCHAR * buf)
{
	if (h_instance) {
		static TCHAR buf_internal[MAX_STRING_SIZE];
		if (buf) {
			return LoadString(h_instance, id, buf, MAX_STRING_SIZE) ? buf : NULL;
		}
		else {
			return LoadString(h_instance, id, buf_internal, sizeof(buf_internal)) ? buf_internal : NULL;
		}
	}
	return NULL;
}

// zz_class_desc_mesh
class zz_class_desc_mesh : public ClassDesc {
	TCHAR str_classname[MAX_STRING_SIZE];
	TCHAR str_category[MAX_STRING_SIZE];

public:
	int	IsPublic() { return 1; }
	void* Create(BOOL loading = FALSE) { return new zz_max_exporter_mesh; } 
	const TCHAR* ClassName() { return get_string(IDS_CLASSNAME_MESH, str_classname); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID ClassID() { return ZZ_CLASS_ID_MESH; }
	const TCHAR* Category() { return get_string(IDS_CATEGORY, str_category); }
};
static zz_class_desc_mesh mesh_desc;

// zz_class_desc_model
class zz_class_desc_model : public ClassDesc {
	TCHAR str_classname[MAX_STRING_SIZE];
	TCHAR str_category[MAX_STRING_SIZE];

public:
	int	IsPublic() { return 1; }
	void* Create(BOOL loading = FALSE) { return new zz_max_exporter_model; } 
	const TCHAR* ClassName() { return get_string(IDS_CLASSNAME_MODEL, str_classname); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID ClassID() { return ZZ_CLASS_ID_MODEL; }
	const TCHAR* Category() { return get_string(IDS_CATEGORY, str_category); }
};
static zz_class_desc_model model_desc;

// zz_class_desc_motion
class zz_class_desc_motion : public ClassDesc {
	TCHAR str_classname[MAX_STRING_SIZE];
	TCHAR str_category[MAX_STRING_SIZE];
public:
	int	IsPublic() { return 1; }
	void* Create(BOOL loading = FALSE) { return new zz_max_exporter_motion; } 
	const TCHAR* ClassName() { return get_string(IDS_CLASSNAME_MOTION, str_classname); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID ClassID() { return ZZ_CLASS_ID_MOTION; }
	const TCHAR* Category() { return get_string(IDS_CATEGORY, str_category); }
};
static zz_class_desc_motion motion_desc;

// zz_class_desc_camera
class zz_class_desc_camera : public ClassDesc {
	TCHAR str_classname[MAX_STRING_SIZE];
	TCHAR str_category[MAX_STRING_SIZE];
public:
	int	IsPublic() { return 1; }
	void* Create(BOOL loading = FALSE) { return new zz_max_exporter_camera; } 
	const TCHAR* ClassName() { return get_string(IDS_CLASSNAME_CAMERA, str_classname); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID ClassID() { return ZZ_CLASS_ID_CAMERA; }
	const TCHAR* Category() { return get_string(IDS_CATEGORY, str_category); }
};
static zz_class_desc_camera camera_desc;

BOOL WINAPI DllMain (HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) 
{
	static int controls_init = FALSE;
	h_instance = hinstDLL;

	setWindowInstance( hinstDLL );

	// Initialize the custom controls. This should be done only once.
	if (!controls_init) {
		controls_init = TRUE;
		InitCustomControls(h_instance);
		InitCommonControls();
	}

	return (TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription() 
{
	return get_string(IDS_LIBDESCRIPTION);
}

/// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS 
__declspec( dllexport ) int LibNumberClasses() 
{
	return 4; // zms, zmd, zmo, zca
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) {
case 0: return &model_desc;
case 1: return &motion_desc;
case 2: return &camera_desc;
case 3: return &mesh_desc;
	}
	return &model_desc; // default;
}

__declspec( dllexport ) ULONG LibVersion() 
{
	return VERSION_3DSMAX;
}

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}

zz_max_exporter::zz_max_exporter()
{
	// These are the default values that will be active when 
	// the exporter is ran the first time.
	// After the first session these options are sticky.
	ip = NULL;
	pstream = NULL;
	is_asciioutput = false;
	biped_interface = NULL;
	pcontrol = NULL;
	total_phase = current_phase = 0;
	number_of_selected_node = 0;
	max_mesh = NULL;
	tri = NULL;
	root_bone = NULL;

	initZnzin(); 

	engine_mesh = zz_new zz_mesh(false);
}

zz_max_exporter::~zz_max_exporter()
{
	zz_delete engine_mesh;

	destZnzin();
}

int zz_max_exporter::ExtCount()
{
	return 1;
}

const TCHAR * zz_max_exporter::LongDesc()
{
	return str_long_desc;
}

const TCHAR * zz_max_exporter::ShortDesc()
{
	return str_short_desc;
}

const TCHAR * zz_max_exporter::AuthorName() 
{
	return get_string(IDS_AUTHORNAME);
}

const TCHAR * zz_max_exporter::CopyrightMessage() 
{
	return get_string(IDS_COPYRIGHT);
}

const TCHAR * zz_max_exporter::OtherMessage1() 
{
	return _T("");
}

const TCHAR * zz_max_exporter::OtherMessage2() 
{
	return _T("");
}

unsigned int zz_max_exporter::Version()
{
	return ZZ_MAX_EXPORTER_VERSION;
}
 
static BOOL CALLBACK about_box_dialog_procedure (HWND hWnd, UINT msg, 
												 WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
case WM_INITDIALOG:
	CenterWindow(hWnd, GetParent(hWnd)); 
	break;
case WM_COMMAND:
	switch (LOWORD(wParam)) {
case IDOK:
	EndDialog(hWnd, 1);
	break;
	}
	break;
default:
	return FALSE;
	}
	return TRUE;
}       

void zz_max_exporter::ShowAbout(HWND hWnd)
{
	DialogBoxParam(h_instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, about_box_dialog_procedure, 0);
}

BOOL zz_max_exporter::SupportsOptions(int ext, DWORD options)
{
	assert(ext == 0);	// We only support one extension
	return (options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}

void zz_max_exporter::set_asciioutput (bool true_if_ascii)
{
	is_asciioutput = (true_if_ascii) ? true : false;
}

bool zz_max_exporter::get_asciioutput (void)
{
	return is_asciioutput;
}

void zz_max_exporter::dump_uint (unsigned int ui, const TCHAR * msg)
{
	if (is_asciioutput)	fprintf(pstream, "%s %d\r\n", msg, ui);
	else fwrite(&ui, sizeof(unsigned int), 1, pstream);
}

void zz_max_exporter::dump_float (float f, const TCHAR * msg)
{
	if (is_asciioutput)	fprintf(pstream, "%s %f\r\n", msg, f);
	else fwrite( &f, sizeof(float), 1, pstream );
}

void zz_max_exporter::dump_float3 (float f1, float f2, float f3, const TCHAR * msg)
{
	if (is_asciioutput) fprintf(pstream, "%s %f %f %f\r\n", msg, f1, f2, f3);
	else {
		fwrite(&f1, sizeof(float), 1, pstream);
		fwrite(&f2, sizeof(float), 1, pstream);
		fwrite(&f3, sizeof(float), 1, pstream);
	}
}

void zz_max_exporter::dump_quaternion (Quat q, const TCHAR * msg)
{
	q.Invert();
	if (is_asciioutput) fprintf(pstream, " %s %f %f %f %f\r\n", msg, q.w, q.x, q.y, q.z);
	else {
		fwrite(&q.w, sizeof(float), 1, pstream);
		fwrite(&q.x, sizeof(float), 1, pstream);
		fwrite(&q.y, sizeof(float), 1, pstream);
		fwrite(&q.z, sizeof(float), 1, pstream);
	}
}

void zz_max_exporter::dump_float4 (float f1, float f2, float f3, float f4, const TCHAR * msg)
{
	if (is_asciioutput) fprintf(pstream, " %s %f %f %f %f\r\n", msg, f1, f2, f3, f4);
	else {
		fwrite(&f1, sizeof(float), 1, pstream);
		fwrite(&f2, sizeof(float), 1, pstream);
		fwrite(&f3, sizeof(float), 1, pstream);
		fwrite(&f4, sizeof(float), 1, pstream);
	}
}

void zz_max_exporter::dump_string (const TCHAR * str, int size, const TCHAR * msg)
{
	if (!str) return;
	if (size == 0) size = (int)strlen(str);

	if (is_asciioutput)	fprintf(pstream, " %s %s\r\n", msg, str);
	else fwrite(str, sizeof(TCHAR), size, pstream);
}

void zz_max_exporter::dump_nullstring (const TCHAR * str, const TCHAR * msg)
{
	int size = (int)strlen(str);

	if (is_asciioutput) fprintf( pstream, "%s %s\r\n", msg, str);
	else fwrite(str, sizeof(TCHAR), size+1, pstream);
}

void zz_max_exporter::dump_comment (const TCHAR * format, ...)
{
	if (!is_asciioutput) return;
	// fprintf(pstream, " %s\r\n", str);
	
	va_list va;
	va_start(va, format);
	
	if (pstream) {
		vfprintf(pstream, format, va);
		fflush(pstream);
	}

	va_end(va);
}

// dump transformation matrix 4x4 opengl format
void zz_max_exporter::dump_matrix(float tm[4][4])
{
	Point3 row;

	for (int i=0; i < 4; i++) {
		dump_float4(tm[i][0], tm[i][1], tm[i][2], tm[i][3]);
	}
}

// dump transformation matrix 4x3 max format
void zz_max_exporter::dump_matrix_transpose(float tm[4][4])
{
	Point3 row;

	for (int i=0; i < 4; i++) {
		(tm[0][i], tm[1][i], tm[2][i], tm[3][i]);
	}
}

// it's comment only
void zz_max_exporter::dump_matrix (Matrix3 m)
{
	Point3 row;
	for (int r = 0; r < 4; r++) {
		row = m.GetRow(r);
		dump_comment("%f %f %f ",row.x, row.y, row.z);
	}
	dump_comment("\r\n");
}

bool zz_max_exporter::popup_message (const TCHAR * msg, bool okcancel)
{
	if (okcancel) {
		return (MessageBox(NULL, msg, "znzin", MB_OKCANCEL) == IDOK);
	}
	MessageBox(NULL, msg, "znzin", MB_OK);
	return true;
}

void zz_max_exporter::remove_nonuniform_scale(INode * bone)
{
	if (!bone) 
		return;

	// Get the node's transform control
	pcontrol = bone->GetTMController();

	if (!pcontrol) return;

	// You can test whether or not this is a biped controller with the following pseudo code:
	if ((pcontrol->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
		(pcontrol->ClassID() == BIPBODY_CONTROL_CLASS_ID) || // center of mass (biped)
		(pcontrol->ClassID() == FOOTPRINT_CLASS_ID)) {
			// Get the Biped Export Interface from the controller 
			biped_interface = (IBipedExport *) pcontrol->GetInterface(I_BIPINTERFACE);
			// Remove the non uniform scale
			biped_interface->RemoveNonUniformScale(1);
			// Begin or End figure mode
			// Release the interface
			pcontrol->ReleaseInterface(I_BIPINTERFACE,biped_interface);
		}
}

void zz_max_exporter::figure_mode_begin (INode * bone)
{
	if (!bone) 
		return;

	// Get the node's transform control
	pcontrol = bone->GetTMController();

	if (!pcontrol) return;

	// You can test whether or not this is a biped controller with the following pseudo code:
	if ((pcontrol->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
		(pcontrol->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
		(pcontrol->ClassID() == FOOTPRINT_CLASS_ID)) {
			// Get the Biped Export Interface from the controller 
			biped_interface = (IBipedExport *) pcontrol->GetInterface(I_BIPINTERFACE);
			// Remove the non uniform scale
			biped_interface->RemoveNonUniformScale(1);
			dump_comment("Non-UniformScaled.\r\n");
			// Begin or End figure mode
			biped_interface->BeginFigureMode(0);
		}
}

void zz_max_exporter::figure_mode_end ()
{
	if (biped_interface) {
		biped_interface->EndFigureMode(0);
		if (pcontrol) {
			// Release the interface
			pcontrol->ReleaseInterface(I_BIPINTERFACE,biped_interface);
		}
	}
}

// extract just filename from full path filename
const TCHAR * zz_max_exporter::get_filename (const TCHAR * fullname)
{
	int i, start_point = 0, length = 0;

	for (i = ((int)strlen(fullname)-2); i >= 0; i--) {
		if ((*(fullname + i)) == '\\') {
			return (const TCHAR *)(fullname + i + 1);
		}
	}
	return fullname;
}

// Dummy function for progress bar
DWORD WINAPI fn (LPVOID arg)
{
	return(0);
}

TimeValue zz_max_exporter::get_time_by_frame (int frame)
{
	return start_time + frame * GetTicksPerFrame();
}

// dump sample position data
void zz_max_exporter::dump_sample_pos (INode* node, int i)
{
	if (!node) return;

	Matrix3 localTM, parentWTM, nodeWTM;
	Point3 pos = node->GetNodeTM(get_time_by_frame(i)).GetTrans();

	dump_float3(pos.x, pos.y, pos.z, "translation");
}

Point3 zz_max_exporter::get_world_position(INode* node, int frame)
{
	return node->GetNodeTM(get_time_by_frame(frame)).GetTrans();
}

Quat zz_max_exporter::get_world_rotation(INode* node, int frame)
{
	TimeValue t = get_time_by_frame(frame);
	Quat q = node->GetNodeTM(t);
	return q;
}

Quat zz_max_exporter::get_local_rotation(INode* node, int frame)
{
	Quat q;
	Matrix3 localTM, parentWTM, nodeWTM;
	
	TimeValue t = get_time_by_frame(frame);
	
	nodeWTM = node->GetNodeTM(t);
	parentWTM = node->GetParentNode()->GetNodeTM(t);
	nodeWTM.NoScale();
	parentWTM.NoScale();
	localTM = nodeWTM * Inverse(parentWTM);

	//AffineParts ap;
	//decomp_affine(localTM, &ap);	
	//q = ap.q;
	q = localTM;
	return q;
}

// dump sample quaternion rotation data
void zz_max_exporter::dump_sample_quat (INode* node, int i)
{
	if (!node) return;

	Matrix3 localTM, parentWTM, nodeWTM;
	
	TimeValue t = get_time_by_frame(i);
	Quat q;

	if (node == root_bone) { // if pelvis use TM for world
		q = node->GetNodeTM(t);
	}
	else {
		nodeWTM = node->GetNodeTM(t);
		parentWTM = node->GetParentNode()->GetNodeTM(t);
		nodeWTM.NoScale();
		parentWTM.NoScale();
		localTM = nodeWTM * Inverse(parentWTM);
		
		//AffineParts ap;
		//decomp_affine(localTM, &ap);
		//q = ap.q;
		q = localTM;
	}
	dump_quaternion(q, "quaternion");
}

//// dump sample euler rotation data
//void zz_max_exporter::dump_sample_euler (INode* node, int i)
//{
//	if (!node) return;
//	Matrix3 localTM, parentWTM, nodeWTM;
//
//	TimeValue t = i * GetTicksPerFrame();
//	nodeWTM = node->GetNodeTM(t);
//	parentWTM = node->GetParentNode()->GetNodeTM(t);
//	localTM = nodeWTM * Inverse(parentWTM);
//
//	float euler[3];
//
//	MatrixToEuler(localTM, euler, EULERTYPE_XYZ);
//
//	dump_float(euler[0], "x");
//	dump_float(euler[1], "y");
//	dump_float(euler[2], "z");
//}
//
//// dump sample axis-angle rotation data
//void zz_max_exporter::dump_sample_axis_angle (INode* node, int i)
//{
//	if (!node) return;
//
//	Matrix3 localTM, parentWTM, nodeWTM;
//
//	TimeValue t = i * GetTicksPerFrame();
//	nodeWTM = node->GetNodeTM(t);
//	parentWTM = node->GetParentNode()->GetNodeTM(t);
//	localTM = nodeWTM * Inverse(parentWTM);
//
//	AngAxis aaxis(localTM);
//
//	dump_float(aaxis.angle, "angle");
//	dump_float(aaxis.axis.x, "axis_x");
//	dump_float(aaxis.axis.y, "axis_y");
//	dump_float(aaxis.axis.z, "axis_z");
//}

// dump sample camera
void zz_max_exporter::write_sample_camera (INode * node, CameraObject * camera, int i)
{
	if (!camera) return;
	
	TimeValue t = get_time_by_frame(i);

	ip->SetTime(t, FALSE);
	
	float eye[3], center[3], up[3];
	float projTM[4][4], viewTM[4][4];
	float fovy, aspect, near_plane, far_plane;

	get_camera_direct3d(projTM, viewTM, &fovy, &aspect, &near_plane, &far_plane, eye, center, up);

	script_file.write_float3("eye", eye[0], eye[1], eye[2]);
	script_file.write_float3("center", center[0], center[1], center[2]);
	script_file.write_float3("up", up[0], up[1], up[2]);
	script_file.write_float3("fovy-near-far", fovy, near_plane, far_plane);
}

// return false if orthogonal, true if perspective
bool zz_max_exporter::get_camera_opengl (
	float projTM[4][4],
	float viewTM[4][4],
	float * pfovy,
	float * paspect,
	float * pnear,
	float * pfar,
	float eye[3],
	float center[3],
	float up[3]
	)
{
	float mat[4][4]; // The transformation matrix times the projection matrix. 
	Matrix3 invTM; // This is the inverse of the affine part of the camera transformation matrix (not the inverse of the projection part). 
	int persp; // nonzero indicates this is a perspective view; 0 is orthogonal. 
	float near_plane; // near clipping plane
	float far_plane; // far clipping plane

	ViewExp * pView = ip->GetActiveViewport();  // Get the viewport in question
	GraphicsWindow *gw = pView->getGW();  // Get the GraphicsWindow context

	gw->getCameraMatrix(mat, &invTM, &persp, &near_plane, &far_plane); // getting these values to work with ... see above for their types

	int ki, kj;
	Matrix3 camTM(invTM);
	camTM.Invert();
	MRow * pcvm = camTM.GetAddr();
	for (ki = 0; ki < 4; ki++) {
		for (kj = 0; kj < 3; kj++) {
			viewTM[ki][kj] = pcvm[ki][kj];
		}
	}
	viewTM[0][3] = viewTM[1][3] = viewTM[2][3] = 0.0f;
	viewTM[3][3] = 1.0f;

	// copy perspTM from mat
	MRow *pa = invTM.GetAddr();
	for (ki = 0; ki < 4; ki++) {
		float val = (float)(ki==3);
		for (kj = 0; kj < 4; kj++) {
			projTM[ki][kj] = pa[ki][0] * mat[0][kj] +
				pa[ki][1] * mat[1][kj] +
				pa[ki][2] * mat[2][kj] +
				val		 * mat[3][kj];
		}
	}

	//// In OpenGL, 3ds max did it already. It's just the same thing.
	//if (persp == 0) { // orthogonal
	//	projTM[2][2] = -2 / (far_plane - near_plane);
	//	projTM[3][3] = 1.0;
	//	projTM[2][3] = 0.0f;
	//	projTM[3][2] = (far_plane + near_plane)/(far_plane - near_plane);
	//	dump_comment("orthogonal\r\n");
	//}
	//else { // perspective
	//	projTM[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
	//	projTM[3][3] = 0.0;
	//	projTM[2][3] = -1.0f;
	//	projTM[3][2] = -(2.0f*far_plane*near_plane) / (far_plane - near_plane);
	//	dump_comment("perspective\r\n");
	//}

	// up vector == Y-axis
	up[0] = viewTM[0][1];
	up[1] = viewTM[1][1];
	up[2] = viewTM[2][1];

	// eye point = z + center;
	eye[0] = -viewTM[0][0] * viewTM[3][0] - viewTM[0][1] * viewTM[3][1] - viewTM[0][2] * viewTM[3][2];
	eye[1] = -viewTM[1][0] * viewTM[3][0] - viewTM[1][1] * viewTM[3][1] - viewTM[1][2] * viewTM[3][2];
	eye[2] = -viewTM[2][0] * viewTM[3][0] - viewTM[2][1] * viewTM[3][1] - viewTM[2][2] * viewTM[3][2];

	center[0] = eye[0] - viewTM[0][2];
	center[1] = eye[1] - viewTM[1][2];
	center[2] = eye[2] - viewTM[2][2];
	
	if (persp) { // perspective
		if (pnear) *pnear = near_plane;
		if (pfar) *pfar = far_plane;
		if (pfovy) {
			*pfovy = (float)RadToDeg(atan(1.0f/projTM[1][1])*2.0f);
		}
		if (paspect) { // aspect ratio = with / height
			*paspect = projTM[1][1]/projTM[0][0];
		}
	}
	else {
		//if (pnear) *pnear = near_plane;
		//if (pfar) *pfar = far_plane;
		//if (pfovy) {
		//	*pfovy = (float)RadToDeg(atan(1.0f/projTM[1][1])*2.0f);
		//}
		//if (paspect) { // aspect ratio = with / height
		//	*paspect = projTM[1][1]/projTM[0][0];
		//}
		*pnear = *pfar = *pfovy = *paspect = 0.0f;
	}

	return (persp != 0);
}


// return false if orthogonal, true if perspective
bool zz_max_exporter::get_camera_direct3d (
	float projTM[4][4],
	float viewTM[4][4],
	float * pfovy,
	float * paspect,
	float * pnear,
	float * pfar,
	float eye[3],
	float center[3],
	float up[3]
	)
{
	float mat[4][4]; // The transformation matrix times the projection matrix. 
	Matrix3 invTM; // This is the inverse of the affine part of the camera transformation matrix (not the inverse of the projection part). 
	int persp; // nonzero indicates this is a perspective view; 0 is orthogonal. 
	float near_plane; // near clipping plane
	float far_plane; // far clipping plane

	ViewExp * pView = ip->GetActiveViewport();  // Get the viewport in question
	GraphicsWindow *gw = pView->getGW();  // Get the GraphicsWindow context

	gw->getCameraMatrix(mat, &invTM, &persp, &near_plane, &far_plane); // getting these values to work with ... see above for their types

	int ki, kj;
	Matrix3 camTM(invTM);
	camTM.Invert();
	MRow * pcvm = camTM.GetAddr();
	for (ki = 0; ki < 4; ki++) {
		for (kj = 0; kj < 3; kj++) {
			viewTM[ki][kj] = pcvm[ki][kj];
		}
	}
	viewTM[0][3] = viewTM[1][3] = viewTM[2][3] = 0.0f;
	viewTM[3][3] = 1.0f;

	// copy perspTM from mat
	MRow *pa = invTM.GetAddr();
	for (ki = 0; ki < 4; ki++) {
		float val = (float)(ki==3);
		for (kj = 0; kj < 4; kj++) {
			projTM[ki][kj] = pa[ki][0] * mat[0][kj] +
				pa[ki][1] * mat[1][kj] +
				pa[ki][2] * mat[2][kj] +
				val		 * mat[3][kj];
		}
	}

/* from D3DXMatrixPerspectiveOffCenterRH()
2*zn/(r-l)   0            0                0
0            2*zn*(t-b)   0                0
(l+r)/(r-l)  (t+b)/(t-b)  zf/(zn-zf)      -1
0            0            zn*zf/(zn-zf)    0
*/
	if (persp) {
		// transposed.
		projTM[2][2] = far_plane/(near_plane - far_plane);
		projTM[3][2] = near_plane*far_plane/(near_plane - far_plane);
		projTM[2][3] = -1.0f;
		projTM[3][3] = 0.0f;
	}
	else {
/* from D3DXMatrixOrthoRH ()
2/w  0    0           0
0    2/h  0           0
0    0    1/(zn-zf)   0
0    0    zn/(zn-zf)  1
*/
	}

	// up vector == Y-axis
	up[0] = viewTM[0][1];
	up[1] = viewTM[1][1];
	up[2] = viewTM[2][1];

	// eye point = z + center;
	eye[0] = -viewTM[0][0] * viewTM[3][0] - viewTM[0][1] * viewTM[3][1] - viewTM[0][2] * viewTM[3][2];
	eye[1] = -viewTM[1][0] * viewTM[3][0] - viewTM[1][1] * viewTM[3][1] - viewTM[1][2] * viewTM[3][2];
	eye[2] = -viewTM[2][0] * viewTM[3][0] - viewTM[2][1] * viewTM[3][1] - viewTM[2][2] * viewTM[3][2];

	float scale_center = fabs(far_plane - near_plane);
	center[0] = eye[0] - viewTM[0][2]*scale_center;
	center[1] = eye[1] - viewTM[1][2]*scale_center;
	center[2] = eye[2] - viewTM[2][2]*scale_center;
	
	if (persp) { // perspective
		if (pnear) *pnear = near_plane;
		if (pfar) *pfar = far_plane;
		if (pfovy) {
			//d3d :
			*pfovy = (float)RadToDeg(2.0f*atan(1.0f/projTM[1][1]));
			//ogl : *pfovy = (float)RadToDeg(atan(1.0f/projTM[1][1])*2.0f);

			// TODEL: d3d
			//*pfovy = (float)RadToDeg(atan(projTM[1][1]/(2.0f*near_plane))*2.0f);
		}
		if (paspect) { // aspect ratio = with / height
			*paspect = projTM[1][1]/projTM[0][0];
		}
	}
	else {
		if (pnear) *pnear = near_plane;
		if (pfar) *pfar = far_plane;
		if (pfovy) *pfovy = 0.0f;
		if (paspect) {
			*paspect = projTM[1][1]/projTM[0][0];
		}
	}

	return (persp != 0);
}

// get selected node 
INode * zz_max_exporter::get_selected_node (INode * current)
{
	INode * selected = NULL;

	if (current == NULL) return NULL;

	if (current->Selected()) {
		return current;
	}
	int num_children = current->NumberOfChildren();

	for (int i = 0; i < num_children; ++i) {
		selected = get_selected_node(current->GetChildNode(i));
		if (selected) return selected;
	}
	return NULL;
}

int zz_max_exporter::get_number_of_selected_node (INode* node)
{
	if (node == NULL) return number_of_selected_node;

	if (node->Selected()) {
		++number_of_selected_node;
	}

	int num_children = node->NumberOfChildren();

	for (int i = 0; i < num_children; ++i) {
		get_number_of_selected_node(node->GetChildNode(i));
	}

	return number_of_selected_node;
}

// saves the node pointer and name of all bones
// recurse from root to leaf with children nodes
// ALERT: sync with zz_max_exporter_motion::allocate_channel ()
bool zz_max_exporter::allocate_bones (INode* node)
{
	if (!node) return false;

	ObjectState os = node->EvalWorldState(0);
	Control * cont = node->GetTMController();
	//Object* obj = node->GetObjectRef();

	// discard dummy node and footsteps
	// [ref] maxsdk/plugapi.h
	if (os.obj->ClassID() != Class_ID(DUMMY_CLASS_ID, 0) && 
		(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0) || 
		 os.obj->ClassID() == BONE_OBJ_CLASSID || // New procedural bone object for Magma
		 cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID))
		//if (os.obj->ClassID() != Class_ID(DUMMY_CLASS_ID, 0) &&
		// ((cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
		// (cont->ClassID() == BIPBODY_CONTROL_CLASS_ID))
		// )
	{
		bone_list[number_of_bones].bone = node;
		bone_list[number_of_bones].name = node->GetName();
		ZZ_LOG("bone_id : %d [%s]\r\n", number_of_bones, node->GetName());
		number_of_bones++;
	}
	else {
		// ignore non-bone truee
		ZZ_LOG("bone: [parent: %s]->[%s] is not a bone object. \r\n", node->GetParentNode()->GetName(), node->GetName()  );
		return true;
	}

	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
	map<string, INode *> child_map;
	INode * child;
	list<string> child_names;
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		child = node->GetChildNode(c);
		child_map[child->GetName()] = child;
		child_names.push_back(child->GetName());
	}
	// sort child name list order by name
	child_names.sort();
	list<string>::iterator it;
	map<string, INode *>::iterator map_it;
	for (it = child_names.begin(); it != child_names.end(); it++) {
		//map_it = child_map.find(*it);
		//allocate_bones(map_it->second);
		allocate_bones(child_map[*it]);
	}

	return true;
}

// get bone index
unsigned int zz_max_exporter::get_bone_index(INode* node)
{
	if (!node) return -999;

	int i;

	for (i = 0; i < number_of_bones; i++) {
		if (node == bone_list[i].bone) {
			assert(!strcmp(node->GetName(), bone_list[i].name));
			return i;
		}
	}

	return -999; // no corresponding index
}

// Return a pointer to a TriObject given an INode or return NULL
// if the node cannot be converted to a TriObject
TriObject* zz_max_exporter::get_triobject_from_node (INode *node, TimeValue t, bool &delete_it)
{
	delete_it = false;
	Object * obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject * tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) delete_it = true;
		return tri;
	}
	else {
		return NULL;
	}
}

BOOL zz_max_exporter::TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0) ? 1 : 0;
}

void zz_max_exporter::get_face_vertex_index (INode * node, int& vx1, int& vx2, int& vx3)
{
	Matrix3 tm = node->GetObjTMAfterWSM(0);
	BOOL negScale = TMNegParity(tm);

	if (negScale)
		dump_comment("negScaled\r\n");

	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}
}

Point3 zz_max_exporter::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return vertexNormal;
}

bool zz_max_exporter::has_physique_or_skin (INode* node)
{
	zz_phy_info phy_info;
	zz_skin_info skin_info;

	if (phy_info.get_modifier(node) || skin_info.get_modifier(node)) {
		dump_comment("-- has physique or skin()");
		return true;
	}
	dump_comment("-- does not have physyque or skin\r\n");
	return false;
}

// character studio default link is [r|l]thigh->belly.
// but, we use thigh->pelvis link relation.
// return true if relink process has done.
bool zz_max_exporter::relink_thigh_to_pelvis (INode * pelvis)
{
	INode * lthigh, * rthigh;

	if (!pelvis) return false;

	// find nodes
	lthigh = ip->GetINodeByName(LTHIGH_NAME);
	rthigh = ip->GetINodeByName(RTHIGH_NAME);
	
	bool relink_done = false;

	if (lthigh) {
		if (lthigh->GetParentNode() != pelvis) {
			relink_done = true;
			ZZ_LOG("relink_thigh_to_pelvis() lthigh->pelvis done.\r\n");
			lthigh->Detach(0);
			pelvis->AttachChild(lthigh);
		}
	}
	else {
		ZZ_LOG("relink_thigh_to_pelvis(). lthigh 를 찾을 수 없음\r\n");
	}

	// rthigh
	if (rthigh) {
		if (rthigh->GetParentNode() != pelvis) {
			relink_done = true;
			ZZ_LOG("relink_thigh_to_pelvis() rthigh->pelvis done.\r\n");
			rthigh->Detach(0);
			pelvis->AttachChild(rthigh);
		}
	}
	else {
		ZZ_LOG("relink_thigh_to_pelvis(). rthigh 를 찾을 수 없음\r\n");
	}

	if (relink_done) {
		popup_message("[r|l]thigh를 pelvis에 재연결시켰습니다.");
	}

	return relink_done;
}

// find root bone (= pelvis bone) by biped bone
INode * zz_max_exporter::get_rootbone (INode * node)
{
	assert(node);
	if (!node) {
		return NULL;
	}

	INode * current, * saved;
	Control * pcontrol;
	current = node;
	do {
		saved = current;
		current = current->GetParentNode();
		ZZ_LOG("exporter::get_rootbone(). saved(%s). current(%s)\r\n", saved->GetName(), current->GetName());
		pcontrol = current->GetTMController();
		// if we meets BIPED(center of mass), return the child of BIPED.
	} while (pcontrol && (pcontrol->ClassID() != BIPBODY_CONTROL_CLASS_ID));

	return saved;
}

// find physique modifier
Modifier * zz_phy_info::get_modifier (INode* node)
{
	if (!node) return NULL;

	Object* obj = node->GetObjectRef();

	if (!obj) return NULL;

	while (obj->SuperClassID() == GEN_DERIVOB_CLASS_ID && obj) {
		IDerivedObject * derived_obj = (IDerivedObject *)(obj);

		int mod_stack_index = 0;
		while (mod_stack_index < derived_obj->NumModifiers()) {
			Modifier* mod = derived_obj->GetModifier(mod_stack_index);
			if (mod->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)) {
				return mod;
			}
			mod_stack_index++;
		}
		obj = derived_obj->GetObjRef();
	}

	return NULL;
}

Modifier * zz_skin_info::get_modifier (INode* node)
{
	if (!node) return NULL;

	// Get object from node. Abort if no object.
	Object* pObj = node->GetObjectRef();

	if (!pObj) return NULL;

	// Is derived object ?
	while (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* pDerObj = (IDerivedObject*) pObj;

		// Iterate over all entries of the modifier stack.
		int Idx = 0;
		while (Idx < pDerObj->NumModifiers())
		{
			// Get current modifier.
			Modifier* mod = pDerObj->GetModifier(Idx);

			// Is this Skin ?
			if (mod->ClassID() == SKIN_CLASSID )
			{
				// Yes -> Exit.
				return mod;
			}

			// Next modifier stack entry.
			Idx++;
		}
		pObj = pDerObj->GetObjRef();
	}

	// Not found.
	return NULL;
}

// returns true if has physique, or returns false
bool zz_phy_info::get_info (INode * node)
{
	assert(node);
	
	// for physique modifier
	phy_mod = NULL;
	phy = NULL;
	phy_context = NULL;

	// Get the physique!
	if (phy_mod = get_modifier(node)) {
		phy = (IPhysiqueExport *)phy_mod->GetInterface(I_PHYINTERFACE);
		if (!phy) return false;
		phy_context = (IPhyContextExport *)phy->GetContextInterface(node);
		if (!phy_context) return false;
		phy_context->ConvertToRigid(true);
		//allow blending to export multi-link assignments
		phy_context->AllowBlending(true);
		return true;
	}
	return false;
}

// returns true if has skin, or returns false
bool zz_skin_info::get_info (INode * node)
{
	assert(node);

	skin_mod = NULL;
	skin = NULL;
	skin_context = NULL;

	if (skin_mod = get_modifier(node)) {
		skin = (ISkin *)skin_mod->GetInterface(I_SKIN);
		if (!skin) return false;
		skin_context = skin->GetContextInterface(node);
		if (!skin_context) return false;
		return true;
	}
	return false;
}


// find root bone (= pelvis bone) by skin
// we find 
#if (0)
INode * zz_skin_info::get_rootbone (INode * node)
{
	// if does not has skin modifier
	if (!get_modifier(node)) return NULL;

	int num_bones = skin->GetNumBones();
	assert(num_bones >= 1);

	return zz_max_exporter::get_rootbone(skin->GetBone(0));
}

INode * zz_phy_info::get_rootbone (INode * node)
{
	return NULL;
}
#endif

zz_edit_normal::zz_edit_normal (INode * node)
{
	n = node;
	Interface * ip = zz_max_exporter::GetInterface();
	save_mode = ip->GetCommandPanelTaskMode();

	en = get_modifier (node);

	//// only support trimesh
	if (en) {
		ObjectState os = node->EvalWorldState(0);
        if (os.obj->ClassID() != Class_ID(EDITTRIOBJ_CLASS_ID, 0)) {
			zz_max_exporter::popup_message("EditNormal ignored on non-TriMesh!");
			en = NULL;
			return;
		}
		ZZ_LOG("edit_normal: found EditNormal modifier.\n");
	}
	else {
		ZZ_LOG("edit_normal: EditNormals mod not found.\n");
        return; // do nothing
	}
	
	// if current mode is already MODIFY, it may be not to activate MODIFY mode.
	// then, we cannot get the correct results.

	ip->SetCommandPanelTaskMode(TASK_MODE_CREATE); // to activate modify mode.
	ip->SetCommandPanelTaskMode(TASK_MODE_MODIFY);

	num_faces = en->EnfnGetNumFaces(n);
	num_normals = en->EnfnGetNumNormals(n);
	num_vertices = en->EnfnGetNumVertices(n);

	ZZ_LOG("EN(%d, %d, %d)\n", num_faces, num_normals, num_vertices);
}

zz_edit_normal::~zz_edit_normal ()
{
	zz_max_exporter::GetInterface()->SetCommandPanelTaskMode(save_mode);
}

// TODO: check EditPoly
IEditNormalsMod * zz_edit_normal::get_modifier (INode* node)
{
	if (!node) return NULL;

	// Get object from node. Abort if no object.
	Object* pObj = node->GetObjectRef();

	if (!pObj) return NULL;

	// Is derived object ?
	while (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* pDerObj = (IDerivedObject*) pObj;

		// Iterate over all entries of the modifier stack.
		int Idx = 0;
		while (Idx < pDerObj->NumModifiers())
		{
			// Get current modifier.
			Modifier* mod = pDerObj->GetModifier(Idx);

			// Is this EditNormal ?
			if (mod->ClassID() == ZZ_CLASS_ID_EDIT_NORMAL )
			{
				ZZ_LOG("modifier(%s) found.\n", mod->GetName());
				en = (IEditNormalsMod *)mod->GetInterface(EDIT_NORMALS_MOD_INTERFACE);
				return en;
			}

			// Next modifier stack entry.
			Idx++;
		}
		pObj = pDerObj->GetObjRef();
	}

	// Not found.
	return NULL;
}

int zz_edit_normal::GetNormalID (int faceNo, int cornerNo)
{
	assert(enabled());

	return en->EnfnGetNormalID(faceNo, cornerNo);
}

Point3 zz_edit_normal::GetNormal(int faceNo, int cornerNo)
{
	assert(enabled());

	int normalID = en->EnfnGetNormalID(faceNo, cornerNo);

	return *(en->EnfnGetNormal(normalID));
}

bool zz_max_exporter::remap_texture_coord (
	int mapchannel,
	INode * node,
	const Point3& texture_coord,
	Point3& remapped_texture_coord)
{
	remapped_texture_coord = texture_coord;

	if (node && (mapchannel == 1)) {
		// get diffuse material texture
		Mtl * material = node->GetMtl();
		if (!material) {
			popup_message("등록된 재질이 없습니다.");
			return false;
		}
		if (material->ClassID() != Class_ID(DMTL_CLASS_ID, 0)) {
			popup_message("Standard 재질이 아닙니다.");
			return false;
		}

		StdMat* std = (StdMat*)material;

		Texmap * texMap = std->GetSubTexmap(1); // get diffuse map

		// uvgen animation test code
#if (0)
		UVGen * uvgen = texMap->GetTheUVGen();

		Interval val;
		static TimeValue t = 0;
		uvgen->Update(t, val);
		t += 10;
#endif

		if (!texMap) {
			popup_message("디퓨즈 텍스쳐 이미지가 설정되지 않았습니다.");
			return false;
		}
		Matrix3 uvTransform;
		texMap->GetUVTransform(uvTransform);
		
#if (0) // uvgen animation test code
		Point3 p = uvTransform.GetTrans();
		ZZ_LOG("[%d] [%f, %f, %f]\n", t, p.x, p.y, p.z);
#endif

		remapped_texture_coord = uvTransform * remapped_texture_coord;
	}
	//// if negative?
	//if (texture_coord.x < 0) {
	//	result.x = 0;
	//}
	//if (texture_coord.y < 0) {
	//	result.y = 0;
	//}
	remapped_texture_coord.y = 1.0f - remapped_texture_coord.y;
	return true;
}

//Point3 zz_max_exporter::remap_texture_coord (const Point3& texture_coord)
//{
//	Point3 result = texture_coord;
//
//	if (texture_coord.x < 0) {
//		result.x = 1.0f - texture_coord.x;
//	}
//	if (texture_coord.y < 0) {
//		result.y = 1.0f - texture_coord.y;
//	}
//	result.y = 1.0f - result.y;
//	return result;
//}

static BOOL SelectedAncestor(INode *node)
{
	if (!node->GetParentNode()) return FALSE;
	if (node->GetParentNode()->Selected()) return TRUE;
	else return SelectedAncestor(node->GetParentNode());
}

//*************** IT'S NOT CORRECT ****************
// reset transform (including scale)
// from [\MAXSDK\SAMPLES\MODIFIERS\RESETTM.CPP]
bool zz_max_exporter::reset_tm (INode * node)
{
	if (node->IsGroupMember() || node->IsGroupHead()) return false;
	if (SelectedAncestor(node)) return false;
	Matrix3 ntm, ptm, rtm(1), piv(1), tm;

	// Get Parent and Node TMs
	ntm = node->GetNodeTM(ip->GetTime());
	ptm = node->GetParentTM(ip->GetTime());

	// Compute the relative TM
	ntm = ntm * Inverse(ptm);

	// The reset TM only inherits position
	rtm.SetTrans(ntm.GetTrans());

	// Set the node TM to the reset TM		
	tm = rtm*ptm;
	node->SetNodeTM(ip->GetTime(), tm);

	// Compute the pivot TM
	piv.SetTrans(node->GetObjOffsetPos());
	PreRotateMatrix(piv,node->GetObjOffsetRot());
	ApplyScaling(piv,node->GetObjOffsetScale());

	// Reset the offset to 0
	node->SetObjOffsetPos(Point3(0,0,0));
	node->SetObjOffsetRot(IdentQuat());
	node->SetObjOffsetScale(ScaleValue(Point3(1,1,1)));

	// Take the position out of the matrix since we don't reset position
	ntm.NoTrans();

	// Apply the offset to the TM
	ntm = piv * ntm;

	// Apply a derived object to the node's object
	Object *obj = node->GetObjectRef();
	IDerivedObject *dobj = CreateDerivedObject(obj);

	// Create an XForm mod
	SimpleMod *mod = (SimpleMod*)ip->CreateInstance(
		OSM_CLASS_ID,
		Class_ID(CLUSTOSM_CLASS_ID,0));

	// Apply the transformation to the mod.
	SetXFormPacket pckt(ntm);
	mod->tmControl->SetValue(ip->GetTime(),&pckt);

	// Add the bend modifier to the derived object.
	dobj->SetAFlag(A_LOCK_TARGET); // RB 3/11/99: When the macro recorder is on the derived object will get deleted unless it is locked.
	dobj->AddModifier(mod);
	dobj->ClearAFlag(A_LOCK_TARGET);

	// Replace the node's object
	node->SetObjectRef(dobj);

	GetSystemSetting(SYSSET_CLEAR_UNDO);
	ip->RedrawViews(ip->GetTime());

	return true;
}

