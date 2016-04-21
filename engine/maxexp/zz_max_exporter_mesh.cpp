/** 
* @file zz_max_exporter_mesh.cpp
* @brief 3ds max exporter mesh class for znzin.
* @author Jiho Choi (zho@korea.com)
* @version 2.0
* @date    09-aug-2002
*
* $Id: zz_max_exporter_mesh.cpp,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
*/

#include "zz_max_exporter_mesh.h"
#include <string>
#include <stdio.h>
#include <map>

using namespace std;

zz_max_exporter_mesh::zz_max_exporter_mesh ()
{
	weld_vertices = true;
	number_of_bones = 0;
	need_delete = false;
	get_string(IDS_LONGDESC_MESH, str_long_desc);
	get_string(IDS_SHORTDESC_MESH, str_short_desc);
}

zz_max_exporter_mesh::~zz_max_exporter_mesh ()
{
	if (need_delete) {
		need_delete = false;
		delete tri;
	}
}

const TCHAR * zz_max_exporter_mesh::Ext (int n)
{
	return get_string(IDS_EXT_MESH);
}

// Dialog proc
static BOOL CALLBACK mesh_export_dlg_proc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	zz_max_exporter_mesh * exp = reinterpret_cast<zz_max_exporter_mesh *>((LPARAM)GetWindowLong(hWnd, GWL_USERDATA));

	switch (msg) {
	case WM_INITDIALOG:
		exp = (zz_max_exporter_mesh *)lParam;
		SetWindowLong(hWnd, GWL_USERDATA, LONG(lParam)); 
		//SetDlgItemText(hWnd, IDC_NUM_MAP_CHANNEL, "1");

		// Setup the spinner controls for the controller key sample rate 
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
	case IDOK:
		exp->set_asciioutput(IsDlgButtonChecked(hWnd, IDC_ASCIIOUTPUT) == TRUE);
		if (IsDlgButtonChecked(hWnd, IDC_NOT_SHARE_VERTEX) == TRUE) {
			exp->set_dynamic(true);
			exp->set_weld_vertices(false);
		}
		else {
			exp->set_dynamic(false);
			exp->set_weld_vertices(true);
		}
		//char strNumMapChannel[255];
		//GetDlgItemText(hWnd, IDC_NUM_MAP_CHANNEL, strNumMapChannel, 255);
		//exp->num_map_channel = atoi(strNumMapChannel);

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

void zz_max_exporter_mesh::read_vertex_format (INode* node)
{
	// check user property if use skin info
	BOOL mesh_use_skin, mesh_use_normal;
	int mesh_num_mapchannel;
	int vertex_format = (int)ZZ_VF_POSITION;

	// set by user defined property
	if (node->GetUserPropBool("mesh_use_skin", mesh_use_skin)) {
		ZZ_LOG("max_exporter_mesh: mesh_use_skin\r\n");
		if (mesh_use_skin) {
			vertex_format |= (int)ZZ_VF_BLEND_WEIGHT | (int)ZZ_VF_BLEND_INDEX;
		}
	}
	else { // set by default
		zz_skin_info skin_info;
		zz_phy_info phy_info;
		if (skin_info.get_modifier(node) ||
			phy_info.get_modifier(node))
		{
			vertex_format |= (int)ZZ_VF_BLEND_WEIGHT | (int)ZZ_VF_BLEND_INDEX;
		}
	}

	if (node->GetUserPropBool("mesh_use_normal", mesh_use_normal)) {
		ZZ_LOG("max_exporter_mesh: mesh_use_normal\r\n");
		if (mesh_use_normal) {
			vertex_format |= ZZ_VF_NORMAL;
		}
	}
	else { // set by default
		vertex_format |= ZZ_VF_NORMAL;
	}

	if (node->GetUserPropInt("mesh_num_mapchannel", mesh_num_mapchannel)) {
		ZZ_LOG("max_exporter_mesh: mesh_get_num_mapchannel(%d)\r\n", mesh_num_mapchannel);
		if (mesh_num_mapchannel >= 1) {
			vertex_format |= ZZ_VF_UV0;
		}
		if (mesh_num_mapchannel >= 2) {
			vertex_format |= ZZ_VF_UV1;
		}
		if (mesh_num_mapchannel >= 3) {
			vertex_format |= ZZ_VF_UV2;
		}
		if (mesh_num_mapchannel >= 4) {
			vertex_format |= ZZ_VF_UV3;
		}
	}
	else { // set by default
		vertex_format |= ZZ_VF_UV0;
	}

	engine_mesh->set_vertex_format(vertex_format);
	//engine_mesh->vertex_format = vertex_format;
}

Matrix3& zz_max_exporter_mesh::get_offsetTM (INode * node, bool has_skin, Matrix3& tm)
{
	if (has_skin) { // In out exporter, skinned mesh uses its orientation for world orientation.
		tm = node->GetObjTMAfterWSM(0);
	}
	else { // get offset tm
		// get offsetTM : Object Offset TM = Offset Scale * Offset Rotation * Offset Position
		Matrix3 offsetTM(1);
		offsetTM.PreTranslate(node->GetObjOffsetPos());
		PreRotateMatrix(offsetTM, node->GetObjOffsetRot());
		ApplyScaling(offsetTM, node->GetObjOffsetScale());
		tm = offsetTM;
	}
	return tm;
}

bool zz_max_exporter_mesh::read_into_engine_mesh (INode * node)
{
	if (!node) return false;

	ObjectState os = node->EvalWorldState(0);

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return false; // Safety net. This shouldn't happen.
	}

	// BE CAREFUL: tri and max_mesh deallocate
	if (!(tri = get_triobject_from_node(node, 0, need_delete))) {
		return false; // No triangle objects
	}

	ZZ_LOG("read_into_engine_mesh(). buildNormals()\r\n");
	max_mesh = &tri->GetMesh();

	max_mesh->buildNormals();

	Matrix3 tm;
	Matrix3 tm_rot; // for rotation only

	const zz_vertex_format& format = engine_mesh->get_vertex_format();

	// get localTM : NodeLocalTM = NodeWorldTM * Inverse(ParentWorldTM)
	tm = get_offsetTM(node, format.use_skin(), tm);

	tm_rot = tm;
	tm_rot.NoTrans();
	tm_rot.NoScale();

	ZZ_LOG("read_into_engine_mesh(). read_minmax()\r\n");
	INode * bbox = find_bbox_node(node);
	Matrix3 bbox_tm;
	Mesh * bbox_mesh = NULL;
	
	TriObject * tobj;
	bool need_to_del = false;

	if (bbox) {
		bbox_tm = get_offsetTM(bbox, format.use_skin(), bbox_tm);
		ObjectState os = bbox->EvalWorldState(0);
		if (os.obj->CanConvertToType( triObjectClassID )) {
			Object * oldobj = os.obj;
            tobj = (TriObject *)(os.obj->ConvertToType(0, triObjectClassID));
			need_to_del = (tobj != oldobj);
		}
		else {
			ZZ_LOG("read_into_engine_mesh() failed. bbox exists but cannot convert to triobject.\n");
			return false;
		}
		bbox_mesh = &tobj->GetMesh();
	}
	else {
		bbox_tm = tm;
		bbox_mesh = max_mesh;
	}

	read_minmax(bbox_mesh, &bbox_tm);

	if (need_to_del) {
		tobj->DeleteThis();
	}
	
	zz_edit_normal en(node);

	// [ number_of_vertices : U ]
	int num_faces = max_mesh->getNumFaces();
	
	if (num_faces == 0) {
		popup_message("no faces");
		return false;
	}
	if (max_mesh->faces == NULL) {
		popup_message("no faces array");
		return false;
	}

	if (en.enabled()) {
		// In max5, EditNormal does not work for TriMesh.
		// In max6, it's OK for EditMesh and EditPoly.
		if (en.GetNumFaces() != num_faces) { // not same face
			en.disable();
		}
	}

	int vx1, vx2, vx3;
	int vi[3];
	Point3 v[3], vp, vn;
	int i, j, k;
	Face * f;

	////////////////////////////
	// initialize

	// get face-vertex ordering
	get_face_vertex_index(node, vx1, vx2, vx3);

	ZZ_LOG("read_into_engine_mesh(). set num verts and faces\r\n");
	engine_mesh->set_num_verts(num_faces*3);
	engine_mesh->set_num_faces(num_faces);

	////////////////////////
	// get vertex position
	for (i = 0; i < num_faces; i++) {
		f = &max_mesh->faces[i];
		v[0] = max_mesh->verts[f->getVert(vx1)];
		v[1] = max_mesh->verts[f->getVert(vx2)];
		v[2] = max_mesh->verts[f->getVert(vx3)];
		vi[0] = f->getVert(vx1);
		vi[1] = f->getVert(vx2);
		vi[2] = f->getVert(vx3);

		// read positions
		for (j = 0; j < 3; j++) {
			vp = tm * v[j];
			// floor 3 digit after decimal point
			vp.x = floorf(vp.x*1000.0f)/1000.f;
			vp.y = floorf(vp.y*1000.0f)/1000.f;
			vp.z = floorf(vp.z*1000.0f)/1000.f;
			// scale
			vp.x *= ZZ_SCALE_IN;
			vp.y *= ZZ_SCALE_IN;
			vp.z *= ZZ_SCALE_IN;
			engine_mesh->set_pos(i*3+j, vec3(vp.x, vp.y, vp.z));
		}

		// read normals
		if (format.use_normal()) {
			for (j = 0; j < 3; j++) {
				if (en.enabled()) { // get normal from EditNormals modifier.
					vn = tm_rot * en.GetNormal(i, j);
				}
				else {
					vn = tm_rot * GetVertexNormal
						(max_mesh, i, max_mesh->getRVertPtr(vi[j]));
				}
				engine_mesh->set_normal(i*3+j, vec3(vn.x, vn.y, vn.z));
			}
		}
	}
	ZZ_LOG("read_into_engine_mesh(). set vertex position and normal done\r\n");

	//////////////////////////////////////////////////
	// read skinning info (blend_weight, blend_index)
	zz_skin_info skin_info;
	zz_phy_info phy_info;
	uivec4 blend_index;
	vec4 blend_weight;
	typedef map<int, int> bone_indices_t;
	bone_indices_t bone_indices;
	float weight_sum = 0.0f;

	// check skinning info
	if (format.use_skin()) {
		ZZ_LOG("read_into_engine_mesh() use_skin\r\n");
		if (skin_info.get_modifier(node)) {
			skin_info.get_info(node);
		}
		else if (phy_info.get_modifier(node)) {
			phy_info.get_info(node);
		}
		else {
			popup_message("no skinning info");
			return false;
		}
		for (i = 0; i < num_faces; i++) {
			f = &max_mesh->faces[i];
			// vertex index
			vi[0] = f->getVert(vx1);
			vi[1] = f->getVert(vx2);
			vi[2] = f->getVert(vx3);
			// vertex
			v[0] = max_mesh->verts[vi[0]];
			v[1] = max_mesh->verts[vi[1]];
			v[2] = max_mesh->verts[vi[2]];

			// for each three vertex
			for (j = 0; j < 3; j++) {
				if (skin_info.skin_mod) { // if uses skin
					if (!get_skin_blend_info(&skin_info, vi[j],
						blend_weight, blend_index))	return false;
				}
				else if (phy_info.phy_mod) {
					if (!get_physique_blend_info(&phy_info, vi[j],
						blend_weight, blend_index))	return false;
				}
				else {
					popup_message("no skinning modifier info");
					ZZ_LOG("max_exporter_mesh: read_into_engine_mesh() failed. no skinning modifier\r\n");
					ZZ_LOG("  face_num(%d), vertex_num(%d)\r\n", i, j);
					return false;
				}
				// for each four indices
				weight_sum = 0.0f;
				for (k = 0; k < 4; k++) {
					// rearrange blend_index
					if (blend_weight[k] > 0.0f) { // insert into bone_indices
						// if already exist
						if (bone_indices.find(blend_index[k]) != bone_indices.end()) {
							// remap blend_index to existing index
							blend_index[k] = bone_indices[blend_index[k]];
						}
						else { // if not exist
							// map blend_index to new index
							int new_index = (int)bone_indices.size();
							bone_indices[blend_index[k]] = new_index;
							// insert original bone index into bone_indices
							engine_mesh->bone_indices.push_back(blend_index[k]);
							blend_index[k] = new_index;
						}
					}
					else { // ignore if weight <= 0.0f
						blend_index[k] = 0;
						blend_weight[k] = 0.0f;
					}
					weight_sum += blend_weight[k];
				}
				if (weight_sum < 0.99f) { // weight_sum is less than 1.0f
					ZZ_LOG("read_into_engine_mesh() failed. total weight_sum is not 1.0f\r\n");
					popup_message("본에 영향을 (거의) 받지 않는 버텍스가 존재합니다.");
					return false;
				}
				// set skinning info
				//engine_mesh->blend_indices.push_back(blend_index);
				//engine_mesh->blend_weights.push_back(blend_weight);
				engine_mesh->set_blend_weight(i*3+j, blend_weight);
				engine_mesh->set_blend_index(i*3+j,
					vec4((float)blend_index.x, (float)blend_index.y, (float)blend_index.z, (float)blend_index.w));
			}
		}
	} // end of use_skin()
	ZZ_LOG("read_into_engine_mesh(). set skin done\r\n");

	/////////////////////////
	// read map channel info
	//
	// multiple map channel support :
	// The mesh mapping channel may be specified as one of the following: 
	// 0: Vertex Color channel.
	// 1: Default mapping channel (the TVert array).
	// 2 through MAX_MESHMAPS-1: The new mapping channels available in release 3.0.
	//
	// it include not-supported map channel and vertex color channel
	int total_map_channel = max_mesh->getNumMaps();
	int num_map_channel = 0; // re-counted num map channel
	// count the number of mapchanels
	// do not count color channel
	ZZ_LOG("max_exporter_mesh: total %d map channel exist\r\n", total_map_channel);
	for (i = 1; i <= total_map_channel; ++i) {
		if (max_mesh->mapSupport(i)) {
			ZZ_LOG("max_exporter_mesh: %d/%d map channel supported\r\n",
				i, total_map_channel);
			num_map_channel++;
		}
	}
	if (num_map_channel < format.get_num_mapchannel()) {
		ZZ_LOG("max_exporter_mesh: map channel does not match. \nsupported(%d). specified(%d)\r\n",
			num_map_channel, format.get_num_mapchannel());
		char msg[256];
		sprintf(msg, "존재하는 채널개수(%d)보다 지정된 채널개수(%d)가 더 많습니다.\nmesh_num_mapchannel 속성을 수정하세요.", 
			num_map_channel, format.get_num_mapchannel());
		popup_message(msg);
        return false;
	}
	int map_channel_index;
	UVVert * tvlist;
	TVFace * tvface;
	Point3 tvalue;
	int count = 0;
	
	ZZ_LOG("max_exporter_mesh: read mapchannel...\r\n");
	
	for (map_channel_index = 1, count = 0;
		(map_channel_index <= total_map_channel) && 
		(count < format.get_num_mapchannel());
		++map_channel_index)
	{
		if (max_mesh->mapSupport(map_channel_index) == FALSE) {
			// if not supported map channel, then just skip this
			ZZ_LOG("max_exporter_mesh: this channel(%d) is not supported\r\n",
				map_channel_index);
			continue;
		}
		if (count >= num_map_channel) {
			// if not supported map channel, then just skip this
			ZZ_LOG("max_exporter_mesh: num_map_channel(%d) exceeded\r\n",
				num_map_channel);
			continue;
		}
		ZZ_LOG("max_exporter_mesh: read map_channel(%d)\r\n", map_channel_index);
		tvlist = max_mesh->mapVerts(map_channel_index);
		ZZ_LOG("max_exporter_mesh: tvlist(%x)\r\n", tvlist);
		tvface = max_mesh->mapFaces(map_channel_index);
		ZZ_LOG("max_exporter_mesh: tvface(%x)\r\n", tvface);

		if (!tvlist || !tvface) {
			popup_message("텍스쳐 UVW 값이 들어있지 않습니다.");
			return false;
		}
		//int num_tvlist = max_mesh->getNumMapVerts(map_channel_index);

		for (i = 0; i < num_faces; i++) {
			//ZZ_LOG("max_exporter_mesh: face(%d)...\r\n", i);
			vi[0] = tvface[i].getTVert(vx1);
			vi[1] = tvface[i].getTVert(vx2);
			vi[2] = tvface[i].getTVert(vx3);
			//ZZ_LOG("max_exporter_mesh: tvert(%d, %d, %d)\r\n", vi[0], vi[1], vi[2]);
			for (j = 0; j < 3; j++) {
				if (!remap_texture_coord(map_channel_index, node, tvlist[vi[j]], tvalue)) return false;

				if (count == 0)
					engine_mesh->set_uv(i*3+j, 0, vec2(tvalue.x, tvalue.y));
				if (count == 1)
					engine_mesh->set_uv(i*3+j, 1, vec2(tvalue.x, tvalue.y));
				if (count == 2)
					engine_mesh->set_uv(i*3+j, 2, vec2(tvalue.x, tvalue.y));
				if (count == 3)
					engine_mesh->set_uv(i*3+j, 3, vec2(tvalue.x, tvalue.y));
			}
			//ZZ_LOG("max_exporter_mesh: face(%d) ok\r\n", i);
		}
		ZZ_LOG("max_exporter_mesh: map channel(%d)->uv(%d) wrote\r\n", map_channel_index, count+1);
		++count;
	}

	//////////////////
	// read face info
	// sort faces by material id
	ZZ_LOG("max_exporter_mesh: num_faces(%d)\r\n", num_faces);
	typedef std::multimap<int, int> type_mat_face; // key = material id, value = face index
	typedef std::pair<int, int> type_pair;

	type_mat_face matface;
	int mat_id;
	int num_matids = 0;
	for (i = 0; i < num_faces; i++) {
		mat_id = max_mesh->getFaceMtlIndex(i); // 0-based
		matface.insert(type_pair(mat_id, i));
		if ((mat_id+1) > num_matids) {
			num_matids = mat_id + 1;
		}
	}

	// num_matids does not consider zero-sized material bins.
	// but, real_num_matids do not count zero-sized material bins.
	int real_num_matids = 0; // do not count zero 
	for (i = 0; i < num_matids; i++) {
		count = (int)matface.count(i);
		if (count > 0) { // count only real
			real_num_matids++;
		}
	}
	// set number of faces with same material id
	if (real_num_matids == 1) {
		// only one material id means no material id
		real_num_matids = 0; // ignore single material id
	}
	engine_mesh->set_num_matids(real_num_matids);
	if (real_num_matids > 0) {
		for (int real_i = 0, i = 0; i < num_matids; i++) {
			count = (int)matface.count(i);
			if (count > 0) {
				engine_mesh->set_matid_numfaces(real_i++, count); // increase real index
				ZZ_LOG("mat_id(%d) : count(%d)\r\n", i, count);
			}
		}
		assert(real_i == real_num_matids);
	}
	
	int face_index;
	std::multimap<int, int>::iterator it;
	for (it = matface.begin(), count = 0; it != matface.end(); it++, count++) {
		//mat_id = it->first;
		face_index = it->second;

		// we now use sorted-vertex by original face,
		// so we can calculate vertex-index directly by face-index.
		vi[0] = face_index*3 + 0;
		vi[1] = face_index*3 + 1;
		vi[2] = face_index*3 + 2;

		//ZZ_LOG("max_exporter_mesh: #%d: [%d : mat_id]-[%d : face_index]\r\n", count, mat_id, face_index);

		engine_mesh->set_face(count, usvec3(vi[0], vi[1], vi[2]));
	}

	if (need_delete) {
		need_delete = false;
		max_mesh = NULL;
		delete tri;
	}
	return true;
}

