#include "MeshComponent.h"
#include "Mesh.h"

MeshComponent::MeshComponent(SRenderer* InContext, IMesh* InMesh)
	: Context(InContext), Mesh(InMesh) {
	SMeshProxy* Proxy = new SMeshProxy();
	Sections.push_back(Proxy);
	InContext->AddProxy(Proxy);
}