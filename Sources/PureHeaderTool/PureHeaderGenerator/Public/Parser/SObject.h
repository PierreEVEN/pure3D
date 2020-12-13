#pragma once
#include "Types/String.h"
#include "STypes.h"
#include "SStateMachine.h"

namespace Parser {

	struct SObject {
		SObject(const String& inObjectName, EObjectType inObjectType)
			: ObjectName(inObjectName), ObjectType(inObjectType) {}
		virtual void ParseContent(const String& Content) = 0;
		virtual void Log() = 0;
		const String& GetName() const { return ObjectName; }
		const EObjectType& GetType() const { return ObjectType; }
	private:
		String ObjectName;
		EObjectType ObjectType;
	};

	struct SEnum : public SObject {
		using SObject::SObject;
		void ParseContent(const String& Content);
		virtual void Log();
	private:
		std::vector<String> Fields;
	};


	struct SStruct : public SObject {
		using SObject::SObject;
		void ParseContent(const String& Content);
		virtual void Log();
	private:
		SFunctionData ParseFunction(SStateMachine Content);
		SPropertyData ParseProperty(SStateMachine Content);
		SPropertyData ParseVariable(SStateMachine Content);
		SFunctionData ParseConstructor(SStateMachine Content);

		std::vector<SPropertyData> Properties;
		std::vector<SFunctionData> Functions;
		std::vector<SFunctionData> Constructors;
	};

}