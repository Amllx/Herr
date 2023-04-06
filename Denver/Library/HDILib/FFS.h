/*
 *	========================================================
 *
 *	DENVER
 * 	Date Added: 08/01/2023
 * 	Copyright XPX Technologies, all rights reserved.
 *
 * 	========================================================
 */

#pragma once

#include <HDILib/ACL.h>
#include <HDILib/VFS.h>
#include <HDILib/HDI.h>
#include <Lib.h>

typedef struct FFSSuperBlock
{
    char PartName[32];
    Int32 PartType;
    HDILba StartLba;
    HDILba EndLba;
    Int32 Crc32;
    Int32 PartFlags;
} FFSSuperBlock;

typedef FFSSuperBlock *FFSSuperBlockPtr;

typedef struct FFSBlock
{
    char FileName[256];

    Int32 FileType;
    HDILba FileOff;
    SizeT FileSz; // 5, 4... (in sector count)
} FFSBlock;

typedef FFSBlock *FFSBlockPtr;