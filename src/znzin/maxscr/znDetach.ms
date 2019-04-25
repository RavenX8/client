----------------------------------------------------------
-- znDetach : znzin object detach tool script
-- by zho [2003.02.19]

-- version : .003
-- update log:
-- no vertex mesh removed [2003.02.19]
----------------------------------------------------------

-- user interface by znDetach.vms

global name_list = #()
global node_list = #()
global map_channel = 3
global face_list = #()
global dummy_mesh = undefined

fn delete_dummy =
(
   if (dummy_mesh != undefined) do
   (
      delete dummy_mesh
      dummy_mesh = undefined
   )
)

fn add_one in_list obj =
(
   if (convertToMesh obj == undefined) do
   (
      --messageBox "Editable Mesh가 아닙니다." title:"error"
      return false
   )
   if (obj.numverts == 0) do
   (
   		--messageBox "메쉬에 버텍스가 하나도 없습니다." title:"error"
   		return false
   )
   if (findItem node_list obj > 0) do return false -- skip already inserted
   append name_list obj.name
   append node_list obj
   append face_list obj.faces.count
   in_list.items = #()
   in_list.items = name_list
)

fn add_array in_list in_array =
(
   for obj in in_array do
   (
      add_one in_list obj
   )
)

fn copy_mapchannel input_src input_dest face_start face_end =
(
   does_support_given_map_channel = meshop.getMapSupport input_src map_channel
   if (does_support_given_map_channel == false) do
   (
      messageBox "맵 채널 지원 안됨"
      return false
   )
   meshop.setNumMaps input_dest (map_channel+1)
   meshop.setMapSupport input_dest map_channel true
   num_verts = meshop.getNumMapVerts input_src map_channel
   meshop.setNumMapVerts input_dest map_channel num_verts keep:false
   meshop.setNumMapfaces input_dest map_channel (face_end - face_start + 1) keep:false
   for i = 1 to num_verts do
   (
      xyz = meshop.getMapVert input_src map_channel i
      meshop.setMapVert input_dest map_channel i xyz
   )
   for i = face_start to face_end do
   (
      xyz = meshop.getMapFace input_src map_channel i
      meshop.setMapFace input_dest map_channel (i - face_start + 1) xyz
   )
   return true
)  

utility znDetach "znDetach v.003"
(
   local object_list = #()
   
   rollout ro_attach "어태치 리스트" width:162 height:290
   (
      button btn_deleteAll "deleteAll" pos:[103,12] width:47 height:13
	   listbox lb_object_list "선택상자" items:object_list pos:[11,12] width:139 height:5
	   pickButton btn_pick "Pick" pos:[9,112] width:45 height:23
	   button btn_add "Add" pos:[59,112] width:45 height:23
	   button btn_delete "Delete" pos:[109,112] width:45 height:23
	   checkButton ckb_attach "어태치" pos:[9,145] width:146 height:23
	   button btn_detach "디태치" pos:[9,177] width:146 height:23
	   label lbl1 "사용법 : 선택상자에 객체를 추가하고 어태치한 뒤 라이트맵을 적용하고 디태치시킨다." pos:[15,213] width:136 height:62

      on btn_deleteAll pressed do
      (
         name_list = #()
         node_list = #()
         face_list = #()
         lb_object_list.items = #()
      )
      
      on btn_pick picked obj do
      (
         add_one lb_object_list obj
      )
         
      on btn_add pressed do
      (
         selection_list = selectByName title:"객체 선택" 
         if (selection_list != undefined) do add_array lb_object_list selection_list
      )
      
      on btn_delete pressed do
      (
         sel_num = lb_object_list.selection
         if sel_num == 0 do return false
         
         deleteItem name_list sel_num
         deleteItem node_list sel_num
         deleteItem face_list sel_num
         lb_object_list.items = #()
         lb_object_list.items = name_list
      )
      
      on ckb_attach changed state do
      (
         delete_dummy()
         if (state == false) do
         (
            for i = 1 to node_list.count do
            (
               unhide node_list[i]
            )
            return true
         )
         dummy_mesh = mesh vertices:#() faces:#()
         for i = 1 to node_list.count do
         (
            meshop.attach dummy_mesh node_list[i] attachMat:#IDToMat condenseMat:false deleteSourceNode:false
            hide node_list[i]
         )
         select dummy_mesh
      )

      on btn_detach pressed do
      (
         if (dummy_mesh == undefined) do return false
         -- copy channel info
         fstart = 1
         -- index 1 means original mesh that contains light map uvs
         for i = 1 to name_list.count do
         (
            fend = fstart + face_list[i] - 1
            flist = #{fstart..fend}
            ret = copy_mapchannel dummy_mesh node_list[i] fstart fend
            if (ret == false) do
            (
               messageBox "디태치 실패"
               return false
            )
            fstart = fend + 1
         )
         -- delete faces
         fstart = 1
         for i = 1 to name_list.count do
         (
            fend = face_list[i]
            flist = #{fstart..fend}
            meshop.deleteFaces dummy_mesh flist
            --meshop.detachFaces dummy_mesh flist delete:true asMesh:true
            unhide node_list[i]
         )
         delete_dummy()
         ckb_attach.checked = false
      )
   )
   
   on znDetach open do
   (
      --append object_list "haha"
      --append object_list "hoho"
      addRollout ro_attach
   )

   on znDetach close do
   (
      removeRollout ro_attach
      delete_dummy()
   )
)

