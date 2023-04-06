#include <GraphicsLib/Window.h>
#include <StringUtils.h>

void GLDrawPen(GLPen* pen, GLDeviceContext* ctx)
{
    for (int i = pen->startX; i < pen->endX; ++i) {
        ctx->fGraphicsWritePixelAt(ctx, i,
                                   pen->endY,
                                   pen->colour.red,
                                   pen->colour.green,
                                   pen->colour.blue);
    }
}

GLWindowPtr GLMakeWindow(UInt16* windowName, GLDeviceContext* deviceContext)
{
    if (windowName == NULL) return NULL;

    GLWindowPtr windowPtr = MemAlloc(sizeof(GLWindow));
    CopyMem(windowName, windowPtr->windowTitle.textStr, WINDOW_TITLE_LEN);
    windowPtr->deviceContext = deviceContext;

    return windowPtr;
}

static void Attribute((unused)) ShowDesktop(GLDeviceContext* devCtx)
{
    devCtx->fGraphicsWriteRect(devCtx, 1920, 1080,  236, 245, 180);
}
