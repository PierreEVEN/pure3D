#pragma once
#include "Types/String.h"

namespace Parser {
	struct SObject;
}


namespace Writer {

	struct SCodeGenerator {

		void Include(const String& file);
		void Line(const String& Content, const String& Comment = "");
		void Define(const String& Key, const String& Def = "", const String& Comment = "");
		void UnDef(const String& Def);
		void IfNDef(const String& Def);
		void EndIf(String Comment = "");
		void PragmaOnce(const String& FileName);
		void ForwardDeclaration(Parser::SObject* Object);
		void DeclareTypeName(const String& TypeName);
		void Br(int Number = 1);

		inline const String& GetData() const { return Data; }

		inline void Indent() { IndentLevel++; }
		inline void UnIndent() { if (IndentLevel > 0) IndentLevel--; }

	private:

		size_t IndentLevel = 0;

		String Data;
	};
}


