----------------------------------------------------------
-- znTool : znzin tool script
-- by zho [2002-11-25-01]
-- mod by zho [2003-03-24-01]
----------------------------------------------------------
global install_path = getdir #scripts
global viewer_path = install_path+"view_release.exe"
global export_path = getDir #export

global NUM_MESHES = 10;
global HEAD = 1; global ARMOR = 2; global GLOVEL = 3; global GLOVER = 4; global BOOTSL = 5; global BOOTSR = 6;
global ITEM_START = 7; -- item index start
global SWORD = 7; AXE = 8; BOW = 9; SHIELD = 10;
global L_AMBIENT = 1; global L_DIFFUSE = 2; global L_DIRECTION = 3;
global shader_name = "lit_shader"; -- "lit_shader" or "nolit_shader"

global bone_list = #()

fn traverse_and_append_bone obj = 
(
	if ((classof obj) != Biped_Object) then
	(
	   return false
	)
	append bone_list obj
	format "% : " bone_list.count
	format "% \n" obj.name
   for child in obj.children do traverse_and_append_bone child
	return true
)

include "convert_slashes.ms"

-- simply get the type name in string
fn get_mesh_type_name in_mesh_type =
case in_mesh_type of
(
   HEAD: "_head"
   ARMOR: "_body"
   GLOVEL: "_handl"
   GLOVER: "_handr"
   BOOTSL: "_footl"
   BOOTSR: "_footr"
   SWORD: "_sword"
   AXE: "_axe"
   BOW: "_bow"
   SHIELD: "_shield"
   default: "_head"
)

