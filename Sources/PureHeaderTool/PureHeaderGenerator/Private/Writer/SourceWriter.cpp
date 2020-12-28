#include "Writer/SourceWriter.h"
#include "Parser/SFileData.h"
#include "Writer/CodeGenerator.h"
#include "Parser/STypes.h"
#include "Parser/SObject.h"
#include "Utils/Utils.h"

using namespace Writer;

String Writer::GenerateSource(Parser::SFileData* Data, const String& ReflHeaderPath)
{
	SCodeGenerator Result;

	String HeaderPath = Data->GetFile().GetFilePath().string().c_str();
	for (auto& Chr : HeaderPath) if (Chr == '\\') Chr = '/';

	Result.Line(Data->GetFile().GetDateFormated());
	Result.Br();
	Result.Include(ReflHeaderPath); 
	Result.Include(HeaderPath);
	Result.Include("Reflection/RProperty.h");
	Result.Include("Reflection/RFunction.h");
	Result.Include("Reflection/RArrayView.h");
	Result.Include("Serialization/GenericSerializers.h");

	Result.Br(2);
	Result.Line("/* ##############################  Dynamic type declarations  ############################## */");
	Result.Br(1);
	Result.Line("void _Dynamic_Type_Registerer_Function_" + Data->GetFile().GetName() + "() {");
	Result.Indent();
	for (const auto& Type : Data->GetDynamicTypes()) {
		if (Utils::IsTypeArray(Type)) {
			Result.Line("if (!RType::GetType(\"" + Type + "\")) {");
			Result.Indent();
			Result.Line("RType* NewArrayType = RArrayType::RegisterType<" + Type + ", " + Utils::GetTemplateInnerType(Type) + ">(\"" + Type + "\", \"" + Utils::GetTemplateInnerType(Type) + "\");");
			Result.Line("NewArrayType->SetSerializer(ISerializerInterface::Get<RSerializerInterface_Array>(\"RSerializerInterface_Array\"));");
			Result.UnIndent();
			Result.Line("}");
		}
		else {
			Result.Line("if (!RType::GetType(\"" + Type + "\")) RType::RegisterType<" + Type + ", RType>(\"" + Type + "\");");
		}
	}
	Result.UnIndent();
	Result.Line("};");
	Result.Line("struct _Dynamic_Type_Registerer_" + Data->GetFile().GetName() + " {");
	Result.Indent();
	Result.Line("_Dynamic_Type_Registerer_" + Data->GetFile().GetName() + "() { _Dynamic_Type_Registerer_Function_" + Data->GetFile().GetName() + "(); }");
	Result.UnIndent();
	Result.Line("};");
	Result.Line("_Dynamic_Type_Registerer_" + Data->GetFile().GetName() + " _Dynamic_Type_Registerer_Object_" + Data->GetFile().GetName() + ";");

	for (const auto& Object : Data->GetObjects()) {

		Result.Br(2);
		Result.Line("/* ##############################  Reflection for " + Object->GetName() + "  ############################## */");
		Result.Br(1);

		if (Object->GetType() == Parser::EObjectType::ObjType_Struct || Object->GetType() == Parser::EObjectType::ObjType_Class) {

			Result.Indent();

			String StaticClassName = "_Static_Item_Class_" + Object->GetName();
			String BuilderName = "_Static_Item_Builder_" + Object->GetName();
			String BuilderFunction = "_Refl_Register_Function_" + Object->GetName();

			Result.Line("RClass* " + StaticClassName + " = nullptr;", "static class reference");
			Result.Line("RClass* " + Object->GetName() + "::GetStaticClass() { return " + StaticClassName + "; }");
			Result.Line("RClass* " + Object->GetName() + "::GetClass() const { return " + StaticClassName + "; }");
			Result.Br();
			Result.Line("void " + BuilderFunction + "() {", "Builder function");
			Result.Indent();
			Result.Line(StaticClassName + " = REFL_REGISTER_CLASS(" + Object->GetName() + ");");
			
			for (const auto& Parent : ((Parser::SStruct*)Object)->GetParents()) {
				Result.Line(StaticClassName + "->AddParent(\"" + Parent + "\");");
				Result.Line(StaticClassName + "->AddCastFunction<" + Object->GetName() + ", " + Parent + ">();");
			}

			for (const auto& Property : ((Parser::SStruct*)Object)->GetProperties()) {
				Result.Line(StaticClassName + "->AddProperty(new RProperty(\"" + Property.PropertyType + "\", \"" + Property.PropertyName + "\", offsetof(" + Object->GetName() + ", " + Property.PropertyName + "), " + (Property.IsTransient ? "true" : "false") + "));");
			}
			for (const auto& Function : ((Parser::SStruct*)Object)->GetFunctions()) {
				String Params = Function.Parameters.size() == 0 ? "" : ", ";
				for (int i = 0; i < Function.Parameters.size(); ++i)
					Params << Function.Parameters[i].PropertyType << (i == Function.Parameters.size() - 1 ? "" : ", ");
				Result.Line(StaticClassName + "->AddFunction(RFunction::MakeFunction<" + Function.ReturnType + ", " + Object->GetName() + Params + ">(\"" + Function.FunctionName + "\", &" + Object->GetName() + "::" + Function.FunctionName + "));");
			}

			for (const auto& Constructor : ((Parser::SStruct*)Object)->GetConstructors()) {
				String Params = Constructor.Parameters.size() == 0 ? "" : ", ";
				for (int i = 0; i < Constructor.Parameters.size(); ++i)
					Params << Constructor.Parameters[i].PropertyType << (i == Constructor.Parameters.size() - 1 ? "" : ", ");
				Result.Line(StaticClassName + "->AddConstructor(RConstructor::MakeConstructor<" + Object->GetName() + Params + ">());");
			}

			Result.Line(StaticClassName + "->SetSerializer(ISerializerInterface::Get<RSerializerInterface_Object>(\"RSerializerInterface_Object\"));");

			Result.UnIndent();
			Result.Line("}");

			Result.Br();
			Result.Line("struct " + BuilderName + " {", "Builder for " + Object->GetName());
			Result.Indent();
			Result.Line(BuilderName + "() {");
			Result.Indent();
			Result.Line(BuilderFunction + "();");
			Result.UnIndent();
			Result.Line("}"); 
			Result.UnIndent();
			Result.Line("};");
			Result.Line(BuilderName + " " + BuilderName + "_Var;", " Register " + Object->GetName() + " on execution");

			Result.UnIndent();
		}
	}



	return Result.GetData();
}

