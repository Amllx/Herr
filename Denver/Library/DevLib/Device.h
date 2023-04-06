#pragma once

#include <Lib.h>

typedef Int32 DEVICE_ID;

#ifndef XPX_DEV_MAX
#define XPX_DEV_MAX (1000)
#endif

#define NULLDEV (-1)

struct DeviceCreation;
struct Device;

struct DeviceCreation
        {
    VoidPtr Address;
    SizeT Size;
    Int64 Flags;
    Int32 AuthorizedPIDs;
    Int32 AuthorizedUsers;

    struct Device* Parent;
};

#define XPX_DEVICE_NAME_LEN (256U)

typedef struct Device
{
	Char DeviceName[XPX_DEVICE_NAME_LEN]; // \\PHYSICAL-DRIVE
    struct DeviceCreation DeviceCreation;
    Boolean Free;
} Device;

typedef Device* DevicePtr;

DEVICE_ID DLCreateDevice(Char* DeviceName, struct DeviceCreation* CreationStruct);
Boolean DLCloseDevice(DEVICE_ID Device, Int32 PID, Int32 User);
VoidPtr DLGetDevicePointer(DEVICE_ID Device);
SizeT DLGetDeviceSize(DEVICE_ID Device);
Char* DLGetDeviceName(DEVICE_ID Device);