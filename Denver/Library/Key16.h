#pragma once

#include <Defs.h>
#include <Bool.h>
#include <Number.h>

#define KEY16_NIL ("Nil")

typedef struct Key16Entry {
	Char upper;
	Char lower;
} __attribute__((packed)) Key16Entry;

Char* Key16(Char* message, SizeT len);
Boolean Key16Check(Char* c16, Char* cmpWith, SizeT lenOfC16);
