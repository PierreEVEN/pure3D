#include "MeshComponent.h"
#include "Mesh.h"

SMeshComponent::SMeshComponent(SRenderer* InContext, IMesh* InMesh, const std::vector<SMaterial*>& inMaterialOverrides)
	: MaterialOverrides(inMaterialOverrides), SPrimitiveComponent(InContext) {
	SetMesh(InMesh);
}

void SMeshComponent::SetMesh(IMesh* inMesh) {
	Mesh = inMesh;
	ClearProxies();
	for (int i = 0; i < Mesh->GetSections().size(); ++i) AddProxy(new SMeshProxy(this, (uint32_t)ERenderPass::ERenderPass_COLOR | (uint32_t)ERenderPass::ERenderPass_NORMAL));
}

void SMeshComponent::UpdateProxies() {
	for (int i = 0; i < GetProxies().size(); ++i) {
		const IMesh::SMeshSection& Section = *(Mesh->GetSections().begin() + i);
		SMeshProxy* MeshProxy = static_cast<SMeshProxy*>(GetProxies()[i]);
		MeshProxy->MeshHandle = Section.MeshHandle;
		MeshProxy->MaterialHandle = Section.MeshData.Material->GetHandle();
		MeshProxy->Transform = GetAbsoluteTransform();
	}
}