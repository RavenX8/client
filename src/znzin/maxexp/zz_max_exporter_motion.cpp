/** 
 * @file zz_max_exporter_motion.cpp
 * @brief 3ds max exporter motion class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    26-jun-2002
 *
 * 2003-06-13	zho		added OBJECT motion
 *
 * $Id: zz_max_exporter_motion.cpp,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#include "zz_max_exporter_motion.h"
#include "zz_channel.h"

#include <stdlib.h>
#include <list>
#include <map>
#include <string>

using namespace std;


zz_max_exporter_motion::zz_max_exporter_motion ()
{
	current_bone_index = 0;
	root_bone = NULL;
	number_of_channels = 0;
	need_delete = false;
	morph_use_position = TRUE;
	morph_use_normal = FALSE;
    morph_num_mapchannel = 0;
	morph_use_alpha = FALSE;
	morph_use_textureanim = FALSE;
	morph_use_world = FALSE;
	
	anim_type = ZZ_BIPED;
	anim_position = TRUE;
	anim_rotation = TRUE;
	anim_scale = TRUE;

	get_string(IDS_LONGDESC_MOTION, str_long_desc);
	get_string(IDS_SHORTDESC_MOTION, str_short_desc);
}

zz_max_exporter_motion::~zz_max_exporter_motion ()
{
	if (need_delete) {
		delete tri;
	}
}

const TCHAR * zz_max_exporter_motion::Ext (int n)
{
	return get_string(IDS_EXT_MOTION);
}

// Dialog proc
static BOOL CALLBACK motion_export_dlg_proc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  zz_max_exporter_motion * exp = reinterpret_cast<zz_max_exporter_motion *>((LPARAM)GetWindowLong(hWnd, GWL_USERDATA));
	
  switch (msg) {
    case WM_INITDIALOG:
		exp = (zz_max_exporter_motion *)lParam;
		SetWindowLong(hWnd, GWL_USERDATA, LONG(lParam)); 

		switch (exp->anim_type) {
			case zz_max_exporter_motion::ZZ_BIPED:
                CheckRadioButton(hWnd, IDC_TYPE_BIPED, IDC_TYPE_OBJECT, IDC_TYPE_BIPED);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_POSITION), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_ROTATION), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_SCALE), FALSE);
				break;
			case zz_max_exporter_motion::ZZ_MORPH:
				CheckRadioButton(hWnd, IDC_TYPE_BIPED, IDC_TYPE_OBJECT, IDC_TYPE_MORPH);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_POSITION), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_ROTATION), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_SCALE), FALSE);
				break;
			case zz_max_exporter_motion::ZZ_CAMERA:
				CheckRadioButton(hWnd, IDC_TYPE_BIPED, IDC_TYPE_OBJECT, IDC_TYPE_CAMERA);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_POSITION), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_ROTATION), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_SCALE), FALSE);
				break;
			case  zz_max_exporter_motion::ZZ_OBJECT:
				CheckRadioButton(hWnd, IDC_TYPE_BIPED, IDC_TYPE_OBJECT, IDC_TYPE_OBJECT);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_POSITION), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_ROTATION), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_SCALE), TRUE);
				break;
		}
		if (exp->anim_position) {
			CheckDlgButton(hWnd, IDC_ANIM_POSITION, BST_CHECKED);
		}
		else {
			CheckDlgButton(hWnd, IDC_ANIM_POSITION, BST_UNCHECKED);
		}
		if (exp->anim_rotation) {
            CheckDlgButton(hWnd, IDC_ANIM_ROTATION, BST_CHECKED);
		}
		else {
			CheckDlgButton(hWnd, IDC_ANIM_ROTATION, BST_UNCHECKED);
		}
		if (exp->anim_scale) {
			CheckDlgButton(hWnd, IDC_ANIM_SCALE, BST_CHECKED);
		}
		else {
			CheckDlgButton(hWnd, IDC_ANIM_SCALE, BST_UNCHECKED);
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_TYPE_MORPH:
		case IDC_TYPE_BIPED:
		case IDC_TYPE_CAMERA:
		case IDC_TYPE_OBJECT:
			if (IsDlgButtonChecked(hWnd, IDC_TYPE_OBJECT)) {
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_POSITION), true);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_ROTATION), true);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_SCALE),    true);
			}
			else {
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_POSITION), false);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_ROTATION), false);
				EnableWindow(GetDlgItem(hWnd, IDC_ANIM_SCALE),    false);
			}
			break;
		case IDOK:
			exp->set_asciioutput(IsDlgButtonChecked(hWnd, IDC_ASCIIOUTPUT) == TRUE);

			exp->anim_position = IsDlgButtonChecked(hWnd, IDC_ANIM_POSITION);
			exp->anim_rotation = IsDlgButtonChecked(hWnd, IDC_ANIM_ROTATION);
			exp->anim_scale = IsDlgButtonChecked(hWnd, IDC_ANIM_SCALE);

			if (IsDlgButtonChecked(hWnd, IDC_TYPE_BIPED)) {
				exp->anim_type = zz_max_exporter_motion::ZZ_BIPED;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_TYPE_CAMERA)) {
				exp->anim_type = zz_max_exporter_motion::ZZ_CAMERA;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_TYPE_MORPH)) {
				exp->anim_type = zz_max_exporter_motion::ZZ_MORPH;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_TYPE_OBJECT)) {
				exp->anim_type = zz_max_exporter_motion::ZZ_OBJECT;
			}
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


int zz_max_exporter_motion::do_export_bone (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	if (!root_bone) {
		popup_message("motion: pelvis를 찾을 수 없습니다");
		return FALSE;
	}

	int current_phase = 1;
	int total_phase = 5;

	ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

	allocate_channels(root_bone);
	ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

	export_motion_header(name, MAGIC_NUMBER_MOTION);
	ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

	export_channel_info();
	ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

	export_sample();
	ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 
	return TRUE;
}

int zz_max_exporter_motion::do_export_biped (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	if (!root_bone) {
		popup_message("motion: pelvis를 찾을 수 없습니다");
		return FALSE;
	}

	remove_nonuniform_scale(root_bone);
	
	allocate_channels(root_bone);
	
	export_motion_header(name, MAGIC_NUMBER_MOTION);
	total_phase += number_of_frames;
	export_channel_info();
	export_sample();

	return TRUE;
}

int zz_max_exporter_motion::do_export_camera (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	Interval valid = FOREVER;

	camera_node = ip->GetINodeByName(CAMERA_NAME);
	ObjectState os = camera_node->EvalWorldState(0);
	camera = (CameraObject *)os.obj;
	
	if (!camera) {
		popup_message("motion: camera 를 찾을 수 없습니다. ["CAMERA_NAME"]");
		return 0;
	}

	// four 3-component position channel is required.
	// (fovy, near_plane, far_plane)(1), eye(1), center(1), up(1)
	number_of_channels = 4;
	
	export_motion_header(name, MAGIC_NUMBER_MOTION);
	total_phase += number_of_frames;
	export_channel_info_4_camera();
	return export_sample_camera();
}

bool zz_max_exporter_motion::do_export_morph (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	if (!(options & SCENE_EXPORT_SELECTED)) {
		// if no node was selected
		popup_message("motion: 익스포트할 오브젝트를 선택하세요");
		return false;
	}

	INode * selected_node = get_selected_node(ip->GetRootNode());
	
	if (!selected_node) {
		popup_message("motion: 선택한 오브젝트를 찾을 수 없습니다");
		ip->ProgressEnd();
		fclose(pstream);
		return false;
	}
	
	if (!allocate_channels_4_morph(selected_node)) {
		ip->ProgressEnd();
		fclose(pstream);
		return false;
	}
	export_motion_header(name, MAGIC_NUMBER_MOTION);
	export_channel_info_4_morph();
	return export_sample_morph(selected_node);
}

int zz_max_exporter_motion::do_export_object (const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{	
	ZZ_LOG("max_exporter_motion: do_export_object()\r\n");
	
	number_of_channels = 0;
	if (anim_position) number_of_channels++;
	if (anim_rotation) number_of_channels++;
	if (anim_scale) number_of_channels++;

	export_motion_header(name, MAGIC_NUMBER_MOTION);
	total_phase += number_of_frames;
	ZZ_LOG("max_exporter_motion: export_channel_info_4_object()\r\n");
	export_channel_info_4_object();

	INode * selected_node = get_selected_node(ip->GetRootNode());
	ZZ_LOG("max_exporter_motion: export_sample_object(%s)\r\n", selected_node->GetName());
	int ret = export_sample_object(selected_node);
	return ret;
}

// Start the exporter!
// This is the real entrypoint to the exporter. After the user has selected
// the filename (and he's prompted for overwrite etc.) this method is called.
int zz_max_exporter_motion::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	// Grab the interface pointer.
	ip = i;

	// find root_bone
	INode * selected = get_selected_node(ip->GetRootNode());

	// read user property
	read_user_property(selected);

	// Popup Dialog Box
	// Prompt the user with our dialogbox, and get all the options.
	if (suppressPrompts == FALSE) {
		if (!DialogBoxParam(h_instance, MAKEINTRESOURCE(IDD_MOTION_EXPORT_OPTION), ip->GetMAXHWnd(), motion_export_dlg_proc, (LPARAM)this)) {
			return 1;
		}
	}

	// Open file stream
	if (!script_file.open(name, zz_vfs::ZZ_VFS_WRITE, is_asciioutput /* text mode */)) {
		popup_message("cannot open file");
		return 0;
	}

	// Startup the progress bar.
	ip->ProgressStart(get_string(IDS_PROGRESS_MSG), TRUE, fn, NULL);

	int ret;

	switch (anim_type) {
		case ZZ_NONE:
		case ZZ_BIPED:
			root_bone = get_rootbone(selected);
			relink_thigh_to_pelvis(root_bone);
			ret = do_export_biped(name, ei, i, suppressPrompts, options);
			break;
		case ZZ_CAMERA:
			ret = do_export_camera(name, ei, i, suppressPrompts, options);
			break;
		case ZZ_MORPH:
			ret = do_export_morph(name, ei, i, suppressPrompts, options) ? 1 : 0;
			break;
		case ZZ_OBJECT:
			ret = do_export_object(name, ei, i, suppressPrompts, options);
			break;
		default:
			ret = 0;
	}


	// We're done. Finish the progress bar.
	ip->ProgressEnd();

	// Close the stream
	script_file.close();

	return ret;
}

