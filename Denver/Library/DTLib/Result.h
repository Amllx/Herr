#pragma once

#include <OSErr.h>

extern OSErr Result;

#define BreakIf(ErrorId) if (ErrorId == Result) { ConsoleLog("Integrity check failed!, Aborting\n"); Asm("int $25"); }

#define ERR_FAILURE     		(-1)
#define ERR_SUCCESS     		(0)
#define ERR_NULL        		(1)
#define ERR_BAD_ARG     		(2)
#define ERR_BAD_ACCESS  	(3)
#define ERR_BUSY        			(4)
#define ERR_NOT_NULL    	(8)
#define ERR_DISK_IO 			(-255)