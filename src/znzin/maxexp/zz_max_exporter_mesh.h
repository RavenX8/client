/** 
 * @file zz_max_exporter_mesh.h
 * @brief 3ds max exporter mesh class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.1
 * @date    09-aug-2002
 *
 * $Id: zz_max_exporter_mesh.h,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#ifndef __ZZ_MAX_EXPORTER_MESH_H__
#define __ZZ_MAX_EXPORTER_MESH_H__

#include "zz_max_exporter.h"

#define MAGIC_NUMBER_MESH "ZMS0005"
#define MAX_BONE_ASSIGNMENTS 4

class zz_max_exporter_mesh : public zz_max_exporter {
	bool weld_vertices;

	// find node with name "bbox" from node
	INode * find_bbox_node (INode * node);
	Matrix3& get_offsetTM (INode * node, bool has_skin, Matrix3& tm);

public:
	zz_max_exporter_mesh();
	~zz_max_exporter_mesh();

	// engine mesh related
	void read_minmax (Mesh* mesh_in, Matrix3 * tm);
	void read_vertex_format (INode* node);
	bool read_into_engine_mesh (INode * node);

	// from SceneExport class
	const TCHAR * Ext(int n);
	int DoExport(const TCHAR *name, ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

	void get_assignments(int& dest_size, int dest_index[], float dest_weight[],  
					 int src_size, int src_index[], float src_weight[]);
	
	bool get_physique_blend_info (zz_phy_info * pi, int vertex_index,
        vec4& weights, uivec4& indices);

	bool get_skin_blend_info (zz_skin_info * si, int vertex_index, 
		vec4& weights, uivec4& indices);

	void set_dynamic (bool true_or_false);
	void set_weld_vertices (bool true_or_false) { weld_vertices = true_or_false; }
	bool get_weld_vertices () { return weld_vertices; }
};

#endif // __ZZ_MAX_EXPORTER_MESH_H__