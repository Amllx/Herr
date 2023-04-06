#pragma once

#include <Bool.h>
#include <HILib/Intel/Cpu.h>

#define Check(expr, message) { if ((expr) == 0) { ConsoleLog(message); Asm("int $25"); } }
#define Breakpoint()  { Asm volatile ("1: jmp 1b"); }
