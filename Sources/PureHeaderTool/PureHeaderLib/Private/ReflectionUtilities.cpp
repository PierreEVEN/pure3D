#include "ReflectionUtilities.h"
#include <iostream>

void ReflEnsure(bool inCondition, const char* inMessage)
{
	if (!inCondition) {
		std::cerr << "PHT_ERROR : " << inMessage << std::endl;
		exit(EXIT_FAILURE);
	}
}