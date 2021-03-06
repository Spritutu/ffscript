/******************************************************************
* File:        ScriptTypeUT.cpp
* Description: Test cases focus on checking usage of scripting types
*              in C Lambda scripting language.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/
#include "fftest.hpp"

#include <CompilerSuite.h>
#include <ScriptTask.h>
#include <Utils.h>

using namespace std;
using namespace ffscript;

namespace ffscriptUT
{	
	namespace ScriptTypeUT
	{
		FF_TEST_FUNCTION(ScriptTypeTest, RefFunctionUT1)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			auto pX = rootScope->registVariable("x");
			pX->setDataType(ScriptType(compiler.getTypeManager()->getBasicTypes().TYPE_INT,"int"));
			auto excutor = compiler.compileExpression(L"ref(x)");
			FF_EXPECT_NE(nullptr, excutor, L"compile expression with ref operator failed");

			int* addressX = (int*)rootScope->getGlobalAddress(pX->getOffset());

			excutor->runCode();
			void* res = excutor->getReturnData();
			int* iRes = *((int**)res);

			FF_EXPECT_EQ(addressX, iRes, L"ref function return wrong");
		}

		FF_TEST_FUNCTION(ScriptTypeTest, RefFunctionUT2)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			auto pX = rootScope->registVariable("x");
			pX->setDataType(ScriptType(compiler.getTypeManager()->getBasicTypes().TYPE_INT, "int"));
			auto excutor = compiler.compileExpression(L"a = ref x");
			FF_EXPECT_NE(nullptr, excutor, L"compile expression with ref operator failed");

			int* addressX = (int*)rootScope->getGlobalAddress(pX->getOffset());
			auto pA = rootScope->findVariable("a");

			size_t* addressA = (size_t*)rootScope->getGlobalAddress(pA->getOffset());

			excutor->runCode();
			void* res = *(void**)excutor->getReturnData();
			int* iRes = *((int**)res);

			FF_EXPECT_EQ(addressX, iRes, L"ref function return wrong");
			FF_EXPECT_EQ(*addressA, (size_t)iRes, L"ref function return wrong");
		}

		FF_TEST_FUNCTION(ScriptTypeTest, RefFunctionUT4)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			auto X = rootScope->registVariable("x");
			X->setDataType(ScriptType(compiler.getTypeManager()->getBasicTypes().TYPE_INT, "int"));
			auto excutor1 = compiler.compileExpression(L"a = ref x");
			FF_EXPECT_NE(nullptr, excutor1, L"compile expression with ref operator failed");
			auto excutor2 = compiler.compileExpression(L"b = ref a");
			FF_EXPECT_NE(nullptr, excutor2, L"compile expression with ref operator failed");

			int* pX = (int*)rootScope->getGlobalAddress(X->getOffset());
			*pX = 1;
			auto A = rootScope->findVariable("a");
			auto B = rootScope->findVariable("b");

			size_t* addressA = (size_t*)rootScope->getGlobalAddress(A->getOffset());
			size_t* addressB = (size_t*)rootScope->getGlobalAddress(B->getOffset());

			excutor1->runCode();			
			void* res = *((void**)excutor1->getReturnData());
			int* iRes = *((int**)res);

			FF_EXPECT_EQ(pX, iRes, L"ref function return wrong");
			FF_EXPECT_EQ(*addressA, (size_t)iRes, L"ref function return wrong");

			int* pA = (int*)*addressA;
			FF_EXPECT_EQ(*pX, *pA, L"value of A is wrong");

			excutor2->runCode();
			FF_EXPECT_EQ((size_t)addressA, *addressB, L"ref of ref gone wrong");
			int** pB = (int**)*addressB;
			FF_EXPECT_EQ(*pX, *(*pB), L"value of B is wrong");
		}

		FF_TEST_FUNCTION(ScriptTypeTest, RefFunctionUT5)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref ref int ppn, ref int pn) {"
				L"	*ppn = pn;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref ref int, ref int");
			FF_EXPECT_TRUE(functionId >= 0, L"can not find function 'foo'");

			int n = 1;
			int* pn;
			ScriptParamBuffer paramBuffer(&pn);
			paramBuffer.addParam(&n);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			FF_EXPECT_EQ(n, *pn, L"function 'foo' return wrong");
		}

		FF_TEST_FUNCTION(ScriptTypeTest, RefFunctionUT6)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"void foo(ref ref int ppa, ref ref int ppb) {"
				L"	ppa = ppb;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref ref int, ref ref int");
			FF_EXPECT_TRUE(functionId >= 0, L"can not find function 'foo'");

			int a = 1;
			int b = 2;
			int* pa = &a;
			int* pb = &b;
			
			ScriptParamBuffer paramBuffer(&pa);
			paramBuffer.addParam(&pb);

			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			FF_EXPECT_EQ(a, *pa, L"function 'foo' return wrong");
			FF_EXPECT_EQ(b, *pb, L"function 'foo' return wrong");
		}

		FF_TEST_FUNCTION(ScriptTypeTest, RefFunctionUT7)
		{
			CompilerSuite compiler;

			//the code does not contain any global scope'code and only a variable
			//so does not need global memory
			compiler.initialize(128);
			GlobalScopeRef rootScope = compiler.getGlobalScope();

			const wchar_t scriptCode[] =
				L"int foo(ref int pn) {"
				L"	return 2 * *pn;"
				L"}"
				;

			auto program = compiler.compileProgram(scriptCode, scriptCode + sizeof(scriptCode) / sizeof(scriptCode[0]) - 1);
			FF_EXPECT_NE(nullptr, program, L"Compile program failed");

			auto nativeCompiler = compiler.getCompiler();
			int functionId = nativeCompiler->findFunction("foo", "ref int");
			FF_EXPECT_TRUE(functionId >= 0, L"can not find function 'foo'");

			int n = 2;
			ScriptParamBuffer paramBuffer(&n);
			ScriptTask task(program);
			task.runFunction(functionId, &paramBuffer);

			int* res = (int*)task.getTaskResult();
			FF_EXPECT_EQ(4, *res, L"function 'foo' return wrong");
		}
	};
}