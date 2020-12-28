
#include "AssetManager.h"
#include "Assets/Asset.h"
#include "IO/Log.h"
#include <filesystem>
#include <fstream>
#include "Serialization/Serialization.h"
#include "AssetFactory.h"
#include "EngineModule.h"


SAsset* AssetManager::GetAsset(RUID AssetID) const {
	const auto& FoundElement = AssetRegistry.find(AssetID);
	if (FoundElement == AssetRegistry.end()) return nullptr;
	return FoundElement->second;
}

bool AssetManager::IsNewAssetNameValid(const String& NewName) {
	RUID ID = MakeUniqueID(NewName);
	if (!ID) return false;
	if (AssetRegistry.find(ID) != AssetRegistry.end()) return false;
	return true;
}

void AssetManager::RegisterAsset(SAsset* InAsset) {
	if (GetAsset(InAsset->GetAssetID())) LOG_ASSERT("Cannot register asset '%s' : Asset name is already used.", InAsset->GetAssetName().GetData());
	if (!IsNewAssetNameValid(InAsset->GetAssetName())) LOG_ASSERT("Cannot register asset '%s' : invalid name", InAsset->GetAssetName().GetData());
	AssetRegistry[InAsset->GetAssetID()] = InAsset;
}

AssetManager::~AssetManager() {
	for (const auto& Asset : AssetRegistry) {
		delete Asset.second;
	}
}

void AssetManager::LoadAssetLibrary(const std::filesystem::path& Path) {
	if (!std::filesystem::exists(Path)) return;
	for (const auto& File : std::filesystem::directory_iterator(Path)) {
		if (File.is_directory()) {
			LoadAssetLibrary(File);
		}
		else {
			LoadAsset(File);
		}
	}
}

void AssetManager::LoadAsset(const std::filesystem::path& AssetPath) {
	if (!std::filesystem::exists(AssetPath)) return;

	std::ifstream FileData(AssetPath, std::ios::binary);

	SArchive Archive;
	Archive.DeserializeAndCreateClass(FileData);

	for (const auto& Asset : Archive.GetObjects()) {
		SAssetFactories::PostLoadData_Internal((SAsset*)Asset.second.ObjectPtr, AssetPath.string().c_str());
	}

	FileData.close();
}

void AssetManager::UnRegisterAsset(SAsset* InAsset) {
	AssetRegistry.erase(InAsset->GetAssetID());
}


MODULE_CONSTRUCTOR(AssetManager) {

	/* Load content */
	//AssetManager::Get()->LoadAssetLibrary(DEFAULT_ASSET_PATH);

	/* Save all */
	for (const auto& AssetIt : AssetManager::Get()->GetAssets()) { AssetIt.second->Save(); }

	/* Create Asset */
	//SAssetFactories::FindFactory(STexture2D::GetStaticClass())->CreateFromFiles("Content/Test/Truc.png");

}

MODULE_DESTRUCTOR() {

}