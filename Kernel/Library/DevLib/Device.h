#pragma once

#include <Lib.h>

typedef Int32 DeviceT;

#ifndef RL_MAX_DEV
#define RL_MAX_DEV (10000)
#endif

#define RT_NULLDEV (-1)

struct DEVICE_CREATION;
struct DeviceController;

struct DEVICE_CREATION {
    VoidPtr Address;
    SizeT Size;
    Int64 Flags;
    Int32 AuthorizedPIDs;
    Int32 AuthorizedUsers;

    struct DeviceController* Parent;
};

typedef struct DeviceController {
	Char DeviceName[256]; // \\PHYSICAL-DRIVE
    Boolean Free;
    struct DEVICE_CREATION DeviceCreation;
} DeviceController;

DeviceT AllocDevice(Char* DeviceName, struct DEVICE_CREATION* CreationStruct);
Boolean CloseDevice(DeviceT Device, Int32 PID, Int32 User);
VoidPtr GetDevicePointer(DeviceT Device);
SizeT GetDeviceSize(DeviceT Device);
Char* GetDeviceName(DeviceT Device);