
#include "Primitives/PrimitiveTypes.h"
#include "Serialization/GenericSerializers.h"
#include <vector>

#define SET_TYPE_SERIALIZER(TYPE, SERIALIZER) RType::GetType<TYPE>()->SetSerializer(ISerializerInterface::Get<SERIALIZER>(#SERIALIZER))
#define ADD_PRIMITIVE_TYPE_SERIALIZER(TYPE) SET_TYPE_SERIALIZER(TYPE, RSerializerInterface_PrimitiveTypes)

#define REGISTER_PRIMITIVE_TYPE(TYPE) \
	REFL_REGISTER_TYPE(TYPE); \
	ADD_PRIMITIVE_TYPE_SERIALIZER(TYPE);


#define REFL_TRY_REGISTER_TYPE(Type) \
	if (!RType::GetType(#Type)) REFL_REGISTER_TYPE(Type);


void RegisterPrimitiveTypes() {
	REGISTER_PRIMITIVE_TYPE(bool);
	REGISTER_PRIMITIVE_TYPE(int8_t);
	REGISTER_PRIMITIVE_TYPE(int16_t);
	REGISTER_PRIMITIVE_TYPE(int32_t);
	REGISTER_PRIMITIVE_TYPE(int64_t);
	REGISTER_PRIMITIVE_TYPE(uint8_t);
	REGISTER_PRIMITIVE_TYPE(uint16_t);
	REGISTER_PRIMITIVE_TYPE(uint32_t);
	REGISTER_PRIMITIVE_TYPE(uint64_t);
	REGISTER_PRIMITIVE_TYPE(float);
	REGISTER_PRIMITIVE_TYPE(double);
	REFL_REGISTER_TYPE(String);
	REFL_TRY_REGISTER_TYPE(std::vector<double>);
	REFL_TRY_REGISTER_TYPE(std::vector<double>);

	SET_TYPE_SERIALIZER(String, RSerializerInterface_String);
}

PrimitiveTypeRegisterer::PrimitiveTypeRegisterer() {
	RegisterPrimitiveTypes();
}
inline static PrimitiveTypeRegisterer _PrimitiveTypeRegisterer;