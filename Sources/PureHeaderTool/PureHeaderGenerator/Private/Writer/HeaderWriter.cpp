#include "Writer/HeaderWriter.h"

#include "Parser/SFileData.h"
#include "Writer/CodeGenerator.h"
#include "Parser/STypes.h"
#include "Parser/SObject.h"
#include "Utils/Utils.h"


using namespace Writer;

String Writer::GenerateHeader(Parser::SFileData* Data)
{
	SCodeGenerator Result;
	String PragmaDefinition = "_REFL_" + Data->GetFile().GetName() + "_H";

	Result.Line(Data->GetFile().GetDateFormated());
	Result.Br();
	Result.Line("/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/");
	Result.Br();
	Result.UnDef("_REFL_FILE_UNIQUE_ID_");
	Result.Define("_REFL_FILE_UNIQUE_ID_ RUID_" + String(Data->GetFileID()));
	Result.Br(2);
	Result.IfNDef(PragmaDefinition);
	Result.Define(PragmaDefinition);
	Result.Br();
	Result.Include("Reflection/ReflectionMacros.h");
	Result.Br(2);
	Result.Line("/* ##############################  Dynamic type declarations  ############################## */");
	Result.Br(1);
	Result.Indent();
	for (const auto& Type : Data->GetDynamicTypes()) {
		String TypeDefine = Type;
		TypeDefine = String::Replace(TypeDefine, ' ', '_');
		TypeDefine = "REFL_DECLARE_TYPENAME_" + TypeDefine;
		for (auto& chr : TypeDefine) if (chr == '<' || chr == '>' || chr == ':' || chr == ',') chr = '_';
		Result.IfNDef(TypeDefine);
		Result.Define(TypeDefine);
		Result.Indent();
		Result.Line("template<> struct RIsReflected<" + Type + "> { constexpr static bool Value = true; };");
		Result.Line("template<> struct RTypeName<" + Type + "> { constexpr static const char* Name = \"" + Type + "\"; };");
		Result.UnIndent();
		Result.EndIf(TypeDefine);
		Result.Br(1);
	}
	Result.UnIndent();

	for (const auto& Object : Data->GetObjects()) {

		Result.Br(2);

		Result.Line("/* ##############################  Reflection for " + Object->GetName() + "  ############################## */");

		Result.Indent();
		Result.Br(1);
		Result.ForwardDeclaration(Object);

		if (Object->GetType() == Parser::EObjectType::ObjType_Struct || Object->GetType() == Parser::EObjectType::ObjType_Class) {

			Result.Define("_REFLECTION_BODY_RUID_" + String(Data->GetFileID()) + "_LINE_" + String(((Parser::SStruct*)Object)->GetReflectionBodyLine()), "\\");
			Result.Line("REFL_DECLARE_CLASS(" + Object->GetName() + ") \\");

			for (const auto& Constructor : ((Parser::SStruct*)Object)->GetConstructors()) {
				String Params = Constructor.Parameters.size() == 0 ? "" : ", ";
				for (int i = 0; i < Constructor.Parameters.size(); ++i)
					Params << Constructor.Parameters[i].PropertyType << (i == Constructor.Parameters.size() - 1 ? "" : ", ");
				Result.Line("friend RConstructor* RConstructor::MakeConstructor<" + Object->GetName() + Params + ">(); \\");
			}
			Result.Br(1);

			Result.DeclareTypeName(Object->GetName());
		}
		Result.UnIndent();
	}

	Result.EndIf(PragmaDefinition);

	return Result.GetData();
}
