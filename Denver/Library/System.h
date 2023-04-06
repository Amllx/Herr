#ifndef SYS_H
#define SYS_H

#ifndef __x86_64__
#error This isnt a x86_64 Target!
#endif

#ifdef cplusplus
extern "C" {
#endif

#include <Bool.h>
#include <Defs.h>
#include <Number.h>
#include <DTLib/Assert.h>

extern struct BootStruct* gBootHdr;

#ifdef cplusplus
};
#endif

#endif // SYS_H
