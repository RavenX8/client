/*
	$Header: /Client/IO_MESH.H 8     03-11-27 3:05p Jeddli $
*/
#ifndef	__IO_MESH_H
#define	__IO_MESH_H
#ifdef _WIN64
typedef size_t HNODE;
#else
typedef unsigned int HNODE;
#endif

//-------------------------------------------------------------------------------------------------

/// mesh list
class CMeshLIST : public CFileLIST<HNODE> {
private :
  bool Load_FILE(tagFileDATA<HNODE>* pHNODE) override;
  void Free_FILE(tagFileDATA<HNODE>* pHNODE) override;

public :
  CMeshLIST() : CFileLIST<HNODE>( "MESH" ) { }
  ~CMeshLIST() { CFileLIST<HNODE>::Free(); }

  HNODE IDX_HNODE(short     nIndex) { return this->Get_DATAUseIDX( nIndex ); }
  HNODE KEY_HNODE(t_HASHKEY HashKEY); //	{	return this->Get_DATAUseKEY(HashKEY);	}
};

extern CMeshLIST g_MeshFILE;

//-------------------------------------------------------------------------------------------------
#endif
