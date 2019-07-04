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

#include "Explorer.h"
#include <iMeshView.h>

#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>

// ****************************************************************************
//  Constructor: Explorer::Explorer
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
Explorer::Explorer(const QString &file, QWidget *p)
    : QMainWindow(p)
{
    view = new iMeshView(file, this);
    if (!view->HasiMeshFile())
    {
        delete view;
        view = 0;
    }
    else
    {
        setCentralWidget(view);
        setWindowTitle("eyeMesh Explorer: "+file);

        QMenu *filemenu = menuBar()->addMenu("&File");
        filemenu->addAction( "&Open",  this, SLOT(open()),  tr("Ctrl+O") );
        filemenu->addSeparator();
        filemenu->addAction( "S&ave as VTK", this, SLOT(save()),  tr("Ctrl+S") );
        filemenu->addSeparator();
        filemenu->addAction( "E&xit", this, SLOT(close()),  tr("Ctrl+X") );
    }
}

// ****************************************************************************
//  Destructor:  Explorer::~Explorer
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
Explorer::~Explorer()
{
    if (view) delete view;
    view = 0;
}

// ****************************************************************************
//  Method:  Explorer::open
//
//  Purpose:
//    Open a new file.
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
// ****************************************************************************
void
Explorer::open()
{
    QString file = 
        QFileDialog::getOpenFileName(this,"Open file...","",
                                     "MOAB iMesh files (*.h5m *.g *.gmsh *.vtk);;"
                                     "All files (*)");
    if (file.isNull())
        return;

    setWindowTitle("eyeMesh Explorer: "+file);
    view->Set(file);
}

void
Explorer::save()
{
    if (!view)
        return;

    QString file = 
        QFileDialog::getSaveFileName(this,"Save file as VTK...","",
                                     "VTK files (*.vtk);;"
                                     "All files (*)");

    view->SaveAsVTK(file);
}
