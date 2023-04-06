#pragma once

#include <System.h>
#include <UUID.h>

// Anchor for non HOFF kernels
struct bootloader_anchor {
    UInt8 anchor[15];
    UInt8 bits;

    Split64(phys_load_addr);
    Split64(phys_bss_start);
    Split64(phys_bss_end);
    Split64(phys_bootloaderhdr);
};

struct BootloaderTag {
    UInt64 identifier;
    Split64(next);
};

/* --- Header --------------------------------------------------------------- */
/*  Information passed from the kernel to the bootloader                      */

struct bootloader_header {
    Split64(entry_point);
    Split64(stack);
    UInt64 flags;
    Split64(tags);
};

#define EKBOOT_HEADER_TAG_ANY_VIDEO_ID 0xc75c9fa92a44c4db

struct bootloader_header_tag_any_video {
    struct BootloaderTag tag;
    UInt64 preference;
};

#define EKBOOT_HEADER_TAG_FRAMEBUFFER_ID 0x3ecc1bc43d0f7971

struct bootloader_header_tag_framebuffer {
    struct BootloaderTag tag;
    UInt16 framebuffer_width;
    UInt16 framebuffer_height;
    UInt16 framebuffer_bpp;
    UInt16 unused;
};

#define EKBOOT_HEADER_TAG_FB_MTRR_ID 0x4c7bb07731282e00

#define EKBOOT_HEADER_TAG_SLIDE_HHDM_ID 0xdc29269c2af53d1d

struct BootHhdm {
    struct BootloaderTag tag;
    UInt64 flags;
    Split64(alignment);
};

#define EKBOOT_HEADER_TAG_TERMINAL_ID 0xa85d499b1823be72

struct BootTerm {
    struct BootloaderTag tag;
    UInt64 flags;
    Split64(callback);
};

#define EKBOOT_TERM_CB_DEC 10
#define EKBOOT_TERM_CB_BELL 20
#define EKBOOT_TERM_CB_PRIVATE_ID 30
#define EKBOOT_TERM_CB_STATUS_REPORT 40
#define EKBOOT_TERM_CB_POS_REPORT 50
#define EKBOOT_TERM_CB_KBD_LEDS 60
#define EKBOOT_TERM_CB_MODE 70
#define EKBOOT_TERM_CB_LINUX 80

#define EKBOOT_TERM_CTX_SIZE ((UInt64)(-1))
#define EKBOOT_TERM_CTX_SAVE ((UInt64)(-2))
#define EKBOOT_TERM_CTX_RESTORE ((UInt64)(-3))
#define EKBOOT_TERM_FULL_REFRESH ((UInt64)(-4))

#define EKBOOT_HEADER_TAG_SMP_ID 0x1ab015085f3273df

struct HeaderBootSMP {
    struct BootloaderTag tag;
    UInt64 flags;
};

#define EKBOOT_HEADER_TAG_5LV_PAGING_ID 0x932f477032007e8f

#define EKBOOT_HEADER_TAG_UNMAP_NULL_ID 0x92919432b16fe7e7

/* --- Struct --------------------------------------------------------------- */
/*  Information passed from the bootloader to the kernel                      */

struct BootStruct {
#define EKBOOT_BOOTLOADER_BRAND_SIZE 64
    char bootloader_brand[EKBOOT_BOOTLOADER_BRAND_SIZE];

#define EKBOOT_BOOTLOADER_VERSION_SIZE 64
    char bootloader_version[EKBOOT_BOOTLOADER_VERSION_SIZE];

    UInt64 tags;
};

#define EKBOOT_STRUCT_TAG_PMRS_ID 0x5df266a64047b6bd

#define EKBOOT_PMR_EXECUTABLE ((UInt64)1 << 0)
#define EKBOOT_PMR_WRITABLE   ((UInt64)1 << 1)
#define EKBOOT_PMR_READABLE   ((UInt64)1 << 2)

struct BootPmr {
    UInt64 base;
    UInt64 length;
    UInt64 permissions;
};

struct TagPmrs {
    struct BootloaderTag tag;
    UInt64 entries;
    struct BootPmr pmrs[];
};

#define EKBOOT_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID 0x060d78874a2a8af0

struct TagKernelBaseAddr {
    struct BootloaderTag tag;
    UInt64 physical_base_address;
    UInt64 virtual_base_address;
};

#define EKBOOT_STRUCT_TAG_CMDLINE_ID 0xe5e76a1b4597a781

struct TagCmdline {
    struct BootloaderTag tag;
    UInt64 cmdline;
};

#define EKBOOT_STRUCT_TAG_MEM_ID 0x2187f79e8612de07

#define EKBOOT_MEM_USABLE                 1
#define EKBOOT_MEM_RESERVED               2
#define EKBOOT_MEM_ACPI_RECLAIMABLE       3
#define EKBOOT_MEM_ACPI_NVS               4
#define EKBOOT_MEM_BAD_MMAN             5
#define EKBOOT_MEM_BOOTLOADER_RECLAIMABLE 0x1000
#define EKBOOT_MEM_KERNEL_AND_MODULES     0x1001
#define EKBOOT_MEM_FRAMEBUFFER            0x1002

struct MmapEntry {
    UInt64 base;
    UInt64 length;
    UInt32 type;
    UInt32 unused;
};

struct TagMemmap {
    struct BootloaderTag tag;
    UInt64 entries;
    struct MmapEntry memmap[];
};

