
fn MakeHeightPatch width length wseg lseg xCount yCount=
(
	Myplane = plane width:width length:length widthsegs:wseg lengthsegs:lseg;	
	Myplane.pos = [ (fStartX + xCount*fPatchSize), ( fStartY - yCount*fPatchSize ), 0];
		
	PlaneArray[ gPlaneCount ] = Myplane;
	gPlaneCount +=1;
	
	convertToMesh Myplane;
	
	local fHeight;
	
	local iStartY = (yCount*iPatchCnt );					-- current patch starting pos in heightsmap 
	local iStartX = (xCount*iPatchCnt );
	
				
	for k = 1 to Myplane.numverts do
	(
		y = iStartY + ( 4 - (k-1)/( iPatchCnt + 1 ) );
		
		if ( mod k ( iPatchCnt + 1 ) ) == 0 
		then x = iStartX + 4;
		else x = iStartX + ( mod k ( iPatchCnt + 1 ) ) - 1;
	
		--x = iStartX + ( mod k ( iPatchCnt + 1 ) ) - 1;
		fHeight = fHeightsArray[ y*lMapWidth + x + 1 ];
		
		--v1 = getVert Myplane.mesh k;		
		--v1 += [ 0, 0, fHeight];
		
		Myplane.verts[ k ].pos += [ 0, 0, fHeight];
		
		--v2 = getVert Myplane.mesh k;
		--format "%:\t%\n" "asdfs" v2;		
	)	
	
)

fn LoadHeighsMap =
(
	global PlaneArray = #();	
	global gPlaneCount = 1;
	
	fname = getOpenFileName \ 
		types:"Data(*.him)|*.him|All|*.*|";
		
	if fname != undefined then
	(

		fs = fopen fname "rb";	
	
		if fs == undefined then messagebox "file open error";
				
		global lMapWidth;
		global lMapHeight;
		
		lMapWidth = ReadLong fs;
		lMapHeight = ReadLong fs;	
		
		iHeightsIdx = 1;
		
		global fHeightsArray = #();	
		local fHeight;
		
		--fHeight = ReadFloat fs;
		--fHeightsArray[ iHeightsIdx ] = fHeight;
	
	
		for i = 1 to lMapHeight do
		(
			for j = 1 to lMapWidth do
			(
				fHeight = ReadFloat fs;
				fHeightsArray[ iHeightsIdx ] = fHeight;
				
				iHeightsIdx = iHeightsIdx + 1;
			)
		)
	
		format "_t%\n" fHeightsArray.count;
		
		MakeHeightsMap lMapWidth lMapHeight;
		
		FirstPlane = PlaneArray[ 1 ];
		for i = 2 to PlaneArray.count do
		(			
			TempPlane = PlaneArray[ i ];
			meshop.attach FirstPlane TempPlane;
		) 
		
		--select $Plane01.baseObject.verts[#{1..5625}]
		--select FirstPlane.baseObject.verts[#{1..5625}]
		meshop.weldVertsByThreshold FirstPlane.mesh FirstPlane.verts[#{1..FirstPlane.numverts}] 1.0
		
		close fs;
	)
)

fn MakeHeightsMap lWidth lHeight =
(
	global iPatchCnt = 4;
	global fPatchSize = 1000;

	global iPatchCountX = lWidth / 4;
	global iPatchCountY = lHeight / 4;
	
	global fStartX = -(iPatchCountX / 2 * fPatchSize);
	global fStartY = iPatchCountY / 2 * fPatchSize;
	
	format "_t%\n" iPatchCountx ;
	
	yCount = 0; 
	xCount = 0; 
	while yCount < iPatchCountY - 1 do
	(
		xCount = 0; 
		while xCount < iPatchCountX - 1 do
		(
		--for i = 0 to iPatchCountx - 1 do
		--(
			MakeHeightPatch fPatchSize fPatchSize iPatchCnt iPatchCnt xCount yCount;
			xCount += 1;
		)
		yCount += 1;
	)	
)

fn LoadObjectList =
(
	global ObjectList = #();
	
	fname = getOpenFileName \ 
		types:"text(*.txt)|*.txt|All|*.*|";
	
	fs = openFile fname;	
	
	if fs == undefined then messagebox "file open error";

	iIndex = 1;
	while (eof fs) == false do
	(
		str = readLine fs;

		ObjectList[ iIndex ] = str;
		iIndex += 1;
		
		bLoad = mergeMAXFile str #select;
				
		if bLoad == true then
		(
			objectcount = $.count
			
			firstObject;

			iCount = 1;
			iIndex = 1;
			
			NonGeomObject = #();
			while iCount < objectcount do
			(
				if superClassOf $[iCount] != GeometryClass  then
				(					
					NonGeomObject[ iIndex ] = $[iCount];
					iIndex +=1;
				)
				
				$.name = str as name;				
				
				iCount += 1;
			)
			
			delete NonGeomObject;
		)		
	)
	
	close fs;
)

rollout LoadHeightsMapImport "LoadHeightsMapImport" width:162 height:167
(
	button LoadHeightsMap "Load Heights Map" pos:[13,19] width:136 height:25
	button LoadObjectMap "Load Object Map" pos:[13,50] width:136 height:25
	on LoadHeightsMap pressed  do
		LoadHeighsMap()
	on LoadObjectMap pressed  do
	(	
		LoadObjectList()
	)
)

addRollout LoadHeightsMapImport 