bool zz_max_exporter_motion::allocate_channels (INode* node) 
{
	ObjectState os = node->EvalWorldState(0);
	Control * cont = node->GetTMController();

	// discard biped root bone, dummy node and footsteps
	if (os.obj->ClassID() != Class_ID(DUMMY_CLASS_ID, 0) && 
		(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0) || 
		 os.obj->ClassID() == BONE_OBJ_CLASSID || // New procedural bone object for Magma
		 cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID))
	{
		if (node == root_bone) { // do not use "Bip01" root bone
			// Root bone needs to be positioned and rotated.
			// Pelvis needs to be positioned and rotated, too.
			channel_list[number_of_channels].type = ZZ_CTYPE_POSITION; // position
			channel_list[number_of_channels].bone = node;
			channel_list[number_of_channels].name = node->GetName();
			channel_list[number_of_channels].bone_index = current_bone_index;
			number_of_channels++;

			channel_list[number_of_channels].type = ZZ_CTYPE_ROTATION; // rotation
			channel_list[number_of_channels].bone = node;
			channel_list[number_of_channels].name = node->GetName();
			channel_list[number_of_channels].bone_index = current_bone_index;
			number_of_channels++;
		}
		else {
			channel_list[number_of_channels].type = ZZ_CTYPE_ROTATION; // rotation
			channel_list[number_of_channels].bone = node;
			channel_list[number_of_channels].name = node->GetName();
			channel_list[number_of_channels].bone_index = current_bone_index;
			number_of_channels++;
		}
		current_bone_index++;
	}

	// For each child of this node, we recurse into ourselves 
	// until no more children are found.
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
	child_names.sort();
	list<string>::iterator it;
	map<string, INode *>::iterator map_it;
	for (it = child_names.begin(); it != child_names.end(); it++) {
		map_it = child_map.find(*it);
		allocate_channels(map_it->second);
		//dump_comment("child_name = %s\r\n", (*it).c_str());
	}

	return true;
}

