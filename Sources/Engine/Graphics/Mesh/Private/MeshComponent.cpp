#include "MeshComponent.h"
#include "Mesh.h"

SMeshComponent::SMeshComponent(SRenderer* InContext, IMesh* InMesh, const std::vector<SMaterial*>& inMaterialOverrides)
	: MaterialOverrides(inMaterialOverrides), SPrimitiveComponent(InContext) {
	SetMesh(InMesh);
}

void SMeshComponent::SetMesh(IMesh* inMesh) {
	Mesh = inMesh;
	ClearProxies();
	for (int i = 0; i < Mesh->GetSections().size(); ++i) AddProxy(new SMeshProxy);
}

void SMeshComponent::UpdateProxy(IPrimitiveProxy* Proxy, size_t ProxyID) {
	const IMesh::SMeshSection& Section = *(Mesh->GetSections().begin() + ProxyID);
	SMeshProxy* MeshProxy = static_cast<SMeshProxy*>(Proxy);
	MeshProxy->MeshHandle = Section.MeshHandle;
	MeshProxy->MaterialHandle = Section.MeshData.Material->GetHandle();
	MeshProxy->Transform = GetAbsoluteTransform();
}