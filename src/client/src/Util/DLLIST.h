/*
	$Header: /7HeartsOnline/Client/Util/DLLIST.h 2     04-05-27 8:32p Icarus $
*/
#ifndef __DLLISTH
#define __DLLISTH

//---------------------------------------------------------------------------
template <class dType>
class classDLLNODE {
private :

public :
  dType DATA;

  classDLLNODE* m_pNEXT;
  classDLLNODE* m_pPREV;

  classDLLNODE() { m_pNEXT = m_pPREV = nullptr; }

  classDLLNODE(dType pArg1) {
    DATA    = pArg1;
    m_pNEXT = m_pPREV = nullptr;
  }
};

template <class dType>
class classDLLIST {
private :
  int                  iNodeCount;
  classDLLNODE<dType>* m_pHEAD;
  classDLLNODE<dType>* m_pTAIL;

public :
  classDLLIST() { this->Init(); }

  void Init(void        );
  bool IsEmpty(void     ) { return !iNodeCount; }
  int  GetNodeCount(void) { return iNodeCount; }

  void ResetNode(void) {
    m_pHEAD    = m_pTAIL = nullptr;
    iNodeCount = 0;
  }

  classDLLNODE<dType>* GetHeadNode(void                 ) { return m_pHEAD; }
  classDLLNODE<dType>* GetTailNode(void                 ) { return m_pTAIL; }
  classDLLNODE<dType>* GetNextNode(classDLLNODE<dType>* CurrentNode) { return CurrentNode->m_pNEXT; }
  classDLLNODE<dType>* GetPrevNode(classDLLNODE<dType>* CurrentNode) { return CurrentNode->m_pPREV; }

  classDLLNODE<dType>* AllocNAppend(dType               DATA);
  classDLLNODE<dType>* AllocNAppend(void                );
  void                 DeleteNFree(classDLLNODE<dType>* pNodeToDelete);

  void AppendNode(classDLLNODE<dType>*       NodeToAppend);
  void DeleteNode(classDLLNODE<dType>*       NodeToDelete);
  void ReplaceNode(classDLLNODE<dType>*      TargetNode, classDLLNODE<dType>* NodeToReplace);
  void InsertNodeToPrev(classDLLNODE<dType>* TargetNode, classDLLNODE<dType>* NodeToInsert);
  void InsertNodeToNext(classDLLNODE<dType>* TargetNode, classDLLNODE<dType>* NodeToInsert);

  void AppendNodeList(classDLLIST<dType>* pList);

  void ClearList();
  void ClearListNFree();
};

//---------------------------------------------------------------------------
template <class dType>
void classDLLIST<dType>::Init(void) {
  m_pHEAD    = nullptr;
  m_pTAIL    = nullptr;
  iNodeCount = 0;
}

//---------------------------------------------------------------------------
template <class dType>
void classDLLIST<dType>::AppendNodeList(classDLLIST<dType>* pList) {
  if ( m_pHEAD == nullptr ) {
    m_pHEAD    = pList->GetHeadNode();
    m_pTAIL    = pList->GetTailNode();
    iNodeCount = pList->GetNodeCount();
  } else {
    classDLLNODE<dType>* pHEAD = pList->GetHeadNode();
    if ( pHEAD ) {
      pHEAD->m_pPREV   = m_pTAIL;
      m_pTAIL->m_pNEXT = pHEAD;

      m_pTAIL = pList->GetTailNode();
      iNodeCount += pList->GetNodeCount();
    }
  }
}

//---------------------------------------------------------------------------
template <class dType>
classDLLNODE<dType>*   classDLLIST<dType>::AllocNAppend(void) {
  classDLLNODE<dType>* pNewNode;

  pNewNode = new classDLLNODE<dType>;
  if ( pNewNode ) {
    AppendNode( pNewNode );
    return pNewNode;
  }
  return nullptr;
}

template <class dType>
classDLLNODE<dType>*   classDLLIST<dType>::AllocNAppend(dType DATA) {
  classDLLNODE<dType>* pNewNode;

  pNewNode = new classDLLNODE<dType>;
  if ( pNewNode ) {
    AppendNode( pNewNode );
    pNewNode->DATA = DATA;
    return pNewNode;
  }
  return nullptr;
}

//---------------------------------------------------------------------------

template <class dType>
void classDLLIST<dType>::DeleteNFree(classDLLNODE<dType>* pNodeToDelete) {
  if ( pNodeToDelete ) {
    DeleteNode( pNodeToDelete );
    delete pNodeToDelete;
  }

  pNodeToDelete = nullptr;
}

//---------------------------------------------------------------------------