void zz_max_exporter_motion::read_user_property (INode * node)
{
	// set by user defined property
	//morph_use_position = TRUE
	//morph_use_normal = FALSE
	//morph_num_mapchannel = 0
	//morph_use_alpha = FALSE
	//morph_use_textureanim = FALSE

	if (node->GetUserPropBool("morph_use_position", morph_use_position)) {
		ZZ_LOG("max_exporter_motion: morph_use_position\r\n");
	}
	if (node->GetUserPropBool("morph_use_normal", morph_use_normal)) {
		ZZ_LOG("max_exporter_motion: morph_use_normal\r\n");
	}
	if (node->GetUserPropInt("morph_num_mapchannel", morph_num_mapchannel)) {
		ZZ_LOG("max_exporter_motion: morph_get_num_mapchannel(%d)\r\n",
			morph_num_mapchannel);
	}
	if (node->GetUserPropBool("morph_use_alpha", morph_use_alpha)) {
		ZZ_LOG("max_exporter_motion: morph_use_alpha\r\n");
	}
	if (node->GetUserPropBool("morph_use_textureanim", morph_use_textureanim)) {
		ZZ_LOG("max_exporter_motion: morph_use_textureanim\r\n");
	}
	if (node->GetUserPropBool("morph_use_world", morph_use_world)) {
		ZZ_LOG("max_exporter_motion: morph_use_world\r\n");
	}
	if (node->GetUserPropInt("anim_type", anim_type)) {
		ZZ_LOG("max_exporter_motion: anim_type\r\n");
	}
	if (node->GetUserPropBool("anim_position", anim_position)) {
		ZZ_LOG("max_exporter_motion: anim_position\r\n");
	}
	if (node->GetUserPropBool("anim_rotation", anim_rotation)) {
		ZZ_LOG("max_exporter_motion: anim_rotation\r\n");
	}
	if (node->GetUserPropBool("anim_scale", anim_scale)) {
		ZZ_LOG("max_exporter_motion: anim_scale\r\n");
	}
}

