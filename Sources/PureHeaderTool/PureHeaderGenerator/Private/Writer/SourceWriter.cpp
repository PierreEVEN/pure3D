#include "Writer/SourceWriter.h"
#include "Parser/SFileData.h"
#include "Writer/CodeGenerator.h"
#include "Parser/STypes.h"
#include "Parser/SObject.h"

using namespace Writer;

String Writer::GenerateSource(Parser::SFileData* Data, const String& ReflHeaderPath)
{
	SCodeGenerator Result;

	String HeaderPath = Data->GetFile().GetFilePath().u8string().c_str();
	for (auto& Chr : HeaderPath) if (Chr == '\\') Chr = '/';

	Result.Line("/// VERSION : " + Data->GetFile().GetDateFormated());
	Result.Br();
	Result.Include(ReflHeaderPath);
	Result.Include(HeaderPath);

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

			Result.Br();
			Result.Line("void " + BuilderFunction + "() {", "Builder function");
			Result.Indent();
			Result.Line("REFL_REGISTER_CLASS(" + Object->GetName() + ");");
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

