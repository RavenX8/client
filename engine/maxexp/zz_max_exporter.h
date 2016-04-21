/** 
 * @file zz_max_exporter.h
 * @brief 3ds max exporter base class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    26-jun-2002
 *
 * $Id: zz_max_exporter.h,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#ifndef __ZZ_MAX_EXPORTER_H__
#define __ZZ_MAX_EXPORTER_H__

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "plugapi.h" // for BONE_CLASS_ID
#include "Phyexp.h"  // for physique
#include "bipexp.h"  // for biped
#include "iparamm2.h"
#include "iskin.h"   // for COM_skin
#include "iEditNormals.h" // for EditNormals
#include "zz_system.h"
#include "zz_mesh_tool.h"
#include "zz_interface.h"
#include "zz_script_simple.h"

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

// hard-coded macros
#define VERSION			202			// Version number * 100
#define CAMERA_NAME     "Camera01"
#define MAX_NUM_BONE    1000
#define PELVIS_NAME "b1_pelvis"
#define RTHIGH_NAME "b1_rthigh"
#define LTHIGH_NAME "b1_lthigh"

#define MAX_STRING_SIZE 256

// for physique
struct zz_phy_info {
	Modifier * phy_mod;
	IPhysiqueExport * phy;
	IPhyContextExport * phy_context;

	zz_phy_info () : phy_mod(NULL), phy(NULL), phy_context(NULL) {}

	~zz_phy_info () {
		if (phy) {
			phy->ReleaseContextInterface(phy_context);
		}
		if (phy_mod) {
			phy_mod->ReleaseInterface(I_PHYINTERFACE, phy);
		}
	}

	bool get_info (INode * node);
	Modifier * get_modifier (INode* node);
	//INode * get_rootbone (INode * node); // not supported yet
};

// for skin
struct zz_skin_info {
	Modifier * skin_mod;
	ISkin * skin;
	ISkinContextData * skin_context;

	zz_skin_info () : skin_mod(NULL), skin(NULL), skin_context(NULL) {}

	bool get_info (INode * node);
	Modifier * get_modifier (INode* node);

	//INode * get_rootbone (INode * node);
};

// for EditNormal Modifier
class zz_edit_normal {
private:
	int save_mode;

	IEditNormalsMod * en;
	int num_faces, num_vertices, num_normals;
	INode * n; // node

public:
	zz_edit_normal (INode * node);
	~zz_edit_normal ();

	bool enabled () { return (en != NULL); }
	void disable () { en = NULL; }

	IEditNormalsMod * get_modifier (INode * node);
	Point3 GetNormal (int faceNo, int cornerNo);
	int GetNormalID (int faceNo, int cornerNo);
	int GetNumFaces () { return num_faces; }
};

class zz_max_exporter : public SceneExport {
protected:
	static Interface * ip;
	static FILE * pstream;
	zz_script_simple script_file;
	static bool is_asciioutput;
	IBipedExport * biped_interface;
	Control * pcontrol;

	int total_phase;
	int current_phase;
	int number_of_selected_node;

	TimeValue start_time;

	Mesh * max_mesh;
	zz_mesh * engine_mesh;

	bool need_delete; // if tri pointer has assigned, it have to be deallocated at the end of use.
	TriObject * tri; // allocated in export_vertex() and freed in ~zz_max_exporter_mesh()
	
	INode * root_bone; // ex. biped pelvis bone
	
	TCHAR str_long_desc[256]; // long description string
	TCHAR str_short_desc[256]; // short description string

public:
	zz_max_exporter();
	~zz_max_exporter();

	static Interface * GetInterface () { return ip; }

	// inherited from SceneExport class
	int ExtCount(); // Number of extensions supported 
	const TCHAR * Ext(int n) = 0; // Extension #n (i.e. "ZMD")
	const TCHAR * LongDesc(); // Long description (i.e. "Zho Model Export") 
	const TCHAR * ShortDesc(); // Short description (i.e. "ZMD")
	const TCHAR * AuthorName(); // Author name
	const TCHAR * CopyrightMessage(); // Copyright message 
	const TCHAR * OtherMessage1(); // Other message #1
	const TCHAR * OtherMessage2(); // Other message #2
	unsigned int Version(); // Version number * 100 (i.e. v3.01 = 301) 
	void ShowAbout(HWND hWnd); // Show DLL's "About..." box
	int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0) = 0; // Export	file
	BOOL SupportsOptions(int ext, DWORD options);

	// new methods
	struct zz_bone_list {
		INode * bone;
		TCHAR * name;
	};
	zz_bone_list bone_list[MAX_NUM_BONE];
	int number_of_bones;

	bool allocate_bones (INode* node);
	unsigned int get_bone_index(INode* node);

	BOOL TMNegParity(Matrix3 &m);
	Point3 GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	void get_face_vertex_index (INode * node, int& vx1, int& vx2, int& vx3);

	TriObject* get_triobject_from_node (INode *node, TimeValue t, bool &delete_it);
	//BOOL allocate_bones (INode* node);
	//int get_bone_index (INode* node);
	INode * get_selected_node (INode * root_node);
	int get_number_of_selected_node (INode* node);

	TimeValue get_time_by_frame (int frame);

	bool remap_texture_coord (int mapchannel, INode * node, const Point3& texture_coord, Point3& remapped_texture_coord);

	// low-level export
	void dump_uint (unsigned int ui, const TCHAR * msg = NULL);
	void dump_float (float f, const TCHAR * msg = NULL);
	void dump_float3 (float f1, float f2, float f3, const TCHAR * msg = NULL);
	void dump_float4 (float f1, float f2, float f3, float f4, const TCHAR * msg = NULL);
	void dump_quaternion (Quat q, const TCHAR * msg);
	void dump_string (const TCHAR * str, int size = 0, const TCHAR * msg = NULL);
	void dump_nullstring (const TCHAR * str, const TCHAR * msg = NULL);
	static void dump_comment (const TCHAR * format, ...);
	void dump_sample_quat (INode* node, int i);
	void dump_sample_pos (INode* node, int i);
	void dump_matrix (Matrix3 m);
	void dump_matrix(float tm[4][4]);
	void dump_matrix_transpose(float tm[4][4]);
	void write_sample_camera (INode * node, CameraObject * camera, int i);

	// output mode. binary or ascii.
	void set_asciioutput (bool true_if_ascii);
	bool get_asciioutput (void); // true if ascii mode

	static bool popup_message (const TCHAR * msg, bool okcancel = false);

	// for Biped
	void figure_mode_begin (INode * bone);
	void figure_mode_end ();
	void remove_nonuniform_scale(INode * bone);

	const TCHAR * get_filename (const TCHAR * fullname);

	// return true if perspective, false if orthogonal.
	bool get_camera_opengl (
		float projTM[4][4],
		float viewTM[4][4],
		float * pfovy,
		float * paspect,
		float * pnear,
		float * pfar,
		float eye[3],
		float center[3],
		float up[3]
	);

	bool get_camera_direct3d (
		float projTM[4][4],
		float viewTM[4][4],
		float * pfovy,
		float * paspect,
		float * pnear,
		float * pfar,
		float eye[3],
		float center[3],
		float up[3]
	);

	// physique & skin section
	bool has_physique_or_skin (INode* node);

	Point3 get_world_position(INode* node, int frame);
	Quat get_local_rotation(INode* node, int frame);
	Quat get_world_rotation(INode* node, int frame);

	// reset tm
	bool reset_tm (INode * node);

	static INode * get_rootbone (INode * node);
	bool relink_thigh_to_pelvis (INode * pelvis);
};

extern TCHAR * get_string (int id, TCHAR * buf = NULL);
extern HINSTANCE h_instance; // from zz_max_exporter.cpp
extern DWORD WINAPI fn (LPVOID arg);

#endif // __ZZ_MAX_EXPORTER_H__

