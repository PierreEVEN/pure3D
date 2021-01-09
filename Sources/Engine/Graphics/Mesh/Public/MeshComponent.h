#pragma once

#include "MeshComponent.refl.h"

struct SRenderer;
struct MeshProxy;
struct IMesh;

REFLECT()
struct MeshComponent {

	REFLECT_BODY()

		MeshComponent(SRenderer* InContext, IMesh* InMesh);

	// transform

private:

	void UpdateProxy() {}

	IMesh* Mesh;

	std::vector<MeshProxy*> Sections;
	SRenderer* Context;
};


