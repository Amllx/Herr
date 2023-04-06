#include <MemLib/CoreAlloc.h>
#include <DevLib/Helpers.h>
#include <StringUtils.h>

Char* DLGenerateName(Char* Base)
{
    SizeT length = StringLength(Base) + 2;
    Char* newString = MemAlloc(sizeof(Char) * length);
    if (newString == NULL) return NULL;
    
    newString[0] = '\\';
    newString[1] = '\\';

    CopyMem(Base, newString + 2, length - 2);
    return newString;
}