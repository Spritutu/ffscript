#include "spylib.h"
#include "spylib_interfaces.h"
#include "spy_interfaces.h"
#include "FFObjectsCommands.h"

using namespace std;

extern "C" {

	/**
	*   brief an export function that will be executed in spy engine, when a spy app send command LOAD_PREDEFINED_FUNCTIONS to the engine
	*	
	*   getPredefinedFunctionCount should return number predefined function that the user want to loaded to the engine
	**/
	SPYLIB_API int getPredefinedFunctionCount() {
		return (int)UserCommandId::PRE_DEFINED_COMMAND_COUNT;
	}

	/**
	*   brief an export function that will be executed in spy engine, when a spy app send command LOAD_PREDEFINED_FUNCTIONS to the engine
	*
	*   params
	*		context: context pass by spy engine, user spy library should pass it back to spy engine when use fx to set a custom command id
	*		fx     : a function pointer in spy engine, uer spy library should use it to load a custom command to spy engine
	*		cmdBase: a command id base for predefined commands id will build up, spy library may store this value if need,
	*				note that, this value is also returned to spy client from LOAD_PREDEFINED_FUNCTIONS commands 
	* 
	*   loadPredefinedFunctions should return zero to the engine know that loaded function should be kept in the engine for using in future
	*                           or nonzero to notify that the engine should discard the loadding result
	**/
	SPYLIB_API int loadPredefinedFunctions(void* context, FSetPredefinedFunction fx, CustomCommandId cmdBase) {
		fx(context, (CustomCommandId)UserCommandId::READ_UNIT_LIST, readUnitList);
		fx(context, (CustomCommandId)UserCommandId::READ_EXPRESSION_NODE, readUnitNode);
		fx(context, (CustomCommandId)UserCommandId::READ_COMMAND_LIST, readCommandList);
		fx(context, (CustomCommandId)UserCommandId::READ_PROGRAM_COMMAND, readProgramCommand);
		return 0;
	}
}