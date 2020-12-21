#include "Reflection/ReflectionUtilities.h"
#include <iostream>
#include "IO/Log.h"

void ReflEnsure(bool inCondition, const String& inMessage)
{
	if (!inCondition) {
		LOG_ASSERT("PHT_ERROR : " + inMessage);
	}
}