rollout roChannel "맵채널" width:162 height:340
(
	editText edt_name "" pos:[7,8] width:147 height:25
	label lbl2 "맵채널개수:" pos:[12,42] width:95 height:22
	label lbl_nummapchannel "0" pos:[116,43] width:35 height:18
	checkbox chk_07 "채널7" pos:[31,202] width:117 height:24
	checkbox chk_02 "채널2" pos:[31,87] width:116 height:24
	checkbox chk_03 "채널3" pos:[31,110] width:116 height:24
	checkbox chk_06 "채널6" pos:[31,179] width:116 height:24
	checkbox chk_05 "채널5" pos:[31,156] width:116 height:24
	checkbox chk_04 "채널4" pos:[31,133] width:116 height:24
	checkbox chk_01 "채널1" pos:[31,64] width:116 height:24
	checkbox chk_09 "채널9" pos:[31,248] width:116 height:24
	checkbox chk_08 "채널8" pos:[31,225] width:116 height:24
	checkbox chk_10 "채널10" pos:[31,271] width:116 height:24
	timer tmr2 "Timer" pos:[127,244] width:24 height:24 enabled:true interval:1000
	button btn_update "오브젝트갱신" pos:[13,301] width:138 height:23
	on roChannel open  do
(
	
	)
	on chk_01 changed state do
(
	
	)
	on tmr2 tick  do
(
	
	)
	on btn_update pressed  do
(
	
	)
)