int zz_max_exporter_mesh::DoExport (const TCHAR *name, ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	ZZ_LOG("max_exporter_mesh: DoExport(%s) begin\r\n", name);

	if (!(options & SCENE_EXPORT_SELECTED)) {
		// if no node was selected
		popup_message("mesh: 익스포트할 오브젝트를 선택하세요");
		return 0;
	}

	// Grab the interface pointer.
	ip = i;

	// Popup Dialog Box
	// Prompt the user with our dialogbox, and get all the options.
	if (suppressPrompts == FALSE) {
		if (!DialogBoxParam(h_instance, MAKEINTRESOURCE(IDD_MESH_EXPORT_OPTION), ip->GetMAXHWnd(), mesh_export_dlg_proc, (LPARAM)this)) {
			return 0;
		}
	}
 
	if (ip->GetSelNodeCount() > 1) {
		popup_message("mesh: 하나의 오브젝트만 선택하세요");
		return 0;
	}

	// Startup the progress bar.
	ip->ProgressStart(get_string(IDS_PROGRESS_MSG), TRUE, fn, NULL);

	ZZ_LOG("max_exporter_mesh: call get_selected_node()\r\n");
	INode * selected_node = get_selected_node(ip->GetRootNode());
	
	if (!selected_node) {
		popup_message("mesh: 선택한 오브젝트를 찾을 수 없습니다");
		ip->ProgressEnd();
		return 0;
	}

	// check scale
	Matrix3 nodetm;
	AffineParts parts;
    
	nodetm = selected_node->GetNodeTM(0);
	decomp_affine(nodetm, &parts);
	Point3 scale(parts.k);
	ZZ_LOG("max_exporter_mesh: scale_component(%f, %f, %f)\r\n", scale.x, scale.y, scale.z);
	
	const float epsilon = 0.001f;

	if ((scale.x > 1.0f + epsilon) || (scale.x < 1.0f - epsilon) ||
		(scale.y > 1.0f + epsilon) || (scale.y < 1.0f - epsilon) ||
		(scale.z > 1.0f + epsilon) || (scale.z < 1.0f - epsilon))
	{
		ip->ProgressEnd();
		char message[256];
		sprintf(message, "mesh: 스케일이 (1, 1, 1)이 아닙니다.\n(%f, %f, %f)\n\n계속하시겠습니까?",
			scale.x, scale.y, scale.z);
		if (!popup_message(message, true)) {
			ip->ProgressEnd();
			return 0;
		}
	}

	ZZ_LOG("max_exporter_mesh: read_vertex_format()\r\n");
	read_vertex_format(selected_node);

	ZZ_LOG("check skin info\r\n");
	const zz_vertex_format& format = engine_mesh->get_vertex_format();
	// find root bone by skinng info
	// check if has skinning info
	if (format.use_skin()) {
		zz_skin_info skin_info;
		zz_phy_info phy_info;
		ZZ_LOG("skip constructed\r\n");
		if (skin_info.get_modifier(selected_node)) {
			ZZ_LOG("skip used\r\n");
			skin_info.get_info(selected_node);
			root_bone = get_rootbone(ip->GetINodeByName(PELVIS_NAME));
			if (!root_bone) {
				popup_message("Cannot find [" PELVIS_NAME "]");
				ZZ_LOG("cannot find root_bone for skin.\r\n");
			}
			else {
				ZZ_LOG("found skin mod. root = %s\r\n", root_bone->GetName());
			}
		}
		else if (phy_info.get_modifier(selected_node)) {
			ZZ_LOG("physique used\r\n");
			phy_info.get_info(selected_node);
			root_bone = get_rootbone(ip->GetINodeByName(PELVIS_NAME));
			if (!root_bone) {
				popup_message("Cannot find [" PELVIS_NAME "]");
				ZZ_LOG("cannot find root_bone for skin\r\n");
			}
			else {
				ZZ_LOG("found physique mod. root = %s\r\n", root_bone->GetName());
			}
		}
		else {
			popup_message("mesh: no skin or physique info");
			ip->ProgressEnd();
			return 0;
		}
		if (!root_bone) {
			popup_message("mesh: pelvis를 찾을 수 없습니다.");
			ip->ProgressEnd();
			return 0;
		}
	}
	else {
		// clear root_bone pointer
		root_bone = NULL;
		ZZ_LOG("no skin or physique\r\n");
	}

	relink_thigh_to_pelvis(root_bone);


	int current_phase = 1;
	int total_phase = 6;

	if (root_bone) figure_mode_begin(root_bone); // for biped
	{		
		// saves all bone's name and pointer
		if (root_bone) {
			ZZ_LOG("max_exporter_mesh: allocate_bones(%S)\r\n", root_bone->GetName());
			allocate_bones(root_bone);
		}
		ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

		ZZ_LOG("max_exporter_mesh: read_into_engine_mesh()\r\n");
		bool ret = read_into_engine_mesh(selected_node);

		ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 

		ZZ_LOG("max_exporter_mesh: save_mesh()\r\n");
		if (!ret) {
			ip->ProgressEnd();
			return 0;
		}
		
		BOOL mesh_use_matid;

		if (selected_node->GetUserPropBool("mesh_use_matid", mesh_use_matid)) {
			if (!mesh_use_matid) {
				engine_mesh->set_num_matids(0);
			}
		}

		if (get_weld_vertices()) {
			znzin->mesh_tool.weld_vertices(engine_mesh);
			ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 
			engine_mesh->generate_strip();
			ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 
		}
		else {
			ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 
			ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 
		}

		znzin->mesh_tool.save_mesh(name, engine_mesh, is_asciioutput);
		ip->ProgressUpdate((int)(100*current_phase++/total_phase)); 
	}
	if (root_bone) figure_mode_end();

 
	// We're done. Finish the progress bar.
	ip->ProgressEnd();

	ZZ_LOG("max_exporter_mesh: DoExport() end successfully\r\n");
	return 1;
}