utility znTool "znTool"
(
   local selected_indices = #() -- dropdownlist's selected item list
   local mesh_names = #() -- ex) "man01-gloveL01"
   local map_names = #() -- ex) "d:\znzin11\data\man011\man01-gloveL01.bmp"
   local light_property = #()

   rollout ro_model "Model"
   (
      --edittext et_prefix "Prefix:" fieldWidth:70
      dropdownlist dd_head "머리:" items:mesh_names[HEAD] height:6
      dropdownlist dd_armor "몸통:" items:mesh_names[ARMOR] height:6
      dropdownlist dd_gloveL "왼장갑:" items:mesh_names[GLOVEL] height:6
      dropdownlist dd_gloveR "오른장갑:" items:mesh_names[GLOVER] height:6
      dropdownlist dd_bootsL "왼부츠:" items:mesh_names[BOOTSL] height:6
      dropdownlist dd_bootsR "오른부츠:" items:mesh_names[BOOTSR] height:6
      dropdownlist dd_sword "검:" items:mesh_names[SWORD] height:6
      dropdownlist dd_shield "방패:" items:mesh_names[SHIELD] height:6
      dropdownlist dd_axe "도끼:" items:mesh_names[AXE] height:6
      dropdownlist dd_bow "활:" items:mesh_names[BOW] height:6

      fn dd_mesh_selected arg_mesh_type arg_dd_index =
      (
         --mesh_names[arg_type][arg_index] = mesh_[arg]
         obj_name = "$'"+mesh_names[arg_mesh_type][arg_dd_index]+"'"
         selected_mesh = execute obj_name
         -- = filenameFromPath selected_mesh.material.maps[2].filename -- get diffuse map
         if (selected_mesh.material.maps[2] == undefined) then
         (
            local message = selected_mesh.name+"의 디퓨즈맵을 설정해 주세요."
            messagebox message
            return OK
         )
         map_names[arg_mesh_type][arg_dd_index] = convert_slashes selected_mesh.material.maps[2].filename
         selected_indices[arg_mesh_type] = arg_dd_index
      )

      on dd_head selected arg_dd_index do dd_mesh_selected HEAD arg_dd_index
      on dd_armor selected arg_dd_index do dd_mesh_selected ARMOR arg_dd_index
      on dd_gloveL selected arg_dd_index do dd_mesh_selected GLOVEL arg_dd_index
      on dd_gloveR selected arg_dd_index do dd_mesh_selected GLOVER arg_dd_index
      on dd_bootsL selected arg_dd_index do dd_mesh_selected BOOTSL arg_dd_index
      on dd_bootsR selected arg_dd_index do dd_mesh_selected BOOTSR arg_dd_index

      on dd_sword selected arg_dd_index do
      (
         -- if selected last item(=none), ignore it.
         if (arg_dd_index == mesh_names[SWORD].count) then selected_indices[SWORD] = 0
         else dd_mesh_selected SWORD arg_dd_index
      )

      on dd_shield selected arg_dd_index do
      (
         -- if selected last item(=none), ignore it.
         if (arg_dd_index == mesh_names[SHIELD].count) then selected_indices[SHIELD] = 0
         else dd_mesh_selected SHIELD arg_dd_index
      )

      on dd_axe selected arg_dd_index do
      (
         -- if selected last item(=none), ignore it.
         if (arg_dd_index == mesh_names[AXE].count) then selected_indices[AXE] = 0
         else dd_mesh_selected AXE arg_dd_index
      )

      on dd_bow selected arg_dd_index do
      (
         -- if selected last item(=none), ignore it.
         if (arg_dd_index == mesh_names[BOW].count) then selected_indices[BOW] = 0
         else dd_mesh_selected BOW arg_dd_index
      )
   ) -- end of rollout "ro_model"
   
   rollout ro_light "Light"
   (
      colorpicker cp_ambient "Ambient:" color:light_property[L_AMBIENT]
      colorpicker cp_diffuse "Diffuse:" color:light_property[L_DIFFUSE]
      fn filter_lights obj = isKindOf obj light
      pickbutton pb_light "<<라이트 선택>>" filter:filter_lights
      checkbox cb_nolight "라이트 생략" checked:true

      on cb_nolight changed state do
      (
         if state == true then shader_name = "nolit_shader"
         else shader_name = "lit_shader"
      )

      on pb_light picked obj do
      (
         pb_light.text = obj.name
         cp_ambient.color = copy obj.rgb
         cp_diffuse.color = copy obj.rgb
         light_property[L_AMBIENT] = copy obj.rgb
         light_property[L_DIFFUSE] = copy obj.rgb
         light_property[L_DIRECTION] = copy obj.pos
      )
      on cp_ambient changed obj do
      (
         light_property[L_AMBIENT] = copy cp_ambient.color
      )
      on cp_diffuse changed obj do
      (
         light_property[L_DIFFUSE] = copy cp_diffuse.color
      )
   )

   rollout ro_polycount "PolyCount"
   (
      button bt_polycount "폴리곤갯수확인"

      fn calc_polycount =
      (
         local polycount = 0;
         for i = 1 to NUM_MESHES do
         (
            if (selected_indices[i] > 0) then
            (
               select_str = "$'"+mesh_names[i][selected_indices[i]]+"'.numfaces"
               polycount += execute select_str
            )
         )
         message = "폴리곤 갯수 = "+polycount as string+"개"
         messageBox message
      )

      on bt_polycount pressed do calc_polycount()
   )

   rollout ro_export "Export"
   (
      checkbox cb_skip_animation "애니메이션 생략" checked:false
      button bt_export_view "익스포트 및 보기"
      button bt_export "익스포트"
      button bt_view "보기"

      fn export_config =
      (
         -- export path.lua
         file_name = export_path+"load_model.lua"
         s = createFile file_name
         format "-- This file was automatically created by znTool.ms script.\n" to:s
         format "-- Do not edit this\n\n" to:s
         -- format "shader_name = \"%\"\n" shader_name to:s
         -- if (cb_skip_animation.state == true) then format "skip_animation = 1\n" to:s
         -- else format "skip_animation = 0\n" to:s

         local data_path = convert_slashes export_path
         -- format "data_path = \"%\"\n" data_path to:s
         format "mesh_name = {}\n" to:s
         format "mesh_name[\"head\"] = \"%\"\n" mesh_names[HEAD][selected_indices[HEAD]] to:s
         format "mesh_name[\"armor\"] = \"%\"\n" mesh_names[ARMOR][selected_indices[ARMOR]] to:s
         format "mesh_name[\"gloveL\"] = \"%\"\n" mesh_names[GLOVEL][selected_indices[GLOVEL]] to:s
         format "mesh_name[\"gloveR\"] = \"%\"\n" mesh_names[GLOVER][selected_indices[GLOVER]] to:s
         format "mesh_name[\"bootsL\"] = \"%\"\n" mesh_names[BOOTSL][selected_indices[BOOTSL]] to:s
         format "mesh_name[\"bootsR\"] = \"%\"\n" mesh_names[BOOTSR][selected_indices[BOOTSR]] to:s
         if (selected_indices[SWORD] > 0) then format "mesh_name[\"sword\"] = \"%\"\n" mesh_names[SWORD][selected_indices[SWORD]] to:s
         if (selected_indices[SHIELD] > 0) then format "mesh_name[\"shield\"] = \"%\"\n" mesh_names[SHIELD][selected_indices[SHIELD]] to:s
         if (selected_indices[AXE] > 0) then format "mesh_name[\"axe\"] = \"%\"\n" mesh_names[AXE][selected_indices[AXE]] to:s
         if (selected_indices[BOW] > 0) then format "mesh_name[\"bow\"] = \"%\"\n" mesh_names[BOW][selected_indices[BOW]] to:s

         format "map_name = {}\n" to:s
         format "map_name[\"head\"] = \"%\"\n" (filenameFromPath map_names[HEAD][selected_indices[HEAD]]) to:s
         format "map_name[\"armor\"] = \"%\"\n" (filenameFromPath map_names[ARMOR][selected_indices[ARMOR]]) to:s
         format "map_name[\"gloveL\"] = \"%\"\n" (filenameFromPath map_names[GLOVEL][selected_indices[GLOVEL]]) to:s
         format "map_name[\"gloveR\"] = \"%\"\n" (filenameFromPath map_names[GLOVER][selected_indices[GLOVER]]) to:s
         format "map_name[\"bootsL\"] = \"%\"\n" (filenameFromPath map_names[BOOTSL][selected_indices[BOOTSL]]) to:s
         format "map_name[\"bootsR\"] = \"%\"\n" (filenameFromPath map_names[BOOTSR][selected_indices[BOOTSR]]) to:s

         if (selected_indices[SWORD] > 0) then format "map_name[\"sword\"] = \"%\"\n" map_names[SWORD][selected_indices[SWORD]] to:s
         if (selected_indices[SHIELD] > 0) then format "map_name[\"shield\"] = \"%\"\n" map_names[SHIELD][selected_indices[SHIELD]] to:s
         if (selected_indices[AXE] > 0) then format "map_name[\"axe\"] = \"%\"\n" map_names[AXE][selected_indices[AXE]] to:s
         if (selected_indices[BOW] > 0) then format "map_name[\"bow\"] = \"%\"\n" map_names[BOW][selected_indices[BOW]] to:s

--         format "light = {}\n" to:s
--         light_temp = [0, 0, 0] as point3
--         light_temp.x = light_property[L_AMBIENT].r / 255
--         light_temp.y = light_property[L_AMBIENT].g / 255
--         light_temp.z = light_property[L_AMBIENT].b / 255
--         format "light[\"ambient\"] = { %, %, % }\n" light_temp.x light_temp.y light_temp.z to:s
--         light_temp.x = light_property[L_DIFFUSE].r / 255
--         light_temp.y = light_property[L_DIFFUSE].g / 255
--         light_temp.z = light_property[L_DIFFUSE].b / 255
--         format "light[\"diffuse\"] = { %, %, % }\n" light_temp.x light_temp.y light_temp.z to:s
--         light_temp = light_property[L_DIRECTION]
--         format "light[\"direction\"] = { %, %, % }\n" light_temp.x light_temp.y light_temp.z to:s
         close s
      )
      
      on bt_export_view pressed do
      (
         bt_export.pressed()
         bt_view.pressed()
      )

      on bt_view pressed do
      (
         ShellLaunch viewer_path ""
      )

      on bt_export pressed do
      (
         if export_path == undefined then
         (
            export_path = getSavePath "익스포트할 폴더를 선택하세요."
            export_path = export_path + "/"
         )
         if export_path != undefined then
         (
            root_bone = $'*Pelvis*'
            
            bone_list = #()
            traverse_and_append_bone root_bone[1]

            -- export meshes
            for i = 1 to NUM_MESHES do
            (
               -- if (selected_indices[i] > 0) and ((i < ITEM_START) or (selected_indices[i] < mesh_names[i].count)) then
               if (selected_indices[i] > 0) then
               (
                  full_name = export_path+mesh_names[i][selected_indices[i]]+".zms"
                  ClearSelection()
                  str_exec = "select $'"+mesh_names[i][selected_indices[i]]+"'"
                  execute str_exec
                  print str_exec
                  exportFile full_name #noprompt selectedonly:true
               )
            )
            file_name = export_path+"camera.zca"
            -- export camera
            exportfile file_name #noprompt

            select $'*Pelvis*'            
            -- export motion
            file_name = export_path+"motion.zmo"
            if (cb_skip_animation.state == false) then exportFile file_name #noprompt selectedonly:true

            select $'*Pelvis*'
            -- export model
            file_name = export_path+"model.zmd"
            exportFile file_name #noprompt selectedonly:true

            -- copy map name
            for i = 1 to map_names.count do
            (
               if (selected_indices[i] > 0) then
               (
                  filename = filenameFromPath map_names[i][selected_indices[i]]
                  copyto_name = export_path + filename
                  deleteFile copyto_name -- first delete existing one (overwrite) 
                  copyfile map_names[i][selected_indices[i]] copyto_name
               )
            )
                  
            export_config()
         ) -- end of if export_path != undefined
      ) -- end of on bt_export pressed
   ) -- end of rollout ro_light

   on znTool open do
   (
      for i = 1 to NUM_MESHES do
      (
         mesh_names[i] = #()
         map_names[i] = #()
         selected_indices[i] = 1
         str_type = get_mesh_type_name i
         exec_str = "$'*"+str_type+"*'"
         for obj in execute exec_str do
         (
            if (obj.material != undefined) then
            (
               if (obj.material.maps[2] == undefined) then
               (
                  message = obj.name+"의 디퓨즈맵을 설정해 주세요"
                  messageBox message
               )
               else
               (
                  append mesh_names[i] obj.name
                  -- mapfile_name = convert_slashes obj.material.maps[2].filename
                  full_mapname = obj.material.maps[2].filename
                  append map_names[i] full_mapname
               )
            )
         )
         if (i >= ITEM_START) then
         (
            append mesh_names[i] "<<none>>"
            selected_indices[i] = 0
         )
         print "mesh_names[i] = "
         print mesh_names[i]
      )

      light_property[L_AMBIENT] = [119, 119, 119] as color
      light_property[L_DIFFUSE] = [166, 166, 166] as color
      light_property[L_DIRECTION] = [0, 0, 1] as point3

      addRollout ro_model
      addRollout ro_light
      addRollout ro_export
      addRollout ro_polycount
   )

   on znTool close do
   (
      removeRollout ro_export
      removeRollout ro_light
      removeRollout ro_model
      removeRollout ro_polycount
   )
)  -- end of utility definition
