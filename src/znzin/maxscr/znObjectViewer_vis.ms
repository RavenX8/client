rollout ro_object_viewer "znObjectViewer" width:162 height:426
(
	listBox lb_object_list "���û���" pos:[12,25] width:137 height:4
	pickButton btn_pick "Pick" pos:[9,123] width:44 height:19
	button btn_add "Add" pos:[58,123] width:44 height:19
	button btn_delete "Delete" pos:[107,123] width:44 height:19
	button btn_deleteAll "deleteAll" pos:[100,27] width:49 height:13
	button btn_export_building "���" pos:[9,327] width:144 height:26 enabled:true
	label lbl_version "����" pos:[12,7] width:89 height:19
	button btn_export_path "������� ����" pos:[12,289] width:138 height:26
	editText edt_export_path "" pos:[9,264] width:141 height:21
	groupBox grp1 "TXT ��°��" pos:[7,195] width:150 height:127
	timer tmr1 "Timer" pos:[126,431] width:24 height:24
	dropDownList ddl_link "���� ����Ʈ :" pos:[11,148] width:139 height:41 items:#("����", "������", "�޼�", "�޼չ���", "��", "��", "��", "������")
	button btn_export_root "��Ʈ���� ����" pos:[13,234] width:138 height:26
	editText edt_export_root "" pos:[10,209] width:141 height:21
	checkbox chk_onlytxt "TXT�� ����ϱ�" pos:[11,359] width:134 height:28
	checkbox chk_pivot_origin "�Ǻ��� ��������" pos:[12,388] width:139 height:21
	on btn_pick picked obj do
(
	
	)
	on btn_add pressed  do
(
	
	)
	on btn_delete pressed  do
(
	
	)
	on btn_deleteAll pressed  do
(
	
	)
	on tmr1 tick  do
	(

	    refresh_list lb_aa

	)
)