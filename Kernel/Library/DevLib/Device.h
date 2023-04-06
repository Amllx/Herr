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

DEVICE_ID CreateDevice(Char* DeviceName, struct DeviceCreation* CreationStruct);
Boolean CloseDevice(DEVICE_ID Device, Int32 PID, Int32 User);
VoidPtr GetDevicePointer(DEVICE_ID Device);
SizeT GetDeviceSize(DEVICE_ID Device);
Char* GetDeviceName(DEVICE_ID Device);