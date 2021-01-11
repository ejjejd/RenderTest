#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>

#include "../math/vectors/vector3.h"

Mesh* CreateMesh(const assets::AssetRef assetRef)
{
   Mesh* resMesh = (Mesh*)(malloc(sizeof(Mesh)));
   if (!resMesh)
      return NULL;

   auto meshData = assetRef.GetData<assets::MeshAssetData>();

   int facesCount = meshData->FacesCount;

   resMesh->VerticesCount = facesCount;

   GLuint vbo;
   GLuint nvbo;
   GLuint uvbo;

   glGenVertexArrays(1, &resMesh->VaoId);

   glGenBuffers(1, &vbo);
   glGenBuffers(1, &nvbo);
   glGenBuffers(1, &uvbo);

   glBindVertexArray(resMesh->VaoId);


   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * facesCount, meshData->Positions, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

   
   glBindBuffer(GL_ARRAY_BUFFER, nvbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * facesCount, meshData->Normals, GL_STATIC_DRAW);

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


   glBindBuffer(GL_ARRAY_BUFFER, uvbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2) * facesCount, meshData->UVs, GL_STATIC_DRAW);

   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);


   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   return resMesh;
}