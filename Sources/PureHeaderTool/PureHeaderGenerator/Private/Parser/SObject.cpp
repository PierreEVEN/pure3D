#include "Parser/SObject.h"
#include "Utils/Utils.h"
#include <iostream>

using namespace Parser;

/************************************************************************/
/* SEnum                                                                */
/************************************************************************/

void Parser::SEnum::ParseContent(const String& Content, size_t LineIndex) {
	SStateMachine Status;

	for (Status.CurrentPos = 0; Status.CurrentPos < Content.Length(); ++Status.CurrentPos) {
		const char* CurrentData = &Content.GetData()[Status.CurrentPos];
		//Wait for structure beginning
		if (!Status.IsReadingStructure) {
			if (CurrentData[0] == '{') Status.IsReadingStructure = true;
			continue;
		}
		//Remove space/tabs...
		if (Utils::IsVoidChar(CurrentData[0]) || CurrentData[0] == '}') continue;

		//Next field
		if (CurrentData[0] == ',') {
			Fields.push_back(Status.Content);
			Status.Content = "";
			Status.PauseCapture = false;
			continue;
		}
		//Skip assigned value
		if (CurrentData[0] == '=') Status.PauseCapture = true;

		if (Status.PauseCapture) continue;

		Status.Content << CurrentData[0];
	}
	if (Status.Content != "") Fields.push_back(Status.Content);
}

void Parser::SEnum::Log() {
	Utils::Log("\t\t\t- " + ObjectTypeToString(GetType()) + " " + GetName() + " : " + String(Fields.size()) + " fields : { " + String::ConcatenateArray(Fields) + " }");
}


/************************************************************************/
/* SStruct                                                              */
/************************************************************************/

void Parser::SStruct::ParseContent(const String& Content, size_t LineIndex) {
	SStateMachine Status;
	Status.Content = Content;

	for (Status.CurrentPos = 0; Status.CurrentPos < Status.Content.Length(); ++Status.CurrentPos) {
		const char* CurrentData = &Status.Content.GetData()[Status.CurrentPos];

		Status.CountLine(CurrentData[0]);

		//Wait for structure beginning
		if (!Status.IsReadingStructure) {
			if (CurrentData[0] == '{') Status.IsReadingStructure = true;
			continue;
		}
		if (Utils::IsStartingWith(CurrentData, "REFLECT_BODY()")) ReflectionBodyLine = Status.CurrentLine + LineIndex;
		if (Utils::IsStartingWith(CurrentData, "RPROPERTY(")) Properties.push_back(ParseProperty(Status));
		else if (Utils::IsStartingWith(CurrentData, "RFUNCTION(")) Functions.push_back(ParseFunction(Status));
		else if (Utils::IsStartingWith(CurrentData, "RCONSTRUCTOR(")) Constructors.push_back(ParseConstructor(Status));
	}
}


void Parser::SStruct::Log() {
	Utils::Log("\t\t\t- " + ObjectTypeToString(GetType()) + " " + GetName() + " : " + String(Properties.size()) + " properties, " + String(Functions.size()) + " functions, " + String(Constructors.size()) + " constructors.");

	Utils::Log("\t\t\t\t+ Constructors : ");
	for (const auto& Constructor : Constructors) {
		String Parameters = "";
		for (int i = 0; i < Constructor.Parameters.size(); ++i) Parameters << Constructor.Parameters[i].PropertyType << " " << Constructor.Parameters[i].PropertyName << (i == Constructor.Parameters.size() - 1 ? "" : ", ");
		Utils::Log("\t\t\t\t\t=> " + Constructor.ReturnType + " " + Constructor.FunctionName + '(' + Parameters + ");");
	}
	Utils::Log("\t\t\t\t+ Properties : ");
	for (const auto& Property : Properties) {
		Utils::Log("\t\t\t\t\t=> " + Property.PropertyType + " " + Property.PropertyName + ';');
	}
	Utils::Log("\t\t\t\t+ Functions : ");
	for (const auto& Function : Functions) {
		String Parameters = "";
		for (int i = 0; i < Function.Parameters.size(); ++i) Parameters << Function.Parameters[i].PropertyType << " " << Function.Parameters[i].PropertyName << (i == Function.Parameters.size() - 1 ? "" : ", ");
		Utils::Log("\t\t\t\t\t=> " + Function.ReturnType + " " + Function.FunctionName + '(' + Parameters + ");");
	}
}