// get four biggest value
// sort weights in reverse order
void zz_max_exporter_mesh::get_assignments(int& dest_size, int dest_index[], float dest_weight[],  
					 int src_size, int src_index[], float src_weight[])
{
	float big_weight;
	int big_index;
	float eps = .1f;
	int i, j;
	dest_size = 0; // minimum size
	int max_assign = 4;
	float sum = 0.0f;
	int selected_index = -1;

	// get first
	for (j = 0; j < max_assign; ++j) {
		dest_weight[j] = 0.0f; // default weight
		dest_index[j] = 0; // default index
		big_weight = -1;
		big_index = -1;
		// find the biggest index
		for (i = 0; i < src_size; ++i) {
			if (src_weight[i] >= big_weight) {
				big_weight = src_weight[i];
				big_index = src_index[i];
				selected_index = i;
			}
		}

		// if the weight can be accepted
		// if (big_weight > eps) {
		if (big_weight > 0) {
			dest_index[dest_size] = big_index;
			dest_weight[dest_size] = big_weight;
			sum += big_weight;
			dest_size++;
			// invalidate the selected source index
			assert(selected_index >= 0);
			src_weight[selected_index] = -1; // this is for next-time ignorance
		}
	}
	
	assert(dest_size >= 1);
	
	// recalc it
	dest_weight[0] = 1.0f - (dest_weight[1] + dest_weight[2] + dest_weight[3]);
	//dest_size = (dest_size == 0) ? 1 : dest_size;

	// normalize
	//for (j = 0; j < dest_size; ++j) {
	//	dest_weight[j] /= sum;
	//}
}

