/******************************************************************
* File:        DataExchange.cpp
* Description: Contains code for compiling, running C Lambda
*              scripting language.
*              See file DataExchange.c955 to understand
*              what the script does.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "../TutorialCommon.h"

// native function that is real implementation of 'println' in the script
void println(const RawString& rs) {
	wcout << rs.elms << endl;
}

void println(const std::string& s) {
	cout << s << endl;
}

void println(const std::wstring& s) {
	wcout << s << endl;
}


RawString getLine() {
	std::wstring wline;
	wcin >> wline;

	RawString rw;
	constantConstructor(rw, wline);

	return rw;
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


	registerFunction
		<void, const std::string&> // native function prototype
		(fb, // register helper object
			println, // native function
			"println", //script function name
			"void", // return type of the script function
			"string&" // parameter type of the function
			);


	registerFunction
		<void, const std::wstring&> // native function prototype
		(fb, // register helper object
			println, // native function
			"println", //script function name
			"void", // return type of the script function
			"wstring&" // parameter type of the function
			);
}

template <typename T>
void setGlobalVariable(CLamdaProg* program, const char* variableName, const T& val) {
	Variable* variable = program->findDeclaredVariable(variableName);
	if (!variable) {
		cout << "Cannot find variable '" << variableName << "' in global scope of the script program." << endl;
		return;
	}

	auto& globalContext = program->getGlobalContext();
	T* pVal = (T*)globalContext->getAbsoluteAddress(variable->getOffset());
	*pVal = val;
}

#pragma pack(push)
#pragma pack(1)

// the layout of Point in C++ is must same as layout of Point in script
struct Point {
	float x;
	float y;
};

#pragma pack(pop)

int main(int argc, char* argv[])
{
	auto program = compileProgram(importApplicationLibrary, "DataExchange.c955");
	if (program) {
		// run the code that place in global scope
		program->runGlobalCode();

		Point p = { 0, 1 };
		setGlobalVariable(program, "p", p) ;
		setGlobalVariable(program, "val", (int)2);

		RawString rw;
		constantConstructor(rw, L"3");
		setGlobalVariable(program, "str", rw);
		
		// run main function of the script
		RawString* rawString = (RawString*) runProgram(program);
		if (rawString != nullptr) {
			wcout << endl << L"message from script: " << rawString->elms << endl << endl;

			// Since, the script function is called from external (C++ program)
			// So, c++ program have responsible to returned object that have constructor if it is longer used.
			freeRawString(*rawString);
		}
		else {
			cout << endl << "The script function does not have a valid return type" << endl << endl;
		}

		// clean up global memory generated by runGlobalCode method
		program->cleanupGlobalMemory();

		delete program;
	}

    return 0;
}

