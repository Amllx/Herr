#include <DevLib/Device.h>

Device gDevices[XPX_DEV_MAX];

static
DEVICE_ID
TryCreateDevice(DEVICE_ID i, Char* DeviceName, struct DeviceCreation* CreationStruct)
{
    if (gDevices[i].Free) {
        gDevices[i].Free = False;
        gDevices[i].DeviceCreation = *CreationStruct;
        CopyMem(DeviceName, gDevices->DeviceName, StringLength(DeviceName));
        gDevices[i].DeviceCreation.Parent = &gDevices[i];

        return i;
    }

    return NULLDEV;
}

DEVICE_ID
DLCreateDevice(Char* DeviceName, struct DeviceCreation* CreationStruct)
{
    if (StringLength(DeviceName) > 256) return NULLDEV;
    if (IsNull(CreationStruct)) return NULLDEV;

    DEVICE_ID right = (XPX_DEV_MAX - 1);
    for (DEVICE_ID left = 0; left < XPX_DEV_MAX; left++)
    {
        if (right < left) return NULLDEV;

        if (TryCreateDevice(left, DeviceName, CreationStruct) != NULLDEV) return left;
        if (TryCreateDevice(right, DeviceName, CreationStruct) != NULLDEV) return right;

        --right;
    }
    
    return NULLDEV;
}

Boolean
TryCloseDevice(DEVICE_ID Device)
{
    if (Device > XPX_DEV_MAX) return False;

    gDevices[Device].DeviceCreation.AuthorizedPIDs = 0;
    gDevices[Device].DeviceCreation.AuthorizedUsers = 0;
    gDevices[Device].DeviceCreation.Address = NULL;
    gDevices[Device].DeviceCreation.Parent = NULL;
    gDevices[Device].DeviceCreation.Size = 0;
    gDevices[Device].Free = True;

    return True;
}

Boolean 
DLCloseDevice(DEVICE_ID Device, Int32 PID, Int32 User)
{
    for (SizeT dev = 0; dev < XPX_DEV_MAX; dev++)
    {
        if ((gDevices[dev].DeviceCreation.AuthorizedPIDs & PID) == PID &&
            (gDevices[dev].DeviceCreation.AuthorizedUsers & User) == User) {
            return TryCloseDevice(dev);
        }
    }
    
    return False;
}

VoidPtr 
DLGetDevicePointer(DEVICE_ID Device)
{
    if (Device > XPX_DEV_MAX) return NULL;
    DevicePtr dev = &gDevices[Device];
    if (!dev->Free) return NULL;

    return dev->DeviceCreation.Address;
}

SizeT 
DLGetDeviceSize(DEVICE_ID Device)
{
    if (Device > XPX_DEV_MAX) return 0;
    DevicePtr dev = &gDevices[Device];
    if (!dev->Free) return 0;
    
    return dev->DeviceCreation.Size;
}

Char* 
DLGetDeviceName(DEVICE_ID Device)
{
    if (Device > XPX_DEV_MAX) return NULL;
    DevicePtr dev = &gDevices[Device];
    if (!dev->Free) return NULL;
    
    return dev->DeviceName;
}