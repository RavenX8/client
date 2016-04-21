/** 
 * @file zz_max_exporter_model.cpp
 * @brief 3ds max exporter model class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    26-jun-2002
 *
 * $Id: zz_max_exporter_model.cpp,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#include "zz_max_exporter_model.h"
#include <list>
#include <map>
#include <string>

using namespace std;

zz_max_exporter_model::zz_max_exporter_model ()
{
	number_of_bones = 0;
	get_string(IDS_LONGDESC_MODEL, str_long_desc);
	get_string(IDS_SHORTDESC_MODEL, str_short_desc);
}

zz_max_exporter_model::~zz_max_exporter_model ()
{
}

const TCHAR * zz_max_exporter_model::Ext (int n)
{
	return get_string(IDS_EXT_MODEL);
}


// Dialog proc
static BOOL CALLBACK model_export_dlg_proc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  zz_max_exporter_model * exp = reinterpret_cast<zz_max_exporter_model *>((LPARAM)GetWindowLong(hWnd, GWL_USERDATA));
	
  switch (msg) {
    case WM_INITDIALOG:
		exp = (zz_max_exporter_model *)lParam;
		SetWindowLong(hWnd, GWL_USERDATA, LONG(lParam)); 
		
		// Setup the spinner controls for the controller key sample rate 
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

int zz_max_exporter_model::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	int current_phase = 1;
	int total_phase = 5;
	
	if (!(options & SCENE_EXPORT_SELECTED)) {
		// if no node was selected
		popup_message("File->Export Selected 를 선택하여 익스포트하세요.");
		return 0;
	}

	// Grab the interface pointer.
	ip = i;

	// Popup Dialog Box
	// Prompt the user with our dialogbox, and get all the options.
	if (suppressPrompts == FALSE) {
		if (!DialogBoxParam(h_instance, MAKEINTRESOURCE(IDD_MODEL_EXPORT_OPTION), ip->GetMAXHWnd(), model_export_dlg_proc, (LPARAM)this)) {
			return 0;
		}
	}

	// Open the stream
	pstream = _tfopen(name,_T("wb"));
	if (!pstream) {
		popup_message("model: 파일을 열 수 없습니다");
		return 0;
	}

	if (ip->GetSelNodeCount() > 1) {
		popup_message("model: 하나의 노드만 선택하세요");
		return 0;
	}
	
	// Startup the progress bar.
	ip->ProgressStart(get_string(IDS_PROGRESS_MSG), TRUE, fn, NULL);

	// first phase
	export_model_header(name, MAGIC_NUMBER_MODEL);
	ip->ProgressUpdate((int)(100*current_phase++/total_phase));

	// second phase
	//dump_uint(get_number_of_selected_node(ip->GetRootNode()), "number_of_meshes");
	//export_mesh_names(ip->GetRootNode());
	ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

	INode * selected = get_selected_node(ip->GetRootNode());
	root_bone = get_rootbone(selected);

	relink_thigh_to_pelvis(root_bone);

	if (root_bone) {
		ZZ_LOG("max_exporter_model: DoExport() root_bone = %s\r\n",
			root_bone->GetName());
	}
	else {
		popup_message("model: pelvis를 찾을 수 없습니다");
		ip->ProgressEnd();
		fclose(pstream);

		return 0;
	}

	figure_mode_begin(root_bone); // for biped == model arrangement
	{
		// third phase
		allocate_bones(root_bone); // saves all bone's name and pointer
		ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

		// fourth phase
		export_skeleton(root_bone);
		ip->ProgressUpdate((int)(100*current_phase++/total_phase));

		// fifth phase
		export_dummies(root_bone);
		ip->ProgressUpdate((int)(100*current_phase++/total_phase));
	}
	figure_mode_end();

	// We're done. Finish the progress bar.
	ip->ProgressEnd();

	// Close the stream
	fclose(pstream);

	return 1;
}

// export skeleton information
void zz_max_exporter_model::export_skeleton (INode * bone)
{
	if (!bone) { // no attached bone system
		dump_uint(0, "number_of_bones_no_bone");
		return;
	}
	dump_uint((unsigned int)number_of_bones, "number_of_bones");
	for (int i = 0; i < number_of_bones; i++) {
		dump_comment("\r\nbone_id = %d\r\n", i);
		dump_skeleton(bone_list[i].bone);
	}
}

// FROM [plugapi.h]
//#define DUMMY_CLASS_ID 			0x876234
//#define BONE_CLASS_ID 			0x8a63c0
//#define TAPEHELP_CLASS_ID 		0x02011
//#define GRIDHELP_CLASS_ID		0x02010
//#define POINTHELP_CLASS_ID		0x02013
//#define PROTHELP_CLASS_ID		0x02014

void count_dummy (INode * node, int& count, std::list<string>& dummy_names, std::map<string, INode *>& dummy_nodes)
{
	ObjectState os = node->EvalWorldState(0);
	Control * cont = node->GetTMController();

	//if (os.obj->SuperClassID() == POINTHELP_CLASS_ID) {
	if (os.obj->ClassID() == Class_ID(POINTHELP_CLASS_ID, 0)) {
		count++;
		dummy_names.push_back(node->GetName());
		dummy_nodes[node->GetName()] = node;
		ZZ_LOG("found dummy point(%s). num_dummy(%d)\r\n",
			node->GetName(), count);
	}
	else if ((cont->ClassID() != BIPSLAVE_CONTROL_CLASS_ID) &&
		(os.obj->ClassID() != BIPBODY_CONTROL_CLASS_ID) &&
		(os.obj->ClassID() != FOOTPRINT_CLASS_ID) &&
		(os.obj->ClassID() != Class_ID(BONE_CLASS_ID, 0)) &&
		(os.obj->ClassID() != Class_ID(DUMMY_CLASS_ID, 0)) &&
		(os.obj->ClassID() != BONE_OBJ_CLASSID))
	{
		ZZ_LOG("[%s] is not a helper object or bone object. ignore this tree.\r\n",
			node->GetName());
		return;
	}
	for (int i = 0; i < node->NumberOfChildren(); i++) {
		count_dummy(node->GetChildNode(i), count, dummy_names, dummy_nodes);
	}
}

void zz_max_exporter_model::dump_dummy (INode * node)
{
	ObjectState os = node->EvalWorldState(0);
	dump_comment("\r\n");
		
	// get parent node
	INode * parent = node->GetParentNode();
	assert(parent);

	// dummy name
	dump_nullstring(node->GetName(), "dummy_name");

	// bone index
	int bone_index = get_bone_index(parent);
	dump_uint((unsigned int)bone_index, "bone_index");

	// local position
	Matrix3 nodeWTM = node->GetNodeTM(0);
	Matrix3 parentWTM = parent->GetNodeTM(0);
	Matrix3 localTM = nodeWTM * Inverse(parentWTM);
	Point3 local_pos = localTM.GetTrans(); // like localTM.GetRow(3);
	Quat q = localTM;

	dump_float3(local_pos.x, local_pos.y, local_pos.z, "local position");
	dump_quaternion(q, "local rotation");
}

void zz_max_exporter_model::export_dummies (INode * root)
{
	assert(root);
	int count = 0;
	dump_comment("\r\n");

	list<string> dummy_names;
	map<string, INode *> dummy_nodes;

	count_dummy(root, count, dummy_names, dummy_nodes);
	dump_uint((unsigned int)count, "number_of_dummies");

	if (count > 0) {
		// sort names
		dummy_names.sort();

		// write dummy
		list<string>::iterator it;
		for (it = dummy_names.begin(); it != dummy_names.end(); it++) {
			dump_dummy(dummy_nodes[*it]);
		}
	}
}

// dump skeleton 
void zz_max_exporter_model::dump_skeleton (INode* node)
{
	if (!node) return;

	Matrix3 localTM, parentWTM, nodeWTM;
	Point3 trans;
	Point3 scale;
	Quat q;

	unsigned int parent_index;

	/////////// ObjTMAfterWSM version
	//nodeWTM = node->GetObjTMAfterWSM(0);
	//nodeWTM.NoScale(); // for bone, remove scale-info. For biped, 
	//// if this is root bone
	//if (node == root_bone) { // use world TM
	//	trans = nodeWTM.GetTrans();
	//	q = nodeWTM;
	//	parent_index = 0;
	//}
	//else {
	//	parentWTM = node->GetParentNode()->GetObjTMAfterWSM(0);
	//	localTM = nodeWTM * Inverse(parentWTM);
	//	trans = localTM.GetTrans(); // like localTM.GetRow(3);
	//	q = localTM;
	//	parent_index = get_bone_index(node->GetParentNode());
	//}

	nodeWTM = node->GetNodeTM(0);
	nodeWTM.NoScale(); // for bone, remove scale-info. For biped, 
	
	// if this is root bone
	if (node == root_bone) { // use world TM
		trans = nodeWTM.GetTrans();
		q = nodeWTM;
		parent_index = 0;
	}
	else {
		parentWTM = node->GetParentNode()->GetNodeTM(0);
		localTM = nodeWTM * Inverse(parentWTM);
		trans = localTM.GetTrans(); // like localTM.GetRow(3);
		q = localTM;
		parent_index = get_bone_index(node->GetParentNode());
	}

	dump_uint(parent_index, "parent_bone_index");

	unsigned int index = get_bone_index(node);
	dump_nullstring(bone_list[index].name, "bone_name");

	dump_float3(trans.x, trans.y, trans.z, "local_trans");

	dump_quaternion(q, "rotation");
	//dump_float(q.w, "q.w");
	//dump_float(q.x, "q.x");
	//dump_float(q.y, "q.y");
	//dump_float(q.z, "q.z");
}

// export header section
void zz_max_exporter_model::export_model_header(const TCHAR *name, const TCHAR * magic_number)
{
	dump_string(magic_number, (int)strlen(magic_number), "magic_number");
//	dump_nullstring(get_filename(name), "file_id");
}

void zz_max_exporter_model::export_mesh_names (INode* node)
{
	if (node == NULL) return;

	if (node->Selected()) {
		dump_nullstring(node->GetName(), "mesh_name");
	}

	int num_children = node->NumberOfChildren();

	for (int i = 0; i < num_children; ++i) {
		export_mesh_names(node->GetChildNode(i));
	}
}