int zz_max_exporter_motion::get_num_properties ()
{
	int num_properties = 0;

	if (morph_use_position) num_properties++;
	if (morph_use_normal) num_properties++;
    num_properties += morph_num_mapchannel;
	if (morph_use_alpha) num_properties++;
	if (morph_use_textureanim) num_properties++;
	return num_properties;
}

bool zz_max_exporter_motion::allocate_channels_4_morph (INode * node)
{	
	assert(tri == NULL);
	assert(max_mesh == NULL);

	tri = get_triobject_from_node(node, 0, need_delete);
	
	if (!tri) {
		popup_message("motion: no triangle defined");
		return false; // No triangle objects
	}

	max_mesh = &tri->GetMesh();
	
	number_of_channels = get_num_properties()*max_mesh->getNumFaces()*3; // per face, per 3 vertex

	if (number_of_channels > MAX_NUM_CHANNEL) {
		popup_message("motion: Too many channels");
		return false;
	}
	
	if (need_delete) {
		need_delete = false;
		delete tri;
	}
	return true;
}

void zz_max_exporter_motion::write_channel_info (uint32 channelid, uint32 channeltype, uint32 referid)
{
	char buf[255];
	sprintf(buf, "channeltype_%03d", channelid);
	script_file.write_uint32(buf, channeltype);
	script_file.write_endl();
	
	sprintf(buf, "referid_%03d", channelid);
	script_file.write_uint32(buf, referid);
	script_file.write_endl();
}

