/******************************************************************
* File:        HelloWorld.cpp
* Description: Contains code for compiling, running C Lambda
*              scripting language.
*              See file HelloWorld.c955 to understand
*              what the script does.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
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

	// register 'getLine' function
	registerFunction
		<RawString> // native function prototype
		(fb, // register helper object
			getLine, // native function
			"getLine", //script function name
			"String", // return type of the script function
			"" // parameter type of the function
			);
}

int main(int argc, char* argv[])
{
	auto program = compileProgram(importApplicationLibrary, "HelloWorld.c955");
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