bool zz_max_exporter_mesh::get_physique_blend_info (
	zz_phy_info * pi, 
    int vertex_index,
    vec4& weights,
    uivec4& indices
    )
{
	IPhyBlendedRigidVertex * vtx_blend = NULL;
	IPhyRigidVertex * vtx_rigid = NULL;
	IPhyVertexExport* vtx_export = NULL;
	int num_assignments = 0;
	unsigned int index_of_bone;
	bool result = true;
	int j;

	if (pi->phy_context)
		vtx_export = pi->phy_context->GetVertexInterface(vertex_index);

	if (vtx_export && vtx_export->GetVertexType() == RIGID_BLENDED_TYPE) {
		vtx_blend = (IPhyBlendedRigidVertex *)vtx_export;

		if (vtx_blend) {
			num_assignments = vtx_blend->GetNumberNodes();
		}
		else {
			num_assignments = 0;
		}
		if (num_assignments > MAX_BONE_ASSIGNMENTS) {
            popup_message("num_assignments <= 4");
			return false;
		}
		for (j=0; j < MAX_BONE_ASSIGNMENTS; j++) {
			if (j < num_assignments) {
                index_of_bone = get_bone_index(vtx_blend->GetNode(j));
				indices[j] = index_of_bone;
				weights[j] = vtx_blend->GetWeight(j);
			}
			else {
				indices[j] = 0; // or -1.0f
				weights[j] = 0.0f;
			}
		} 
	}
	else if (vtx_export && (vtx_export->GetVertexType() == RIGID_NON_BLENDED_TYPE)) {
		vtx_rigid = (IPhyRigidVertex *)vtx_export;
		index_of_bone = get_bone_index(vtx_rigid->GetNode());

		indices = uivec4_null;
		weights = vec4_null;
		indices.x = index_of_bone;
		weights.x = 1.0f;
	}
	else {
		result = false;
	}

	if (pi->phy_context && vtx_export) {
		pi->phy_context->ReleaseVertexInterface(vtx_export);
	}
	return result;
}

