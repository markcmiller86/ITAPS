#include <assert.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

#include "iMeshTester.hh"

using std::cout;
using std::endl;
using std::set;
using std::map;

typedef struct _EntSort_ {
private:
  int m_numVerts;
  iBase_EntityHandle m_ent, m_verts[8];
public:
  _EntSort_ (iBase_EntityHandle ent, int numVerts,
		   iBase_EntityHandle verts[]) :
  m_numVerts(numVerts), m_ent(ent)
  {
    std::copy(verts, verts+numVerts, m_verts);
    std::sort(m_verts, m_verts + m_numVerts);
  }
  bool operator<(const _EntSort_& ES) const
  {
    // This should sort entities in order by increasing number of verts.
    // Quads and tets will, in general, be intermixed as a result.
    if (m_numVerts < ES.m_numVerts) return true;
    if (m_numVerts > ES.m_numVerts) return false;
    return std::lexicographical_compare(m_verts, m_verts + m_numVerts,
					ES.m_verts, ES.m_verts + m_numVerts);
  }
  bool operator==(const _EntSort_& ES) const
  {
    return (m_numVerts == ES.m_numVerts &&
	    std::equal(m_verts, m_verts + m_numVerts, ES.m_verts));
  }
  bool operator!=(const _EntSort_& ES) const
  {
    return !(operator==(ES));
  }
  iBase_EntityHandle entity() const {return m_ent;}
} EntSort;

