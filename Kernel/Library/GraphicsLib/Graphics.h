#pragma once

#include <System.h>
#include <MemLib/CoreAlloc.h>
#include <Prekernel/EKBoot.h>
#include <GraphicsLib/Terminal.h>

/*
	Core Graphics
	The thing behind the PredatorX API.

	Copyright Amlal El Mahrouss all rights reserved.
*/

struct DeviceContext;
struct Rgb32;
struct Pen;

typedef struct Rgb32 {
    UInt8 blue;
    UInt8 green;
	UInt16 red;
} Attribute((packed)) Rgb32;

typedef Int32 Color32;

typedef Boolean(*GfxWriteT)(struct DeviceContext* ctx, Int32 _x, Int32 _y, UInt8 red, UInt8 green, UInt8 blue);

typedef struct DeviceContext {
	GfxWriteT fGraphicsWritePixelAt;
	GfxWriteT fGraphicsWriteRect;
	Rgb32* Framebuffer;
	UInt32 iWidth;
	UInt32 iHeight;
	UInt32 iBpp;
} Attribute((packed)) DeviceContext;

Boolean GraphicsInit(BootloaderHeader* bs);
DeviceContext* NewDeviceContext(SizeT width, SizeT height);