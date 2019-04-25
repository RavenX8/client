----------------------------------------------------------
-- convert_slashes : 
-- by zho [2003-03-24-01]
----------------------------------------------------------
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