#include "Writer/CodeGenerator.h"
#include "Parser/SObject.h"

using namespace Writer;

void Writer::SCodeGenerator::Include(const String& file) {
	Line("#include \"" + file + "\"");
}

void Writer::SCodeGenerator::Line(const String& Content, const String& Comment) {
	for (size_t i = 0; i < IndentLevel; ++i) Data << '\t';
	Data << Content << (Comment == "" ? "" : " // " + Comment) + '\n';
}

void Writer::SCodeGenerator::PragmaOnce() {
	Line("#pragma once", "pragma once");
}

void Writer::SCodeGenerator::ForwardDeclaration(Parser::SObject* Object) {
	Line(Parser::ObjectTypeToString(Object->GetType()) + " " + Object->GetName() + ";", "forward declaration");
}

void Writer::SCodeGenerator::DeclareTypeName(const String& TypeName) {
	Line("REFL_DECLARE_TYPENAME(" + TypeName + ");", "declare type name for " + TypeName);
}

void Writer::SCodeGenerator::Br(int Number /*= 1*/) {
	for (int i = 0; i < Number; ++i) Data << '\n';
}