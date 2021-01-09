#include "MeshComponent.h"
#include "Mesh.h"

MeshComponent::MeshComponent(SRenderer* InContext, IMesh* InMesh)
	: Context(InContext), Mesh(InMesh) {
	MeshProxy* Proxy = new MeshProxy();
	Sections.push_back(Proxy);
	InContext->AddProxy(Proxy);
}