SFunctionData Parser::SStruct::ParseFunction(SStateMachine Content) {
	SStateMachine Status;
	String ReturnType;
	String Name;
	bool IsWaitingNextField = false;
	std::vector<SPropertyData> Properties;
	int64_t TempIndentationLevel = 0;
	SPropertyData CurrentProperty;

	//Skip reflection macro definition
	for (; Content.CurrentPos < Content.Content.Length(); ++Content.CurrentPos) {
		const char* CurrentData = &Content.Content.GetData()[Content.CurrentPos];

		if (Status.IsReadingStructure && Status.IndentationLevel == 0) break;
		if (CurrentData[0] == '(') {
			Status.IsReadingStructure = true;
			Status.IndentationLevel++;
		}
		else if (CurrentData[0] == ')') {
			Status.IndentationLevel--;
		}
	}
	Status.IsReadingStructure = false;

	for (; Content.CurrentPos < Content.Content.Length(); ++Content.CurrentPos) {
		const char* CurrentData = &Content.Content.GetData()[Content.CurrentPos];

		//Handle templates <>
		if (CurrentData[0] == '(') {
			if (Status.IndentationLevel == 0) IsWaitingNextField = true;
			Status.IndentationLevel++;
			if (!Status.IsReadingStructure) {
				Status.IsReadingStructure = true;
				ReturnType << Name;
				Name = Status.Content;
				Status.Content = "";
			}
			continue;
		}
		else if (CurrentData[0] == ')') {
			Status.IndentationLevel--;
			if (Status.IsReadingStructure) break;
		}

		if (Status.IsReadingStructure) {
			if (CurrentData[0] == '<') {
				TempIndentationLevel++;
			}
			else if (CurrentData[0] == '>') {
				TempIndentationLevel--;
			}			
		}
		else {
			if (Utils::IsStartingWith(CurrentData, "virtual")) {
				Content.CurrentPos += String("virtual").Length();
				continue;
			}
		}


		if (Utils::IsVoidChar(CurrentData[0])) {
			IsWaitingNextField = true;
		}
		else if (IsWaitingNextField) {
			IsWaitingNextField = false;
			//Handle properties before '('
			if (!Status.IsReadingStructure) {
				ReturnType << Name;
				Name = Status.Content;
				Status.Content = "";
			}
		}

		if (Status.IsReadingStructure) {
			if (CurrentData[0] == ',') {
				if (Status.Content != "") {
					Properties.push_back(ParseVariable(Status));
					Status.Content = "";
				}
				continue;
			}
		}


		Status.Content << CurrentData[0];
	}
	if (Status.Content != "") {
		Properties.push_back(ParseVariable(Status));
	}

	SFunctionData Result;
	Result.ReturnType = String::RemoveBorderSpaces(ReturnType);
	Result.FunctionName = String::RemoveBorderSpaces(Name);
	Result.Parameters = Properties;
	return Result;
}

SPropertyData Parser::SStruct::ParseProperty(SStateMachine Content) {
	SStateMachine Status;
	String Type;

	//Skip reflection macro definition
	for (; Content.CurrentPos < Content.Content.Length(); ++Content.CurrentPos) {
		const char* CurrentData = &Content.Content.GetData()[Content.CurrentPos];

		if (Status.IsReadingStructure && Status.IndentationLevel == 0) break;
		if (CurrentData[0] == '(') {
			Status.IsReadingStructure = true;
			Status.IndentationLevel++;
		}
		else if (CurrentData[0] == ')') {
			Status.IndentationLevel--;
		}
	}
	return ParseVariable(Content);
}

Parser::SPropertyData Parser::SStruct::ParseVariable(SStateMachine Content) {
	SStateMachine Status;
	String Type;

	for (; Content.CurrentPos < Content.Content.Length(); ++Content.CurrentPos) {
		const char* CurrentData = &Content.Content.GetData()[Content.CurrentPos];

		//Handle templates <>
		if (CurrentData[0] == '<')
			Status.IndentationLevel++;
		else if (CurrentData[0] == '>')
			Status.IndentationLevel--;

		//Stop when encountering =,;
		if (Status.IndentationLevel == 0 && (CurrentData[0] == '=' || CurrentData[0] == ';' || CurrentData[0] == ',')) break;

		if (Utils::IsVoidChar(CurrentData[0])) {
			Status.PauseCapture = true;
		}
		else if (Status.PauseCapture && !Utils::IsVoidChar(CurrentData[0])) {
			Status.PauseCapture = false;
			Type << Status.Content;
			Status.Content = "";
		}
		Status.Content << CurrentData[0];
	}

	SPropertyData Result;
	Result.PropertyName = String::RemoveBorderSpaces(Status.Content);
	Result.PropertyType = String::RemoveBorderSpaces(Type);
	return Result;
}

SFunctionData Parser::SStruct::ParseConstructor(SStateMachine Content) {
	return ParseFunction(Content);
}
