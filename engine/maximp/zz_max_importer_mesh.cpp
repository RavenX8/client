/** 
 * @file zz_max_import.h
 * @brief 3ds max importer base class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    05-apr-2003
 *
 * $Header: /engine/maximp/zz_max_importer_mesh.cpp 3     04-02-09 8:41p Zho $
 */

#include "zz_max_importer_mesh.h"

extern TCHAR * GetString (int id);

const TCHAR *
zz_max_importer_mesh::LongDesc() {			// Long ASCII description (i.e. "Targa 2.0 Image File")
	return GetString(IDS_LONGDESC_MESH);
}

const TCHAR *
zz_max_importer_mesh::ShortDesc() {			// Short ASCII description (i.e. "Targa")
	return GetString(IDS_SHORTDESC_MESH);
}

zz_max_importer_mesh::zz_max_importer_mesh()
{
	engine_mesh = zz_new zz_mesh;
}

zz_max_importer_mesh::~zz_max_importer_mesh()
{
	zz_delete engine_mesh;
}

bool zz_max_importer_mesh::mesh_file_load (const TCHAR* filename, Mesh * mesh)
{
	bool ret = znzin->mesh_tool.load_mesh(filename, engine_mesh);

	if (!ret) return false;

	ZZ_LOG("max_importer_mesh: mesh_file_load(%s)\n", filename);

#define MAX_MAP 4

	int map_indices[] = {1, 3, 4, 5};
	bool map_support[MAX_MAP] = {false, false, false, false}; // max_map is 4
	int i;
	int num_verts, num_faces;
	Point3 pos, normal;

	//------------------------------------------------------
	// set vertex & face info
	num_verts = (int)engine_mesh->get_num_verts();
	num_faces = (int)engine_mesh->get_num_faces();
	mesh->setNumVerts(num_verts);
	mesh->setNumFaces(num_faces);

	const zz_vertex_format& format = engine_mesh->get_vertex_format();
	//------------------------------------------------------
	// set map info
	if (format.get_num_mapchannel() > 0) {
		map_support[0] = true;
		ZZ_LOG("max_importer_mesh: uv0s support\n");
	}
	if (format.get_num_mapchannel() > 1) {
		map_support[1] = true;
		ZZ_LOG("max_importer_mesh: uv1s support\n");
	}
	if (format.get_num_mapchannel() > 2) {
		map_support[2] = true;
		ZZ_LOG("max_importer_mesh: uv2s support\n");
	}
	if (format.get_num_mapchannel() > 3) {
		map_support[3] = true;
		ZZ_LOG("max_importer_mesh: uv3s support\n");
	}
	// testcode
	if (format.get_num_mapchannel() < 1) {
		ZZ_LOG("max_importer_mesh: mapchannel < 2\n");
		return false;
	}

	mesh->setNumTVFaces(num_faces);
	mesh->setNumTVerts(num_verts);
	for (i = 0; i < MAX_MAP; i++) {
		mesh->setMapSupport(map_indices[i], map_support[i]);
		if (map_support[i]) {
			mesh->setNumMapVerts(map_indices[i], num_verts);
			mesh->setNumMapFaces(map_indices[i], num_faces);
		}
	}
	ZZ_LOG("max_importer_mesh: map info done\n");

	//------------------------------------------------------
	// set vertex
	for (i = 0; i < num_verts; i++) {
		pos.x = engine_mesh->get_pos(i).x*ZZ_SCALE_OUT;
		pos.y = engine_mesh->get_pos(i).y*ZZ_SCALE_OUT;
		pos.z = engine_mesh->get_pos(i).z*ZZ_SCALE_OUT;
		mesh->setVert(i, pos);

		normal.x = engine_mesh->get_normal(i).x;
		normal.y = engine_mesh->get_normal(i).y;
		normal.z = engine_mesh->get_normal(i).z;
		mesh->setNormal(i, normal);
	}
	ZZ_LOG("max_importer_mesh: vertex done\n");

	//------------------------------------------------------
	// set map vertex
	if (map_support[0]) {
		UVVert * uvs = new UVVert[num_verts];
		for (i = 0; i < num_verts; i++) {
			uvs[i].x = engine_mesh->get_uv(i, 0).x;
			uvs[i].y = 1.0f - engine_mesh->get_uv(i, 0).y; // invert
			uvs[i].z = 1.0f;
			mesh->setMapVert(map_indices[0], i, uvs[i]);
		}
		delete uvs;
		ZZ_LOG("max_importer_mesh: uv0s done\n");
	}
	if (map_support[1]) {
		UVVert * uvs = new UVVert[num_verts];
		for (i = 0; i < num_verts; i++) {
			uvs[i].x = engine_mesh->get_uv(i, 1).x;
			uvs[i].y = 1.0f - engine_mesh->get_uv(i, 1).y; // invert
			uvs[i].z = 1.0f;
			mesh->setMapVert(map_indices[1], i, uvs[i]);
		}
		delete uvs;
		ZZ_LOG("max_importer_mesh: uv1s done\n");
	}

	//------------------------------------------------------
	// set face info
	int first, second, third;
	TVFace * tvface;
	for (int i = 0; i < num_faces; i++) {
		first = engine_mesh->get_face(i).x;
		second = engine_mesh->get_face(i).y;
		third = engine_mesh->get_face(i).z;

		mesh->faces[i].setVerts(first, second, third);
		mesh->faces[i].setEdgeVisFlags(1, 1, 1);

		// set face map vertex
		if (map_support[0]) {
			tvface = mesh->mapFaces(map_indices[0]);
			tvface[i].setTVerts(first, second, third);
		}
		if (map_support[1]) {
			tvface = mesh->mapFaces(map_indices[1]);
			tvface[i].setTVerts(first, second, third);
		}
	}
	ZZ_LOG("max_importer_mesh: face done\n");

	//------------------------------------------------------
	// end stuff
	//mesh->AutoSmooth(45.0f*ZZ_TO_RAD, false);
	//mesh->buildNormals();
	//mesh->buildRenderNormals();
	mesh->buildBoundingBox();
	mesh->InvalidateEdgeList();

	return true;
}

int
zz_max_importer_mesh::DoImport(const TCHAR *filename,ImpInterface *i,Interface *gi, BOOL suppressPrompts) {
	TriObject *object = CreateNewTriObject();
	if(!object)
		return 0;

	if (mesh_file_load(filename, &object->GetMesh())) {
		ImpNode *node = i->CreateNode();
		if (!node) {
			delete object;
			return 0;
		}
		Matrix3 tm;
		tm.IdentityMatrix();
		node->Reference(object);
		node->SetTransform(0,tm);
		i->AddNodeToScene(node);
		node->SetName(filename);
		i->RedrawViews();
		return 1;
	}
	return 0;
}