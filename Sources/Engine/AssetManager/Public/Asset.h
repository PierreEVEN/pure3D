#pragma once

#include "Types/ByteArray.h"
#include "Asset.refl.h"

class SAssetFactory;

REFLECT()
class SAsset {

	REFLECT_BODY()

public:

	friend SAssetFactory;

	[[nodiscard]] RUID GetAssetId() const { return AssetID; }
	[[nodiscard]] const String& GetAssetName() const { return AssetName; }
	
	void Save();

	virtual void Reload() = 0;

protected:

	virtual void PostLoad() {}

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