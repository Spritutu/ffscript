/******************************************************************
* File:        DynamicFunction.cpp
* Description: implement abstract class of function object that invoke
*              a target function. This function object design to store
*              all necessary arguments of its target function when
*              it is created. The caller just to call 'call' only
*              to invoke the target function.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "DynamicFunction.h"
#include <cstdarg>

DFunction::DFunction() : 
	mParamCount(0)
{
}


DFunction::~DFunction()
{
}

void* DFunction::getReturnValAsVoidPointer() const {
	return *((void**)_ret);
}

__int64 DFunction::getReturnValAsInt64() const {
	return *(__int64*)_ret;
}

__int32 DFunction::getReturnValAsInt32() const {
	return *(__int32*)_ret;
}

__int16 DFunction::getReturnValAsInt16() const {
	return *(__int16*)_ret;
}

__int8 DFunction::getReturnValAsInt8() const {
	return *(__int8*)_ret;
}

float DFunction::getReturnValAsFloat() const {
	return *(float*)_ret;
}

double DFunction::getReturnValAsDouble() const {
	return *(double*)_ret;
}
