#include "Mesh.h"

void SMeshProxy::Render(SRenderer* Context) {
	static_cast<SMeshRenderHelper*>(RenderHelper)->DrawMesh(*Lods[GetLOD()].MeshHandle);
}
