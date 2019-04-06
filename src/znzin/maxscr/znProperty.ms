----------------------------------------------------------
-- znProperty : 오브젝트 속성 편집기
-- by zho [2003-06-12]
--
-- 2003-06-12 zho : 생성
--
-- user interface is znProperty_vis.vms
----------------------------------------------------------

-- mesh property
MESH_NUM_MAPCHANNEL   = 1
MESH_USE_NORMAL       = 2
MESH_COLLISION        = 3
MORPH_USE_POSITION    = 4
MORPH_USE_NORMAL      = 5
MORPH_USE_ALPHA       = 6
MORPH_NUM_MAPCHANNEL  = 7
MORPH_USE_TEXTUREANIM = 8
MORPH_USE_WORLD       = 9
ANIM_TYPE             = 10
ANIM_POSITION         = 11
ANIM_ROTATION         = 12
ANIM_SCALE            = 13
MESH_USE_SPECULAR     = 14
--MESH_USE_SKIN         = 10

-- anim type constant (sync with znObjectViewer.ms)
ANIM_TYPE_NONE   = 0
ANIM_TYPE_BIPED  = 1
ANIM_TYPE_MORPH  = 2
ANIM_TYPE_CAMERA = 3
ANIM_TYPE_OBJECT = 4

names = #(
	"mesh_num_mapchannel",
	"mesh_use_normal",
	"mesh_collision",
	"morph_use_position",
	"morph_use_normal",
	"morph_use_alpha",
	"morph_num_mapchannel",
	"morph_use_textureanim",
	"morph_use_world",
	"anim_type",
	"anim_position",
	"anim_rotation",
	"anim_scale",
    "mesh_use_specular"
);
--	"mesh_use_skin");

vals = #(
	1, -- "mesh_num_mapchannel",
	true, -- "mesh_use_normal", -- not used
	true, -- "mesh_collision",
	true, -- "morph_use_position",
	true, -- "morph_use_normal", -- not used
	false, -- "morph_use_alpha",
	0, -- "morph_num_mapchannel",
	false, -- "morph_use_textureanim", -- not used
	true, -- "morph_use_world");
	ANIM_TYPE_NONE, -- "anim_type"
	true, -- "anim_position",
	true, --"anim_rotation",
	true, -- "anim_scale",
    false -- "mesh_use_specular"
);
--	false); -- "mesh_use_skin", -- not used

selected_object = undefined

fn read_property_from_object obj =
(
	if (obj == undefined) then return false
	for i = 1 to vals.count do
	(
		val = getUserProp obj names[i]
		if (val != undefined) then
		vals[i] = val
	)
)

fn write_property_to_object obj =
(
	if (obj == undefined) then return false

	for i = 1 to vals.count do
	(
		setUserProp obj names[i] (vals[i] as string)
		--print (obj.name + " " + names[i] + " " + (vals[i] as string))
	)
)

