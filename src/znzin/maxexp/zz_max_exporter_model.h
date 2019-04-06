/** 
 * @file zz_max_exporter_model.h
 * @brief 3ds max exporter model class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    26-jun-2002
 *
 * $Id: zz_max_exporter_model.h,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#ifndef __ZZ_MAX_EXPORTER_MODEL_H__
#define __ZZ_MAX_EXPORTER_MODEL_H__

#include "zz_max_exporter.h"

//#define MAGIC_NUMBER_MODEL "ZMD0002"
#define MAGIC_NUMBER_MODEL "ZMD0003"

class zz_max_exporter_model : public zz_max_exporter {
public:
	zz_max_exporter_model();
	~zz_max_exporter_model();

	//Mesh * mesh;

	// from SceneExport class
	const TCHAR * Ext(int n);
	int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

	TriObject* get_triobject_from_node (INode *node, TimeValue t, bool &delete_it);

	//bool allocate_bones (INode* node);
	//unsigned int get_bone_index(INode* node);

	void export_model_header(const TCHAR *name, const TCHAR * magic_number);
	//bool export_vertex (INode* node);
	//bool export_tvertex(INode* node);
	//bool export_face(INode* node);
	void export_skeleton (INode * bone);
	void dump_skeleton (INode* node);
	//Modifier * find_physique_modifier (INode* node_pointer);
	//Modifier * find_skin_modifier (INode* node);
	void export_mesh_names (INode* root_node);
	void dump_dummy (INode * node);
	void export_dummies (INode * root);
};

#endif // __ZZ_MAX_EXPORTER_MODEL_H__