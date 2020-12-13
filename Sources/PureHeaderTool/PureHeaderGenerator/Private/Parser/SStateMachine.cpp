#include "Parser/SStateMachine.h"

using namespace Parser;

bool Parser::SStateMachine::DoesSkipChar(const char* Position) {
	if (Position[1] == '\0') return false;

	//Detect multiline comment only outside strings
	if (!IsInString && !IsInOneLineComment && !IsInMultiLineLineComment) {
		if (Position[0] == '/' && Position[1] == '*') {
			IsInMultiLineLineComment = true;
			return true;
		}
	}

	//Detect multiline comment ending
	if (IsInMultiLineLineComment) {
		if (Position[0] == '/' && Position[-1] == '*') {
			IsInMultiLineLineComment = false;
			return true;
		}
	}

	//Skip multiline comment content
	if (IsInMultiLineLineComment) return true;

	//Detect String outside comments
	if (Position[0] == '"' && Position[-1] != '\\') {
		IsInString = !IsInString;
		return true;
	}

	//Skip Strings
	if (IsInString) return true;

	//Detect OneLine comments
	if (!IsInOneLineComment && Position[0] == '/' && Position[1] == '/') {
		IsInOneLineComment = true;
	}

	//Skip OneLine comments
	if (IsInOneLineComment) {
		if (Position[0] == '\n') IsInOneLineComment = false;
		return true;
	}
	return false;
}
