#include "iMesh_unitTest.hh"

void vTestEntityWithSets(iMesh_Instance& instance)
{
  //    initEntIter, getNextEntIter, resetEntIter, endEntIter
  const int iNumType = iBase_ALL_TYPES - iBase_VERTEX;
  const int iNumTopo = iMesh_ALL_TOPOLOGIES - iMesh_POINT;
  iBase_EntitySetHandle apvSets[10];
  iBase_EntityHandle a2pvTypeEnts[iNumType][iNumType*5];
  iBase_EntityIterator pvIter;
  iBase_EntityHandle a2pvTopoEnts[iNumTopo][iNumTopo*5];
  int aiNumTypeEnts[iNumType] = {0};
  int aiNumTopoEnts[iNumTopo] = {0};
  int i, iGeoDim, iNumTypeVerts, iNumTopoVerts;
  int qGotData = false;
  int err;

  iMesh_getGeometricDimension(instance, &iGeoDim, &err);
  CHECK_ERR(err);

  // Get a different number of entities for each type and record how
  // many were retrieved --a2pvTypeEnts stores the entities,
  // aiNumTypeEnts stores how many for each type
  qGotData = false;

  for ( i = 0; i < iNumType; i++ ) {
    if ( (iGeoDim == 2) && (i > iBase_FACE) )
      break;
    iMesh_initEntIter(instance, pvRootSet,
		      static_cast<iBase_EntityType>(i+iBase_VERTEX),
		      iMesh_ALL_TOPOLOGIES, &pvIter, &err);
    CHECK_ERR(err);

    iBase_EntityHandle entity;
    iMesh_getNextEntIter(instance, pvIter, &entity, &qGotData, &err);
    CHECK_ERR(err);

    int j = 0;
    while (qGotData && aiNumTypeEnts[i] < 5*(iNumType-i)) {
      a2pvTypeEnts[i][j] = entity;
      j++;
      aiNumTypeEnts[i] = j;
      iMesh_getNextEntIter(instance, pvIter, &entity, &qGotData, &err);
      CHECK_ERR(err);
    };
    iMesh_endEntIter(instance, pvIter, &err);
    CHECK_ERR(err);
  }

  // If there aren't the required number of vertices (20 to be exact) or
  // there aren't any of any of the other types of entities, bail out
  // because the subsequent tests would be useless and might not even
  // work.
  if ( (aiNumTypeEnts[0] < 20) ) {
    cout << "\nCould not get mesh data to work with... exiting...\n";
    TEST ( data_preparation );
    return;
  }

  // Get a different number of entities for each topology and record how
  // many were retrieved -- a2pvTopoEnts stores the entities,
  // aiNumTopoEnts stores how many for each topology.
  qGotData = false;
  for ( i = 0; i < iNumTopo; i++ ) {
    if ( (iGeoDim == 2) && (i > iMesh_QUADRILATERAL) )
      break;
    iMesh_initEntIter(instance, pvRootSet, iBase_ALL_TYPES,
		      static_cast<iMesh_EntityTopology>(i), &pvIter, &err);
    CHECK_ERR(err);

    iBase_EntityHandle entity;
    iMesh_getNextEntIter(instance, pvIter, &entity, &qGotData, &err);
    CHECK_ERR(err);
    int j = 0;
    while (qGotData && aiNumTopoEnts[i] < 5*(iNumTopo-i)) {
      a2pvTopoEnts[i][j] = entity;
      j++;
      aiNumTopoEnts[i] = j;
      iMesh_getNextEntIter(instance, pvIter, &entity, &qGotData, &err);
      CHECK_ERR(err);
    }
    iMesh_endEntIter(instance,  pvIter, &err);
    CHECK_ERR(err);
  }

  // If there aren't the required number of points (55 to be exact) or
  // there aren't any of any of the other topologies of entities, bail
  // out because the subsequent tests would be useless and might not
  // even work
  if ( aiNumTopoEnts[0] < 55 ) {
    cout << "\nCould not get mesh data to work with... exiting...\n";
    TEST ( data_preparation );
    return;
  }

  // Create two lists and two sets, one for types and one for topologies each,
  // as well as an empty list
  for ( i=0; i<5; i++ ) {
    iMesh_createEntSet(instance,  i<3, &apvSets[i], &err);
    CHECK_ERR(err);
  }

  // Add the entities of each type to list #1 and set #3, interspersing
  // them with vertices, like this:
  //        V0,V0,V1,V1,V2,V2,...,E0,V0,E1,V1,E2,V2,....R0,V0,R1,V1,R2,V2
  // for the set the order makes no difference, but it does make the
  // list traversal slightly more interesting
  for ( int j = 0; j < aiNumTypeEnts[0]; j++ ) {
    for ( i = 0; i < iNumType; i++ ) {
      if ( j < aiNumTypeEnts[i] ) {
	iMesh_addEntToSet(instance,  a2pvTypeEnts[i][j], apvSets[1], &err);
	CHECK_ERR(err);
	iMesh_addEntToSet(instance,  a2pvTypeEnts[0][j], apvSets[1], &err);
	CHECK_ERR(err);
	iMesh_addEntToSet(instance,  a2pvTypeEnts[i][j], apvSets[3], &err);
	CHECK_ERR(err);
	iMesh_addEntToSet(instance,  a2pvTypeEnts[0][j], apvSets[3], &err);
	CHECK_ERR(err);
      }
    }
  }

  // Calculate the number of vertices used in the list above
  iNumTypeVerts = aiNumTypeEnts[0];
  for ( i=0; i<iNumType; i++ ) {
    iNumTypeVerts += aiNumTypeEnts[i];
  }

  // Do the same thing except for topologies
  for ( int j = 0; j < aiNumTopoEnts[0]; j++ ) {
    for ( i = 0; i < iNumTopo; i++ ) {
      if ( j < aiNumTopoEnts[i] ) {
	iMesh_addEntToSet(instance,  a2pvTopoEnts[i][j], apvSets[2], &err);
	CHECK_ERR(err);
	iMesh_addEntToSet(instance,  a2pvTopoEnts[0][j], apvSets[2], &err);
	CHECK_ERR(err);
	iMesh_addEntToSet(instance,  a2pvTopoEnts[i][j], apvSets[4], &err);
	CHECK_ERR(err);
	iMesh_addEntToSet(instance,  a2pvTopoEnts[0][j], apvSets[4], &err);
	CHECK_ERR(err);
      }
    }
  }

  // Calculate the number of points used in the list above
  iNumTopoVerts = aiNumTopoEnts[0];
  for ( i=0; i<iNumTopo; i++ )
    iNumTopoVerts += aiNumTopoEnts[i];

  cout << "Testing set iterator initialization...                    ";
  iBase_EntityIterator apvTypeIter0[iNumType];
  iBase_EntityIterator apvTopoIter0[iNumTopo];
  iBase_EntityIterator apvTypeIter1[iNumType];
  iBase_EntityIterator apvTypeIter3[iNumType];
  iBase_EntityIterator apvTopoIter2[iNumTopo];
  iBase_EntityIterator apvTopoIter4[iNumTopo];
  bool aqInitType1[iNumType] = {false};
  bool aqInitType3[iNumType] = {false};
  bool aqInitTopo2[iNumTopo] = {false};
  bool aqInitTopo4[iNumTopo] = {false};

  for ( int iType = 0; iType < iNumType; iType++ ) {
    if ( (iGeoDim == 2) && (iType > iBase_FACE) )
      break;
    iMesh_initEntIter(instance, apvSets[0], iType, iMesh_ALL_TOPOLOGIES,
		      &(apvTypeIter0[iType]), &err);
    CHECK_ERR(err);

    if ( aiNumTypeEnts[iType] > 0 ) {
      iMesh_initEntIter(instance,  apvSets[1], iType, iMesh_ALL_TOPOLOGIES,
			&(apvTypeIter1[iType]), &err);
      aqInitType1[iType] = (err == iBase_SUCCESS);
      CHECK_ERR(err);

      iMesh_initEntIter(instance,  apvSets[3], iType, iMesh_ALL_TOPOLOGIES,
			&(apvTypeIter3[iType]), &err);
      aqInitType3[iType] = (err == iBase_SUCCESS);
      CHECK_ERR(err);
    }
  }

  for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
    if ( (iGeoDim == 2) && (iTopo > iMesh_QUADRILATERAL) )
      break;
    iMesh_initEntIter(instance, apvSets[0], iBase_ALL_TYPES,
		      iTopo, &(apvTopoIter0[iTopo]), &err);
    CHECK_ERR(err);

    if ( aiNumTopoEnts[iTopo] > 0 ) {
      iMesh_initEntIter(instance, apvSets[2], iBase_ALL_TYPES,
			iTopo, &(apvTopoIter2[iTopo]), &err);
      aqInitTopo2[iTopo] = (err == iBase_SUCCESS);
      CHECK_ERR(err);

      iMesh_initEntIter(instance, apvSets[4], iBase_ALL_TYPES,
			iTopo, &(apvTopoIter4[iTopo]), &err);
      aqInitTopo4[iTopo] = (err == iBase_SUCCESS);
      CHECK_ERR(err);
    }
  }
  cout << "DONE\n";
  // No point in testing save-load here; no one expects iterators to be
  // valid after this anyway.

  cout << "Testing set iterator entity retrieval...                  ";
  iBase_EntityHandle pvEntTemp;
  iBase_EntityHandle aGotEnts[iNumTypeVerts+iNumTopoVerts];

  if ( aqInitType1[0] ) {
    i = 0;
    iMesh_getNextEntIter(instance, apvTypeIter1[0], &pvEntTemp,
			 &qGotData, &err);
    CHECK_ERR(err);
    while ( i < iNumTypeVerts+1 && qGotData) {
      aGotEnts[i++] =  pvEntTemp;
      iMesh_getNextEntIter(instance, apvTypeIter1[0], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
    }

    qGotData = ( i == iNumTypeVerts );
    if ( qGotData ) {
      i = 0;
      for ( int j=0; j < aiNumTypeEnts[0]; j++ ) {
	qGotData = qGotData && ( a2pvTypeEnts[0][j] == aGotEnts[i++] );
	for ( int iType = 0; iType < iNumType; iType++ )
	  if ( j < aiNumTypeEnts[iType] )
	    qGotData = qGotData && ( a2pvTypeEnts[0][j] == aGotEnts[i++] );
      }
    }
    TEST2 ( qGotData, "Didn't match entity retrieval for list-type set, iterating by type." );
  }


  for ( int iType = 1; iType < iNumType; iType++ ) {
    if ( aqInitType1[iType] ) {
      i = 0;
      iMesh_getNextEntIter(instance, apvTypeIter1[iType], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
      while ( i < iNumTypeVerts+1 && qGotData) {
	aGotEnts[i++] =  pvEntTemp;
	iMesh_getNextEntIter(instance, apvTypeIter1[iType], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }

      qGotData = ( i == aiNumTypeEnts[iType] );
      if ( qGotData ) {
	for ( int j=0; j < aiNumTypeEnts[iType]; j++ )
	  qGotData = qGotData && ( a2pvTypeEnts[iType][j] == aGotEnts[j] );
      }
      TEST ( qGotData );
    }
  }

  for ( int iType = 0; iType < iNumType; iType++ ) {
    if ( (iGeoDim == 2) && (iType > iBase_FACE) )
      break;

    iMesh_getNextEntIter(instance, apvTypeIter0[iType], &pvEntTemp,
			 &qGotData, &err);
    CHECK_ERR(err);
    TEST( !qGotData );

    if ( aqInitType3[iType] ) {
      i = 0;
      iMesh_getNextEntIter(instance, apvTypeIter3[iType], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
      while ( i < aiNumTypeEnts[iType]+1 && qGotData) {
	aGotEnts[i++] =  pvEntTemp ;
	iMesh_getNextEntIter(instance, apvTypeIter3[iType], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }
      qGotData = false;
      if ( i == aiNumTypeEnts[iType] ) {
	GOT_ALL_ITEMS ( a2pvTypeEnts[iType], aGotEnts, i, qGotData );
      }
      TEST ( qGotData );
    }
  }


  if ( aqInitTopo2[0] ) {
    i = 0;
    iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
			 &qGotData, &err);
    CHECK_ERR(err);
    while ( i < iNumTopoVerts+1 && qGotData) {
      aGotEnts[i++] =  pvEntTemp ;
      iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
    }
    qGotData = ( i == iNumTopoVerts);
    if ( qGotData ) {
      i = 0;
      for ( int j=0; j < aiNumTopoEnts[0]; j++ ) {
	qGotData = qGotData && ( a2pvTopoEnts[0][j] == aGotEnts[i++] );
	for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ )
	  if ( j < aiNumTopoEnts[iTopo] )
	    qGotData = qGotData && ( a2pvTopoEnts[0][j] == aGotEnts[i++] );
      }
    }
    TEST2 ( qGotData, "Didn't match entity retrieval for list-type set, iterating by topology." );
  }

  for ( int iTopo = 1; iTopo < iNumTopo; iTopo++ ) {
    if ( aqInitTopo2[iTopo] ) {
      i = 0;
      iMesh_getNextEntIter(instance, apvTopoIter2[iTopo], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
      while ( i < iNumTopoVerts+1 && qGotData) {
	aGotEnts[i++] =  pvEntTemp ;
	iMesh_getNextEntIter(instance, apvTopoIter2[iTopo],
			     &pvEntTemp, &qGotData, &err);
	CHECK_ERR(err);
      }
      qGotData = ( i == aiNumTopoEnts[iTopo] );
      if ( qGotData ) {
	for ( int j=0; j < aiNumTopoEnts[iTopo]; j++ )
	  qGotData = qGotData && ( a2pvTopoEnts[iTopo][j] == aGotEnts[j] );
      }
      TEST ( qGotData );
    }
  }

  for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
    if ( (iGeoDim == 2) && (iTopo > iMesh_QUADRILATERAL) )
      break;
    iMesh_getNextEntIter(instance, apvTopoIter0[iTopo], &pvEntTemp,
			 &qGotData, &err);
    CHECK_ERR(err);
    TEST ( !qGotData );
    if ( aqInitTopo4[iTopo] ) {
      i = 0;
      iMesh_getNextEntIter(instance, apvTopoIter4[iTopo], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
      while ( i < iNumTopoVerts+1 && qGotData) {
	aGotEnts[i++] =  pvEntTemp ;
	iMesh_getNextEntIter(instance, apvTopoIter4[iTopo],
			     &pvEntTemp, &qGotData, &err);
	CHECK_ERR(err);
      }
      qGotData = false;
      if ( i == aiNumTopoEnts[iTopo] )
	GOT_ALL_ITEMS ( a2pvTopoEnts[iTopo], aGotEnts, i, qGotData );
      TEST ( qGotData );
    }
  }
  cout << "DONE\n";
  // No point in testing save-load here; no one expects iterators to be
  // valid after this anyway.

  cout << "Testing reset, with removal and addition mid-traversal... ";
  iBase_EntityHandle pvEntOut;

  if ( aqInitType1[0]  ) {
    iMesh_resetEntIter(instance,  apvTypeIter1[0], &err);
    CHECK_ERR(err);

    i = 0;
    iMesh_getNextEntIter(instance, apvTypeIter1[0],
			 &pvEntTemp, &qGotData, &err);
    CHECK_ERR(err);
    while ( i < (iNumTypeVerts/3) && qGotData ) {
      i++;
      iMesh_getNextEntIter(instance, apvTypeIter1[0],
			   &pvEntTemp, &qGotData, &err);
      CHECK_ERR(err);
    }

    pvEntOut = pvEntTemp;
    while ( i < (iNumTypeVerts/2) && qGotData ) {
      i++;
      iMesh_getNextEntIter(instance, apvTypeIter1[0],
			   &pvEntTemp, &qGotData, &err);
      CHECK_ERR(err);
    }

    iMesh_rmvEntFromSet(instance,  pvEntOut, apvSets[1], &err);
    CHECK_ERR(err);
    iMesh_rmvEntFromSet(instance,  pvEntTemp, apvSets[1], &err);
    CHECK_ERR(err);
    iMesh_addEntToSet(instance,  pvEntTemp, apvSets[1], &err);
    CHECK_ERR(err);

    while ( i < iNumTypeVerts+1 && qGotData ) {
      i++;
      iMesh_getNextEntIter(instance, apvTypeIter1[0],
			   &pvEntTemp, &qGotData, &err);
      CHECK_ERR(err);
    }

    // The following commented code requires iterators over list-type
    // sets to return newly-added entities.  This part of the spec is
    // currently (May, 2011) in question.
//     iMesh_addEntToSet(instance,  pvEntOut, apvSets[1], &err);
//     CHECK_ERR(err);

//     iMesh_getNextEntIter(instance, apvTypeIter1[0],
// 			 &pvEntTemp, &qGotData, &err);
//     CHECK_ERR(err);

//     if ( qGotData ) {
//       TESTEQ(pvEntOut,pvEntTemp);
//     }
//     else {
//       TEST2(false, "Didn't retrieve newly added entity at the end of iteration (list-type set)." );
//     }
    iMesh_getNextEntIter(instance, apvTypeIter1[0],
			 &pvEntTemp, &qGotData, &err);
    CHECK_ERR(err);
    TEST ( !qGotData );
  }

  for ( int iType = 1; iType < iNumType; iType++ ) {
    if ( aqInitType1[iType] ) {
      iMesh_resetEntIter(instance,  apvTypeIter1[iType], &err);
      CHECK_ERR(err);

      i = 0;
      iMesh_getNextEntIter(instance, apvTypeIter1[iType],
			   &pvEntTemp, &qGotData, &err);
      CHECK_ERR(err);
      while ( i < (aiNumTypeEnts[iType]/3) && qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTypeIter1[iType],
			     &pvEntTemp, &qGotData, &err);
	CHECK_ERR(err);
      }

      pvEntOut = pvEntTemp;
      while ( i < (aiNumTypeEnts[iType]/2) && qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTypeIter1[iType],
			     &pvEntTemp, &qGotData, &err);
	CHECK_ERR(err);
      }

      iMesh_rmvEntFromSet(instance,  pvEntOut, apvSets[1], &err);
      CHECK_ERR(err);
      iMesh_rmvEntFromSet(instance,  pvEntTemp, apvSets[1], &err);
      CHECK_ERR(err);
      iMesh_addEntToSet(instance,  pvEntTemp, apvSets[1], &err);
      CHECK_ERR(err);
      while ( i < aiNumTypeEnts[iType]+1 && qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTypeIter1[iType],
			     &pvEntTemp, &qGotData, &err);
	CHECK_ERR(err);
      }

    // The following commented code requires iterators over list-type
    // sets to return newly-added entities.  This part of the spec is
    // currently (May, 2011) in question.
//       iMesh_addEntToSet(instance,  pvEntOut, apvSets[1], &err);
//       CHECK_ERR(err);
//       iMesh_getNextEntIter(instance, apvTypeIter1[iType],
// 			   &pvEntTemp, &qGotData, &err);
//       CHECK_ERR(err);

//       if ( qGotData ) {
// 	TESTEQ(pvEntOut,pvEntTemp);
//       }
//       else {
// 	TEST2(false, "Didn't retrieve newly added entity at the end of iteration (list-type set).");
//       }
//       iMesh_getNextEntIter(instance, apvTypeIter1[iType],
// 			   &pvEntTemp, &qGotData, &err);
//       CHECK_ERR(err);
      iMesh_getNextEntIter(instance, apvTypeIter1[iType], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
      TEST ( !qGotData );
    }
  }
  // No point in testing save-load here; no one expects iterators to be
  // valid after this anyway.

  for ( int iType = 0; iType < iNumType; iType++ ) {
    if ( (iGeoDim == 2) && (iType > iBase_FACE) )
      break;
    iMesh_resetEntIter(instance,  apvTypeIter0[iType], &err);
    CHECK_ERR(err);
    iMesh_getNextEntIter(instance, apvTypeIter0[iType], &pvEntTemp,
			 &qGotData, &err);
    CHECK_ERR(err);
    TEST ( !qGotData );

    if ( aqInitType3[iType] ) {
      iMesh_resetEntIter(instance,  apvTypeIter3[iType], &err);
      CHECK_ERR(err);
      i = 0;
      iMesh_getNextEntIter(instance, apvTypeIter3[iType], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
      while ( i < (aiNumTypeEnts[iType]/3) && qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTypeIter3[iType], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }
      pvEntOut = pvEntTemp;
      while ( i < (aiNumTypeEnts[iType]/2) && qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTypeIter3[iType], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }
      iMesh_rmvEntFromSet(instance,  pvEntOut, apvSets[3], &err);
      CHECK_ERR(err);
      iMesh_rmvEntFromSet(instance,  pvEntTemp, apvSets[3], &err);
      CHECK_ERR(err);

      while ( qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTypeIter3[iType], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }
      TEST ( i >= aiNumTypeEnts[iType]/2 && i <= aiNumTypeEnts[iType]);
    }
  } // Loop over types for set 3

    // The following commented code requires iterators over list-type
    // sets to return newly-added entities.  This part of the spec is
    // currently (May, 2011) in question.
//   if ( aqInitTopo2[0]  ) {
//     iMesh_resetEntIter(instance,  apvTopoIter2[0], &err);
//     CHECK_ERR(err);

//     i = 0;
//     iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
// 			 &qGotData, &err);
//     CHECK_ERR(err);
//     while ( i < (iNumTopoVerts/3) && qGotData ) {
//       i++;
//       iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
// 			   &qGotData, &err);
//       CHECK_ERR(err);
//     }
//     pvEntOut = pvEntTemp;
//     while ( i < (iNumTopoVerts/2) && qGotData ) {
//       i++;
//       iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
// 			   &qGotData, &err);
//       CHECK_ERR(err);
//     }
//     iMesh_rmvEntFromSet(instance,  pvEntOut, apvSets[2], &err);
//     CHECK_ERR(err);
//     iMesh_rmvEntFromSet(instance,  pvEntTemp, apvSets[2], &err);
//     CHECK_ERR(err);
//     iMesh_addEntToSet(instance,  pvEntTemp, apvSets[2], &err);
//     CHECK_ERR(err);

//     while ( i < iNumTopoVerts+1 && qGotData ) {
//       i++;
//       iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
// 			   &qGotData, &err);
//       CHECK_ERR(err);
//     }
//     iMesh_addEntToSet(instance,  pvEntOut, apvSets[2], &err);
//     CHECK_ERR(err);

//     iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
// 			 &qGotData, &err);
//     CHECK_ERR(err);

//     if ( qGotData ) {
//       TESTEQ(pvEntOut,pvEntTemp);
//     }
//     else {
//       TEST2(false, "Didn't retrieve newly added entity at the end of iteration (list-type set)." );
//     }
//     iMesh_getNextEntIter(instance, apvTopoIter2[0], &pvEntTemp,
// 			 &qGotData, &err);
//     CHECK_ERR(err);
//     TEST ( !qGotData );
//   }

    // The following commented code requires iterators over list-type
    // sets to return newly-added entities.  This part of the spec is
    // currently (May, 2011) in question.
//   for ( int iTopo = 1; iTopo < iNumTopo; iTopo++ ) {
//     if ( aqInitTopo2[iTopo] ) {
//       iMesh_resetEntIter(instance,  apvTopoIter2[iTopo], &err);
//       CHECK_ERR(err);

//       i = 0;
//       iMesh_getNextEntIter(instance, apvTopoIter2[iTopo], &pvEntTemp,
// 			   &qGotData, &err);
//       CHECK_ERR(err);
//       pvEntOut = pvEntTemp;
//       while ( i < (aiNumTopoEnts[iTopo]/2) && qGotData) {
// 	i++;
// 	iMesh_getNextEntIter(instance, apvTopoIter2[iTopo], &pvEntTemp,
// 			     &qGotData, &err);
// 	CHECK_ERR(err);
//       }
//       iMesh_rmvEntFromSet(instance,  pvEntOut, apvSets[2], &err);
//       CHECK_ERR(err);
//       iMesh_rmvEntFromSet(instance,  pvEntTemp, apvSets[2], &err);
//       CHECK_ERR(err);
//       iMesh_addEntToSet(instance,  pvEntTemp, apvSets[2], &err);
//       CHECK_ERR(err);

//       while ( i < aiNumTopoEnts[iTopo]+1 && qGotData ) {
// 	i++;
// 	iMesh_getNextEntIter(instance, apvTopoIter2[iTopo], &pvEntTemp,
// 			     &qGotData, &err);
// 	CHECK_ERR(err);
//       }

//       iMesh_addEntToSet(instance,  pvEntOut, apvSets[2], &err);
//       CHECK_ERR(err);

//       iMesh_getNextEntIter(instance, apvTopoIter2[iTopo], &pvEntTemp,
// 			   &qGotData, &err);
//       CHECK_ERR(err);

//       if ( qGotData ) {
// 	TESTEQ(pvEntOut,pvEntTemp);
//       }
//       else {
// 	TEST2(false, "Didn't retrieve newly added entity, with removal and addition mid-iteration (list-type set).");
//       }

//       iMesh_getNextEntIter(instance, apvTopoIter2[iTopo], &pvEntTemp,
// 			   &qGotData, &err);
//       CHECK_ERR(err);
//       TEST ( !qGotData );
//     }
//   }

  for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
    if ( (iGeoDim == 2) && (iTopo > iMesh_QUADRILATERAL) )
      break;
    iMesh_resetEntIter(instance,  apvTopoIter0[iTopo], &err);
    CHECK_ERR(err);
    iMesh_getNextEntIter(instance, apvTopoIter0[iTopo], &pvEntTemp,
			 &qGotData, &err);
    CHECK_ERR(err);
    TEST ( !qGotData );

    if ( aqInitTopo4[iTopo] ) {
      iMesh_resetEntIter(instance,  apvTopoIter4[iTopo], &err);
      CHECK_ERR(err);

      i = 0;
      iMesh_getNextEntIter(instance, apvTopoIter4[iTopo], &pvEntTemp,
			   &qGotData, &err);
      CHECK_ERR(err);
      while ( i < (aiNumTopoEnts[iTopo]/3) && qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTopoIter4[iTopo], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }
      pvEntOut = pvEntTemp;
      while ( i < (aiNumTopoEnts[iTopo]/2) && qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTopoIter4[iTopo], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }
      iMesh_rmvEntFromSet(instance,  pvEntOut, apvSets[4], &err);
      CHECK_ERR(err);
      iMesh_rmvEntFromSet(instance,  pvEntTemp, apvSets[4], &err);
      CHECK_ERR(err);

      while ( qGotData ) {
	i++;
	iMesh_getNextEntIter(instance, apvTopoIter4[iTopo], &pvEntTemp,
			     &qGotData, &err);
	CHECK_ERR(err);
      }
      TEST ( i >= aiNumTopoEnts[iTopo]/2 && i <= aiNumTopoEnts[iTopo]);
    }
  }
  cout << "DONE\n";
  // No point in testing save-load here; no one expects iterators to be
  // valid after this anyway.

  cout << "Testing set iterator termination...                       ";

  for ( int iType = 0; iType < iNumType; iType++ ) {
    if ( (iGeoDim == 2) && (iType > iBase_FACE) )
      break;
    iMesh_endEntIter(instance,  apvTypeIter0[iType], &err);
    CHECK_ERR(err);

    if ( aiNumTypeEnts[iType] > 0 ) {
      iMesh_endEntIter(instance,  apvTypeIter1[iType], &err);
      CHECK_ERR(err);
      iMesh_endEntIter(instance,  apvTypeIter3[iType], &err);
      CHECK_ERR(err);
    }
  }

  for ( int iTopo = 0; iTopo < iNumTopo; iTopo++ ) {
    if ( (iGeoDim == 2) && (iTopo > iMesh_QUADRILATERAL) )
      break;
    iMesh_endEntIter(instance,  apvTopoIter0[iTopo], &err);
    CHECK_ERR(err);
    if ( aiNumTopoEnts[iTopo] > 0 ) {
      iMesh_endEntIter(instance,  apvTopoIter2[iTopo], &err);
      CHECK_ERR(err);
      iMesh_endEntIter(instance,  apvTopoIter4[iTopo], &err);
      CHECK_ERR(err);
    }
  }
  cout << "DONE\n";

  if ( qErrTests ) {
    iBase_EntityIterator pvBadIter;
    cout << "Testing error throwing...                                 ";
    // Error throwing tests simply try to create a situation where the interface
    // should, according to the spec, throw an error, and verify that:
    // (a) an error is thrown, and (b) the error is the one expected.
    // If an error is not thrown, the reversal of the action is attempted, if
    // it would interfere with further tests.

    // Invalid entity/entity set/tag handle tests usually involve passing in an
    // entity as an entity set or vice versa, etc., but sometimes try more
    // difficult tests such as passing in a pointer to character data as an
    // entity handle.
    //   Initializing an iterator for an invalid entity type.
    iMesh_initEntIter(instance,  apvSets[0], iBase_ALL_TYPES+1,
		      iMesh_ALL_TOPOLOGIES, &pvBadIter, &err);
    CHECK_ERR2(err, iBase_INVALID_ENTITY_TYPE);

    //   Initializing an iterator for an invalid entity topology.
    iMesh_initEntIter(instance,  apvSets[0], iBase_ALL_TYPES,
		      iMesh_ALL_TOPOLOGIES+1, &pvBadIter, &err);
    CHECK_ERR2(err, iBase_INVALID_ENTITY_TOPOLOGY);

    //  Initializing an iterator for an invalid entity type topology
    //  combination.
    for ( int iType = iBase_VERTEX; iType <= iBase_ALL_TYPES; iType++ ) {
      for ( int iTopo = iMesh_POINT;
	    iTopo <= iMesh_ALL_TOPOLOGIES; iTopo++ ) {
	if ( !a2qLegalTypeAndTopo[iType][iTopo] ) {
	  if ( (iGeoDim == 2) && (iType == iBase_REGION ||
				  (iTopo > iMesh_QUADRILATERAL &&
				   iTopo != iMesh_ALL_TOPOLOGIES)) )
	    continue;
	  iMesh_initEntIter(instance,  apvSets[0], (iType), (iTopo),
			    &pvBadIter, &err);
	  CHECK_ERR2(err, iBase_BAD_TYPE_AND_TOPO );
	} // if ( !a2q.. )
      } // for ( int iTopo... )
    }

    cout << "DONE\n";
    // No point in testing save-load here; no one expects iterators to be
    // valid after this anyway.
  }

  for ( i=0; i<5; i++ ) {
    iMesh_destroyEntSet(instance,  apvSets[i], &err);
    CHECK_ERR(err);
  }
}
