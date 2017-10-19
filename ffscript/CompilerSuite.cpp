#include "stdafx.h"
#include "CompilerSuite.h"
#include "ExpresionParser.h"

namespace ffscript{
	CompilerSuite::CompilerSuite()
	{
	}

	CompilerSuite::~CompilerSuite()
	{
	}

	void CompilerSuite::initialize(int globalMemSize) {
		_pCompiler = (ScriptCompilerRef)(new ScriptCompiler());
		_globalScopeRef = (GlobalScopeRef)(new GlobalScope(globalMemSize, _pCompiler.get()));

		FunctionRegisterHelper funcLibHelper(_pCompiler.get());
		
		auto& typeManager = _pCompiler->getTypeManager();

		typeManager->registerBasicTypes(_pCompiler.get());
		typeManager->registerBasicTypeCastFunctions(_pCompiler.get(), funcLibHelper);
		typeManager->registerConstants(_pCompiler.get());

		importBasicfunction(funcLibHelper);

		_pCompiler->beginUserLib();
	}

	Program* CompilerSuite::compileProgram(const wchar_t* codeStart, const wchar_t* codeEnd) {
		_pCompiler->clearUserLib();

		Program* program = new Program();
		_pCompiler->bindProgram(program);

		if (_globalScopeRef->parse(codeStart, codeEnd) == nullptr) {
			return nullptr;
		}

		if (_globalScopeRef->correctAndOptimize(program) != 0) {
			return nullptr;
		}

		if (_globalScopeRef->extractCode(program) == false) {
			delete program;
			return nullptr;
		}

		return program;
	}

	ExpUnitExecutor* CompilerSuite::compileExpression(const wchar_t* expression) {
		ExpressionParser parser(_pCompiler.get());
		_pCompiler->pushScope(_globalScopeRef.get());

		list<ExpUnitRef> units;
		EExpressionResult eResult = parser.stringToExpList(expression, units);
		if (eResult != E_SUCCESS) return nullptr;

		list<ExpressionRef> expList;
		bool res = parser.compile(units, expList);
		if (res == false) return nullptr;

		Expression* expressionPtr = expList.front().get();
		eResult = parser.link(expressionPtr);
		if (eResult != E_SUCCESS) return nullptr;

		//all variable in the scope will be place at right offset by bellow command
		//if this function is not execute before extract the code then all variable
		//will be placed at offset 0
		_globalScopeRef->updateVariableOffset();

		ExpUnitExecutor* pExcutor = new ExpUnitExecutor(_globalScopeRef.get());
		pExcutor->extractCode(_pCompiler.get(), expressionPtr);
		return pExcutor;
	}

	const GlobalScopeRef& CompilerSuite::getGlobalScope() const {
		return _globalScopeRef;
	}

	const TypeManagerRef& CompilerSuite::getTypeManager() const {
		return _pCompiler->getTypeManager();
	}

	ScriptCompilerRef& CompilerSuite::getCompiler() {
		return _pCompiler;
	}
}