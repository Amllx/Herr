#pragma once

#include <System.h>

#define ZeroMem(pointer, len) SetMem(pointer, 0, len)
#define T(TEXT) ((UInt16*)TEXT)
#define WideString(TEXT) T(TEXT)

Int32 CopyMemWide(VoidPtr src, VoidPtr dst, SizeT len);
Int32 CopyMem(VoidPtr pointer, VoidPtr dst, SizeT len); /* copies a memory region to another using a size as our scope. */
VoidPtr SetMem(VoidPtr pointer, Char val, SizeT len); /* Sets the memory region to a size with a value */
Int32 StringCompare(const Char* src, const Char* cmp, SizeT len);
Int32 StringCompareWide(const UInt16* src, const UInt16* cmp, SizeT size);
UInt16* AllocWideString(UInt16* text);
SizeT WideStringLength(UInt16* str);
Char* StringToLowerCase(Char* in);
Int32 IsAha(Int32 character);
Char* AllocString(Char* text);
SizeT StringLength(char* str);
Boolean IsNewLn(Char chr);
Boolean IsSpace(Char chr);
Int32 ToUpper(Int32 c);
Int32 ToLower(Int32 c);
