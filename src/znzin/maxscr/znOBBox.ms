plugin Geometry znOBBox
	name:"znOBBox"
	category:"znzin"
	classID:#(0xbcc798f8, 0x44477dba)
	extends:Box
	version:3
(
	parameters main	rollout:params
	(
		obb_object type:#node ui:btn_pick	
		
--		on obb_object set obj do
--		(
--			if (obj != undefined) do
--			(
--				$.pos = obj.pos
--				$.rotation = obj.rotation
--			)
--		)
	)
	rollout params "znOBBox Parameters" width:162 height:32
	(
		pickbutton btn_pick "Pick" pos:[87,6] width:68 height:21
		label lbl1 "¿¬°áµÈ °´Ã¼ :" pos:[7,8] width:75 height:19
		
		on params open do
		(
			if obb_object != undefined do
			(
				btn_pick.text = obb_object.name
			)
		)
		on btn_pick picked obj do
		(
			btn_pick.text = obj.name
			$.pos = obj.center + $.pos - $.center
			$.rotation = obj.rotation
		)
	)
)