void zz_max_exporter_motion::export_channel_info_4_morph ()
{
	int num_properties = get_num_properties();
	int vertex_index = 0;
	int num_vertices = number_of_channels/num_properties;
	int channel_index = 0;
	
	for (int i = 0; i < num_vertices; i++) {
		if (morph_use_position)
			write_channel_info(channel_index++, ZZ_CTYPE_POSITION, i);
		if (morph_use_normal)
			write_channel_info(channel_index++, ZZ_CTYPE_NORMAL, i);
		if (morph_use_alpha)
			write_channel_info(channel_index++, ZZ_CTYPE_ALPHA, i);
		if (morph_num_mapchannel >= 1)
			write_channel_info(channel_index++, ZZ_CTYPE_UV0, i);
		if (morph_num_mapchannel >= 2)
			write_channel_info(channel_index++, ZZ_CTYPE_UV1, i);
		if (morph_num_mapchannel >= 3)
			write_channel_info(channel_index++, ZZ_CTYPE_UV2, i);
		if (morph_num_mapchannel >= 4)
			write_channel_info(channel_index++, ZZ_CTYPE_UV3, i);
		if (morph_use_textureanim)
			write_channel_info(channel_index++, ZZ_CTYPE_TEXTUREANIM, i);
	}
}

void zz_max_exporter_motion::export_channel_info_4_camera ()
{
	// 4 position channel
	write_channel_info(0, ZZ_CTYPE_POSITION, 0); // eye vector
	write_channel_info(1, ZZ_CTYPE_POSITION, 0); // center vector
	write_channel_info(2, ZZ_CTYPE_POSITION, 0); // up vector
	write_channel_info(3, ZZ_CTYPE_POSITION, 0); // fovy, near, far vector
}

void zz_max_exporter_motion::export_channel_info_4_object ()
{
	// position, rotation and scale channel
	int current_channel = 0;
	
	if (anim_position) {
        write_channel_info(current_channel++, ZZ_CTYPE_POSITION, 0); // position
	}
	if (anim_rotation) {
		write_channel_info(current_channel++, ZZ_CTYPE_ROTATION, 0); // rotation
	}
	if (anim_scale) {
        write_channel_info(current_channel++, ZZ_CTYPE_SCALE,    0); // scale
	}
}

void zz_max_exporter_motion::export_channel_info ()
{
	char buf[255];

	for (int i = 0; i < number_of_channels; i++) {
		sprintf(buf, "channeltype_%03d", i);
		script_file.write_uint32(buf, channel_list[i].type);
		script_file.write_endl();

		sprintf(buf, "referid_%03d", i);
		script_file.write_uint32(buf, channel_list[i].bone_index);
		script_file.write_endl();
	}
}

//
// export header section
void zz_max_exporter_motion::export_motion_header(const TCHAR *name, const TCHAR * magic_number)
{
	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	this->start_time = start;

	int delta = GetTicksPerFrame();

	number_of_frames = (end - start) / delta + 1;

	frame_per_second = GetFrameRate();

	//int lap_time = int((float)(number_of_frames) / frame_per_second * ZZ_TIME_TICK);

	script_file.write_string("version", magic_number);
	script_file.write_endl();
	script_file.write_uint32("frame_per_second", frame_per_second);
	script_file.write_endl();
	script_file.write_uint32("num_frames", number_of_frames);
	script_file.write_endl();
	script_file.write_uint32("num_channels", number_of_channels);
	script_file.write_endl();
}

// export skeleton information
int zz_max_exporter_motion::export_sample (void)
{
	char buf[255];
	int i, j;
	Point3 p;
	Quat q;

	for (i = 0; i < number_of_frames; i++) {
		if (ip->GetCancel())
			return 0;
		for (j = 0; j < number_of_channels; j++) {
			sprintf(buf, "frame%03d_channel_%03d", i, j);
			ObjectState os = channel_list[j].bone->EvalWorldState(get_time_by_frame(i));
		
			if (channel_list[j].type == ZZ_CTYPE_POSITION) {
				p = get_world_position(channel_list[j].bone, i);
				script_file.write_float3(buf, p.x, p.y, p.z);
			}
			else if (channel_list[j].type == ZZ_CTYPE_ROTATION) {
				if (channel_list[j].bone == root_bone) {
                    q = get_world_rotation(channel_list[j].bone, i);
				}
				else {
					q = get_local_rotation(channel_list[j].bone, i);
				}
				// ******************************************************
				// CAUTION: max's rotation order does not match engine's
				// thus, we should invert quaternion
				
				q.Invert();  // <<--- DO NOT OMIT THIS AT QUATERNION EXPORT
				
				// *******************************************************

				script_file.write_float4(buf, q.w, q.x, q.y, q.z);
			}
			script_file.write_endl();
		}		
		ip->ProgressUpdate((int)(100*(++current_phase)/total_phase)); 
	}
	return 1;
}

