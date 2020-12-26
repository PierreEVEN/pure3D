#pragma once
#include "Types/String.h"
#include <unordered_map>

struct RClass;
class SAsset;
class AssetManager;


class SAssetFactories {
public:
	friend AssetManager;
	static SAssetFactories* FindFactory(RClass* InFactoryClass);
protected:
	static void PostLoadData_Internal(SAsset* InAsset, const String& InAssetPath);
	virtual void PostLoadData(SAsset* InAsset) = 0;
};