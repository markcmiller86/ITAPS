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

#include <iMeshSetView.h>
#include <iMeshSetTreeView.h>
#include <iMeshFile.h>
#include <iBase_names.h>
#include <iostream>
#include <cstdio>


#include <QTreeWidgetItem>

// ****************************************************************************
//  Constructor:  iMeshSetView::iMeshSetView
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
iMeshSetView::iMeshSetView(QWidget *p)
    : QTreeWidget(p)
{
    total_items = 0;

    //setRootIsDecorated(true);
    //setSorting(-1);
    setColumnCount(4);
    setAllColumnsShowFocus(true);
    headerItem()->setText(0,"Name");
    headerItem()->setText(1,"Type");
    headerItem()->setText(2,"Count/Size");
    headerItem()->setText(3,"Value(s)");
}

// ****************************************************************************
//  Method:  iMeshSetView::Set
//
//  Purpose:
//    View the TOC of a new directory.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
void
iMeshSetView::Set(iMeshSet *d)
{
    clear();
    total_items = 0;

    //
    // Output information on Root set if we're at the root
    //
    if (d->fileObj)
    {
        char tmp[64];
        QStringList sl;
        sl << "Geometric Dimension" << "integer" << "1" << QString((char) (48+d->fileObj->geomDim));
        new iMeshSetViewItem(d, this, sl);
        total_items++;
        sl.clear();
        sprintf(tmp, "%d (%s)", d->fileObj->dfltOrder, iBase_StorageOrder_names[d->fileObj->dfltOrder]);
        sl << "Default storage" << "integer" << "1" << tmp;
        new iMeshSetViewItem(d, this, sl);
        total_items++;
        for (int i = 0; i < 4; i++)
        {
            QStringList sltmp;
            char tmp1[64], tmp2[256];
            sprintf(tmp1, "Adjacency table[%d]", i);
            sprintf(tmp2, "%d (%s), %d (%s), %d (%s), %d (%s)",
                d->fileObj->adjTable[i][0], iBase_AdjacencyCost_names[d->fileObj->adjTable[i][0]],
                d->fileObj->adjTable[i][1], iBase_AdjacencyCost_names[d->fileObj->adjTable[i][1]],
                d->fileObj->adjTable[i][2], iBase_AdjacencyCost_names[d->fileObj->adjTable[i][2]],
                d->fileObj->adjTable[i][3], iBase_AdjacencyCost_names[d->fileObj->adjTable[i][3]]);
            sltmp << tmp1 << "integer" << "4" << tmp2;
            new iMeshSetViewItem(d, this, sltmp);
            total_items++;
        }

    }

    //
    // Output information on contained ent sets and parent/child ent sets.
    //
    int miscCounts[5] = {d->isList, d->containedSetCount, d->descendentSetCount, d->numParent, d->numChild};
    char *miscHeaders[5] = {"isList", "numEntSets (hops=1)","numEntSets (hops=inf)", "numPrnt", "numChld"};
    for (int j = 0; j < 5; j++)
    {
        if (j > 0 && miscCounts[j] <= 0)
            continue;
        char count[32];
        snprintf(count, sizeof(count), "%d", miscCounts[j]);
        QStringList sl;
        sl << miscHeaders[j] << "integer" << "1" << count; 
        new iMeshSetViewItem(d, this, sl);
        total_items++;
    }

    //
    // Output information on contained entity types
    //
    for (int j = 0; j < iBase_ALL_TYPES; j++)
    {
        if (d->typeCounts[j] == 0)
            continue;

        char count[32];
        snprintf(count, sizeof(count), "%d", d->typeCounts[j]); 

        QStringList sl;
        sl << iBase_EntityType_names[j] << "EntityType" << count << "handles (ids)"; 
        new iMeshSetViewItem(d, this, sl);
        total_items++;
    }

    //
    // Output information on contained entity topologies
    //
    for (int j = 0; j < iMesh_ALL_TOPOLOGIES; j++)
    {
        if (d->topoCounts[j] == 0)
            continue;

        char count[32];
        snprintf(count, sizeof(count), "%d", d->topoCounts[j]); 

        QStringList sl;
        sl << iMesh_EntityTopology_names[j] << "EntityTopology" << count << "handles (ids)"; 
        new iMeshSetViewItem(d, this, sl);
        total_items++;
    }

    //
    // Output information on tags
    //
    for (unsigned int i = 0; i < d->tagNames.size(); i++)
    {
        char *type;
        switch (d->tagTypes[i])
        {
            case iBase_INTEGER: type = "integer"; break;
            case iBase_DOUBLE:  type = "double"; break;
            case iBase_BYTES:   type = "bytes"; break;
            case iBase_ENTITY_HANDLE: type = "handle"; break;
        }
        char size[32];
        snprintf(size, sizeof(size), "%d", d->tagSizes[i]); 

        QStringList sl;
        sl << QString(d->tagNames[i].c_str()) << type << size << QString(d->tagVals[i].c_str()); 
        // Use existence of 'entities' in tagVals to decided if this item
        // should be a clickable SetViewItem (which will make an array view window).
        // This works because all the dense tag values are are of the form
        // 'on <entity type> entities'
        if (d->tagVals[i].find("entities") != string::npos)
            new iMeshSetViewItem(d, this, sl);
        else
            new QTreeWidgetItem(this, sl);
        total_items++;
    }

    //
    // Add a line for coords if we have verts
    //
    if (d->typeCounts[0] > 0)
    {
        QStringList sl;
        sl << "coords" << "double" << "3" << "on VERTEX entities";
        new iMeshSetViewItem(d, this, sl);
        total_items++;
    }

    resizeColumnToContents(0);
    resizeColumnToContents(1);
    resizeColumnToContents(2);
    resizeColumnToContents(3);
}

// ****************************************************************************
//  Method:  iMeshSetView::ChangeSet
//
//  Purpose:
//    Wrapper for "Set" which is suitable for a QTreeWidgetItem callback.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
void
iMeshSetView::ChangeSet(QTreeWidgetItem *i)
{
    iMeshSetTreeViewItem *item = (iMeshSetTreeViewItem*)i;
    Set(item->set);
}

// ****************************************************************************
//  Method:  iMeshSetView::resizeEvent
//
//  Purpose:
//    Make the column header fill the width of the listview.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
// ****************************************************************************
void
iMeshSetView::resizeEvent(QResizeEvent *re)
{
    QTreeWidget::resizeEvent(re);
    //setColumnWidth(0, width() - 4);
}