template <class dType>
void classDLLIST<dType>::AppendNode(classDLLNODE<dType>* NodeToAdd) {
  if ( m_pHEAD == nullptr ) {
    m_pHEAD            = m_pTAIL = NodeToAdd;
    NodeToAdd->m_pNEXT = nullptr;
    NodeToAdd->m_pPREV = nullptr;
  } else {
    NodeToAdd->m_pNEXT = nullptr;
    NodeToAdd->m_pPREV = m_pTAIL;
    m_pTAIL->m_pNEXT   = NodeToAdd;

    m_pTAIL = NodeToAdd;
  }

  iNodeCount ++;
} ;

//---------------------------------------------------------------------------

template <class dType>
void classDLLIST<dType>::DeleteNode(classDLLNODE<dType>* NodeToDel) {
  if ( NodeToDel == nullptr ) return;

  if ( NodeToDel == m_pHEAD ) {
    m_pHEAD = m_pHEAD->m_pNEXT;

    if ( m_pHEAD != nullptr )
      m_pHEAD->m_pPREV = nullptr;
    else
      m_pTAIL = nullptr;
  } else if ( NodeToDel == m_pTAIL ) {
    m_pTAIL          = m_pTAIL->m_pPREV;
    m_pTAIL->m_pNEXT = nullptr;
  } else {
    NodeToDel->m_pPREV->m_pNEXT = NodeToDel->m_pNEXT;
    NodeToDel->m_pNEXT->m_pPREV = NodeToDel->m_pPREV;
  }

  //    NodeToDel->m_pPREV = NodeToDel->m_pNEXT = nullptr;
  iNodeCount --;
} ;

//---------------------------------------------------------------------------

template <class dType>
void classDLLIST<dType>::ReplaceNode(classDLLNODE<dType>* TargetNode, classDLLNODE<dType>* NodeToReplace) {
  if ( TargetNode == nullptr ) return;

  NodeToReplace->m_pPREV = TargetNode->m_pPREV;
  NodeToReplace->m_pNEXT = TargetNode->m_pNEXT;

  if ( TargetNode == m_pHEAD )
    m_pHEAD = NodeToReplace;
  else
    TargetNode->m_pPREV->m_pNEXT = NodeToReplace;

  if ( TargetNode == m_pTAIL )
    m_pTAIL = NodeToReplace;
  else
    TargetNode->m_pNEXT->m_pPREV = NodeToReplace;
}

//---------------------------------------------------------------------------

template <class dType>
void classDLLIST<dType>::InsertNodeToPrev(classDLLNODE<dType>* TargetNode, classDLLNODE<dType>* NodeToIns) {
  if ( TargetNode == nullptr ) return;

  if ( TargetNode == m_pHEAD ) {
    NodeToIns->m_pPREV = nullptr;
    NodeToIns->m_pNEXT = m_pHEAD;
    m_pHEAD->m_pPREV   = NodeToIns;
    m_pHEAD            = NodeToIns;
  } else {
    NodeToIns->m_pPREV           = TargetNode->m_pPREV;
    NodeToIns->m_pNEXT           = TargetNode;
    TargetNode->m_pPREV->m_pNEXT = NodeToIns;
    TargetNode->m_pPREV          = NodeToIns;
  }

  iNodeCount ++;
}

//---------------------------------------------------------------------------

template <class dType>
void classDLLIST<dType>::InsertNodeToNext(classDLLNODE<dType>* TargetNode, classDLLNODE<dType>* NodeToIns) {
  if ( TargetNode == nullptr ) return;

  if ( TargetNode == m_pTAIL ) {
    NodeToIns->m_pPREV = m_pTAIL;
    NodeToIns->m_pNEXT = nullptr;
    m_pTAIL->m_pNEXT   = NodeToIns;
    m_pTAIL            = NodeToIns;
  } else {
    NodeToIns->m_pPREV           = TargetNode;
    NodeToIns->m_pNEXT           = TargetNode->m_pNEXT;
    TargetNode->m_pNEXT->m_pPREV = NodeToIns;
    TargetNode->m_pNEXT          = NodeToIns;
  }

  iNodeCount ++;
}

//---------------------------------------------------------------------------

template <class dType>
void                   classDLLIST<dType>::ClearList() {
  classDLLNODE<dType>* pNode;

  pNode = GetHeadNode();
  while ( pNode ) {
    DeleteNode( pNode );
    pNode = GetHeadNode();
  }
}

//---------------------------------------------------------------------------

template <class dType>
void                   classDLLIST<dType>::ClearListNFree() {
  classDLLNODE<dType>* pNode;

  pNode = GetHeadNode();
  while ( pNode ) {
    DeleteNFree( pNode );
    pNode = GetHeadNode();
  }
}

//---------------------------------------------------------------------------
#endif