#define EKBOOT_STRUCT_TAG_FRAMEBUFFER_ID 0x506461d2950408fa

#define EKBOOT_FBUF_MMODEL_RGB 1

struct BootloaderFrameBufferTag {
    struct BootloaderTag tag;
    UInt64 framebuffer_addr;
    UInt16 framebuffer_width;
    UInt16 framebuffer_height;
    UInt16 framebuffer_pitch;
    UInt16 framebuffer_bpp;
    UInt8  memory_model;
    UInt8  red_mask_size;
    UInt8  red_mask_shift;
    UInt8  green_mask_size;
    UInt8  green_mask_shift;
    UInt8  blue_mask_size;
    UInt8  blue_mask_shift;
    UInt8  unused;
};

#define EKBOOT_STRUCT_TAG_EDID_ID 0x968609d7af96b845

struct TagEdid {
    struct BootloaderTag tag;
    UInt64 edid_size;
    UInt8  edid_information[];
};

#define EKBOOT_STRUCT_TAG_TEXTMODE_ID 0x38d74c23e0dca893

struct TagTextmode {
    struct BootloaderTag tag;
    UInt64 address;
    UInt16 unused;
    UInt16 rows;
    UInt16 cols;
    UInt16 bytes_per_char;
};

#define EKBOOT_STRUCT_TAG_FB_MTRR_ID 0x6bc1a78ebe871172

#define EKBOOT_STRUCT_TAG_TERMINAL_ID 0xc2b3f4c3233b0974

struct TagTerminal {
    struct BootloaderTag tag;
    UInt32 flags;
    UInt16 cols;
    UInt16 rows;
    UInt64 term_write;
    UInt64 max_length;
};

#define EKBOOT_STRUCT_TAG_MODULES_ID 0x4b6fe466aade04ce

struct BootModule {
    UInt64 begin;
    UInt64 end;

#define EKBOOT_MODULE_STRING_SIZE 128
    char string[EKBOOT_MODULE_STRING_SIZE];
};

struct TagModules {
    struct BootloaderTag tag;
    UInt64 module_count;
    struct BootModule modules[];
};

#define EKBOOT_STRUCT_TAG_RSDP_ID 0x9e1786930a375e78

struct TagRsdp {
    struct BootloaderTag tag;
    UInt64 rsdp;
};

#define EKBOOT_STRUCT_TAG_EPOCH_ID 0x566a7bed888e1407

struct TagEpoch {
    struct BootloaderTag tag;
    Int64 epoch;
};

#define EKBOOT_STRUCT_TAG_FIRMWARE_ID 0x359d837855e3858c

#define EKBOOT_FIRMWARE_BIOS (1 << 0)

struct TagFirmware {
    struct BootloaderTag tag;
    UInt64 flags;
};

#define EKBOOT_STRUCT_TAG_EFI_SYSTEM_TABLE_ID 0x4bc5ec15845b558e

struct TagEFI {
    struct BootloaderTag tag;
    UInt64 system_table;
};

#define EKBOOT_STRUCT_TAG_KERNEL_FILE_ID 0xe599d90c2975584a

struct TagKernelFile {
    struct BootloaderTag tag;
    UInt64 kernel_file;
};

#define EKBOOT_STRUCT_TAG_KERNEL_FILE_V2_ID 0x37c13018a02c6ea2

struct TagKernelFile_v2 {
    struct BootloaderTag tag;
    UInt64 kernel_file;
    UInt64 kernel_size;
};

#define EKBOOT_STRUCT_TAG_BOOT_VOLUME_ID 0x9b4358364c19ee62

struct BootVol {
    struct BootloaderTag tag;
    UInt64 flags;
    GUID guid;
    GUID part_guid;
};

#define EKBOOT_STRUCT_TAG_KERNEL_SLIDE_ID 0xee80847d01506c57

struct TagKernelSlide {
    struct BootloaderTag tag;
    UInt64 kernel_slide;
};

#define EKBOOT_STRUCT_TAG_SMBIOS_ID 0x274bd246c62bf7d1

struct TagSMBios {
    struct BootloaderTag tag;
    UInt64 flags;
    UInt64 smbios_entry_32;
    UInt64 smbios_entry_64;
};

#define EKBOOT_STRUCT_TAG_SMP_ID 0x34d1d96339647025

struct BootSMP {
    UInt32 threador_id;
    UInt32 lapic_id;
    UInt64 target_stack;
    UInt64 goto_address;
    UInt64 extra_argument;
};

struct TagSMP {
    struct BootloaderTag tag;
    UInt64 flags;
    UInt32 bsp_lapic_id;
    UInt32 unused;
    UInt64 cpu_count;
    struct BootSMP smp_info[];
};

#define EKBOOT_STRUCT_TAG_MMIO32_UART 0xb813f9b8dbc78797

struct TagMMIO32Uart {
    struct BootloaderTag tag;
    UInt64 addr;
};

#define EKBOOT_STRUCT_TAG_DTB 0xabb29bd49a2833fa

struct TagDtb {
    struct BootloaderTag tag;
    UInt64 addr;
    UInt64 size;
};

#define EKBOOT_STRUCT_TAG_HHDM_ID 0xb0ed257db18cb58f

struct TagHhdm {
    struct BootloaderTag tag;
    UInt64 addr;
};

typedef struct BootStruct BootloaderHeader;

VoidPtr BootloaderTag(struct BootStruct *Struct, UInt64 id);

#undef Split64