bool zz_max_exporter_mesh::get_skin_blend_info (
	zz_skin_info * si,
	int vertex_index,
	vec4& weights,
    uivec4& indices
	)
{
	int index;
	int size;
	int j;

	size = si->skin_context->GetNumAssignedBones(vertex_index);
	
	if (size == 0) {
		popup_message("본에 영향을 받지 않는 버텍스가 존재합니다.");
		return false;
	}
	if (size > MAX_BONE_ASSIGNMENTS) {
		popup_message("bone affect limit <= 4");
		return false;
	}
	indices = uivec4_null;
	weights = vec4_null;
	ZZ_LOG("skin(v%03d) : size(%d)", vertex_index, size);
	
	for (j = 0; j < MAX_BONE_ASSIGNMENTS; ++j) {
		if (j < size) {
			index = get_bone_index(si->skin->GetBone(si->skin_context->GetAssignedBone(vertex_index,j)));
			//ZZ_LOG("[%s] ",	si->skin->GetBone(si->skin_context->GetAssignedBone(vertex_index,j))->GetName());

			if (index == -999) {
				char msg[255];
				sprintf(msg, "mesh: Missing Bone is %s", si->skin->GetBone(si->skin_context->GetAssignedBone(vertex_index,j))->GetName());
				popup_message(msg);
				return false;
			}
			indices[j] = index;
			weights[j] = si->skin_context->GetBoneWeight(vertex_index, j);
		}
		else {
			indices[j] = 0;
			weights[j] = 0.0f;
			//ZZ_LOG("%d >= size\r\n", j);
		}
	}
	ZZ_LOG("-(%d, %d, %d, %d)-(%f, %f, %f, %f)\r\n",
		indices[0], indices[1], indices[2], indices[3], 
		weights[0], weights[1], weights[2], weights[3]);

	return true;
}

