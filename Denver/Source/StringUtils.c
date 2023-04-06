#include <MemLib/Alloc.h>
#include <StringUtils.h>

Int32 StringCompare(const Char* src, const Char* cmp, SizeT size) {
    Int32 counter = 0;

    for (SizeT index = 0; index < size; ++index) {
        if (src[index] != cmp[index])
            ++counter;
    }

    return counter;
}

Int32 StringCompareWide(const UInt16* src, const UInt16* cmp, SizeT size) {
    Int32 counter = 0;

    for (SizeT index = 0; index < size; ++index) {
        if (src[index] != cmp[index])
            ++counter;
    }

    return counter;
}

SizeT StringLength(Char* str) {
    if (*str == 0) return 0;
    
    SizeT len = 0;

    while (str[len] != '\0')
        ++len;

    return len;
}

SizeT WideStringLength(UInt16* str) {
	SizeT len = 0;

	while (str[len] != '\0')
		++len;

	return len;
}

VoidPtr SetMem(VoidPtr pointer, char value, SizeT len) {
	if (pointer == NULL) return NULL;
	char* start = pointer;

	while (len) {
		*start = value;
		++start; // increment the buffer
		--len; // decrement the counter
	}

	return (VoidPtr)start;
}

Int32 CopyMem(VoidPtr src, VoidPtr dst, SizeT len) {
	if (src == NULL || dst == NULL) return -1;

	char* srcChr = src;
	char* dstChar = dst;
	SizeT index = 0;

	while (index < len) {
		dstChar[index] = srcChr[index];
		index++;
	}

	return 0;
}

Int32 CopyMemWide(VoidPtr src, VoidPtr dst, SizeT len) {
	if (src == NULL || dst == NULL) return -1;

	UInt16* srcChr = src;
	UInt16* dstChar = dst;
	SizeT index = 0;

	while (index < len) {
		dstChar[index] = srcChr[index];
		index++;
	}

	return 0;
}

Char* AllocString(Char* text) {
    if (text == NULL) return((Char *)"Nil");
	SizeT len =  StringLength(text);

    Char* alloc = MemAlloc(sizeof(Char) * len);
    if (alloc == NULL) return((Char *)"Nil");

    CopyMem(text, alloc, len);
    return alloc;
}

UInt16* AllocWideString(UInt16* text) {
    if (text == NULL) return((UInt16 *)"Nil");
	SizeT len =  WideStringLength(text);

    UInt16* alloc = MemAlloc(sizeof(UInt16) * len);
    if (alloc == NULL) return((UInt16 *)"Nil");

    CopyMem(text, alloc, len);
    return alloc;
}

Int32 IsAlpha(Int32 character) {
	return (((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z')) ? 1 : 0);
}

Int32 ToUpper(Int32 character) {
	if (character >= 'a' && character <= 'z')
		return character - 0x20;
	
	return character;
}

Int32 ToLower(Int32 character) {
	if (character >= 'A' && character <= 'Z')
		return character + 0x20;
	
	return character;
}

Boolean IsSpace(Char chr) { return chr == ' '; }
Boolean IsNewLn(Char chr) { return chr == '\n'; }

char* StringToLowerCase(Char* in) {
	char* out = MemAlloc(sizeof(char) * StringLength(in));
    SetMem(out, 1, StringLength(in));

    if (in && out) {
        for (int index = 0; index < StringLength(in); ++index) {
            if (IsAlpha(in[index])) {
                Boolean found = False;

                for (int ascii_index = 0; ascii_index < 27; ++ascii_index) {
                    if ((ascii_index + 65) == in[index]) {
                        out[index] = ascii_index + 97;
                        found = True;
                    }
                }

                if (found) continue;
                out[index] = in[index];
            }
        }

        out[StringLength(in)] = '\0';
        return out;
    }

	if (out) MemFree(out);
    return NULL;
}
