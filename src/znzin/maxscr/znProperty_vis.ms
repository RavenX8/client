rollout ro_property "znProperty" width:160 height:437
(
	checkbox chk_mesh_collision "�浹üũ��" pos:[16,87] width:130 height:26 enabled:false checked:true
	groupBox grp1 "�޽� �ִϸ��̼�" pos:[14,169] width:134 height:130
	checkbox chk_morph_pos "��ġ" pos:[25,186] width:113 height:21 enabled:false checked:true
	checkbox chk_morph_visibility "��������Ƽ" pos:[25,214] width:113 height:21 enabled:false
	checkbox chk_morph_uv "UV" pos:[25,242] width:113 height:21 enabled:false
	checkbox chk_morph_world "�������" pos:[25,270] width:113 height:21 enabled:false checked:true
	spinner spn_num_mapchannel "��ä�� ����" pos:[22,44] width:117 height:16 enabled:false range:[1,2,1] type:#integer
	groupBox grp2 "������Ʈ �ִϸ��̼�" pos:[14,312] width:134 height:110
	checkbox chk_obj_position "��ġ" pos:[25,335] width:106 height:20 enabled:false checked:true
	checkbox chk_obj_rotation "ȸ��" pos:[25,361] width:109 height:23 enabled:false checked:true
	checkbox chk_obj_scale "������" pos:[25,390] width:111 height:21 enabled:false checked:true
	radioButtons rdo_animation_type "�ִϸ��̼� Ÿ�� :" pos:[14,118] width:142 height:46 enabled:false labels:#("����", "�޽�", "������Ʈ") columns:2
	timer tmr1 "Timer" pos:[143,401] width:24 height:24
	editText edt_selected "" pos:[14,10] width:131 height:20
	checkbox chk_mesh_specular "����ŧ�� �����" pos:[16,66] width:117 height:19
	on chk_mesh_collision changed state do
(
	
	)
	on chk_morph_pos changed state do
(
	
	)
	on chk_morph_visibility changed state do
(
	
	)
	on chk_morph_uv changed state do
(
	
	)
	on chk_morph_world changed state do
(
	
	)
	on tmr1 tick  do
(
	
	)
)