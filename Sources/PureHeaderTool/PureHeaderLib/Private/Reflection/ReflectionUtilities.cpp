#include "Reflection/ReflectionUtilities.h"
#include <iostream>

void ReflEnsure(bool inCondition, const String& inMessage)
{
	if (!inCondition) {
		std::cerr << "PHT_ERROR : " << inMessage.GetData() << std::endl;
		exit(EXIT_FAILURE);
	}
}