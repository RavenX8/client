include "znChannelFunc.ms"

utility znChannel "znChannel"
(
--x--x--x--x--x--x--x--x--x--x-- from znChannel_vis.ms --x--x--x--x--x--x--x--x--x--x--x--x
rollout roChannel "��ä��" width:162 height:340
(
	editText edt_name "" pos:[7,8] width:147 height:25
	label lbl2 "��ä�ΰ���:" pos:[12,42] width:95 height:22
	label lbl_nummapchannel "0" pos:[116,43] width:35 height:18
	checkbox chk_07 "ä��7" pos:[31,202] width:117 height:24
	checkbox chk_02 "ä��2" pos:[31,87] width:116 height:24
	checkbox chk_03 "ä��3" pos:[31,110] width:116 height:24
	checkbox chk_06 "ä��6" pos:[31,179] width:116 height:24
	checkbox chk_05 "ä��5" pos:[31,156] width:116 height:24
	checkbox chk_04 "ä��4" pos:[31,133] width:116 height:24
	checkbox chk_01 "ä��1" pos:[31,64] width:116 height:24
	checkbox chk_09 "ä��9" pos:[31,248] width:116 height:24
	checkbox chk_08 "ä��8" pos:[31,225] width:116 height:24
	checkbox chk_10 "ä��10" pos:[31,271] width:116 height:24
	timer tmr2 "Timer" pos:[127,244] width:24 height:24 enabled:true interval:1000
	button btn_update "������Ʈ����" pos:[13,301] width:138 height:23
--x--x--x--x--x--x--x--x--x--x-- from znChannel_vis.ms --x--x--x--x--x--x--x--x--x--x--x--x
    
	fn updateToControls =
	(
	    if (selection.count != 1) then
	    (
	        edt_name.text = ""
	        return false
	    )
	    edt_name.text = $.name
	    lbl_nummapchannel.text = (get_num_mapchannel $) as string
	    chk_01.checked = (get_map_support $ 1)
	    chk_02.checked = (get_map_support $ 2)
	    chk_03.checked = (get_map_support $ 3)
	    chk_04.checked = (get_map_support $ 4)
	    chk_05.checked = (get_map_support $ 5)
	    chk_06.checked = (get_map_support $ 6)
	    chk_07.checked = (get_map_support $ 7)
	    chk_08.checked = (get_map_support $ 8)
	    chk_09.checked = (get_map_support $ 9)
	    chk_10.checked = (get_map_support $ 10)
    )
    
	fn updateFromControls =
	(
	    if (selection.count != 1) then
	    (
	        edt_name.text = ""
	        return false
	    )
	    edt_name.text = $.name
	    set_map_support $ 1 (chk_01.checked)
	    set_map_support $ 2 (chk_02.checked)
	    set_map_support $ 3 (chk_03.checked)
	    set_map_support $ 4 (chk_04.checked)
	    set_map_support $ 5 (chk_05.checked)
	    set_map_support $ 6 (chk_06.checked)
	    set_map_support $ 7 (chk_07.checked)
	    set_map_support $ 8 (chk_08.checked)
	    set_map_support $ 9 (chk_09.checked)
	    set_map_support $ 10 (chk_10.checked)
	    lbl_nummapchannel.text = (get_num_mapchannel $) as string
	    
	    local num_mapchannel = 0
   		if (get_map_support $ 1) do num_mapchannel = num_mapchannel + 1
        if (get_map_support $ 3) do num_mapchannel = num_mapchannel + 1
        if (num_mapchannel == 0) do messageBox ($.name + "�� 1�� �Ǵ� 3�� ��ä���� �����Ǿ� ���� �ʽ��ϴ�.")        
		setUserProp $ "mesh_num_mapchannel" num_mapchannel        
    )

	on chk_01 changed state do ( updateFromControls() )
	on chk_02 changed state do ( updateFromControls() )
	on chk_03 changed state do ( updateFromControls() )
    on chk_04 changed state do ( updateFromControls() )
    on chk_05 changed state do ( updateFromControls() )
    on chk_06 changed state do ( updateFromControls() )
    on chk_07 changed state do ( updateFromControls() )
    on chk_08 changed state do ( updateFromControls() )
    on chk_09 changed state do ( updateFromControls() )
    on chk_10 changed state do ( updateFromControls() )

	on btn_update pressed  do
    (
	    updateToControls()
	)
	
	on tmr2 tick  do
    (
        --updateToControls()
    )    
    
    on roChannel open do
    (
        updateToControls()
        updateFromControls()
    )

    on roChannel close do
    (
        updateFromControls()
    )    
)

on znChannel open  do
(
    addRollout roChannel
)

on znChannel close do
(
	removeRollout roChannel
)
)