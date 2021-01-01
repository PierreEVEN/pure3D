#include "AssetFactory.h"
#include "Reflection/ReflectionUtilities.h"
#include "Asset.h"
#include "AssetManager.h"

inline static std::unordered_map<RClass*, SAssetFactories*> FactoryForClass;

SAssetFactories* SAssetFactories::FindFactory(RClass* InFactoryClass) {
	const auto& FoundFactory = FactoryForClass.find(InFactoryClass);
	if (FoundFactory != FactoryForClass.end()) return FoundFactory->second;
	return nullptr;
}

void SAssetFactories::PostLoadData_Internal(SAsset* InAsset, const String& InAssetPath) {
	/** Set asset internal data */
	InAsset->AssetPath = InAssetPath;
	InAsset->AssetID = MakeUniqueID(InAsset->GetAssetName());

	/** post load internal data */
	SAssetFactories* Factory = FindFactory(InAsset->GetClass());
	if (Factory) Factory->PostLoadData(InAsset);

	/** Register asset */
	AssetManager::Get()->RegisterAsset(InAsset);
}
