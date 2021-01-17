#include "StaticMesh.h"


void SStaticMesh::PostLoad()
{
	Sections = { IRendererApi::Get()->CreateMesh(&Vertices, Triangles); };
	
}
