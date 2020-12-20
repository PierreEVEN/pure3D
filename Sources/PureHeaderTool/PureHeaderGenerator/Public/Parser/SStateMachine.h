#pragma once
#include "Types/String.h"

namespace Parser {

	struct SStateMachine {
		size_t CurrentPos = 0;
		bool IsReadingStructure = false;
		bool IsInString = false;
		bool IsInOneLineComment = false;
		bool IsInMultiLineLineComment = false;
		bool PauseCapture = false;
		size_t IndentationLevel = 0;
		String Content = "";
		size_t CurrentLine = 0;

		bool DoesSkipChar(const char* Position);
		bool SkipString(const char* Content, const String& inPattern);
		void CountLine(const char CurrentChar);
	};
}