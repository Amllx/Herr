#pragma once

#include <GraphicsLib/Graphics.h>

typedef struct GLPen {
	UInt32 width, height;
	UInt32 startX, startY;
    UInt32 endX, endY;
	Rgb32 colour;
} Attribute((packed)) GLPen;

#define WINDOW_TITLE_LEN (256U)

typedef struct GLWindowText {
    UInt16 textStr[WINDOW_TITLE_LEN];
    GLPen textPen;
} GLWindowText;

typedef struct GLWindow {
    GLPen windowPen;
    GLWindowText windowTitle;
    GLDeviceContext* deviceContext;
} Attribute((packed)) GLWindow;

typedef GLWindow* GLWindowPtr;

void GLDrawPen(GLPen* pen, GLDeviceContext* ctx);
GLWindowPtr GLMakeWindow(UInt16* windowName, GLDeviceContext* deviceContext);