utility znProperty "znProperty"
(
--x--x--x--x--x--x--x--x--x--x-- from znProperty_vis.ms --x--x--x--x--x--x--x--x--x--x--x--x
rollout ro_property "znProperty" width:160 height:437
(
	checkbox chk_mesh_collision "충돌체크함" pos:[16,87] width:130 height:26 enabled:false checked:true
	groupBox grp1 "메시 애니메이션" pos:[14,169] width:134 height:130
	checkbox chk_morph_pos "위치" pos:[25,186] width:113 height:21 enabled:false checked:true
	checkbox chk_morph_visibility "비져빌러티" pos:[25,214] width:113 height:21 enabled:false
	checkbox chk_morph_uv "UV" pos:[25,242] width:113 height:21 enabled:false
	checkbox chk_morph_world "월드기준" pos:[25,270] width:113 height:21 enabled:false checked:true
	spinner spn_num_mapchannel "맵채널 개수" pos:[22,44] width:117 height:16 enabled:false range:[1,2,1] type:#integer
	groupBox grp2 "오브젝트 애니메이션" pos:[14,312] width:134 height:110
	checkbox chk_obj_position "위치" pos:[25,335] width:106 height:20 enabled:false checked:true
	checkbox chk_obj_rotation "회전" pos:[25,361] width:109 height:23 enabled:false checked:true
	checkbox chk_obj_scale "스케일" pos:[25,390] width:111 height:21 enabled:false checked:true
	radioButtons rdo_animation_type "애니메이션 타입 :" pos:[14,118] width:142 height:46 enabled:false labels:#("없음", "메시", "오브젝트") columns:2
	timer tmr1 "Timer" pos:[143,401] width:24 height:24
	editText edt_selected "" pos:[14,10] width:131 height:20
	checkbox chk_mesh_specular "스페큘러 사용함" pos:[16,66] width:117 height:19
--x--x--x--x--x--x--x--x--x--x-- from znProperty_vis.ms --x--x--x--x--x--x--x--x--x--x--x--x

		fn value_2_control ro_property =
		(
			chk_mesh_collision.checked = vals[MESH_COLLISION]
			chk_morph_pos.checked = vals[MORPH_USE_POSITION]
			chk_morph_visibility.checked = vals[MORPH_USE_ALPHA]
			if (vals[MORPH_NUM_MAPCHANNEL] >= 1) then
			chk_morph_uv.checked = true
			else
			chk_morph_uv.checked = false
			chk_morph_world.checked = vals[MORPH_USE_WORLD]
			spn_num_mapchannel.value = vals[MESH_NUM_MAPCHANNEL]
			chk_obj_position.checked = vals[ANIM_POSITION]
			chk_obj_rotation.checked = vals[ANIM_ROTATION]
			chk_obj_scale.checked = vals[ANIM_SCALE]
			if (vals[ANIM_TYPE] == ANIM_TYPE_NONE) then
			rdo_animation_type.state = 1
			else if (vals[ANIM_TYPE] == ANIM_TYPE_MORPH) then
			rdo_animation_type.state = 2
			else
			rdo_animation_type.state = 3
            chk_mesh_specular.checked = vals[MESH_USE_SPECULAR]
		)

		fn control_2_value ro_property =
		(
			vals[MESH_COLLISION] = chk_mesh_collision.checked
			vals[MORPH_USE_POSITION] = chk_morph_pos.checked
			vals[MORPH_USE_ALPHA] = chk_morph_visibility.checked
			if (chk_morph_uv.checked == true) then
			vals[MORPH_NUM_MAPCHANNEL] = 1
			else
			vals[MORPH_NUM_MAPCHANNEL] = 0
			vals[MORPH_USE_WORLD] = chk_morph_world.checked
			vals[MESH_NUM_MAPCHANNEL] = spn_num_mapchannel.value
			if (rdo_animation_type.state == 1) then
			vals[ANIM_TYPE] = ANIM_TYPE_NONE
			else if (rdo_animation_type.state == 2) then
			vals[ANIM_TYPE] = ANIM_TYPE_MORPH
			else
			vals[ANIM_TYPE] = ANIM_TYPE_OBJECT
			vals[ANIM_POSITION] = chk_obj_position.checked
			vals[ANIM_ROTATION] = chk_obj_rotation.checked
			vals[ANIM_SCALE] = chk_obj_scale.checked
            vals[MESH_USE_SPECULAR] = chk_mesh_specular.checked
		)

		fn disable_controls =
		(
			chk_mesh_collision.enabled = false
            chk_mesh_specular.enabled = false
			spn_num_mapchannel.enabled = false
			rdo_animation_type.enabled = false
			
			chk_morph_pos.enabled = false
			chk_morph_uv.enabled = false
			chk_morph_visibility.enabled = false
			chk_morph_world.enabled = false

			chk_obj_position.enabled = false
			chk_obj_rotation.enabled = false
			chk_obj_scale.enabled = false
		)
		
		fn enable_anim_type state =
		(
			if (state == 1) then
			(
				chk_morph_pos.enabled = false
				chk_morph_uv.enabled = false
				chk_morph_visibility.enabled = false
				chk_morph_world.enabled = false

				chk_obj_position.enabled = false
				chk_obj_rotation.enabled = false
				chk_obj_scale.enabled = false
			)
			else if (state == 2) then
			(
				chk_morph_pos.enabled = true
				chk_morph_uv.enabled = true
				chk_morph_visibility.enabled = true
				chk_morph_world.enabled = true

				chk_obj_position.enabled = false
				chk_obj_rotation.enabled = false
				chk_obj_scale.enabled = false
			)
			else if (state == 3) then
			(
				chk_morph_pos.enabled = false
				chk_morph_uv.enabled = false
				chk_morph_visibility.enabled = false
				chk_morph_world.enabled = false

				chk_obj_position.enabled = true
				chk_obj_rotation.enabled = true
				chk_obj_scale.enabled = true
			)
		)
		
		on tmr1 tick  do
		(
			if ($ == undefined) then
			(
				disable_controls
				return false
			)
			
			local obj
			if ((classOf $) == ObjectSet) then
				obj = $[1]
			else 
				obj = $
			end
			edt_selected.text = obj.name
			selected_object = obj
			if (obj != undefined) then
			(
				chk_mesh_collision.enabled = true
                chk_mesh_specular.enabled = true
				spn_num_mapchannel.enabled = true
				rdo_animation_type.enabled = true
				
				read_property_from_object obj
				value_2_control ro__property
				write_property_to_object obj
				enable_anim_type rdo_animation_type.state
			)
		)
		on spn_num_mapchannel changed val do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_mesh_collision changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)        
		on chk_mesh_specular changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_morph_pos changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_morph_visibility changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_morph_uv changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_morph_world changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on rdo_animation_type changed state do
		(
			enable_anim_type state
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_obj_position changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_obj_rotation changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
		on chk_obj_scale changed state do
		(
			control_2_value ro_property
			write_property_to_object selected_object
		)
	)

	on znProperty open do
	(
		addRollout ro_property
		ro_property.value_2_control ro_property
	)

	on znProperty close do
	(
		removeRollout ro_property
	)
)

