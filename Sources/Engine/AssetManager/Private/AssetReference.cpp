
#include "AssetReference.h"
#include "Assetmanager.h"
#include "Assets/Asset.h"

SAssetReference::SAssetReference(RUID InAssetID)
	: AssetID(InAssetID), AssetPtr(nullptr) {
	AssetManager::Get()->OnUpdateAssetID.Add(this, &SAssetReference::OnUpdateAssetID);
}

void SAssetReference::SetAsset(SAsset* InAsset) {
	AssetPtr = InAsset;
	AssetID = InAsset ? InAsset->GetAssetID() : 0;
}

void SAssetReference::OnUpdateAssetID(RUID OldId, RUID NewId) {
	if (OldId == AssetID) AssetID = NewId;
}

void SAssetReference::SafeLoadAsset()
{
	AssetPtr = AssetManager::Get()->GetAsset(AssetID);
}