// export camera information
int zz_max_exporter_motion::export_sample_camera (void)
{
	int i;

	for (i = 0; i < number_of_frames; i++) {
		if (ip->GetCancel())
			return 0;		
		write_sample_camera(camera_node, camera, i);
	}
	ip->ProgressUpdate((int)(100*(++current_phase)/total_phase)); 
	return 1;
}

void zz_max_exporter_motion::write_morph_data(uint32 frame,
											  uint32& channelid,
											  Point3 pos,
											  Point3 normal,
											  float alpha,
											  Point3 uv0)
{
	char buf[255];
	if (morph_use_position) {
		sprintf(buf, "frame_%03d_channel_%03d_pos", frame, channelid++);
		script_file.write_float3(buf, pos.x, pos.y, pos.z);
		script_file.write_endl();
	}
	if (morph_use_normal) {
		sprintf(buf, "frame_%03d_channel_%03d_normal", frame, channelid++);
		script_file.write_float3(buf, normal.x, normal.y, normal.z);
		script_file.write_endl();
	}
	if (morph_use_alpha) {
		sprintf(buf, "frame_%03d_channel_%03d_alpha", frame, channelid++);
		script_file.write_float(buf, alpha);
		script_file.write_endl();
	}
	if (morph_num_mapchannel >= 1) {
		sprintf(buf, "frame_%03d_channel_%03d_mapchnnel", frame, channelid++);
		script_file.write_float(buf, uv0.x);
		script_file.write_float(NULL, uv0.y);
		script_file.write_endl();
	}
	
}

float zz_max_exporter_motion::get_material_alpha(INode * node, TimeValue t)
{
	Mtl * material = node->GetMtl();

	if (!material) {
		ZZ_LOG("max_exporter: get_material_alpha() failed. do not have material\r\n");
		return 1.0f;
	}

	// confirm Standard material?
	if (material->ClassID() != Class_ID(DMTL_CLASS_ID, 0)) {
		ZZ_LOG("max_exporter: get_material_alpha() failed. not a standard material\r\n");
		return 1.0f;
	}

	StdMat* std = (StdMat*)material;
	return std->GetOpacity(t);
}

