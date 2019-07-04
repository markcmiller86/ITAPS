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

#include "iMeshView.h"
#include <iMeshFile.h>
#include <iMeshSetTreeView.h>
#include <iMeshSetView.h>
#include <iMeshArrayView.h>
#include <QHeaderView>
#include <QMessageBox>
#include <iostream>

using std::cerr;
using std::endl;


// ----------------------------------------------------------------------------
//                            iMesh View
// ----------------------------------------------------------------------------

// ****************************************************************************
//  Constructor: iMeshView::iMeshView
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
iMeshView::iMeshView(const QString &file, QWidget *p)
    : QSplitter(p)
{
    imesh        = new iMeshFile(file);
    if (!imesh->IsOpen())
    {
        delete imesh;
        imesh = 0;
        setTreeView = 0;
        setView = 0;
    }
    else
    {
        setTreeView = new iMeshSetTreeView(imesh, this);
        setView     = new iMeshSetView(this);

        connect(setTreeView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                this,        SLOT(SetSet(QTreeWidgetItem*,QTreeWidgetItem*)));
        connect(setView,     SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
                this,        SLOT(ShowItem(QTreeWidgetItem*,int)));

        setTreeView->OpenRootSet();
        setView->Set(imesh->rootObj);

        setTreeView->header()->setResizeMode(QHeaderView::Interactive);
        setView->header()->setResizeMode(QHeaderView::Interactive);
    }
}

// ****************************************************************************
//  Destructor:  iMeshView::~iMeshView
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
iMeshView::~iMeshView()
{
    if (imesh) delete imesh;
    if (setTreeView) delete setTreeView;
    if (setView) delete setView;
}

// ****************************************************************************
//  Method:  iMeshView::Set
//
//  Purpose:
//    Replace with a new file.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
void
iMeshView::Set(const QString &file)
{
    delete imesh;
    imesh = new iMeshFile(file);
    if (!imesh->IsOpen())
    {
        setTreeView = 0;
        setView = 0;
    }
    else
    {
        setTreeView->Set(imesh);
        setTreeView->OpenRootSet();
        setView->ChangeSet(setTreeView->topLevelItem(0));
    }
}

// ****************************************************************************
//  Method:  iMeshView::SetSet
//
//  Purpose:
//    Tell the current iMeshSetView directories.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
void
iMeshView::SetSet(QTreeWidgetItem *i, QTreeWidgetItem*)
{
    if (i)
        setView->ChangeSet(i);
}

// ****************************************************************************
//  Method:  iMeshView::ShowVariable
//
//  Purpose:
//    Pop up a new window to show a variable or simple array.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
// ****************************************************************************
void
iMeshView::ShowVariable(QTreeWidgetItem *i)
{
    // This logic is because we only want to instantiate ArraViewWindows
    // for dense tags (with 'value' field like 'on VERTEX entities')
    // Also, we only instantiate iMeshSetViewItems for those types of
    // entries in a set view too.
    if (! i->text(3).contains("entities") && ! i->text(3).contains("handles"))
        return;

    iMeshSetViewItem *item = (iMeshSetViewItem*)i;
    try
    {
        iMeshArrayViewWindow *av = new iMeshArrayViewWindow(imesh, item, this);
        av->show();
        av->resize(av->sizeHint());
    }
    catch (...)
    {
        ;
    } 
}

// ****************************************************************************
//  Method:  iMeshView::ShowItem
//
//  Purpose:
//    Wrapper for ShowUnknown appropriate for a QTreeWidgetItem callback.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
//  Modifications:
//    Mark C. Miller, Tue Jul 27 21:25:39 PDT 2010
//    Added conditional compilation for HAVE_VTK and error message.
// ****************************************************************************
void
iMeshView::ShowItem(QTreeWidgetItem *i, int)
{
    ShowVariable(i);
}

void
iMeshView::SaveAsVTK(const QString &file)
{
    if (!imesh)
        return;

#if HAVE_VTK
    try
    {
        imesh->SaveAsVTK(file);
    }
    catch (...)
    {
        ;
    }
#else
    cerr << "Save as VTK option not supported in this build" << endl;
#endif
}
