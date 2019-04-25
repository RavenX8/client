----------------------------------------------------------
-- znModel : znzin model binding script
-- by zho [2003-01-13-01]
----------------------------------------------------------
global install_path = getDir #scripts
global export_path = (getDir #export)+"/test/"
global model_path = (getDir #export)+"/models/"
global viewer_path = (getDir #export)+"/znviewer.exe"

-- 사용법
-- 1. Items 리스트에 모델 객체들과 루트본(Bip01 Pelvis)를 추가한다.
-- 2. Export를 눌러 익스포트한다.
-- 3. View를 눌러 본다.

-- Given string, converts a backslash to slash in the string
fn convert_slashes in_string =
(
   out_string = "" as string
   for i = 1 to in_string.count do
   (
      if in_string[i] == "\\" then
      out_string = out_string+"/"
      else
      out_string = out_string+in_string[i]
   )
   return out_string
)

fn append_one in_list obj =
(
	-- this make the biped object to editable_mesh, then unlinked skin-relation
	-- if (convertToMesh obj == undefined)	do 
	-- (
		-- messageBox "Editable	Mesh가 아닙니다." title:"error"
	--		return false
	-- )
	if (findItem in_list obj > 0) do return	false -- skip already inserted
	append in_list obj
)

fn export_config model_name node_list map_names =
(
	s = createFile (model_path+model_name+".lua")
	format "-- This file was automatically created by znModel.ms script.\n" to:s
	format "-- Do not edit this\n\n" to:s
    
    format "model_name = \"%\"\n\n" model_name to:s
    
    format "mesh_name = {\n" to:s	
	for i = 1 to node_list.count do
	(
		format "\t\"%\"" node_list[i].name to:s
		if (i != node_list.count) do format ", \n" to:s
	)
	format "\n}\n\n" to:s
	format "map_name = {\n" to:s
	for i = 1 to map_names.count do
	(
		map_path = (filenameFromPath map_names[i])
		map_path = convert_slashes map_path
		format "\t\"%\"" map_path to:s
		if (i != map_names.count) do format ", \n" to:s
	)
	format "\n}\n\n" to:s
	
	format "motion_name = {\n" to:s
	format "\t\"%\"" (model_name) to:s
	format "\n}\n" to:s
	
	close s
)
      
plugin Helper znModel
	name:"znModel"
	category:"znzin"
	classID:#(0xc0c8c131, 0x2ade4089)
	extends:Dummy
	version:28
(
	parameters main	rollout:params
	(
		nodes type:#nodeTab	tabSizeVariable:true
	)
	rollout	params "znModel	Parameters"	width:162 height:245
	(
		listbox	lb_object_list "Items" pos:[11,12] width:139 height:5
		pickbutton btn_pick	"Pick" pos:[8,112] width:45	height:23
		button btn_add "Add" pos:[58,112] width:45 height:23
		button btn_delete "Delete" pos:[108,112] width:45 height:23
		label lbl1 "사용법 : Items에 오브젝트 및 루트본(Pelvis)를 추가하고 Export버튼을 누르고 View버튼을 누른다"	pos:[11,196] width:136 height:37
		button btn_deleteAll "deleteAll" pos:[103,12] width:47 height:13
		button btn_export "Export" pos:[8,142] width:70	height:23
		button btn_view	"View" pos:[89,142]	width:64 height:23
		on params open do
		(
			lb_object_list.items = #()
			lb_object_list.items = for obj in nodes	collect	obj.name
		)
		on btn_pick	picked obj do
		(
			append_one nodes obj
			lb_object_list.items = #()
			lb_object_list.items = for obj in nodes	collect	obj.name
		)
		on btn_add pressed do
		(
			selection_list = selectByName title:"객체 선택"
			if (selection_list != undefined) do
			(
				for obj in selection_list do append_one nodes obj
				lb_object_list.items = #()
				lb_object_list.items = for obj in nodes collect obj.name
			)
		)
		on btn_delete pressed do
		(
			sel_num	= lb_object_list.selection
			if sel_num == 0 do return false
			deleteItem nodes sel_num
			lb_object_list.items = #()
			lb_object_list.items = for obj in nodes collect obj.name
		)
		on btn_deleteAll pressed do
		(
			nodes =	#()
			lb_object_list.items = #()
		)
		on btn_view pressed do
		(
			ShellLaunch viewer_path ""
		)
		on btn_export pressed do
		(
			saved_selection = $

			-- export camera
			exportFile (export_path+"camera.zca") #noprompt

			local thisObj = (refs.dependents this)[1]
			local model_name = thisObj.name
			local root_bone = undefined
			local mesh_list = #()
			local map_names = #()
			
			for obj in nodes do
			(
				cont = getTMController obj
				if ((classOf cont) == BipSlave_Control) or ((classOf cont) == IKControl) then
				(
					-- set this object to root bone
					root_bone = obj
				)
				else if (classOf obj) == Editable_Mesh then
				(
					select obj
					append map_names obj.material.maps[2].filename
					append mesh_list obj
					exportFile (model_path + obj.name + ".zms")  #noprompt selectedonly:true
				)
				else
				(
					print obj
				)
			)
			select root_bone
			
			-- export default motion
			 exportFile (model_path + model_name + ".zmo") #noprompt selectedonly:true

			-- export model
			 exportFile (model_path + model_name + ".zmd") #noprompt selectedonly:true

			-- copy map name
			for map_name in map_names do
			(
				copyto_name = model_path + (filenameFromPath map_name)
				deleteFile copyto_name -- first delete existing one (overwrite)
				-- for test
				print map_name
				print copyto_name
		
				copyfile map_name copyto_name
			)
			
			-- set this.name to model_name
			export_config model_name mesh_list map_names
			select saved_selection -- recover selection
		)
	)
)
