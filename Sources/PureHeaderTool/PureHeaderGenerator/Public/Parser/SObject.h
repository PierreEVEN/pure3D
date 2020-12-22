#pragma once
#include "Types/String.h"
#include "STypes.h"
#include "SStateMachine.h"
#include <unordered_map>

namespace Parser {

	struct SObject;

	inline std::unordered_map<String, SObject*> ReflectedObjects;

	struct SObject {
		SObject(const String& inObjectName, EObjectType inObjectType)
			: ObjectName(inObjectName), ObjectType(inObjectType) {
			ReflectedObjects[inObjectName] = this;
		}
		virtual void ParseContent(const String& Content, size_t LineIndex) = 0;
		virtual void Log() = 0;
		const String& GetName() const { return ObjectName; }
		const EObjectType& GetType() const { return ObjectType; }
	private:
		String ObjectName;
		EObjectType ObjectType;
	};

	struct SEnum : public SObject {
		using SObject::SObject;
		void ParseContent(const String& Content, size_t LineIndex);
		virtual void Log();
	private:
		std::vector<String> Fields;
	};


	struct SStruct : public SObject {
		using SObject::SObject;
		void ParseContent(const String& Content, size_t LineIndex);
		virtual void Log();
		const size_t& GetReflectionBodyLine() const { return ReflectionBodyLine; }
		const std::vector<SPropertyData>& GetProperties() const { return Properties; }
		const std::vector<SFunctionData>& GetFunctions() const { return Functions; }
		const std::vector<SFunctionData>& GetConstructors() const { return Constructors; }
		const std::vector<String>& GetParents() const { return Parents; }
		const std::vector<String> GetLocalTypesDeclarations() const { return LocalTypeDeclarations; }

	private:
		SFunctionData ParseFunction(SStateMachine Content);
		SPropertyData ParseProperty(SStateMachine Content);
		SPropertyData ParseVariable(SStateMachine Content);
		SFunctionData ParseConstructor(SStateMachine Content);

		size_t ReflectionBodyLine = 0;
		std::vector<String> Parents;
		std::vector<SPropertyData> Properties;
		std::vector<SFunctionData> Functions;
		std::vector<SFunctionData> Constructors;
		std::vector<String> LocalTypeDeclarations;
	};
}