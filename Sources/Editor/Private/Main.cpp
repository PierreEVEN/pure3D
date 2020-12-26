#include "Assetmanager.h"
#include "AssetReference.h"
#include "IO/Log.h"
#include "Assets/Asset.h"
#include "Primitives/PrimitiveTypes.h"
#include "Texture.h"
#include "AssetFactory.h"

enum operations
{
	get_value,
	serialize,
	set_value
};

template<typename T, typename Policy>
class ArrayViewPolicyBase
{
	static std::any visit(operations op, void* container_ptr, std::any additional_data)
	{
		switch (op)
		{
		case operations::get_value:
			return Policy::get_value(container_ptr, additional_data);
		}
	}
};

template<typename T>
class ArrayViewPolicyStdVector : public ArrayViewPolicyBase<T, ArrayViewPolicyStdVector<T>>
{
	static T get_value(operations op, void* ptr, std::any additional_data)
	{
		return reinterpret_cast<std::vector<T>*>(ptr)->at(std::any_cast<size_t>(additional_data));
	}

};

using ArrayViewVisitFunc = std::any(*)(const operations&, void*, std::any);

class ArrayView
{
	ArrayView(void* in_container, ArrayViewVisitFunc in_func) {}

	template<typename T>
	T get_value(size_t index) { return std::any_cast<T>(func(operations::get_value, container, index)) };

	void* container;
	ArrayViewVisitFunc func;
};


int main() {



	std::vector<int> a = { 1, 2, 3, 4 };
	ArrayView a_view(&a, ArrayViewPolicyStdVector<int>::visit);
	int c = a_view.get_value(0);


	/* Load content */
	AssetManager::Get()->LoadAssetLibrary(DEFAULT_ASSET_PATH);

	/* Create Asset */
	//SAssetFactories::FindFactory(STexture2D::GetStaticClass())->CreateFromFiles("Content/Test/Truc.png");


	IFunctionPointer* Function = STexture2D::GetStaticClass()->GetFunction("GetFactory");



	/* Save all */
	for (const auto& AssetIt : AssetManager::Get()->GetAssets()) { AssetIt.second->Save(); }
}