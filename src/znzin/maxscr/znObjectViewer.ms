----------------------------------------------------------
-- znObjectViewer : znzin object viewer tool script
-- by zho [2003-06-18]
--
-- 2003-06-04 zho : 건물에 링크된 포인트 익스포트 및 사용자 속성 기본 세팅
-- 2003-05-07 zho : 충돌전용 메쉬 익스포트(재질이 없으면 충돌전용 메쉬로 익스포트함)
-- 2003-06-13 zho : 디폴트로 normal TRUE
-- 2003-06-18 zho : 저장 경로 지정시 .txt 파일 안 보이는 문제 해결
--                  linkDummy 기능 추가. 리스트박스에 있는 오브젝트 삭제시 자동 제거(타이머사용)
-- 2003-06-20 zho : 애니메이션 속성별 저장 기능 추가.
-- 2004-01-06 zho : 강제로 맵채널 2로 세팅. zms 저장할 때, 맵채널이 2가 아니면 메세지 출력하기 때문에 문제 없음.
-- 2004-01-26 zho : + 루트폴더 선택기능 추가.
--                  + 2,3번 맵채널 체크. 2번 맵채널 있으면 경고. 3번 맵채널 없으면 경고.
--                  + txt 만 출력하는 옵션 추가.
-- 2004-01-27 zho : .txt 에 /3ddata... 를 3ddata 로 수정. 루트경로에 /가 들어가면 안됨.
-- 2004-02-03 zho : .txt 에 type 0 을 추가.
--                  num_map_channel에 2번은 제외하고 계산.             
--                  collision level 디폴트로 12로 함.
-- 2004-02-06 zho : 무기 뽑을 때에는 피봇 고정. 3dsmax.ini 에 usePivotOrigin 추가
-- 2004-02-23 zho : PolyMeshObject 도 폴리라네요.
-- 2004-03-12 zho : znChannelFunc.ms 인클루드함. 3번 채널 없으면 알려줌.
-- user interface by znObjectViewer.vms
----------------------------------------------------------
-- if meshop.getNumMaps $ > 2, then do this:
-- > for obj in selection do meshop.setnummaps $ 2
-- 지원되는 사용자속성(User-Defined Property) 필드
-- mesh = "참조할 원본 메쉬를 가진 오브젝트의 이름"
-- material = "참조할 원본 재질을 가진 오브젝트의 이름"

include "znChannelFunc.ms"

global install_path = getDir #scripts
global export_name = ""                                         -- 익스포트할 파일이름
global export_path_relative = ""                                -- 익스포트할 상대경로
global export_path_root = ""                                    -- 익스포트할 경로의 최상위 루트
global export_path = ""                                         -- 익스포트할 풀경로(이름제외)

maxroot = getDir #maxroot
global ini_path = maxroot + "3dsmax.ini"

