#include <DevLib/Device.h>

DeviceController gDevices[RL_MAX_DEV];

static
DeviceT
TryAllocDevice(DeviceT i, Char* DeviceName, struct DEVICE_CREATION* CreationStruct)
{
    if (gDevices[i].Free) {
        gDevices[i].Free = False;
        gDevices[i].DeviceCreation = *CreationStruct;
        CopyMem(DeviceName, gDevices->DeviceName, StringLength(DeviceName));
        gDevices[i].DeviceCreation.Parent = &gDevices[i];

        return i;
    }

    return RT_NULLDEV;
}

DeviceT 
AllocDevice(Char* DeviceName, struct DEVICE_CREATION* CreationStruct) 
{
    if (StringLength(DeviceName) > 256) return RT_NULLDEV;
    if (IsNull(CreationStruct)) return RT_NULLDEV;

    DeviceT right = (RL_MAX_DEV - 1);
    for (DeviceT left = 0; left < RL_MAX_DEV; left++)
    {
        if (right < left) return RT_NULLDEV;

        if (TryAllocDevice(left, DeviceName, CreationStruct) != RT_NULLDEV) return left;
        if (TryAllocDevice(right, DeviceName, CreationStruct) != RT_NULLDEV) return right;

        --right;
    }
    
    return RT_NULLDEV;
}

Boolean
TryCloseDevice(DeviceT Device)
{
    if (Device > RL_MAX_DEV) return False;

    gDevices[Device].DeviceCreation.AuthorizedPIDs = 0;
    gDevices[Device].DeviceCreation.AuthorizedUsers = 0;
    gDevices[Device].DeviceCreation.Address = NULL;
    gDevices[Device].DeviceCreation.Parent = NULL;
    gDevices[Device].DeviceCreation.Size = 0;
    gDevices[Device].Free = True;

    return True;
}

Boolean 
CloseDevice(DeviceT Device, Int32 PID, Int32 User)
{
    for (SizeT dev = 0; dev < RL_MAX_DEV; dev++)
    {
        if ((gDevices[dev].DeviceCreation.AuthorizedPIDs & PID) == PID &&
            (gDevices[dev].DeviceCreation.AuthorizedUsers & User) == User) {
            return TryCloseDevice(dev);
        }
    }
    
    return False;
}

VoidPtr 
GetDevicePointer(DeviceT Device)
{
    if (Device > RL_MAX_DEV) return NULL;
    DeviceController* dev = &gDevices[Device];
    if (!dev->Free) return NULL;

    return dev->DeviceCreation.Address;
}

SizeT 
GetDeviceSize(DeviceT Device)
{
    if (Device > RL_MAX_DEV) return 0;
    DeviceController* dev = &gDevices[Device];
    if (!dev->Free) return 0;
    
    return dev->DeviceCreation.Size;
}

Char* 
GetDeviceName(DeviceT Device)
{
    if (Device > RL_MAX_DEV) return NULL;
    DeviceController* dev = &gDevices[Device];
    if (!dev->Free) return NULL;
    
    return dev->DeviceName;
}