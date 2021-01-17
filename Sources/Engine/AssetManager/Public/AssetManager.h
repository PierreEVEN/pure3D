#pragma once
#include "Reflection/ReflectionUtilities.h"
#include "Events/EventManager.h"
#include <unordered_map>
#include <filesystem>

class SAsset;

DECLARE_DELEGATE_MULTICAST(DOnRegisterAsset, RUID, SAsset*);
DECLARE_DELEGATE_MULTICAST(DOnUpdateAssetID, RUID, RUID);

#define DEFAULT_ASSET_PATH "Content/"


class AssetManager final {

public:
	
	SAsset* GetAsset(RUID AssetID) const;
	void LoadAssetLibrary(const std::filesystem::path& Path);

	void LoadAsset(const std::filesystem::path& AssetPath);

	inline static AssetManager* Get() {
		if (!Instance) Instance = new AssetManager();
		return Instance;
	}

	inline std::unordered_map<RUID, SAsset*> GetAssets() const { return AssetRegistry;  }

	DOnRegisterAsset OnRegisterAsset;
	DOnUpdateAssetID OnUpdateAssetID;

	bool RenameAsset(SAsset* InAsset, const String& NewName);
	bool IsNewAssetNameValid(const String& NewName);
private:
	AssetManager() = default;
	~AssetManager();
	inline static AssetManager* Instance;

	void RegisterAsset(SAsset* InAsset);
	void UnRegisterAsset(SAsset* InAsset);

	std::unordered_map<RUID, SAsset*> AssetRegistry;
};