// export vertex morph information
bool zz_max_exporter_motion::export_sample_morph (INode * node)
{
	int frame;
	int i;
	
	int vx1, vx2, vx3;

	get_face_vertex_index(node, vx1, vx2, vx3);

	// copy from zz_max_exporter_max_mesh::export_vertex ()
	// get offsetTM : Object Offset TM = Offset Scale * Offset Rotation * Offset Position
	Matrix3 offsetTM(1);
	offsetTM.PreTranslate(node->GetObjOffsetPos());
	PreRotateMatrix(offsetTM, node->GetObjOffsetRot());
	ApplyScaling(offsetTM, node->GetObjOffsetScale());
	Matrix3 tm = offsetTM;
	
	TimeValue t;
	//ObjectState os;
	Face * f;
	Point3 vn;
	Point3 v;
	int vert;
	UVVert * tvList;
	TVFace * tvFace;
	int texture_index;
	Point3 tvalue;
	uint32 channelid = 0;
	float alpha;

	for (frame = 0; frame < number_of_frames; ++frame) {
		if (ip->GetCancel())
			return false;
		t = get_time_by_frame(frame);
		ZZ_LOG("export_sample_morph() TimeValue(%d)\r\n", t);

		if (morph_use_world) {
            //os = node->EvalWorldState(t);
			tm = node->GetObjTMAfterWSM(t); // CAUTION: this is to world tm
		}
		else {
			// use preset
		}
		tri = get_triobject_from_node(node, t, need_delete);
		if (!tri) {
			return false; // No triangle objects
		}
		max_mesh = &tri->GetMesh();

		max_mesh->buildNormals();

		// 9 = 3(vertices per face) * 3(position + normal + uvw)
		assert(get_num_properties()*max_mesh->getNumFaces()*3 == number_of_channels);

		// currently, does not support mapchannel > 1
		assert(morph_num_mapchannel <= 1);

		// initialize channel id
		channelid = 0;

		//alpha = get_material_alpha(node, t);
		alpha = node->GetVisibility(t);

		for (i = 0; i < max_mesh->getNumFaces(); ++i) { // per face
			f = &max_mesh->faces[i];
			tvFace = max_mesh->mapFaces(1); // diffuse map channel
			tvList = max_mesh->mapVerts(1); // diffuse map channel
			
			vert = f->getVert(vx1);
			//v = max_mesh->getVert(vert) * offsetTM * localTM;
			v = tm * max_mesh->getVert(vert);
			vn = tm * GetVertexNormal(max_mesh, i, max_mesh->getRVertPtr(vert));
			//vn = vn * offsetTM * localTM;
			texture_index = tvFace[i].t[vx1];
			if (!remap_texture_coord(1, node, tvList[texture_index], tvalue)) return false;
			write_morph_data(frame, channelid, v, vn, alpha, tvalue);
			
			vert = f->getVert(vx2);
			v = tm * max_mesh->getVert(vert);
			vn = tm * GetVertexNormal(max_mesh, i, max_mesh->getRVertPtr(vert));
			texture_index = tvFace[i].t[vx2];
			if (!remap_texture_coord(1, node, tvList[texture_index], tvalue)) return false;
			write_morph_data(frame, channelid, v, vn, alpha, tvalue);

			vert = f->getVert(vx3);
			v = tm * max_mesh->getVert(vert);
			vn = tm * GetVertexNormal(max_mesh, i, max_mesh->getRVertPtr(vert));
			texture_index = tvFace[i].t[vx3];
			if (!remap_texture_coord(1, node, tvList[texture_index], tvalue)) return false;
			write_morph_data(frame, channelid, v, vn, alpha, tvalue);
		}

		if (need_delete) {
			need_delete = false;
			delete tri;
		}
	}

	return true;
}

// export object animation information
bool zz_max_exporter_motion::export_sample_object (INode * node)
{
	char buf[255];
	Point3 p, s;
	Quat q;
	TimeValue t;
	Matrix3 localTM, parentWTM, nodeWTM;

	for (int frame = 0; frame < number_of_frames; frame++) {
		if (ip->GetCancel())
			return false;
		
		// move to current frame
		t = get_time_by_frame(frame);
			
		// get local TM
		nodeWTM = node->GetNodeTM(t);
		if (node->GetParentNode()) {
			parentWTM = node->GetParentNode()->GetNodeTM(t);
		}
		else {
			parentWTM.IdentityMatrix();
		}
		localTM = nodeWTM * Inverse(parentWTM);

		AffineParts ap;
		decomp_affine(localTM, &ap);	
		q = ap.q; // essential rotation
		s = ap.k; // stretch factor
		p = ap.t; // translation
		
		//-----------------------------
		// export position channel
		//-----------------------------
		if (anim_position) {
			sprintf(buf, "frame%03d_channel_000", frame);
			p = get_world_position(node, frame);
			script_file.write_float3(buf, p.x, p.y, p.z);
			script_file.write_endl();
		}

		//-----------------------------
		// export rotation channel
		//-----------------------------
		// ******************************************************
		// CAUTION: max's rotation order does not match engine's
		// thus, we should invert quaternion
		// but, max script's rotation order does match engine's
		// 
				
		q.Invert();  // <<--- DO NOT OMIT THIS AT QUATERNION EXPORT
		
		// *******************************************************
		if (anim_rotation) {
			sprintf(buf, "frame%03d_channel_001", frame);
			script_file.write_float4(buf, q.w, q.x, q.y, q.z);
			script_file.write_endl();
		}

		//-----------------------------
		// export scale channel
		//-----------------------------
		if (anim_scale) {
			sprintf(buf, "frame%03d_channel_002", frame);
			//script_file.write_float3(buf, s.x, s.y, s.z);
			script_file.write_float(buf, s.x);
			script_file.write_endl();
		}
		
		ip->ProgressUpdate((int)(100*(++current_phase)/total_phase)); 
	}
	return true;
}


