#include "MeshComponent.h"
#include "Mesh.h"
#include "RenderPass.h"

SMeshComponent::SMeshComponent(SRenderer* InContext, IMesh* InMesh, const std::vector<SMaterial*>& inMaterialOverrides)
	: MaterialOverrides(inMaterialOverrides), SPrimitiveComponent(InContext) {
	SetMesh(InMesh);
}

void SMeshComponent::SetMesh(IMesh* inMesh) {
	Mesh = inMesh;
	ClearProxies();
	for (int i = 0; i < Mesh->GetProxies().size(); ++i) {
		AddProxy(IRendererApi::Get()->CreateProxyFor(SMeshProxy::GetStaticClass(), this, (uint32_t)ERenderPass::ERenderPass_COLOR | (uint32_t)ERenderPass::ERenderPass_NORMAL));
	}
}

void SMeshComponent::UpdateProxies() {
	for (int i = 0; i < GetProxies().size(); ++i) {
		const SMeshProxy& Section = *(Mesh->GetProxies().begin() + i);
		SMeshProxy* MeshProxy = static_cast<SMeshProxy*>(GetProxies()[i]);
		*MeshProxy = Section;
		MeshProxy->Transformation = GetAbsoluteTransform().GetTransformation();
	}
}