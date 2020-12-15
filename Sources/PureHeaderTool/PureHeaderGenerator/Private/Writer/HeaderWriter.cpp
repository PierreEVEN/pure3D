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
	size_t FileUniqueID = Utils::GenURID();

	Result.Line(Data->GetFile().GetDateFormated());
	Result.Br();
	Result.Line("/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/");
	Result.Br();
	Result.UnDef("_REFL_FILE_UNIQUE_ID_");
	Result.Define("_REFL_FILE_UNIQUE_ID_ RUID_" + String(FileUniqueID));
	Result.Br(2);
	Result.IfNDef(PragmaDefinition);
	Result.Define(PragmaDefinition);
	Result.Br();
	Result.Include("Reflection/ReflectionMacros.h");

	for (const auto& Object : Data->GetObjects()) {

		Result.Br(2);

		Result.Line("/* ##############################  Reflection for " + Object->GetName() + "  ############################## */");

		Result.Indent();
		Result.Br(1);
		Result.ForwardDeclaration(Object);

		if (Object->GetType() == Parser::EObjectType::ObjType_Struct || Object->GetType() == Parser::EObjectType::ObjType_Class) {

			Result.Define("_REFLECTION_BODY_RUID_" + String(FileUniqueID) + "_LINE_" + String(((Parser::SStruct*)Object)->GetReflectionBodyLine()), "REFL_DECLARE_CLASS(" + Object->GetName() + ")");

			Result.DeclareTypeName(Object->GetName());
		}
		Result.UnIndent();
	}

	Result.EndIf(PragmaDefinition);

	return Result.GetData();
}
