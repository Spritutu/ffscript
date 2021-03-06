/******************************************************************
* File:        Expression.cpp
* Description: implement Expression class. A class used to store the
*              the expression by hold the root of expression tree.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "Expression.h"
#include "ScriptCompiler.h"

namespace ffscript {

	Expression::Expression(ExecutableUnitRef rootUnit) :
		_rootUnit(rootUnit)
	{
	}


	Expression::~Expression()
	{
	}

	void Expression::setExpString(const std::wstring& exp) {
		_expressionString = exp;
	}

	const std::wstring& Expression::getExpString() {
		return _expressionString;
	}

	const ExecutableUnitRef& Expression::getRoot() const {
		return _rootUnit;
	}

	ExecutableUnitRef& Expression::getRoot() {
		return _rootUnit;
	}
}
