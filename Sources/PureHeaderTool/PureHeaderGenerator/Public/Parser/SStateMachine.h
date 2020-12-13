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

		bool DoesSkipChar(const char* Position);
	};
}