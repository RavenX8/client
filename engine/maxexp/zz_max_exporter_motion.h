/** 
 * @file zz_max_exporter_motion.h
 * @brief 3ds max exporter motion class for znzin.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    26-jun-2002
 *
 * $Id: zz_max_exporter_motion.h,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $
 */

#ifndef __ZZ_MAX_EXPORTER_MOTION_H__
#define __ZZ_MAX_EXPORTER_MOTION_H__

#include "zz_max_exporter.h"

#define MAGIC_NUMBER_MOTION "ZMO0002"

#define MAX_NUM_CHANNEL 20000

class zz_max_exporter_motion : public zz_max_exporter {
public:
	zz_max_exporter_motion();
	~zz_max_exporter_motion();

	// for mesh animation
	BOOL morph_use_position;
	BOOL morph_use_normal;
    BOOL morph_num_mapchannel;
	BOOL morph_use_alpha;
	BOOL morph_use_textureanim; // use texture index for texture animation
	BOOL morph_use_world; // if effect, set to true. if character, set to false

	enum {
		ZZ_NONE  =  0,
		ZZ_BIPED =  1,
		ZZ_MORPH =  2,
		ZZ_CAMERA = 3,
		ZZ_OBJECT = 4
	};

	int anim_type; // ZZ_BIPED...
	BOOL anim_position;
	BOOL anim_rotation;
	BOOL anim_scale;
	
	int number_of_frames;
	int frame_per_second;

	INode * camera_node; // pointer to camera node
	CameraObject * camera; // pointer to current camera
	int number_of_channels;                      // number of channels
	int current_bone_index;

	struct channel_struct {
		INode * bone;
		TCHAR * name;
		int bone_index;
		int type;
	};

	channel_struct channel_list[MAX_NUM_CHANNEL];

	// from SceneExport class
	const TCHAR * Ext(int n);
	int do_export_bone (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);
	int do_export_biped (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);
	int do_export_camera (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);
	bool do_export_morph (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);
	int do_export_object (const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);
	
	int DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

	void read_user_property (INode * node);

	// get the number of
	int get_num_properties ();
	void write_channel_info (uint32 channelid, uint32 channeltype, uint32 referid);
	void write_morph_data(uint32 frame, uint32& channelid,
		Point3 pos, Point3 normal, float alpha, Point3 uv0);

	int export_sample (void);
	int export_sample_camera (void);
	bool export_sample_morph (INode * node);
	bool export_sample_object (INode * node);

	void export_motion_header(const TCHAR *name, const TCHAR * magic_number);
	bool allocate_channels (INode* node);
	bool allocate_channels_4_morph (INode* node);
	void export_channel_info();
	void export_channel_info_4_camera ();
	void export_channel_info_4_morph ();
	void export_channel_info_4_object ();
	float get_material_alpha(INode * node, TimeValue t);
};

#endif // __ZZ_MAX_EXPORTER_MOTION_H__