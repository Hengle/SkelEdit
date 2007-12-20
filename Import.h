#ifndef __IMPORT_H__
#define __IMPORT_H__


#include "SkeletalMesh.h"
#include "AnimSet.h"


void ImportPsk(CArchive &Ar, CSkeletalMesh &Mesh);

//!!!!! TESTING !!!!!
void ShowMesh(const CSkeletalMesh &Mesh, bool Wireframe, bool ShowSkel);


#endif