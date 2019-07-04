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

#include <iMeshArrayView.h>
#include <iMeshSetView.h>
#include <iMeshFile.h>
#include <QListWidget>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
using std::cerr;
using std::map;

// ----------------------------------------------------------------------------
//                            Array View
// ----------------------------------------------------------------------------

// ****************************************************************************
//  Constructor: iMeshArrayViewWindow::iMeshArrayViewWindow
//
//  Programmer:  Mark C. Miller (plagarized from Jeremy Meredith's Silex)
//  Creation:    April 15, 2009 
//
// ****************************************************************************
iMeshArrayViewWindow::iMeshArrayViewWindow(iMeshFile *s, iMeshSetViewItem *i,
                                         QWidget *p)
    : QMainWindow(p)
{
    if (!p || !s || !i)
    {
        cerr << "iMeshArrayViewWindow::iMeshArrayViewWindow -- invalid args passed\n";
        return;
        //throw;
    }

    iMeshFile *imesh = s;
    iMeshSet *set = i->set;
    QString tagn = i->text(0);
    QString entyp = i->text(3);

    setWindowTitle(QString("Array: ")+tagn+" "+entyp);

    lb = new QListWidget(this);
    setCentralWidget(lb);

    void *var = imesh->GetVar(set, tagn, entyp);
    if (!var)
    {
        cerr << "iMeshArrayViewWindow::iMeshArrayViewWindow -- not an array var\n";
        return;
        //throw;
    }

    //
    // If we're here to display entity handles,
    // create a map back to global (root) context.
    //
    map<iBase_EntityHandle,int> entIdMap;
    if (entyp == "handles (ids)")
    {
        iBase_EntityHandle *allents = (iBase_EntityHandle*) imesh->GetVar(s->rootObj, tagn, entyp);
        int cnt = imesh->GetVarLength(s->rootObj, tagn, entyp);
        for (int i = 0; i < cnt; i++)
            entIdMap[allents[i]] = i;
        free(allents);
    }

    int type = imesh->GetVarType(set, tagn, entyp);
    int len  = imesh->GetVarLength(set, tagn, entyp);
    int size = imesh->GetVarTupleSize(set, tagn, entyp);
    int mult; 
    switch (type)
    {
        case iBase_INTEGER: mult = 11; break; // 9 digit int plus ',' plus ' '
        case iBase_DOUBLE:  mult = 18; break; // 16 digit double plus ',' plus ' '
        case iBase_BYTES:   mult = 1; break;  // single character
        case iBase_ENTITY_HANDLE: mult = 2*sizeof(iBase_EntityHandle)+13; break;
    }
    int str_size = size * mult;
    char *str = new char[10*str_size+6]; // 6 for lead in 

    // Just a bunch of numbers: one per line
    for (int i=0; i<len; i++)
    {
        str[0] = '\0';
        sprintf(str, "%-4d: ", i); // lead in
        for (int j=0; j<size; j++)
        {
            switch (type)
            {
                  case iBase_INTEGER:
                    sprintf(&str[j*mult+6], "% 9d, ", ((int*)var)[i*size+j]);
                    break;
                  case iBase_DOUBLE:
                    sprintf(&str[j*mult+6], "% 16.8g, ", ((double*)var)[i*size+j]);
                    break;
                  case iBase_BYTES:
                    sprintf(&str[j*mult+6], "%c", ((char*)var)[i*size+j]);
                    break;
                  case iBase_ENTITY_HANDLE:
                  {
                      iBase_EntityHandle hval = ((iBase_EntityHandle*)var)[i*size+j];
#if 0
                      char *hp = (char*) &hval;
                      char hexChars[2*sizeof(iBase_EntityHandle)+1];
                      for (int k = sizeof(iBase_EntityHandle) - 1; k >= 0; k--, hp++)
                          sprintf(&hexChars[2*k],"%.2X", ((unsigned int) (*hp)) & 0x00FF);
                      hexChars[2*sizeof(iBase_EntityHandle)] = '\0';
                      sprintf(&str[j*mult+6], "0x%s (%d), ", hexChars, entIdMap[hval]);
#endif
                      if (entIdMap.find(hval) == entIdMap.end())
                          sprintf(&str[j*mult+6], "%p (???), ", hval);
                      else
                          sprintf(&str[j*mult+6], "%p (%d), ", hval, entIdMap[hval]);
                      break;
                  }
            }
        }
        str[str_size+6-1] = '\0';
        lb->addItem(str);
    }
    delete [] str;
    free(var);
}
