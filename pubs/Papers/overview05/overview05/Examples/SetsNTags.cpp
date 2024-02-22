/**
 * Copyright 2006 Sandia Corporation.  Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Coroporation, the U.S. Government
 * retains certain rights in this software.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 */
/** SetsNTags: reading sets & tags from a mesh
 * 
 * This program shows how to read sets and tags from a mesh
 *
 * Usage: SetsNTags <mesh_file_name>
 *
 */

#include <iostream>
#include <set>
#include "TSTTM.hh"
#include "TSTTB.hh"

typedef void* EntityHandle;
typedef void* EntitySetHandle;
typedef void* TagHandle;

int main( int argc, char *argv[] )
{
  // Check command line arg
  std::string filename = argv[1];

    // create the Mesh instance
  TSTTM::Mesh mesh = TSTTM::Factory::newMesh("MOAB");
  
    // load the mesh
  mesh.load(0, filename);

    // get all sets; use EntSet interface
  sidl::array<EntitySetHandle> sets;
  int sets_size;
  TSTTB::EntSet mesh_eset = mesh;
  mesh_eset.getEntSets(0, 1, sets, sets_size);

    // iterate through them, checking whether they have tags
  TSTTB::SetTag mesh_stag = mesh;
  std::set<TagHandle> tag_handles;
  for (int i = 0; i < sets_size; i++) {
      // get connectivity
    sidl::array<TagHandle> tags;
    int tags_size;

    mesh_stag.getAllEntSetTags(sets[i], tags, tags_size);
    
      // put them in a std::set
    for (int j = 0; j < tags_size; j++) {
      std::string tname = mesh_stag.getTagName(tags[j]);
      tag_handles.insert(tags[j]);
    }
  }

    // now print information about the tags found on sets
  for (std::set<TagHandle>::iterator sit = tag_handles.begin(); 
       sit != tag_handles.end(); sit++) {
    std::string tname = mesh_stag.getTagName(*sit);
    int tsize = mesh_stag.getTagSizeBytes(*sit);
    std::cout << "Tag name = '" << tname 
              << "', size = " << tsize << " bytes." << std::endl;
  }
  
  return true;
}
