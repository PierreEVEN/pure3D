#include <IO/Log.h>
#include "Utils/Utils.h"
#include <iostream>


int main(int argc, const char* argv[]) {
	String ModulePath;
	String ModuleName;

	Utils::Ensure(Utils::GetOption(argc, argv, "ModuleName", ModuleName), "Missing 'ModuleName' option");

	Utils::Ensure(Utils::GetOption(argc, argv, "ModulePath", ModulePath), "Missing 'ModulePath' option");

	Utils::Log("Running reflection tool on " + ModuleName);
}