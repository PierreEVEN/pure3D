#pragma once

#include "Types/Transform.h"
#include "Scene/PrimitiveComponent.h"
#include "MeshComponent.refl.h"

class SRenderer;
struct SMeshProxy;
class IMesh;
class SMaterial;

REFLECT()
class SMeshComponent : public SPrimitiveComponent {

	REFLECT_BODY()
public:

	SMeshComponent(SRenderer* InContext, IMesh* InMesh, const std::vector<SMaterial*>& inMaterialOverrides);

	void SetMesh(IMesh* inMesh);

protected:

	virtual void UpdateProxies();

private:
	// Data
	IMesh* Mesh;
	std::vector<SMaterial*> MaterialOverrides;
};


