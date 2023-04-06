#include <Time.h>

Epoch BootloaderEpoch(BootloaderHeader* Header) {
    struct TagEpoch* epoch = BootloaderTag(Header, EKBOOT_STRUCT_TAG_EPOCH_ID);
    if (!epoch) return EPOCH_1970;

    return epoch->epoch;
}