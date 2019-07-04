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

#include <iMeshFile.h>
#include <iMesh.h>
#include <iBase.h>
#include <iBase_names.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;
using std::cerr;
using std::endl;

#define IMESH_ADEFN(TN,ON,N)			\
    TN* ON = (TN*) malloc(N * sizeof(TN));	\
    int ON ## _allocated = 1;			\
    int ON ## _size = N;

#define IMESH_ADEF(TN,ON)		\
    TN* ON = 0;				\
    int ON ## _allocated = 0;		\
    int ON ## _size = 0;

#define IMESH_AARG(ON) &ON, &ON ## _allocated, &ON ## _size

#define IMESH_AFREE(ON)			\
    if (ON) free(ON);			\
    ON ## _allocated = 0;		\
    ON ## _size = 0;

#if IMESH_VERSION_GE(1,3,0)
#define CheckITAPSError2(IMI, ERR, FN, ARGS, THELINE, THEFILE)                                  \
    if (ERR != 0)                                                                               \
    {                                                                                           \
        char msg[1024];                                                                         \
        char desc[256];                                                                         \
        for (int i = 0; i < sizeof(desc); i++) desc[i] = '\0';                                  \
        iMesh_getDescription(IMI, desc, sizeof(desc));                             		\
        snprintf(msg, sizeof(msg), "Encountered ITAPS error (%d) (%s) after call to \"%s\""     \
            " at line %d in file \"%s\"\nThe description is...\n"                               \
            "    \"%s\"\n", ERR, iBase_ErrorType_names[ERR], #FN, THELINE, THEFILE, desc);      \
        if (messageCounts.find(msg) == messageCounts.end())                                     \
            messageCounts[msg] = 1;                                                             \
        else                                                                                    \
            messageCounts[msg]++;                                                               \
        if (messageCounts[msg] < 6)                                                             \
        {                                                                                       \
            cerr << msg << endl;                                                                \
        }                                                                                       \
        else if (messageCounts[msg] == 6)                                                       \
        {                                                                                       \
            cerr << supressMessage << endl;                                                     \
        }                                                                                       \
    }
#else
#define CheckITAPSError2(IMI, ERR, FN, ARGS, THELINE, THEFILE)                                  \
    if (ERR != 0)                                                                               \
    {                                                                                           \
        char msg[1024];                                                                         \
        char desc[256];                                                                         \
        for (int i = 0; i < sizeof(desc); i++) desc[i] = '\0';                                  \
        int dummyError = ERR;                                                                   \
        iMesh_getDescription(IMI, desc, &dummyError, sizeof(desc));                             \
        snprintf(msg, sizeof(msg), "Encountered ITAPS error (%d) (%s) after call to \"%s\""     \
            " at line %d in file \"%s\"\nThe description is...\n"                               \
            "    \"%s\"\n", ERR, iBase_ErrorType_names[ERR], #FN, THELINE, THEFILE, desc);      \
        if (messageCounts.find(msg) == messageCounts.end())                                     \
            messageCounts[msg] = 1;                                                             \
        else                                                                                    \
            messageCounts[msg]++;                                                               \
        if (messageCounts[msg] < 6)                                                             \
        {                                                                                       \
            cerr << msg << endl;                                                                \
        }                                                                                       \
        else if (messageCounts[msg] == 6)                                                       \
        {                                                                                       \
            cerr << supressMessage << endl;                                                     \
        }                                                                                       \
    }
#endif

#define CheckITAPSError(IMI, FN, ARGS) CheckITAPSError2(IMI, itapsError, FN, ARGS, __LINE__, __FILE__)

#define EoL (void*)-1
#define NoL (0,EoL)

static void ITAPSErrorCleanupHelper(int dummy, ...)
{
   va_list ap;
   va_start(ap, dummy);
   void *ptr = va_arg(ap, void*);
   while (ptr != EoL)
   {
       if (ptr)
           free(ptr);
       ptr = va_arg(ap, void*);
   }
   va_end(ap);
}

static string imeshGetTagName(iMesh_Instance theMesh, iBase_TagHandle theTag)
{
    int err;
    static char tmpName[256];
    iMesh_getTagName(theMesh, theTag, tmpName, &err, sizeof(tmpName));
    if (err != iBase_SUCCESS)
        tmpName[0] = '\0';
    return string(tmpName);
}

static int itapsError;
static map<string, int> messageCounts;
static const char *supressMessage = "further warnings regarding this error will be supressed";

// ----------------------------------------------------------------------------
//                             iMesh Set
// ----------------------------------------------------------------------------


// ****************************************************************************
//  Constructor:  iMeshSet::iMeshSet
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
//  Modifications:
//  
//    Mark C. Miller, Mon Apr 20 14:43:37 PDT 2009
//    Reverted to use iter funcs to query dense tag info rather than
//    getEntities (which results in allocations of size on the order of
//    total entity count in the mesh). Made tag parsing for non-unity-sized
//    tags use generic getData method (getEntSetData).
//
//    Mark C. Miller, Tue Apr 28 10:37:52 PDT 2009
//    Removed some code that existed only for debugging purposes.
//
//    Mark C. Miller, Thu Feb 17 19:11:35 PST 2011
//    Deal with change in interface from char** to void** for generic data
//    methods in iMesh.
// ****************************************************************************
iMeshSet::iMeshSet(iMesh_Instance mesh, iBase_EntitySetHandle set_, iMeshFile *fobj_) 
{
    fileObj = fobj_;
    set = set_;
    IMESH_ADEF(iBase_EntitySetHandle, entSets);

    // get info on whether this set is a 'list' or not
    iMesh_isList(mesh, set, &isList, &itapsError);

    // Examine all the tags defined on this entity set
    IMESH_ADEF(iBase_TagHandle, tags);
    iMesh_getAllEntSetTags(mesh, set, IMESH_AARG(tags), &itapsError);

    //
    // Get all the information about tags on this entity set including
    // even the values of these tags. But, the values will be sprintf'd
    // into strings, here. We should really make EH-valued tags able to
    // be clickable.
    //
    for (int t = 0; t < tags_size; t++)
    {
        tagHdls.push_back(tags[t]);

        int typeId;
        iMesh_getTagType(mesh, tags[t], (int*) &typeId, &itapsError);
        CheckITAPSError(mesh, iMesh_getTagType, NoL);
        tagTypes.push_back(typeId);

        int tagSize;
        iMesh_getTagSizeValues(mesh, tags[t], &tagSize, &itapsError);
        CheckITAPSError(mesh, iMesh_getTagSizeValues, NoL); 
        tagSizes.push_back(tagSize);

        string tagName = imeshGetTagName(mesh, tags[t]);
        CheckITAPSError(mesh, imeshGetTagName, NoL); 
        tagNames.push_back(tagName);

        tagVals.push_back("");
        char lineBuf[256];
        if (typeId == iBase_INTEGER)
        {
            if (tagSize == 1)
            {
                int theVal;
                iMesh_getEntSetIntData(mesh, set, tags[t], &theVal, &itapsError);
                CheckITAPSError(mesh, iMesh_getEntSetIntData, NoL); 
                snprintf(lineBuf, sizeof(lineBuf), "%d", theVal);
            }
            else if (tagSize > 1)
            {
                IMESH_ADEF(void, ctagvals);
                iMesh_getEntSetData(mesh, set, tags[t], IMESH_AARG(ctagvals), &itapsError);
                //iMesh_getEntSetIntData(mesh, set, tags[t], tagvals, &itapsError);
                CheckITAPSError(mesh, iMesh_getIntData, (0,tagvals,EoL)); 
                int *tagvals = (int *) ctagvals; 
                std::string valBuf;
                for (int k = 0; k < tagSize; k++)
                {
                    char tmpChars[32];
                    sprintf(tmpChars, "%d ", tagvals[k]);
                    valBuf += std::string(tmpChars);
                }
                IMESH_AFREE(ctagvals);
                snprintf(lineBuf, sizeof(lineBuf), "%s", valBuf.c_str());
            }
        }
        else if (typeId == iBase_DOUBLE)
        {
            if (tagSize == 1)
            {
                double theVal;
                iMesh_getEntSetDblData(mesh, set, tags[t], &theVal, &itapsError);
                CheckITAPSError(mesh, iMesh_getEntSetIntData, NoL); 
                snprintf(lineBuf, sizeof(lineBuf), "%f", theVal);
            }
            else if (tagSize > 1)
            {
                IMESH_ADEF(void, ctagvals);
                iMesh_getEntSetData(mesh, set, tags[t], IMESH_AARG(ctagvals), &itapsError);
                //iMesh_getEntSetDblData(mesh, set, tags[t], tagvals, &itapsError);
                CheckITAPSError(mesh, iMesh_getEntSetDblData, (0,tagvals,EoL)); 
                double *tagvals = (double*) ctagvals; 
                std::string valBuf;
                for (int k = 0; k < tagSize; k++)
                {
                    char tmpChars[32];
                    sprintf(tmpChars, "%f ", tagvals[k]);
                    valBuf += std::string(tmpChars);
                }
                IMESH_AFREE(ctagvals);
                snprintf(lineBuf, sizeof(lineBuf), "%s", valBuf.c_str());
            }
        }
        else if (typeId == iBase_BYTES)
        {
            IMESH_ADEF(void, vtagvals);
            char *ctagvals = (char *) vtagvals;
            iMesh_getEntSetData(mesh, set, tags[t], IMESH_AARG(vtagvals), &itapsError);
            CheckITAPSError(mesh, iMesh_getEntSetData, (0,theVal,EoL));
            std::string valBuf;
            std::string strBuf;
            bool allascii = true;
            for (int k = 0; k < vtagvals_size; k++)
            {
                char tmpChars[32];
                sprintf(tmpChars, "%02X", (unsigned int) ctagvals[k]);
                valBuf += std::string(tmpChars);
                sprintf(tmpChars, "%c", ctagvals[k]);
                strBuf += std::string(tmpChars);
                if (!isascii(ctagvals[k]))
                    allascii = false;
            }
            IMESH_AFREE(vtagvals);
            if (allascii)
                snprintf(lineBuf, sizeof(lineBuf), "%s", strBuf.c_str());
            else
                snprintf(lineBuf, sizeof(lineBuf), "%s", valBuf.c_str());
        }
        else if (typeId == iBase_ENTITY_HANDLE)
        {
            if (tagSize == 1)
            {
                iBase_EntityHandle theVal;
                iMesh_getEntSetEHData(mesh, set, tags[t], &theVal, &itapsError);
                CheckITAPSError(mesh, iMesh_getEntSetEHData, NoL); 
                snprintf(lineBuf, sizeof(lineBuf), "%X", theVal);
            }
            else if (tagSize > 1)
            {
                IMESH_ADEF(void, ctagvals);
                iMesh_getEntSetData(mesh, set, tags[t], IMESH_AARG(ctagvals), &itapsError);
                //iMesh_getEntSetEHData(mesh, set, tags[t], tagvals, &itapsError);
                CheckITAPSError(mesh, iMesh_getEntSetData, (0,tagvals,EoL)); 
                iBase_EntityHandle *tagvals = (iBase_EntityHandle *) ctagvals;
                std::string valBuf;
                for (int k = 0; k < tagSize; k++)
                {
                    char tmpChars[32];
                    sprintf(tmpChars, "%#x ", (void*) tagvals[k]);
                    valBuf += std::string(tmpChars);
                }
                IMESH_AFREE(ctagvals);
                snprintf(lineBuf, sizeof(lineBuf), "%s", valBuf.c_str());
            }
        }
        tagVals[tagVals.size()-1] = string(lineBuf);
    }
    IMESH_AFREE(tags);

    // Collect information on entity types in this set
    for (int i = 0; i < iBase_ALL_TYPES; i++)
        iMesh_getNumOfType(mesh, set, i, &typeCounts[i], &itapsError);

    // Collect information on entity topologies in this set
    for (int i = 0; i < iMesh_ALL_TOPOLOGIES; i++)
        iMesh_getNumOfTopo(mesh, set, i, &topoCounts[i], &itapsError);

    // Collect information on tags defined over entities (dense tags)
    // that are in this set.
    for (int i = 0; i < iBase_ALL_TYPES; i++)
    {
        iBase_EntityHandle oneEntity;
        int has_data;
        int testTopo=-1, testType=-1;

        // initialize an entity iterator and get the first entity
#define USE_ITER
#ifdef USE_ITER
        iBase_EntityIterator entIt;
        iMesh_initEntIter(mesh, set, i, iMesh_ALL_TOPOLOGIES, &entIt, &itapsError);
        CheckITAPSError(mesh, iMesh_initEntIter, NoL);
        iMesh_getNextEntIter(mesh, entIt, &oneEntity, &has_data, &itapsError);
        CheckITAPSError(mesh, iMesh_getNextEntIter, NoL);
#else
        IMESH_ADEF(iBase_EntityHandle, tmpEnts);
        iMesh_getEntities(mesh, set, i, iMesh_ALL_TOPOLOGIES, IMESH_AARG(tmpEnts), &itapsError);
        CheckITAPSError(mesh, iMesh_getEntities, NoL);
        if (tmpEnts_size > 0)
        {
            oneEntity = tmpEnts[0];
            has_data = 1;
        }
        else
        {
            has_data = 0;
        }
        IMESH_AFREE(tmpEnts);
#endif

        if (has_data && itapsError == iBase_SUCCESS)
        {
            // get all the tags defined on this one entity
            IMESH_ADEF(iBase_TagHandle, tagsOnOneEntity);
            iMesh_getAllTags(mesh, oneEntity, IMESH_AARG(tagsOnOneEntity), &itapsError);
            CheckITAPSError(mesh, iMesh_getAllTags, (0,tagsOnOneEntity,EoL));

            // Treat these (entity level) tags just like we do for tags on this
            // entity set in all respects except 'value'. We'll treat that like
            // an honest to goodness 'variable'.
            for (int t = 0; t < tagsOnOneEntity_size; t++)
            {
                tagHdls.push_back(tagsOnOneEntity[t]);

                int typeId;
                iMesh_getTagType(mesh, tagsOnOneEntity[t], (int*) &typeId, &itapsError);
                CheckITAPSError(mesh, iMesh_getTagType, NoL);
                tagTypes.push_back(typeId);

                int tagSize;
                iMesh_getTagSizeValues(mesh, tagsOnOneEntity[t], &tagSize, &itapsError);
                CheckITAPSError(mesh, iMesh_getTagSizeValues, NoL); 
                tagSizes.push_back(tagSize);

                string tagName = imeshGetTagName(mesh, tagsOnOneEntity[t]);
                CheckITAPSError(mesh, imeshGetTagName, NoL); 
                tagNames.push_back(tagName);

                tagVals.push_back("on " + string(iBase_EntityType_names[i]) + " entities");
            }
            IMESH_AFREE(tagsOnOneEntity);
        }

#ifdef USE_ITER
        iMesh_endEntIter(mesh, entIt, &itapsError);
        CheckITAPSError(mesh, iMesh_endEntIter, NoL);
#endif

    }

    //
    // Collect information on sets contained in this set.
    //
    iMesh_getNumEntSets(mesh, set, 1, &containedSetCount, &itapsError);
    iMesh_getNumEntSets(mesh, set, INT_MAX, &descendentSetCount, &itapsError);
    iMesh_getNumPrnt(mesh, set, 1, &numParent, &itapsError);
    iMesh_getNumChld(mesh, set, 1, &numChild, &itapsError);

    // Recurse on subsets
    iMesh_getEntSets(mesh, set, 1, IMESH_AARG(entSets), &itapsError); 
    CheckITAPSError(mesh, iMesh_getEntSets, (0,entSets,EoL)); 
    for (int i = 0; i < entSets_size; i++)
        subObjs.push_back(new iMeshSet(mesh, entSets[i]));

    IMESH_AFREE(entSets);

funcEnd: ;
}

// ****************************************************************************
//  Destructor:  iMeshSet::~iMeshSet
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
// ****************************************************************************
iMeshSet::~iMeshSet()
{
    for (unsigned int i=0; i<subObjs.size(); i++)
    {
        delete subObjs[i];
    }
}




// ----------------------------------------------------------------------------
//                              iMesh File
// ----------------------------------------------------------------------------


// ****************************************************************************
//  Constructor:  iMeshFile::iMeshFile
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
iMeshFile::iMeshFile(const QString &name)
{
    rootObj = 0;

    char dummyStr[32] = "";
    iMesh_newMesh(dummyStr, &itapsMesh, &itapsError, 0);
    CheckITAPSError(itapsMesh, iMesh_newMesh, NoL);
    iMesh_getRootSet(itapsMesh, &rootSet, &itapsError);
    CheckITAPSError(itapsMesh, iMesh_getRootSet, NoL);

    iMesh_load(itapsMesh, rootSet, (const char*)name.toAscii(), dummyStr, &itapsError,
        strlen(name.toAscii()), 0);
    CheckITAPSError(itapsMesh, iMesh_load, NoL);

    // Get some global attributes of the iMesh instance and implementation.
    iMesh_getGeometricDimension(itapsMesh, &geomDim, &itapsError);
    CheckITAPSError(itapsMesh, iMesh_getGeometricDimension, NoL);
    iMesh_getDfltStorage(itapsMesh, &dfltOrder, &itapsError);
    IMESH_ADEF(int, tmpTable);
    iMesh_getAdjTable(itapsMesh, IMESH_AARG(tmpTable), &itapsError);
    for (int i = 0; i < tmpTable_size; i++)
        adjTable[i/4][i%4] = tmpTable[i];

    rootObj = new iMeshSet(itapsMesh, rootSet, this);

    // This code block is totally hackish. I needed a way of testing
    // whether iMesh_save would save out tag data. It was easist just
    // to hack some code into this tool instead of writing a new app.
    // Just ignore this block of junk!
#if 0
    // Test creation and storage of tag data
    IMESH_ADEF(iBase_EntityHandle, tmpEnts);
    iMesh_getEntities(itapsMesh, rootSet, iBase_VERTEX, iMesh_ALL_TOPOLOGIES, IMESH_AARG(tmpEnts), &itapsError);
    iBase_TagHandle varTag;
    iMesh_createTag(itapsMesh, "Mark Miller", 1, iBase_DOUBLE, &varTag,
                    &itapsError, strlen("Mark Miller")+1);
    double *data = new double[tmpEnts_size];
    for (int i = 0; i < tmpEnts_size; i++)
        data[i] = i;
    iMesh_setDblArrData(itapsMesh, tmpEnts, tmpEnts_size, varTag, data, tmpEnts_size, &itapsError);
    iMesh_save(itapsMesh, rootSet, "foo", "", &itapsError, strlen("foo")+1, 0);
#endif

funcEnd: ;
}


// ****************************************************************************
//  Destructor:  iMeshFile::~iMeshFile
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
iMeshFile::~iMeshFile()
{
    if (rootObj)
    {
        delete rootObj;
        iMesh_dtor(itapsMesh, &itapsError);
    }
    rootObj = 0;
}

// ****************************************************************************
//  Methods:     iMesh API Functions
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
//  Modifications:
//    Mark C. Miller, Mon May 11 17:42:37 PDT 2009
//    Added logic to treat handle inquiries as a 'variable' over the entities.
// ****************************************************************************

void*
iMeshFile::GetVar(iMeshSet *s, const QString &tagn, const QString &entyp)
{
    if (tagn == "coords")
    {
        // Get vertex entity handles
        IMESH_ADEF(iBase_EntityHandle, verts);
        iMesh_getEntities(itapsMesh, s->set, iBase_VERTEX, iMesh_ALL_TOPOLOGIES,
            IMESH_AARG(verts), &itapsError);

        if (verts_size == 0 || itapsError != iBase_SUCCESS)
            return 0;
         
        // Get coords of these entities
        IMESH_ADEF(double, coords);
        iMesh_getVtxArrCoords(itapsMesh, verts, verts_size,
            iBase_INTERLEAVED, IMESH_AARG(coords), &itapsError);

        if (itapsError != iBase_SUCCESS)
        {
            IMESH_AFREE(verts);
            IMESH_AFREE(coords);
            return 0;
        }

        IMESH_AFREE(verts);
        return coords;
    }
    else if (entyp == "handles (ids)")
    {
        int j;
        int entType = iBase_ALL_TYPES;
        int entTopo = iMesh_ALL_TOPOLOGIES;

        // See if 'tag' name is an entity type
        for (j = 0; j < iBase_ALL_TYPES; j++)
        {
            if (tagn == QString(iBase_EntityType_names[j]))
            {
                entType = j;
                break;
            }
        }
        if (entType == iBase_ALL_TYPES)
        {
            for (j = 0; j < iMesh_ALL_TOPOLOGIES; j++)
            {
                if (tagn == QString(iMesh_EntityTopology_names[j]))
                {
                    entTopo = j;
                    break;
                }
            }
        }

        // Get handles for these entities 
        IMESH_ADEF(iBase_EntityHandle, ents);
        iMesh_getEntities(itapsMesh, s->set, entType, entTopo,
            IMESH_AARG(ents), &itapsError);

        return (void*) ents;
    }
    else
    {
        int entType = -1;
        iBase_TagHandle tagh;
        string tn = (const char*) tagn.toAscii();
        string et = (const char*) entyp.toAscii();
        for (unsigned int i = 0; entType == -1 && i < s->tagNames.size(); i++)
        {
            if (tn == s->tagNames[i] && et == s->tagVals[i])
            {
                tagh = s->tagHdls[i];
                for (int j = 0; entType == -1 && j < iBase_ALL_TYPES; j++)
                {
                    if (s->tagVals[i].find(iBase_EntityType_names[j]) != string::npos)
                        entType = j;
                }
            }
        }
        if (entType == -1)
            return 0;

        // Get handles for these entities 
        IMESH_ADEF(iBase_EntityHandle, ents);
        iMesh_getEntities(itapsMesh, s->set, entType, iMesh_ALL_TOPOLOGIES,
            IMESH_AARG(ents), &itapsError);

        if (ents_size == 0 || itapsError != iBase_SUCCESS)
            return 0;

        int varType = GetVarType(s, tagn, entyp);
        switch (varType)
        {
            case iBase_INTEGER:
            {
                IMESH_ADEF(int, vals);
                iMesh_getIntArrData(itapsMesh, ents, ents_size, tagh,
                    IMESH_AARG(vals), &itapsError);
                if (itapsError != iBase_SUCCESS)
                {
                    IMESH_AFREE(ents);
                    IMESH_AFREE(vals);
                    return 0;
                }
                IMESH_AFREE(ents);
                return vals;
            }
            case iBase_DOUBLE:
            {
                IMESH_ADEF(double, vals);
                iMesh_getDblArrData(itapsMesh, ents, ents_size, tagh,
                    IMESH_AARG(vals), &itapsError);
                if (itapsError != iBase_SUCCESS)
                {
                    IMESH_AFREE(ents);
                    IMESH_AFREE(vals);
                    return 0;
                }
                IMESH_AFREE(ents);
                return vals;
            }
        }
    }

    return 0;
}

int
iMeshFile::GetVarType(iMeshSet *s, const QString &tagn, const QString &entyp)
{
    if (tagn == "coords")
        return iBase_DOUBLE;
    if (entyp == "handles (ids)")
        return iBase_ENTITY_HANDLE;

    string tn = (const char*) tagn.toAscii();
    string et = (const char*) entyp.toAscii();
    for (unsigned int i = 0; i < s->tagNames.size(); i++)
    {
        if (tn == s->tagNames[i] && et == s->tagVals[i])
            return s->tagTypes[i];
    }
}

int
iMeshFile::GetVarLength(iMeshSet *s, const QString &tagn, const QString &entyp)
{
    if (tagn == "coords")
        return s->typeCounts[0];

    if (entyp == "handles (ids)")
    {
        int j;
        // See if its an entity type entry
        for (j = 0; j < iBase_ALL_TYPES; j++)
        {
            if (tagn == QString(iBase_EntityType_names[j]))
                return s->typeCounts[j];
        }
        // See if its an entity topology entry 
        for (j = 0; j < iMesh_ALL_TOPOLOGIES; j++)
        {
            if (tagn == QString(iMesh_EntityTopology_names[j]))
                return s->topoCounts[j];
        }
        return 0;
    }

    string tn = (const char*) tagn.toAscii();
    string et = (const char*) entyp.toAscii();
    for (unsigned int i = 0; i < s->tagNames.size(); i++)
    {
        if (tn == s->tagNames[i] && et == s->tagVals[i])
        {
            for (int j = 0; j < iBase_ALL_TYPES; j++)
            {
                if (s->tagVals[i].find(iBase_EntityType_names[j]) != string::npos)
                    return s->typeCounts[j];
            }
        }
    }
    return 0;
}

int
iMeshFile::GetVarTupleSize(iMeshSet *s, const QString &tagn, const QString &entyp)
{
    if (tagn == "coords")
        return 3;
    if (entyp == "handles (ids)")
        return 1;

    string tn = (const char*) tagn.toAscii();
    string et = (const char*) entyp.toAscii();
    for (unsigned int i = 0; i < s->tagNames.size(); i++)
    {
        if (tn == s->tagNames[i] && et == s->tagVals[i])
            return s->tagSizes[i];
    }
}

//
//  Modifications
//    Mark C. Miller, Tue Jul 27 21:26:14 PDT 2010
//    Made conditional on HAVE_VTK

#ifdef HAVE_VTK 

#include <vtkCellType.h>
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>

int ITAPSEntityTopologyToVTKZoneType(int ttype)
{
    switch (ttype)
    {
    // 0D
    case iMesh_POINT:         return VTK_VERTEX;

    // 1D
    case iMesh_LINE_SEGMENT:  return VTK_LINE;

    // 2D
    case iMesh_POLYGON:       return VTK_POLYGON;
    case iMesh_TRIANGLE:      return VTK_TRIANGLE;
    case iMesh_QUADRILATERAL: return VTK_QUAD;

    // 3D
    case iMesh_TETRAHEDRON:   return VTK_TETRA;
    case iMesh_PYRAMID:       return VTK_PYRAMID;
    case iMesh_PRISM:         return VTK_WEDGE;
    case iMesh_HEXAHEDRON:    return VTK_HEXAHEDRON;
    }
    return -1;
}

void
iMeshFile::SaveAsVTK(const QString &file)
{
    if (file == "")
        return;

    vtkUnstructuredGrid *ugrid = vtkUnstructuredGrid::New();
    vtkPoints *pts = vtkPoints::New();
    pts->SetNumberOfPoints(rootObj->typeCounts[0]);

    //
    // We know we've got to at least have coordinates (defined on vertex entities)
    // So, get 'em and add them to the ugrid object.
    //
    double *coords = (double*) GetVar(rootObj, "coords", "");
    vtkDoubleArray *da = vtkDoubleArray::New();
    da->SetNumberOfComponents(3);
    da->SetArray(coords, rootObj->typeCounts[0]*3, 0);
    pts->SetData(da);
    da->Delete();
    ugrid->SetPoints(pts);
    pts->Delete();

    //
    // Handle any tags defined on
    // 

    //
    // In mapping this iMesh object into a VTK unstructured grid, we want to make
    // it as VTK friendly as possible. This means that we should at least instantiate
    // VTK 'cells' for analagous iMesh entities if indeed we have 'em. We determine 
    // which entity type that is now. 
    //
    int entTypeForVTKAnalogs = -1;
    if (geomDim == 1)
    {
        if (rootObj->typeCounts[1] > 0)
            entTypeForVTKAnalogs = iBase_EDGE;
    }
    else if (geomDim == 2)
    {
        if (rootObj->typeCounts[2] > 0)
            entTypeForVTKAnalogs = iBase_FACE;
        else if (rootObj->typeCounts[1] > 0)
            entTypeForVTKAnalogs = iBase_EDGE;
    }
    else
    {
        if (rootObj->typeCounts[3] > 0)
            entTypeForVTKAnalogs = iBase_REGION;
        else if (rootObj->typeCounts[2] > 0)
            entTypeForVTKAnalogs = iBase_FACE;
        else if (rootObj->typeCounts[1] > 0)
            entTypeForVTKAnalogs = iBase_EDGE;
    }
    

    //
    // Ok, now we'll gather up ent-to-node mapping for these entities and
    // create these entities as VTK cells in the ugrid object.
    // 
    if (entTypeForVTKAnalogs != -1)
    {
        //
        // Get the entities in terms of their nodes so we can add 'em to the VTK
        // unstructured grid representation.
        //
        IMESH_ADEF(iBase_EntityHandle, entHdls);
        IMESH_ADEF(iBase_EntityHandle, adjHdls);
        IMESH_ADEF(int, adjInds);
        IMESH_ADEF(int, offs);
        iMesh_getAdjEntIndices(itapsMesh, rootObj->set, entTypeForVTKAnalogs, iMesh_ALL_TOPOLOGIES, iBase_VERTEX,
            IMESH_AARG(entHdls), IMESH_AARG(adjHdls), IMESH_AARG(adjInds), IMESH_AARG(offs), &itapsError);
        CheckITAPSError(itapsMesh, iMesh_getAdjEntIndices, (0,entHdls,adjHdls,adjInds,offs,EoL));
        if (entHdls_size <= 0 || adjHdls_size <= 0)
            ITAPSErrorCleanupHelper(0,entHdls,adjHdls,adjInds,offs,EoL);

        //
        // Ok, iterate over all these entities, adding them to ugrid
        //
        for (int i = 0; i < entHdls_size; i++)
        {
            int topoType;
            iMesh_getEntTopo(itapsMesh, entHdls[i], &topoType, &itapsError);
            int vtkZoneType = ITAPSEntityTopologyToVTKZoneType(topoType);
            if (vtkZoneType == -1)
            {
                cerr << "Invalid VTK zone type for iMesh entity " << i
                     << " and topology type \"" << iMesh_EntityTopology_names[topoType]
                     << "\"" << endl;
                continue;
            }
            vtkIdType vertIds[8];
            int jj = 0;
            bool valid = true;
            for (int idx = offs[i];
                 idx < ((i+1) < offs_size ? offs[i+1] : offs_size); idx++)
            {
                if (jj >= sizeof(vertIds) / sizeof(vertIds[0]))
                    valid = false;
                if (valid)
                    vertIds[jj++] = adjInds[idx];
                else
                   jj++;
            }
            if (!valid)
            {
                cerr << "Unsupported vert count " << jj
                     << " for iMesh entity " << i << endl;
                continue;
            }
            ugrid->InsertNextCell(vtkZoneType, jj, vertIds);
        }

        //
        // Ok, now handle 'dense' tags defined on these entities, if any, 
        //

        IMESH_AFREE(entHdls);
        IMESH_AFREE(adjHdls);
        IMESH_AFREE(adjInds);
        IMESH_AFREE(offs);
    }

    //
    // Ok, what to do with other entities that are defined in the iMesh root set?
    // It is not a good idea to add these as cells to the VTK ugrid object
    // because they'll wind up living in the same 'address space' as the ones
    // we just added in the logic above and so break our ability to later define
    // CellData or PointData on them representing dense tags over the analagous
    // entities in the iMesh object. Instead, if these 'other' entities exist
    // we'll add them as opaque field data.
    // 


    //
    // Ok, that takes care of the mesh itself. Now, lets handle the sets
    //

    vtkUnstructuredGridWriter *ugwrtr = vtkUnstructuredGridWriter::New();
    ugwrtr->SetFileName(file.toAscii());
    ugwrtr->SetInput(ugrid);
    ugwrtr->Write();
    ugwrtr->Delete();
    ugrid->Delete();
}
#endif
