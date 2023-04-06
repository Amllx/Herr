#pragma once

#include <AHCI/AHCI.h>

Int32 SataRead(HbaPort *port, UInt64 baseLba, UInt32 length, Char* buf);
