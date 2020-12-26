#pragma once

#include "Types/ByteArray.h"
#include "Assets/Asset.refl.h"

class SAssetFactories;

REFLECT()
class SAsset {

	REFLECT_BODY()

public:

	friend SAssetFactories;

	inline RUID GetAssetID() const { return AssetID; }
	inline const String& GetAssetName() const { return AssetName; }
	
	void Save();

protected:

	RCONSTRUCTOR()
		SAsset() = default;

private:

	RPROPERTY()
		String AssetName;

	RPROPERTY()
		String AssetPath;

	RPROPERTY(Transient)
		RUID AssetID;

	RPROPERTY(Transient)
		String ImportPath;

	void Serialize(std::ostream& Output);
};