#include "Writer/HeaderWriter.h"

#include "Parser/SFileData.h"
#include "Writer/CodeGenerator.h"
#include "Parser/STypes.h"
#include "Parser/SObject.h"


using namespace Writer;

String Writer::GenerateHeader(Parser::SFileData* Data)
{
	SCodeGenerator Result;


	Result.Line(Data->GetFile().GetDateFormated());
	Result.Br();
	Result.PragmaOnce();
	Result.Br();
	Result.Include("Reflection/ReflectionMacros.h");

	for (const auto& Object : Data->GetObjects()) {

		Result.Br(2);

		Result.Line("/* ##############################  Reflection for " + Object->GetName() + "  ############################## */");

		Result.Indent();
		Result.Br(1);
		Result.ForwardDeclaration(Object);

		if (Object->GetType() == Parser::EObjectType::ObjType_Struct || Object->GetType() == Parser::EObjectType::ObjType_Class) {
			Result.DeclareTypeName(Object->GetName());
		}
		Result.UnIndent();
	}
	return Result.GetData();
}
