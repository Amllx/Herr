#include <GraphicsLib/Graphics.h>
#include <DT/Result.h>

static Boolean GraphicsWriteRect(struct DeviceContext* ctx, Int32 _x, Int32 _y, UInt8 red, UInt8 green, UInt8 blue);
static Boolean GraphicsWritePixelAt(struct DeviceContext* ctx, Int32 x, Int32 y, UInt8 red, UInt8 green, UInt8 blue);
static struct BootloaderFrameBufferTag* GlobalFramebuffer = NULL;
static Rgb32* GlobalFramebufferPixel = NULL;

Boolean GraphicsInit(BootloaderHeader* bs) {
    if (GlobalFramebuffer != NULL) return False;
	if (bs == NULL) return False;

    GlobalFramebuffer = BootloaderTag(bs, EKBOOT_STRUCT_TAG_FRAMEBUFFER_ID);

    if (GlobalFramebuffer == NULL)
		Asm("int $24");

    GlobalFramebufferPixel = (Rgb32*)GlobalFramebuffer->framebuffer_addr;
    return True;
}

#define GRAPHICS_RGB_EQUATION(X) (X * GlobalFramebuffer->framebuffer_bpp) / (8)

static Boolean GraphicsWriteRect(struct DeviceContext* ctx, Int32 _x, Int32 _y, UInt8 red, UInt8 green, UInt8 blue) {
    Result = ERR_BAD_ARG;
    if (!ctx) return False;

    Result = ERR_NULL;
    if (!ctx->Framebuffer) return False;

	if (ctx->iWidth < _x) return False;
	if (ctx->iHeight < _y) return False;

    for (SizeT x = 0; x < _x; x++) {
        for (SizeT y = x; y < _y; y++) {
            ctx->Framebuffer[y].blue = (blue & 255);  // BLUE
            ctx->Framebuffer[y+1].green = (green & 255);  // GREEN
            ctx->Framebuffer[y+2].red = (red & 255);  // RED
        }

        ctx->Framebuffer = (Rgb32*)((UIntPtr)ctx->Framebuffer + GlobalFramebuffer->framebuffer_pitch);
    }

    for (SizeT x = _x; x > 0; x--) {
        for (SizeT y = _y; y > 0; y--) {
            ctx->Framebuffer[y].blue = (blue & 255);  // BLUE
            ctx->Framebuffer[y+1].green = (green & 255);  // GREEN
            ctx->Framebuffer[y+2].red = (red & 255);  // RED
        }

        ctx->Framebuffer = (Rgb32*)((UIntPtr)ctx->Framebuffer - GlobalFramebuffer->framebuffer_pitch);
    }

    Result = ERR_SUCCESS;
    return True; 
}

static Boolean GraphicsWritePixelAt(struct DeviceContext* ctx, Int32 x, Int32 y, UInt8 red, UInt8 green, UInt8 blue) {
    Result = ERR_BAD_ARG;
    if (!ctx) return False;

    Result = ERR_NULL;
    if (!ctx->Framebuffer) return False;

	if (ctx->iWidth < x) return False;
	if (ctx->iHeight < y) return False;

    ctx->Framebuffer[x+y].blue = (blue & 255);  // BLUE
    ctx->Framebuffer[x+y+1].green = (green & 255);  // GREEN
    ctx->Framebuffer[x+y+2].red = (red & 255);  // RED

    return True;
}

DeviceContext* NewDeviceContext(SizeT width, SizeT height) {
	if (width < 0) return False;
	if (height < 0) return False;

    DeviceContext* context = MemAlloc(sizeof(DeviceContext));
    Check(context, "Out of Memory! Device Context was not created.");
    
    context->fGraphicsWriteRect = GraphicsWriteRect;
    context->fGraphicsWritePixelAt = GraphicsWritePixelAt;
    context->iWidth = width;
    context->iHeight = height;
    context->iBpp = GlobalFramebuffer->framebuffer_bpp;
    context->Framebuffer = (Rgb32*)GlobalFramebuffer->framebuffer_addr;

    ConsoleLog("New Framebuffer allocated!\n");

    ConsoleLog("%s %i %n", "Width: ", context->iWidth);
    ConsoleLog("%s %i %n", "Height: ", context->iHeight);

    ConsoleLog("%s %i %n", "Green Mask Shift: ", GlobalFramebuffer->green_mask_shift);
    ConsoleLog("%s %i %n", "Blue Mask Shift: ", GlobalFramebuffer->blue_mask_shift);
    ConsoleLog("%s %i %n", "Red Mask Shift: ", GlobalFramebuffer->red_mask_shift);

    ConsoleLog("%s %i %n", "Green Mask Size: ", GlobalFramebuffer->green_mask_size);
    ConsoleLog("%s %i %n", "Blue Mask Size: ", GlobalFramebuffer->blue_mask_size);
    ConsoleLog("%s %i %n", "Red Mask Size: ", GlobalFramebuffer->red_mask_size);

    return context;
}