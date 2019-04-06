-- export all meshes automatically
-- by Jiho Choi [2002.08.23]

--select $man01*
for obj in $man01* do
(
  file_path = "d:\\znzin11\\data\\man011\\"
  obj_name = obj.name
  full_name = file_path+obj_name+".zms"
  clearselection()
  select obj
  exportfile full_name #noprompt selectedonly:true
)
--for obj in selection do print obj.name
