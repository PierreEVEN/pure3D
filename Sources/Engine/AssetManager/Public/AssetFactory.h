#pragma once
#include "Types/String.h"
#include <unordered_map>

#include "AssetFactory.refl.h"

struct RClass;
class SAsset;

REFLECT()
class SAssetFactory {
protected:
	~SAssetFactory() = default;
private:
REFLECT_BODY()
public:
	static SAssetFactory* GetFactory(RClass* InAssetClass);

	template<typename Factory, typename Asset>
	static void RegisterAssetFactory() {
		RegisterFactory_Internal(Asset::GetStaticClass(), new Factory());
	}
	
	static SAsset* DeserializeObject(std::istream InputStream);

	template<typename AssetType, typename ... Arguments>
	static SAsset* ImportFromFiles(Arguments ... Args) {
		if (SAssetFactory* Factory = GetFactory(AssetType::GetStaticClass())) {
			if (RFunction* Function = Factory->GetClass()->GetFunction("ImportFromFiles_Internal")) {
				return static_cast<AssetType*>(Function->Execute<SAsset*, Arguments...>(Factory, std::forward<Arguments>(Args)...));
			}
			LOG_WARNING("Factory for %s does not contains CreateFromData_Internal function", AssetType::GetStaticClass()->GetName().GetData());
		}
		LOG_WARNING("No asset factory found for %s", AssetType::GetStaticClass()->GetName().GetData());
		return nullptr;
	}
	
	template<typename AssetType, typename ... Arguments>
	static AssetType* CreateFromData(Arguments ... Args) {
		if (SAssetFactory* Factory = GetFactory(AssetType::GetStaticClass())) {
			if (RFunction* Function = Factory->GetClass()->GetFunction("CreateFromData_Internal")) {
				if (!Function->IsValid<SAsset*, Arguments...>())
				{
					LOG_WARNING("Wrong parameter for CreateFromData_Internal on %s's factory", AssetType::GetStaticClass()->GetName().GetData());
				}
				return static_cast<AssetType*>(Function->Execute<SAsset*, Arguments...>(Factory, std::forward<Arguments>(Args)...));
			}
			LOG_WARNING("Factory for %s does not contains CreateFromData_Internal function", AssetType::GetStaticClass()->GetName().GetData());
		}
		LOG_WARNING("No asset factory found for %s", AssetType::GetStaticClass()->GetName().GetData());
		return nullptr;
	}

private:

	static void RegisterFactory_Internal(RClass* inAssetClass, SAssetFactory* Factory);
};