// read bounding volume min-max into the mesh
void zz_max_exporter_mesh::read_minmax (Mesh* mesh_in, Matrix3 * tm)
{
	if (!mesh_in) return;

	Box3 bbox = mesh_in->getBoundingBox(tm);
	Point3 pmin(bbox.Min()), pmax(bbox.Max());
	pmin.x *= ZZ_SCALE_IN;
	pmin.y *= ZZ_SCALE_IN;
	pmin.z *= ZZ_SCALE_IN;
	pmax.x *= ZZ_SCALE_IN;
	pmax.y *= ZZ_SCALE_IN;
	pmax.z *= ZZ_SCALE_IN;
	engine_mesh->set_min(vec3(pmin.x, pmin.y, pmin.z));
	engine_mesh->set_max(vec3(pmax.x, pmax.y, pmax.z));

	ZZ_LOG("set_minmax(). min(%f, %f, %f), max(%f, %f, %f)\n", pmin.x, pmin.y, pmin.z, pmax.x, pmax.y, pmax.z);

	//Point3 center = bbox.Center();
	//Point3 width = bbox.Width();
	//dump_float3(center.x, center.y, center.z, "center");
	//dump_float3(width.x/2, width.y/2, width.z/2, "half_length");
}

#define BBOX_NAME "bbox"

INode * zz_max_exporter_mesh::find_bbox_node (INode * node)
{
	INode * child;
	for (int i = 0; i < node->NumberOfChildren(); i++) {
		child = node->GetChildNode(i);
		if (strncmp(child->GetName(), BBOX_NAME, strlen(BBOX_NAME)) == 0) {
			// found bbox
			//ObjectState os = child->EvalWorldState(0);
			//if (os.obj->ClassID() != Class_ID( BOXOBJ_CLASS_ID, 0 )) {
			//	continue;
			//}
           	ZZ_LOG("max_exporter_mesh:find_bbox_node(%s)\n", child->GetName());
			return child;
		}
	}
	return NULL;
}

void zz_max_exporter_mesh::set_dynamic (bool true_or_false)
{
	engine_mesh->set_dynamic(true_or_false);
}
