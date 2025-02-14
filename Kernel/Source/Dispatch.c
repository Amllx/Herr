#include <Dispatch.h>

Boolean 
NewDispatcher(struct Dispatcher* Dispatcher, struct Dispatcher** DispatchTbl, SizeT* iCounter, SizeT iMax) {
	Result = ERR_FAILURE;

	if (Dispatcher == NULL) return False;
	if (iCounter == NULL) return False;
	if (*iCounter >= iMax) return False;

	DispatchTbl[*iCounter] = Dispatcher;
	++*iCounter;

	Dispatcher->iIdent = *iCounter;
	Dispatcher->iNumSiblings = iMax - *iCounter - 1;

	Result = ERR_SUCCESS;
	return True;
}
