#include "StaticMesh.h"


void SStaticMesh::PostLoad() {

	for (const auto& Section : SectionsData)
	{
		SMeshProxy SectionProxy;
		for (const auto& Lod : Section.Lods)
		{
			SMeshProxy::Lod LodProxy;

			LodProxy.MeshHandle = IRendererApi::Get()->CreateMesh(&Lod);
			SectionProxy.Lods.push_back(LodProxy);
		}
		LodProxies.push_back(SectionProxy);
	}
}
