fn CopyPivot =
(
	src = pickObject prompt:"¿øº»°´Ã¼"
	dest = pickObject prompt:"´ë»ó°´Ã¼"
	if (src == undefined) then return false
	if (dest == undefined) then return false
	newoffset = dest.objecttransform * (inverse src.transform)
	dest.transform = src.transform
	dest.objectoffsetrot = newoffset.rotation
	dest.objectoffsetpos = newoffset.pos
)
CopyPivot()