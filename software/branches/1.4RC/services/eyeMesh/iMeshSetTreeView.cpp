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

#include "iMeshSetTreeView.h"
#include <iMeshFile.h>
#include <QPixmap>

#include <folder.xpm>

// ****************************************************************************
//  Constructor:  iMeshSetTreeView::iMeshSetTreeView
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
iMeshSetTreeView::iMeshSetTreeView(iMeshFile *s, QWidget *p)
    : QTreeWidget(p), imesh(s)
{
    folder_pixmap = new QPixmap(folder_xpm);
 
    setColumnCount(1);
    headerItem()->setText(0,"Contents");

    total_items = 0;
    QTreeWidgetItem *root = AddSet(imesh->rootObj, NULL);
    root->setExpanded(true);
    resizeColumnToContents(0);
}

// ****************************************************************************
//  Method:  iMeshSetTreeView::AddSet
//
//  Purpose:
//    Add a new directory to the SetTree.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
QTreeWidgetItem *
iMeshSetTreeView::AddSet(iMeshSet *d, QTreeWidgetItem *parent)
{
    total_items++;

    QTreeWidgetItem *item;
    if (!parent)
        item = new iMeshSetTreeViewItem(d, this, "Root Set");
    else
        item = new iMeshSetTreeViewItem(d, parent, "Entity Set");

    item->setIcon(0,*folder_pixmap);
    for (unsigned int i=0; i<d->subObjs.size(); i++)
        AddSet(d->subObjs[i], item);

    return item;
}

// ****************************************************************************
//  Method:  iMeshSetTreeView::Set
//
//  Purpose:
//    Reset the view to a new file.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
void
iMeshSetTreeView::Set(iMeshFile *s)
{
    imesh = s;
    clear();
    total_items = 0;
    QTreeWidgetItem *root = AddSet(imesh->rootObj, NULL);
    root->setExpanded(true);
}

// ****************************************************************************
//  Method:  iMeshSetTreeView::OpenRootSet
//
//  Purpose:
//    Select the root directory of the tree.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
// ****************************************************************************
void
iMeshSetTreeView::OpenRootSet()
{
    setCurrentItem(topLevelItem(0));
}

// ****************************************************************************
//  Method:  iMeshSetTreeView::resizeEvent
//
//  Purpose:
//    Make the column header fill the width of the listview.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
void
iMeshSetTreeView::resizeEvent(QResizeEvent *re)
{
    QTreeWidget::resizeEvent(re);
    //resizeColumnToContents(0);
}

