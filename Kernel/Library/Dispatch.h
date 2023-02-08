#pragma once

#include <System.h>
#include <DT/Result.h>

typedef struct Dispatcher {
	Int64 iNumSiblings;
	Int64 iIdent;

	void(*OnDispatch)();
	void(*OnPostDispatch)();
} Dispatcher;

Boolean 
NewDispatcher(struct Dispatcher* Dispatcher, struct Dispatcher** DispatchTbl, SizeT* iCounter, SizeT iMax);