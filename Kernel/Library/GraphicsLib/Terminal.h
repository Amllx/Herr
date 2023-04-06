#pragma once

#include <System.h>
#include <HILib/Intel/Cpu.h>
#include <GraphicsLib/Ansi.h>
#include <Prekernel/EKBoot.h>

void Console(BootloaderHeader* pBootHdr);

#ifndef ConsoleLogWide
#define ConsoleLogWide(FMT) ConsoleLog(((Char*)FMT))
#endif // !ConsoleLogWide

#ifndef ConsoleLogWideFmt
#define ConsoleLogWideFmt(FMT, ...) ConsoleLog(((Char*)FMT), __VA_ARGS__)
#endif // !ConsoleLogWideFmt

void ConsoleEnableLogging(Boolean shouldShow);
void ConsoleLogInteger(SizeT x);
void ConsoleLogChar(Char x);
void ConsoleLogEx(char* fmt, ...);

#ifndef ConsoleLog
#define ConsoleLog(...) {\
    Log(" [ ");\
	Log(GRN);\
	Log(__DATE__);\
	Log(" | Line: ");\
	ConsoleLogInteger(__LINE__);\
	Log(" At File: ");\
	Log( __FILE__);\
	Log("\e[0m ] \e[0;37m");\
    ConsoleLogEx( __VA_ARGS__);\
}\


#endif // !ConsoleLog

void ConsoleLogHex(SizeT x);
void Log(char* message);
void ConsoleClear(void);
