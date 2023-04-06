#include <Prekernel/EKBoot.h>

VoidPtr BootloaderTag(struct BootStruct *Struct, UInt64 id) {
    struct BootloaderTag* current = (struct BootloaderTag*)Struct->tags;

    while (current != NULL) {
        if (current->identifier == id)
            return current;

        current = (VoidPtr)current->next;
    }

    return NULL;
}

__attribute__((aligned(16))) char stack[STACK_SIZE];

static struct bootloader_header_tag_framebuffer framebuffer_hdr_tag = {
        .tag = {
                .identifier = EKBOOT_HEADER_TAG_FRAMEBUFFER_ID,
                .next = 0
        },
        .framebuffer_width  = 800,
        .framebuffer_height = 600,
        .framebuffer_bpp    = 32,
};

static struct BootTerm terminal = {
		.tag = {
				.identifier = EKBOOT_HEADER_TAG_TERMINAL_ID,
				.next = (UInt64)&framebuffer_hdr_tag
		},

		.flags = 0
};

Attribute((section(".stivale2hdr"), used))
static struct bootloader_header stivale_hdr = {
        .entry_point = 0,
        .stack = (UIntPtr)stack + sizeof(stack),
        .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
        .tags = ((UIntPtr)&terminal),
};
