#pragma once
#include "Reflection/ReflectionUtilities.h"

class SAsset;

struct SAssetReference {
	SAssetReference(RUID InAssetID);

	inline SAsset* GetAssetPtr() { SafeLoadAsset(); return AssetPtr; }
	void SetAsset(SAsset* InAsset);

private:

	void OnUpdateAssetID(RUID OldId, RUID NewId);

	void SafeLoadAsset();
	SAsset* AssetPtr;
	RUID AssetID;
};

template<typename Type = SAsset>
struct TAssetReference : public SAssetReference {
	using SAssetReference::SAssetReference;

	Type* GetAsset() { return static_cast<Type*>(GetAssetPtr()); }
};