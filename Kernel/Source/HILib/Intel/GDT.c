#include <HILib/Intel/Intel.h>

static GDT Descriptors[] = {
        {0}, {
                .limit       = 0xffff,
                .baseLow    = 0x0000,
                .baseMid    = 0x00,
                .access      = 0b10011010,
                .granularity = 0b00000000,
                .baseHigh     = 0x00
        },
        {
                .limit       = 0xffff,
                .baseLow    = 0x0000,
                .baseMid    = 0x00,
                .access      = 0b10010010,
                .granularity = 0b00000000,
                .baseHigh     = 0x00
        },
        {
                .limit       = 0xffff,
                .baseLow    = 0x0000,
                .baseMid    = 0x00,
                .access      = 0b10011010,
                .granularity = 0b11001111,
                .baseHigh     = 0x00
        },
        {
                .limit       = 0xffff,
                .baseLow    = 0x0000,
                .baseMid    = 0x00,
                .access      = 0b10010010,
                .granularity = 0b11001111,
                .baseHigh     = 0x00
        },

        {
                .limit       = 0x0000,
                .baseLow    = 0x0000,
                .baseMid    = 0x00,
                .access      = 0b10011010,
                .granularity = 0b00100000,
                .baseHigh     = 0x00
        },
        {
                .limit       = 0x0000,
                .baseLow    = 0x0000,
                .baseMid    = 0x00,
                .access      = 0b10010010,
                .granularity = 0b00000000,
                .baseHigh     = 0x00
        }
};

Boolean GdtInit(void) {
    LdGdt(Descriptors);
    return True;
}
