#pragma once

#include <System.h>
#include <MemLib/Alloc.h>
#include <Prekernel/EKBoot.h>
#include <GraphicsLib/Terminal.h>

struct GLDeviceContext;
struct Rgb32;
struct GLPen;

typedef struct Rgb32 {
    UInt8 blue;
    UInt8 green;
	UInt16 red;
} Attribute((packed)) Rgb32;

typedef Int32 Color32;

typedef Boolean(*GfxWriteT)(struct GLDeviceContext* ctx, Int32 _x, Int32 _y, UInt8 red, UInt8 green, UInt8 blue);

typedef struct GLDeviceContext {
	GfxWriteT fGraphicsWritePixelAt;
	GfxWriteT fGraphicsWriteRect;
	Rgb32* Framebuffer;
	UInt32 iBpp;
} Attribute((packed)) GLDeviceContext;

Boolean GLInit(BootloaderHeader* bs);
GLDeviceContext* GLNewDeviceContext(SizeT width, SizeT height);