void iMeshTester::setupEntityMaps() {
  startTest(MU_setup);
  // Now get the vert-vert map set up.

  {
    // First, get all the vertices, sorted lexicographically.
    set<VertMatch> setVertsRef, setVertsTest;
    m_pMW_ref->sortVertices(setVertsRef);
    m_pMW_test->sortVertices(setVertsTest);
    
    set<VertMatch>::iterator iterRef, iterRefEnd = setVertsRef.end();
    set<VertMatch>::iterator iterTest, iterTestEnd = setVertsTest.end();
    
    // Now match them up and put them in a map.
    std::stringstream ss;
    for (iterRef = setVertsRef.begin(), iterTest = setVertsTest.begin();
	 iterRef != iterRefEnd && iterTest != iterTestEnd;
	 iterRef++, iterTest++) {
      VertMatch VMRef = *iterRef;
      VertMatch VMTest = *iterTest;
      if (VMRef != VMTest) {
	ss << "Vertices don't match in setup!" << endl
	   << "  Ref:  (" << VMRef.x << ", " << VMRef.y << ", " << VMRef.z
	   << ")" << endl
	   << "  Test: (" << VMTest.x << ", " << VMTest.y << ", " << VMTest.z
	   << ")" << endl;
	test(VMRef == VMTest, ss.str());
      }
      else {
	m_refToTestVertMap.
	  insert(std::pair<iBase_EntityHandle, iBase_EntityHandle>
		 (reinterpret_cast<iBase_EntityHandle>(VMRef.handle),
		  reinterpret_cast<iBase_EntityHandle>(VMTest.handle)));
      }	    
    }
    ss << "Problem creating map between vertices! " << endl
       << "  Tested implementation has " << setVertsTest.size() << " vertices"
       << endl
       << "  Reference implementation has " << setVertsRef.size() << " vertices"
       << endl
       << "  Total size of vertex map is " << m_refToTestVertMap.size()
       << endl;
    test(m_refToTestVertMap.size() == setVertsRef.size() &&
	 m_refToTestVertMap.size() == setVertsTest.size(), ss.str());
    
    if (m_numSubtestsFailed == 0) {
      cout << "Matched up " << m_refToTestVertMap.size() << " vertices."
	   << endl;
    }
    else {
      cout << "Couldn't map vertices between meshes; "
	   << "no adjacency or coordinate testing is possible." << endl;
      throw(iBase_FAILURE);
    }
  }
    
  // Now create a map of all other entities.  The following code
  // produces collections of entity->vert maps (with vertex handles
  // expressed for the test mesh and in sorted order).  This certain to
  // work for the reference implementation, because all functions exist
  // there. 

  {
    set<EntSort> setEntsRef;
    {
      iBase_EntityHandle* handles;
      int handles_allocated = 0, handles_size;
      m_pMW_ref->getEntities(m_refInstance, m_refRootSet, iBase_ALL_TYPES,
			     iMesh_ALL_TOPOLOGIES, &handles, &handles_allocated,
			     &handles_size, &m_refErr);
      iBase_EntityHandle* adj_handles;
      adj_handles = reinterpret_cast<iBase_EntityHandle*>
	(malloc(sizeof(iBase_EntityHandle) * 8));
      int adj_handles_allocated = 8, adj_handles_size;
      
      for (int i = 0; i < handles_size; i++) {
	int type;
	m_pMW_ref->getEntType(m_refInstance, handles[i], &type, &m_refErr);
	if (type == iBase_VERTEX) continue;
	
	m_pMW_ref->getEntAdj(m_refInstance, handles[i], iBase_VERTEX,
			     &adj_handles, &adj_handles_allocated,
			     &adj_handles_size, &m_refErr);
	if (m_refErr != iBase_SUCCESS) test(m_refErr, "Failure in ref impl!");
	
	map<iBase_EntityHandle, iBase_EntityHandle>::iterator iterEnd =
	  m_refToTestVertMap.end();
	// Map from ref mesh vert handles to test mesh vert handles.
	for (int ii = 0; ii < adj_handles_size; ii++) {
	  map<iBase_EntityHandle, iBase_EntityHandle>::iterator iter =
	    m_refToTestVertMap.find(adj_handles[ii]);
	  assert(iter != iterEnd);
	  adj_handles[ii] = iter->second;
	}
	EntSort ES(handles[i], adj_handles_size, adj_handles);
	setEntsRef.insert(ES);
      }
      free(adj_handles);
      free(handles);
    }
    
    set<EntSort> setEntsTest;
    // NOT GENERAL
    // The following code isn't nearly general enough!  It assumes that
    // getEntities and getEntAdj exist.
    {
      iBase_EntityHandle* handles;
      int handles_allocated = 0, handles_size;
      m_pMW_test->getEntities(m_testInstance, m_testRootSet, iBase_ALL_TYPES,
			      iMesh_ALL_TOPOLOGIES, &handles, &handles_allocated,
			      &handles_size, &m_err);
      iBase_EntityHandle* adj_handles;
      adj_handles = reinterpret_cast<iBase_EntityHandle*>
	(malloc(sizeof(iBase_EntityHandle) * 8));
      int adj_handles_allocated = 8, adj_handles_size;
      
      for (int i = 0; i < handles_size; i++) {
	int type;
	m_pMW_test->getEntType(m_testInstance, handles[i], &type, &m_err);
	if (type == iBase_VERTEX) continue;
	
	m_pMW_test->getEntAdj(m_testInstance, handles[i], iBase_VERTEX,
			      &adj_handles, &adj_handles_allocated,
			      &adj_handles_size, &m_err);
	if (m_err != iBase_SUCCESS) test(m_err, "Failure in test impl!");
	
	EntSort ES(handles[i], adj_handles_size, adj_handles);
	setEntsTest.insert(ES);
      }
      free(adj_handles);
      free(handles);
    }
    
    // If this is false, then there are certainly entities represented in
    // the test mesh that aren't represented in the reference mesh.
    assert(setEntsTest.size() <= setEntsRef.size());
    
    set<EntSort>::iterator iterRef, iterRefEnd = setEntsRef.end();
    set<EntSort>::iterator iterTest, iterTestEnd = setEntsTest.end();
    int matchTopoCounts[] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int refTopoCounts[12];
    for (int ii = 0; ii < 12; ii++) {
      m_pMW_ref->getNumOfTopo(m_refInstance, m_refRootSet, ii,
			      &(refTopoCounts[ii]), &m_refErr);
    }
    for (iterRef = setEntsRef.begin(), iterTest = setEntsTest.begin();
	 iterRef != iterRefEnd && iterTest != iterTestEnd;
	 iterRef++, iterTest++) {
      // Get the two iterators pointing at the same entity.
      if ((*iterRef) != (*iterTest)) {
	while ((*iterRef) < (*iterTest)) {
	  if (iterRef == iterRefEnd) break;
	  iterRef++;
	}
	while ((*iterTest) < (*iterRef)) {
	  if (iterTest == iterTestEnd) break;
	  iterTest++;
	}
      }
      assert(*iterRef == *iterTest);
      iBase_EntityHandle entRef = iterRef->entity();
      iBase_EntityHandle entTest = iterTest->entity();
      
      int topo;
      m_pMW_ref->getEntTopo(m_refInstance, entRef, &topo, &m_refErr);
      assert(m_refErr == iBase_SUCCESS);
      
      matchTopoCounts[topo]++;
      m_testToRefEntMap.insert
	(std::pair<iBase_EntityHandle, iBase_EntityHandle>(entTest, entRef));
      m_refToTestEntMap.insert
	(std::pair<iBase_EntityHandle, iBase_EntityHandle>(entRef, entTest));
    }
    cout << "Matched up other entities.  Got hits on: " << endl;
    for (int ii = iMesh_LINE_SEGMENT; ii < iMesh_ALL_TOPOLOGIES; ii++) {
      cout << "      " << topoNames[ii] << ": " << matchTopoCounts[ii]
	   << " / " << refTopoCounts[ii] << endl;
    }
  }

  finishTest();
}
