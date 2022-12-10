#pragma once

#include <AHCI/AHCI.h>
#include <Number.h>

int SataWrite(HbaPort *port, UInt64 baselba, UInt64 count, Char* buf);
