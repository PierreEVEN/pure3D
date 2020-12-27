#include "Assetmanager.h"
#include "AssetReference.h"
#include "IO/Log.h"
#include "Assets/Asset.h"
#include "Primitives/PrimitiveTypes.h"
#include "Texture.h"
#include "AssetFactory.h"
#include <tuple>
#include "Reflection/RArrayView.h"

int main() {

	std::vector<int> a = { 1, 2, 3, 4, 5 };
	RArrayView a_view(&a, &TArrayViewPolicyStdVector<int>::Get);
	size_t c = a_view.GetLength();

	int value = 4;

	a_view.Insert(1, &value);
	for (int i = 0; i < a.size(); ++i) {
		LOG(String(i) + String(" : ") + *(int*)a_view.GetValuePtr(i));
	}


	/* Load content */
	AssetManager::Get()->LoadAssetLibrary(DEFAULT_ASSET_PATH);

	/* Create Asset */
	//SAssetFactories::FindFactory(STexture2D::GetStaticClass())->CreateFromFiles("Content/Test/Truc.png");


	IFunctionPointer* Function = STexture2D::GetStaticClass()->GetFunction("GetFactory");



	/* Save all */
	for (const auto& AssetIt : AssetManager::Get()->GetAssets()) { AssetIt.second->Save(); }
}