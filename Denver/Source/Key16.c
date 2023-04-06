// Copyright EL Mahrouss Amlal, all rights reserved.
// Simple Key algorithm to use over more reliable algorithms.

#include <Key16.h>
#include <StringUtils.h>
#include <MemLib/Alloc.h>
#include <GraphicsLib/Terminal.h>

#define ADD_LETTER(LOW, HIGH) {LOW, HIGH}
#define MAX_LETTERS 31

static Key16Entry gLetters[MAX_LETTERS + 1] = {
    ADD_LETTER('a', 'A'),
    ADD_LETTER('b', 'B'),
    ADD_LETTER('c', 'C'),
    ADD_LETTER('d', 'D'),
    ADD_LETTER('e', 'E'),
    ADD_LETTER('f', 'F'),
    ADD_LETTER('g', 'G'),
    ADD_LETTER('h', 'H'),
    ADD_LETTER('i', 'I'),
    ADD_LETTER('j', 'J'),
    ADD_LETTER('k', 'K'),
    ADD_LETTER('l', 'L'),
    ADD_LETTER('m', 'M'),
    ADD_LETTER('n', 'N'),
    ADD_LETTER('o', 'O'),
    ADD_LETTER('p', 'P'),
    ADD_LETTER('q', 'Q'),
    ADD_LETTER('r', 'R'),
    ADD_LETTER('s', 'S'),
    ADD_LETTER('t', 'T'),
    ADD_LETTER('u', 'U'),
    ADD_LETTER('v', 'V'),
    ADD_LETTER('w', 'W'),
    ADD_LETTER('x', 'X'),
    ADD_LETTER('y', 'Y'),
    ADD_LETTER('z', 'Z'),
    ADD_LETTER('-', '-'),
    ADD_LETTER('_', '_'),
    ADD_LETTER('0', '0'),
    ADD_LETTER('}', '{'),
    ADD_LETTER('&', '&'),
    ADD_LETTER('!', '@'),
};

Int32 Key16TokenToIndex(Key16Entry chr) {
    Int32 begin = 0;
    Int32 right = (MAX_LETTERS - 1);

	while (begin < right) {
		if (gLetters[begin].lower == chr.lower || gLetters[begin].upper == chr.upper)
			return begin;
		else if (gLetters[right].lower == chr.lower || gLetters[right].upper == chr.upper)
			return right;
		else {
			if (gLetters[begin].lower != chr.lower)
				begin += 1;
			else if (gLetters[right].lower != chr.lower)
				right -= 1;
		}
	}

	return -1; // Index is invalid.
}

Key16Entry Key16IndexToToken(Int32 index) {
	Int32 right = (MAX_LETTERS - 1);
	if (index <= right) return gLetters[index];

	return gLetters[26];
}

Boolean Key16IsInBounds(Int32 index) {
	Int32 right = (MAX_LETTERS - 1);
	return index <= right;
}

Char* Key16(Char* message, SizeT len) {
	if (message == NULL) return KEY16_NIL;
	if (len < 1) return KEY16_NIL;

	Char* fmt = MemAlloc(sizeof(Char) * len);
    Key16Entry token;
    
	for (SizeT index = 0; index < len; ++index) {
        token.lower = ToLower(message[index]); // force cast for both of them.
        token.upper = ToUpper(message[index]);

		Int32 x = Key16TokenToIndex(token);

        if (x == -1) goto Free;
        
		fmt[index] = (Char)x;
	}

    return fmt;

	Free:
        MemFree(fmt);
        return NULL;
}

Boolean Key16Check(Char* code, Char* toCompare, SizeT len) {
    if (len < 1) return False;
	if (code == NULL) return False;
	if (toCompare == NULL) return False;

    Int32 cache = -1;
    Int32 cast = 0; // it's important to not initialize with -1, so we won't miss the first character.

    for (SizeT index = 0; index < len; ++index) {
        cast = (Int32)code[index];
        if (cache == cast) continue;

        if (!Key16IsInBounds(index)) {
            ConsoleLog( "Key16 : Bad checksum [ErrorCode{OUT_OF_BOUNDS}]");
            return False;
        } else {
            Key16Entry chr = Key16IndexToToken(cast);
            cache = cast;

            if (chr.lower != toCompare[index]) {
                if (chr.upper != toCompare[index]) {
                    ConsoleLog( "Key16 : Bad checksum [ErrorCode{NOT_HERE}]");
                    return False;
                }
            }
        }
    }

    ConsoleLog( "Key16: Is valid [ErrorCode{VALID}]");
	return True;
}