#pragma once

#include <System.h>
#include <DT/Result.h>

typedef struct DispatchHeader {
	Int64 iNumSiblings;
	Int64 iIdent;

	void(*OnDispatch)();
	void(*OnPostDispatch)();
} DispatchHeader;

Boolean 
NewDispatch(struct DispatchHeader* Dispatcher, struct DispatchHeader** DispatchTbl, SizeT* iCounter, SizeT iMax);