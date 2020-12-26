#include "Assets/Asset.h"
#include "Assetmanager.h"
#include "Serialization/Serialization.h"
#include <fstream>
#include "IO/Log.h"
#include <filesystem>

void SAsset::Save() {

	String Path = String::GetFilePath(AssetPath);

	std::filesystem::create_directories(Path.GetData());

	std::ofstream OutputFile((Path / AssetName + ".asset").GetData(), std::ios::binary);
	Serialize(OutputFile);
	OutputFile.close();
}

void SAsset::Serialize(std::ostream& Output) {
	SArchive Archive;
	Archive.LinkObject("Asset", GetClass(), this);
	Archive.Serialize(Output);
}