global viewer_path = (getDir #export)+"znclient.exe"
global version = "2004-03-12"
global all_list = #() -- 선택상자에 등록된 모든 노드들 목록
global node_list = #() -- 선택상자에 등록된 노드들 중 일반 노드 목록
global mapname_list = #() -- 익스포트할 오브젝트들의 재질 목록
global point_list = #() -- 선택상자에 등록된 노드들 중 포인트 노드 목록

global num_maps = 2
global use_lightmap = false
global use_absolute_texture_path = false

global link_dummy = -1 -- default -1. one of DUMMY_XXXX
global use_pivot_origin = "1"

DUMMY_RHAND    = 0
DUMMY_LHAND    = 1
DUMMY_LFOREARM = 2
DUMMY_CHEST    = 3
DUMMY_MOUSE    = 4
DUMMY_EYE      = 5
DUMMY_HEADTOP  = 6

-- anim type constant (sync with znProperty.ms)
ANIM_TYPE_NONE   = 0
ANIM_TYPE_BIPED  = 1
ANIM_TYPE_MORPH  = 2
ANIM_TYPE_CAMERA = 3
ANIM_TYPE_OBJECT = 4

include "convert_slashes.ms"
    
fn set_export_path_root root_path_in =
(
    last_char = subString root_path_in root_path_in.count -1
    if (last_char == "\\") then
        export_path_root = root_path_in
    else
        export_path_root = root_path_in + "\\"
        
    setINISetting ini_path "Directories" "ZnzinExportRoot" export_path_root
)

fn set_export_path export_path_in =
(
    export_name = filenameFromPath export_path_in
    export_path = getFilenamePath export_path_in
    export_path_relative = subString export_path (export_path_root.count+1) -1
    setINISetting ini_path "Directories" "ZnzinExportPath" export_path_in
)

fn init_export_path =
(
    export_path_root_ini = getINISetting ini_path "Directories" "ZnzinExportRoot"
    if (export_path_root_ini == "") then
    (
        export_path_root_ini = getDir #export
    )
    set_export_path_root export_path_root_ini
    export_path_ini = getINISetting ini_path "Directories" "ZnzinExportPath"
    if (export_path_ini == "") then
    (
        export_path_ini = export_path_root + "test.txt"
    )
    set_export_path export_path_ini
)

fn add_one in_list obj =
(
	if (findItem in_list.items obj.name > 0) do return false -- skip already inserted
	append in_list.items obj.name
	append all_list obj
)

fn add_array in_list in_array =
(
	for obj in in_array do
		add_one in_list obj
)

fn get_top_node_in_list in_list =
(
	local index = 1
	for obj in in_list do
	(
		if (isDeleted obj) do
		(
			return index
		)
		if (obj.parent == 0) do return index
		if ((findItem in_list obj.parent) == 0) do return index -- this is to node
		index = index + 1
	)
	messageBox "오류"
	return undefined
)

fn refresh_list in_list = 
(
	point_list = #()
	node_list = #()
	
	local temp_list = #()
	local new_list = #()
	
	join temp_list all_list -- copy all elements
	
	local index
	while (temp_list.count > 0) do 
	( 
		index = get_top_node_in_list temp_list
		top_node = temp_list[index]
		deleteItem temp_list index
		if ((isDeleted top_node) == false) then
			append new_list top_node
	)

	in_list.items = #()
	all_list = #()
	
	for obj in new_list do
	(
		if ((classOf obj) == point) then
		(
			append point_list obj
		)
		else (
			append node_list obj
		)
		append in_list.items obj.name
		append all_list obj
	)
	items = in_list.items					
	in_list.items = #()
	in_list.items = items	
)
	
fn export_mesh in_list =
(
	for obj in in_list do
	(
		-- skip if has mesh user-property
		if ((getUserProp obj "mesh") != undefined) then continue
		if (export_path == undefined) then return false
		local full_name = (export_path+obj.name+".zms")
		ClearSelection()
		select obj
		exportfile full_name #noprompt selectedonly:true
		print full_name
	)
)

fn check_scale in_list =
(
	for obj in in_list do
	(
		scale = in coordsys world obj.scale
		local epsilon = .01
		if ((scale.x > 1 + epsilon) or (scale.x < 1 - epsilon) or
		    (scale.y > 1 + epsilon) or (scale.y < 1 - epsilon) or
		    (scale.z > 1 + epsilon) or (scale.z < 1 - epsilon)) do
		(
			local msg_in = obj.name + ". <"+ scale.x as string + ", " + scale.y as string + ", " + scale.z as string + ">"
			messageBox ("스케일이 1이 아닙니다.\n"+msg_in+"\n스케일이 리셋되었는지 확인해주세요.\n(주의: 링크를 먼저 해제해야 합니다.)")
		)
	)
	return true
)

fn check_hierarchy in_list =
(
	local top_count = 0
	for obj in in_list do
	(
		if (obj.parent == undefined) do top_count = top_count + 1
	)
	if (top_count > 1) do
	(
		messageBox "링크를 연결하지 않았습니다."
		return false
	)
	if (top_count == 0) do
	(
		messageBox "최상위 부모 노드가 선택상자에 포함되지 않았습니다."
		return false
	)
	return true
)

fn align_root_to_origin in_list =
(
	local index = get_top_node_in_list in_list
	if (index == undefined) do return false
	top_node = in_list[index]
	if (top_node == undefined) do return false
	top_node.pivot = [0, 0, 0]
	print (top_node.name + "의 피봇을 원점으로 이동시켰습니다.")
	return true
)

fn check_user_property in_list =
(
	for obj in in_list do
	(
        if (is_mesh_or_poly obj) == false do continue -- 메시나 폴리가 아닌 경우에는 건너뜀.
		if ((getUserProp obj "mesh_use_skin") == undefined) do
			setUserProp obj "mesh_use_skin" "FALSE"
		local num_mapchannel = 0
		
		if (get_map_support obj 1) do num_mapchannel = num_mapchannel + 1
        if (get_map_support obj 3) then num_mapchannel = num_mapchannel + 1
        else messageBox(obj.name + "에 3번 채널이 없습니다")
        
        if ((getUserProp obj "mesh_num_mapchannel") != undefined) do
        (
            local user_num_mapchannel = (getUserProp obj "mesh_num_mapchannel") as integer
            if (user_num_mapchannel != num_mapchannel) do
            (
                messageBox(obj.name + "맵채널개수를 "+ (num_mapchannel as string) + "로 갱신합니다.")
            )
        )
        setUserProp obj "mesh_num_mapchannel" num_mapchannel
        if (num_mapchannel == 2) then
        (
            if (get_map_support obj 2) do messageBox (obj.name + " 의 2번 채널이 활성화되어 있습니다. 확인해 주세요.") 
            if ((get_map_support obj 3) == false) do messageBox (obj.name + " 의 3번 채널이 활성화되어 있지 않습니다. 확인해 주세요.")
        )
        else if (num_mapchannel > 2) do messageBox (obj.name + " 에 속성으로 지정된 맵채널 개수가 2보다 많습니다. znProperty로 속성을 확인해 주세요.")
		if ((getUserProp obj "mesh_use_normal") == undefined) do
			setUserProp obj "mesh_use_normal" "TRUE"
		if ((getUserProp obj "mesh_collision") == undefined) do
			setUserProp obj "mesh_collision" "TRUE"
		if ((getUserProp obj "effect") == undefined) do
			setUserProp obj "effect" ""
		if ((getUserProp obj "morph_use_position") == undefined) do
			setUserProp obj "morph_use_position" "TRUE"
		if ((getUserProp obj "morph_use_normal") == undefined) do
			setUserProp obj "morph_use_normal" "FALSE"
		if ((getUserProp obj "morph_use_alpha") == undefined) do
			setUserProp obj "morph_use_alpha" "FALSE"
		if ((getUserProp obj "morph_num_mapchannel") == undefined) do
			setUserProp obj "morph_num_mapchannel" "0"
		if ((getUserProp obj "morph_use_textureanim") == undefined) do
			setUserProp obj "morph_use_textureanim" "FALSE"
		if ((getUserProp obj "morph_use_world") == undefined) do
			setUserProp obj "morph_use_world" "FALSE"
        if ((getUserProp obj "mesh_use_specular") == undefined) do
            setUserProp obj "mesh_use_specular" "FALSE"
	)
	return true
)

fn check_material in_list only_txt =
(
	for i = 1 to in_list.count do
	(
		obj = in_list[i]

		-- skip if it has "material" user-property
		if ((getUserProp obj "material") != undefined) then continue

		-- skip if it is obbox
		if ((classOf obj) == znOBBox) then continue


		if (obj.material == undefined) do
		(
			mapname_list[i] = "none"
			--messageBox "충돌전용 메쉬"
			continue
		)

		if ((classOf obj.material) != Standardmaterial) do
		(
			message = "오류: Standard 재질이 아닙니다. 재질(" + (classOf obj.material) as string + ")"
			messageBox message
			return false
		)

		mat = obj.material
		map = obj.material.maps[2]
		if (map == undefined) do
		(
			msg = obj.name + "has no material map[2]"
			messageBox msg
			return false
		)
		full_mapname = map.filename
		filename = filenameFromPath full_mapname
		if (export_path == undefined) then return false
		copyto_name = export_path + filename

        if (only_txt == false) then -- txt 만 뽑고 싶다면, 실제 삭제나 복사는 수행 안함.
        (
    		deleteFile copyto_name -- first delete existing one (to overwrite)
	    	copyFile full_mapname copyto_name
        )
        
		if (use_absolute_texture_path) then
		(
			mapname_list[i] = convert_slashes full_mapname
		)
		else
		(
			mapname_list[i] = convert_slashes (export_path_relative+filename)
		)
	)
	return true
)

fn get_mesh_name obj =
(
	mesh_name = getUserProp obj "mesh"
	if (mesh_name == undefined) then return ("mesh_"+obj.name)
	return "mesh_"+mesh_name
)

fn get_material_name obj =
(
	material_name = getUserProp obj "material"
	if (material_name == undefined) then return ("mat_"+obj.name)
	return "mat_"+material_name
)

fn load_mesh obj s =
(
	-- skip if "mesh" user-property was defined
	if ((getUserProp obj "mesh") != undefined) then return 0

	object_name = obj.name
	line_string = "mesh = loadMesh( \"mesh_" + object_name + "\", \"test/" + object_name + ".zms\" )"
	format "%\n" line_string to:s
)
		
fn load_material obj map_name s =
(
	-- skip if "material" user-property was defined
	if (getUserProp obj "material") != undefined then return()

	if ((getUserProp obj "mesh_num_collision") == "TRUE") then
	(
		format "mat = 0\n"
		return()
	)

	if (use_lightmap) then print "use_lightmap"
	if (get_map_support obj 3) then print "map3 support"

	if (use_lightmap and (get_map_support obj 3)) then
	(
		set_map_support obj 2 false -- ignore second channel
		line_string = "mat_diffuse = loadColormapMaterial( \"mat_diffuse_" + obj.name + "\", shader_lightmap_nolit, \"" + map_name + "\" )"
		format "%\n" line_string to:s
		line_string = "mat = loadMaterialMixer( \"mat_" + obj.name + "\", mat_diffuse, mat_lightmap, 3 );"
		format "%\n" line_string to:s
	)
	else
	(
		line_string = "mat = loadColormapMaterial( \"mat_" + obj.name + "\", shader, \"" + map_name + "\" )"
		format "%\n" line_string to:s
	)
	-- alpha channel
	-- if (in_list[i].material.maps[2].alphasource == 0) do -- 0 : use image alpha, 1 : rgb alpha, 2 : none
	if (obj.material.mapEnables[7]) do -- if opacity channel enabled
	format "setMaterialUseAlpha( mat, 1 )\n" obj.name to:s
	-- two sided material
	if (obj.material.twoSided == true) do
	format "setMaterialUseTwoSide( mat, 1 )\n" obj.name to:s
)

fn export_camera =
(
	if (export_path == undefined) then return false
	local full_name = export_path+"camera.zca"
	exportFile full_name #noprompt
)

fn getListIndex in_list parent_node =
(
	if (parent_node == undefined) then return 0
	for i = 1 to in_list.count do
	(
		if (in_list[i].name == parent_node.name) then return i
	)
	return 0
)

fn get_transform_info obj =
(
	tm = obj.transform
	if (obj.parent != undefined) then -- 최상위
	(
		parent_index = getListIndex node_list obj.parent
		tm = tm * inverse (obj.parent.transform) -- get local TM
	)
	else parent_index = 0
	rot = tm.rotation
	rot.w = -rot.w -- max use left-handed rotation system. znzin use right-handed rotation system
	pos = tm.position
	scale = in coordsys parent obj.scale
	return #(parent_index, pos, rot, scale)
)		

fn find_skin_modifier mynode =
(
	for obj in mynode.modifiers
	where classof obj == Skin
	do return obj
	return undefined
)

fn do_export_point in_list s =
(
	if (in_list.count == 0) do return true
	
	format "numpoint %\n" (in_list.count as string) to:s

	for i = 1 to in_list.count do
	(
		format "point %\n" i to:s

		effect_path = getUserProp in_list[i] "effect"
		if effect_path == undefined do effect_path = ""
		format "\teffect %\n" effect_path to:s
		format "\ttype 0\n" to:s
		info = get_transform_info in_list[i]
		parent_index = info[1]
		pos = info[2]
		rot = info[3]
		scale = info[4]
		format "\tparent %\n" (parent_index as string) to:s
		format "\tpos %\n" (pos.x as string+" "+pos.y as string+" "+pos.z as string) to:s
		format "\trot %\n" (rot.w as string+" "+rot.x as string+" "+rot.y as string+" "+rot.z as string) to:s		
		format "\tscale %\n" (scale.x as string+" "+scale.y as string+" "+scale.z as string) to:s
	)
	return true
)
		
fn do_export_building only_txt =
(
	if (node_list.count == 0) do
	(
		messageBox "선택상자에서 객체를 선택해 주세요."
		return false
	)
	if (export_name == undefined) do
	(
		messageBox "출력할 파일이름을 지정해 주세요."
		return false
	)
	
    if (only_txt == false) then
    (
    	if (export_mesh node_list == false) do return false
    )

	file_name = export_path + export_name

	s = createFile file_name

	format "numobj %\n" (node_list.count as string) to:s

	for i = 1 to node_list.count do
	(
		-- [index]
		format "obj %\n" i to:s
		is_point = false

		-- [mesh]
		format "\tmesh %.zms\n" (convert_slashes (export_path_relative+node_list[i].name)) to:s
		-- [material]
		format "\tmat %\n" mapname_list[i] to:s
		
		-- [link_dummy]
		if (i == 1 and link_dummy != -1) then -- only root
		(
			format "\tlinkdummy %\n" link_dummy to:s
		)
		
		use_alpha = "0"
		use_twoSide = "0"
		-- [alpha]
		if (node_list[i].material != undefined) and (node_list[i].material.mapEnables[7]) do -- if opacity channel enabled
			use_alpha = "1"
		format "\talpha %\n" use_alpha to:s
	
		-- [two_side]
		if (node_list[i].material != undefined) and (node_list[i].material.twoSided == true) do
			use_twoside = "1"
		format "\ttwoside %\n" use_twoside to:s
	
		info = get_transform_info node_list[i]
		parent_index = info[1]
		pos = info[2]
		rot = info[3]
		scale = info[4]

		format "\tparent %\n" (parent_index as string) to:s
		format "\tpos %\n" (pos.x as string+" "+pos.y as string+" "+pos.z as string) to:s
		format "\trot %\n" (rot.w as string+" "+rot.x as string+" "+rot.y as string+" "+rot.z as string) to:s		
		format "\tscale %\n" (scale.x as string+" "+scale.y as string+" "+scale.z as string) to:s
		
		-- [skin]
		--if ((find_skin_modifier node_list[i]) != undefined) then format "\tisskin 1\n" to:s
		--else format "\tisSkin 0\n" to:s
			
		-- [collision]
		if ((getUserProp node_list[i] "mesh_collision") == true) then collision = "12" -- polygon level
		else collision = "0"
		format "\tcollision %\n" collision to:s
		
        -- [specular]
        if ((getUserProp node_list[i] "mesh_use_specular") == true) then 
        (
            format "\tspecular 1\n" to:s
        )
        
		-- [animation]
		if ((getUserProp node_list[i] "anim_type") == ANIM_TYPE_OBJECT) then
		(
			-- export motion
			if (export_path != undefined) then
			(
				local only_name = node_list[i].name+".zmo"
				local full_name = (export_path+only_name)
				ClearSelection()
				select in_list[i]
                if (only_txt == false) do -- txt 만 뽑는 경우가 아니라면, 실제 익스포트도 수행함.
    				exportfile full_name #noprompt selectedonly:true
				
				format "\tanim %\n" (convert_slashes (export_path_relative+only_name)) to:s to:s
			)
		)
	)
	
	do_export_point point_list s
	
	close s
	messageBox "성공적으로 출력되었습니다"
	return true
)

fn do_export in_list =
(
    only_txt = false
	if (check_material in_list only_txt == false) do return false

	-- export meshes
	export_mesh in_list

	file_name = export_path+"test.lua"
	s = createFile file_name
	format "-- This file was automatically created by znObjectViewer.ms script.\n" to:s
	format "-- Do not edit this\n" to:s
	format "shader = shader_nolit\n\n" to:s

	if (use_lightmap) do
	format "mat_lightmap = loadColormapMaterial( \"mat_lightmap\", shader_nolit, \"lightmap.tga\" )\n" to:s

	for i = 1 to in_list.count do
	(
		-- skip OBBox
		if ((classOf in_list[i]) == znOBBox) then continue

		load_mesh in_list[i] s
		load_material in_list[i] mapname_list[i] s

		object_name = in_list[i].name
		line_string = "vis = loadVisible( \"" + object_name + "\", mesh, mat, light )"
		format "%\n" line_string to:s
		tm = in_list[i].transform
		if (in_list[i].parent != undefined) do
		(
			tm = tm * inverse (in_list[i].parent.transform) -- get local TM
		)
		rot = tm.rotation
		rot.w = -rot.w -- max use left-handed rotation system. znzin use right-handed rotation system
		pos = tm.position
		scale = in_list[i].scale -- globally applied scale
		line_string = "setQuaternion( vis, "+rot.w as string+", "+rot.x as string+", "+rot.y as string+", "+rot.z as string+" )"
		format "%\n" line_string to:s
		line_string = "setPosition( vis, "+pos.x as string+", "+pos.y as string+", "+pos.z as string+" )"
		format "%\n" line_string to:s
		line_string = "setScale( vis, "+scale.x as string+", "+scale.y as string+", "+scale.z as string+" )"
		format "%\n" line_string to:s
		
		-- [collision]
		if ((getUserProp in_list[i] "mesh_collision") == true) then
		(
			line_string = "setCollisionLevel( vis, 12 )"
			format "%\n" line_string to:s
		)
		
		line_string = "setReceiveShadow( vis, 1 )"
		format "%\n" line_string to:s
		
		format "\n" to:s
	)
	format "\n--- linkage section\n" to:s
	-- parent-child link
	for i = 1 to in_list.count do
	(
		object_name = in_list[i].name
		if (in_list[i].parent != undefined) do
		(
			parent_name = in_list[i].parent.name
			line_string = "linkNode( findNode(\""+parent_name+"\"), findNode(\""+object_name+"\") )"
			format "%\n" line_string to:s
		)
	)
	close s
	return true
)

utility znObjectViewer "znObjectViewer"
(
--x--x--x--x--x--x--x--x--x--x-- from znObjectViewer_vis.ms --x--x--x--x--x--x--x--x--x--x--x--x
rollout ro_object_viewer "znObjectViewer" width:162 height:426
(
	listBox lb_object_list "선택상자" pos:[12,25] width:137 height:4
	pickButton btn_pick "Pick" pos:[9,123] width:44 height:19
	button btn_add "Add" pos:[58,123] width:44 height:19
	button btn_delete "Delete" pos:[107,123] width:44 height:19
	button btn_deleteAll "deleteAll" pos:[100,27] width:49 height:13
	button btn_export_building "출력" pos:[9,327] width:144 height:26 enabled:true
	label lbl_version "버전" pos:[12,7] width:89 height:19
	button btn_export_path "출력파일 선택" pos:[12,289] width:138 height:26
	editText edt_export_path "" pos:[9,264] width:141 height:21
	groupBox grp1 "TXT 출력경로" pos:[7,195] width:150 height:127
	timer tmr1 "Timer" pos:[126,431] width:24 height:24
	dropDownList ddl_link "연결 포인트 :" pos:[11,148] width:139 height:41 items:#("없음", "오른손", "왼손", "왼손방패", "등", "입", "눈", "정수리")
	button btn_export_root "루트폴더 선택" pos:[13,234] width:138 height:26
	editText edt_export_root "" pos:[10,209] width:141 height:21
	checkbox chk_onlytxt "TXT만 출력하기" pos:[11,359] width:134 height:28
	checkbox chk_pivot_origin "피봇을 원점으로" pos:[12,388] width:139 height:21
--x--x--x--x--x--x--x--x--x--x-- from znObjectViewer_vis.ms --x--x--x--x--x--x--x--x--x--x--x--x
			
        on btn_export_root pressed do
        (
            local root_path = getSavePath "익스포트할 루트경로를 지정해 주세요"
            if (root_path == undefined) then return false
            set_export_path_root root_path
            edt_export_root.text = export_path_root
        )
        on chk_pivot_origin changed state do
        (
            if (state == true) then use_pivot_origin = "1"
            else use_pivot_origin = "0"
            setINISetting ini_path "znzin" "usePivotOrigin" use_pivot_origin    
        )
        
		on btn_export_path pressed do
		(
			--local local_path = getSavePath "익스포트할 파일명을 지정해 주세요"
            
            local export_filename = export_path_root+"build.txt"
			local local_path = getSaveFileName caption:"익스포트할 파일명을 지정해 주세요" filename:export_filename types:"TXT(*.txt)|*.txt|All|*.*|"
			
			if (local_path == undefined) then return false
			temp = findString local_path export_path_root
			if (temp == undefined) do
			(
				messageBox (export_path_root + "의 하위폴더 중에서 선택해 주세요") title:"error"
				return false
			)
            set_export_path local_path
			edt_export_path.text = export_path_relative + export_name
		)

		on btn_pick picked obj do
		(
			add_one lb_object_list obj
			refresh_list lb_object_list
		)
		on btn_add pressed do
		(
			selection_list = selectByName title:"객체 선택"
			if (selection_list != undefined) do 
				add_array lb_object_list selection_list			
			a = lb_object_list.items
			refresh_list lb_object_list
		)
		on btn_delete pressed do
		(
			sel_num = lb_object_list.selection
			if sel_num == 0 do return false
			deleteItem all_list sel_num
			deleteItem lb_object_list.items sel_num
			refresh_list lb_object_list
		)
		on btn_deleteAll pressed do
		(
			all_list = #()
			node_list = #()
			point_list = #()
			lb_object_list.items = #()
			refresh_list lb_object_list
		)
		on btn_view pressed do
		(
			if (do_export node_list) do
			(
				print "정상적으로 익스포트되었습니다."
				ShellLaunch viewer_path ""
			)
		)
		on btn_export_building pressed do
		(
            only_txt = chk_onlytxt.checked
			if (check_hierarchy all_list == false) do return false
			if (check_scale all_list == false) do return false
			if (check_material node_list only_txt == false) do return false
			if (check_user_property node_list == false) do return false
			if (chk_pivot_origin.checked) do
			(
			    if (align_root_to_origin node_list == false) do return false
			)
			if (do_export_building only_txt) do
			(
				print "정상적으로 출력되었습니다."
			)
		)			
		on tmr1 tick  do
		(
	    		refresh_list lb_object_list
		)
		on ddl_link selected sel do
		(
			-- "없음", "오른손", "왼손", "왼손방패", "등", "입", "눈", "정수리"
			if (sel == 1) then
				link_dummy = -1
			else if (sel == 2) then
				link_dummy = DUMMY_RHAND
			else if (sel == 3) then
				link_dummy = DUMMY_LHAND
			else if (sel == 4) then
				link_dummy = DUMMY_LFOREARM
			else if (sel == 5) then
				link_dummy = DUMMY_CHEST
			else if (sel == 6) then
				link_dummy = DUMMY_MOUSE
			else if (sel == 7) then
				link_dummy = DUMMY_EYE
			else if (sel == 8) then
				link_dummy = DUMMY_HEADTOP
		)
	)

	on znObjectViewer open do
	(
		addRollout ro_object_viewer
		ro_object_viewer.lbl_version.caption = "버전 : "+version
		if (export_name != undefined) do
		(
			ro_object_viewer.edt_export_path.text = export_path_relative + export_name
            ro_object_viewer.edt_export_root.text = export_path_root
		)
		use_pivot_origin = getINISetting ini_path "znzin" "usePivotOrigin"
		if (use_pivot_origin == "") do
		(
		    use_pivot_origin = "1"
		    setINISetting ini_path "znzin" "usePivotOrigin" use_pivot_origin
		)
		if (use_pivot_origin == "1") then
		    ro_object_viewer.chk_pivot_origin.checked = true
	)

	on znObjectViewer close do
	(
		removeRollout ro_object_viewer
	)
)

init_export_path() -- 경로관련 초기화
