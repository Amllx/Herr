#include <HILib/Intel/Fpu.h>

Void FpuLoadControlWord(const UInt16 control) {
    VolatileAsm("fldcw %0;" :: "m"(control));
}

