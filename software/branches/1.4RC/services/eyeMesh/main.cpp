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

#include "qapplication.h"
#include "Explorer.h"

#include <cstdlib>

#ifndef Q_WS_MACX
#include "qwindowsstyle.h"
#endif
#include "qfiledialog.h"
#include "qstring.h"
#include <iostream>

// ****************************************************************************
//  Main Function: main()  
//
//  Purpose: initialize and start the main window
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************

int main( int argc, char **argv )
{
    QApplication::setColorSpec(QApplication::ManyColor);
    QApplication a(argc, argv);
#ifndef Q_WS_MACX
    a.setStyle(new QWindowsStyle);
#endif
    Explorer *w = 0;
    if (argc > 1)
    {
        w = new Explorer(argv[1], NULL);
        if (!w->HasiMeshView())
        {
            delete w;
            w = 0;
            std::cerr << "Unable to open file \"" << argv[1] << "\"" << std::endl;
        }
    }

    if (!w)
    {
        QString file =
            QFileDialog::getOpenFileName(NULL, "Open file....", "",
                                         "MOAB iMesh files (*.h5m *.g *.gmsh *.vtk);;"
                                         "All files (*)");
        if (file.isNull())
            return 0;
        w = new Explorer(file, NULL);
    }

    w->show();

    return a.exec();
}
