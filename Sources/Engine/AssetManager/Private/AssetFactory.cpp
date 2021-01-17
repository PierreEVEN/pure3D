#include "AssetFactory.h"
#include "Reflection/ReflectionUtilities.h"
#include "Asset.h"
#include "AssetManager.h"
#include "Serialization/Serialization.h"

inline static std::unordered_map<RClass*, SAssetFactory*> FACTORY_FOR_CLASS;

SAssetFactory* SAssetFactory::GetFactory(RClass* InAssetClass) {
	const auto& FoundFactory = FACTORY_FOR_CLASS.find(InAssetClass);
	if (FoundFactory != FACTORY_FOR_CLASS.end()) return FoundFactory->second;
	return nullptr;
}

SAsset* SAssetFactory::DeserializeObject(std::istream InputStream) {
	SArchive ObjectArchive;
	ObjectArchive.DeserializeAndCreateClass(InputStream);

	if (ObjectArchive.GetObjects().size() != 1) {
		LOG_ERROR("failed to deserialize asset");
		return nullptr;
	}
	SAsset* Asset = static_cast<SAsset*>(ObjectArchive.GetObjects()[0].ObjectPtr);
	Asset->PostLoad();
	
	return Asset;
}

void SAssetFactory::RegisterFactory_Internal(RClass* inAssetClass, SAssetFactory* Factory) {
	FACTORY_FOR_CLASS[inAssetClass] = Factory;
}
