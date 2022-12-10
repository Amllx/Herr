#pragma once

#include <GraphicsLib/Graphics.h>

typedef struct Pen {
	UInt32 width, height;
	UInt32 x, y;
	Rgb32 colour;
} Attribute((packed)) Pen;
