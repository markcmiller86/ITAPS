/*****************************************************************************
*
* Copyright (c) 2000 - 2009, The Regents of the University of California
* Produced at the Lawrence Livermore National Laboratory
* All rights reserved.
*
* This file is part of VisIt. For details, see http://www.llnl.gov/visit/. The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or materials provided with the distribution.
*  - Neither the name of the UC/LLNL nor  the names of its contributors may be
*    used to  endorse or  promote products derived from  this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED.  IN  NO  EVENT  SHALL  THE  REGENTS  OF  THE  UNIVERSITY OF
* CALIFORNIA, THE U.S.  DEPARTMENT  OF  ENERGY OR CONTRIBUTORS BE  LIABLE  FOR
* ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#ifndef IMESHFILE_H
#define IMESHFILE_H

#include <iMesh.h>
#include <iBase.h>
#include <QString>
#include <vector>
#include <string>
using std::string;
using std::vector;

class iMeshFile;

// ****************************************************************************
//  Class:  iMeshSet
//
//  Purpose: Encapsulation of an iMesh entity set.
//
//  Programmer:  Mark C. Miller 
//  Creation:    April 15, 2009 
//
// ****************************************************************************
class iMeshSet
{
  public:
    iBase_EntitySetHandle set;
    vector<iBase_TagHandle> tagHdls;
    vector<int> tagTypes;
    vector<int> tagSizes;
    vector<string> tagNames;
    vector<string> tagVals;
    int typeCounts[iBase_ALL_TYPES+1];
    int topoCounts[iMesh_ALL_TOPOLOGIES+1];
    int containedSetCount;
    int descendentSetCount;
    int numParent;
    int numChild;
    iMeshFile *fileObj;
    int isList;

    vector<iMeshSet*> subObjs;

  public:
    iMeshSet(iMesh_Instance mesh, iBase_EntitySetHandle esh_, iMeshFile *fobj_=0); 
    ~iMeshSet();
};

// ****************************************************************************
//  Class:  iMeshFile
//
//  Purpose:
//    Encapsulation of a iMesh file.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
//  Modifications:
//    Mark C. Miller, Tue Apr 28 09:27:49 PDT 2009
//    Fixed missing 'return' statement in IsOpen().
// ****************************************************************************
class iMeshFile
{
  public:
    iMeshSet *rootObj;
    int geomDim;
    int dfltOrder;
    int adjTable[4][4];

  public:
    iMeshFile(const QString &name);
    ~iMeshFile();

    bool          IsOpen() const { return rootObj != 0; };
    void         *GetVar(iMeshSet *s, const QString &tagn, const QString &entyp);
    int           GetVarType(iMeshSet *s, const QString &tagn, const QString &entyp);
    int           GetVarLength(iMeshSet *s, const QString &tagn, const QString &entyp);
    int           GetVarTupleSize(iMeshSet *s, const QString &tagn, const QString &entyp);

    void          SaveAsVTK(const QString &file);

  private:
    iMesh_Instance itapsMesh;
    iBase_EntitySetHandle rootSet;
};

#endif
