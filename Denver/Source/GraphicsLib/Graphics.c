#include <GraphicsLib/Graphics.h>
#include <DTLib/Result.h>

static Boolean GraphicsWriteRect(struct GLDeviceContext* ctx, Int32 _x, Int32 _y, UInt8 red, UInt8 green, UInt8 blue);
static Boolean GraphicsWritePixelAt(struct GLDeviceContext* ctx, Int32 x, Int32 y, UInt8 red, UInt8 green, UInt8 blue);
static struct BootloaderFrameBufferTag* GlobalFramebuffer = NULL;
static Rgb32* GlobalFramebufferPixel = NULL;

Boolean GLInit(BootloaderHeader* bs) {
    if (GlobalFramebuffer != NULL) return False;
	if (bs == NULL) return False;

    GlobalFramebuffer = BootloaderTag(bs, EKBOOT_STRUCT_TAG_FRAMEBUFFER_ID);

    if (GlobalFramebuffer == NULL)
		Asm("int $24");

    GlobalFramebufferPixel = (Rgb32*)GlobalFramebuffer->framebuffer_addr;
    return True;
}

#define GRAPHICS_RGB_EQUATION(X) (X * GlobalFramebuffer->framebuffer_bpp) / (8)

static Boolean GraphicsWriteRect(struct GLDeviceContext* ctx, Int32 _x,
        Int32 _y, UInt8 red,
        UInt8 green, UInt8 blue) {
    Result = ERR_BAD_ARG;
    if (!ctx) return False;

    Result = ERR_NULL;

    if (!ctx->Framebuffer) return False;

    for (SizeT x = 0; x < _x; ++x) {
        for (SizeT y = x; y < _y; ++y) {
            ctx->Framebuffer[y].blue = (blue & 255);  // BLUE
            ctx->Framebuffer[y+1].green = (green & 255);  // GREEN
            ctx->Framebuffer[y+2].red = (red & 255);  // RED
        }

        ctx->Framebuffer = (Rgb32*)((UIntPtr)ctx->Framebuffer + GlobalFramebuffer->framebuffer_pitch);
    }

    for (SizeT x = _x; x > 0; --x) {
        for (SizeT y = _y; y > 0; --y) {
            ctx->Framebuffer[y].blue = (blue & 255);  // BLUE
            ctx->Framebuffer[y+1].green = (green & 255);  // GREEN
            ctx->Framebuffer[y+2].red = (red & 255);  // RED
        }

        ctx->Framebuffer = (Rgb32*)((UIntPtr)ctx->Framebuffer - GlobalFramebuffer->framebuffer_pitch);
    }

    Result = ERR_SUCCESS;
    return True; 
}

static Boolean GraphicsWritePixelAt(struct GLDeviceContext* ctx, Int32 x, Int32 y, UInt8 red, UInt8 green, UInt8 blue) {
    Result = ERR_BAD_ARG;
    if (!ctx) return False;

    Result = ERR_NULL;
    if (!ctx->Framebuffer) return False;

    ctx->Framebuffer[x+y].blue = (blue & 255);  // BLUE
    ctx->Framebuffer[x+y+1].green = (green & 255);  // GREEN
    ctx->Framebuffer[x+y+2].red = (red & 255);  // RED

    return True;
}

GLDeviceContext* GLNewDeviceContext(SizeT width, SizeT height) {
	if (width < 1) return False;
	if (height < 1) return False;

    GLDeviceContext* context = MemAlloc(sizeof(GLDeviceContext));
    Check(context, "Out of Memory! Device Context was not created.");
    
    context->fGraphicsWriteRect = GraphicsWriteRect;
    context->fGraphicsWritePixelAt = GraphicsWritePixelAt;
    context->iBpp = GlobalFramebuffer->framebuffer_bpp;
    context->Framebuffer = (Rgb32*)GlobalFramebuffer->framebuffer_addr;

    return context;
}