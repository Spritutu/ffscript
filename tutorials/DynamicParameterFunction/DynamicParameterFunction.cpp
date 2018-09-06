/******************************************************************
* File:        DynamicParameterFunction.cpp
* Description: Contains code for compiling, running C Lambda
*              scripting language. This application demonstrate
*              registering a dynamic function in C++ side and using
*              it in script side.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "../TutorialCommon.h"
#include <DynamicFunctionFactory.h>

// native function that is real implementation of 'println' in the script

void println(const RawString& s) {
	wcout << s.elms << endl;
}

double sum(SimpleVariantArray* params) {
	double sum = 0;
	for (int i = 0; i < params->size; i++) {
		auto& param = params->elems[i];
		if (strcmp(param.typeName, "int") == 0) {
			sum += (double)*(int*)param.pData;
		}
		else if (strcmp(param.typeName, "float") == 0) {
			sum += (double)*(float*)param.pData;
		}
		else if (strcmp(param.typeName, "double") == 0) {
			sum += (double)*(double*)param.pData;
		}
		else if (strcmp(param.typeName, "long") == 0) {
			sum += (double)*(long long*)param.pData;
		}
		else {
			throw std::runtime_error("argument must be a number");
		}
	}
	return sum;
}

void importApplicationLibrary(ScriptCompiler* scriptCompiler) {
	FunctionRegisterHelper fb(scriptCompiler);

	// register 'println' functions
	registerFunction
		<void, const RawString&> // native function prototype
		(fb, // register helper object
			println, // native function
			"println", //script function name
			"void", // return type of the script function
			"String&" // parameter type of the function
			);

	//register dynamic function
	registerDynamicFunction<double>(fb, sum, "sum", "double");
}

int main(int argc, char* argv[])
{
	auto program = compileProgram(importApplicationLibrary, "DynamicParameterFunction.c955");
	if (program) {
		// run the code that place in global scope
		program->runGlobalCode();

		// run main function of the script
		runProgram(program);

		// clean up global memory generated by runGlobalCode method
		program->cleanupGlobalMemory();

		delete program;
	}

	return 0;
}

