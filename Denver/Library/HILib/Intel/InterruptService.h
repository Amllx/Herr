#pragma once

#include <System.h>
#include <HILib/Intel/APIC.h>

#define ISSendEoiSlave() ISSendEoi(0xA0)
#define ISSendEoiMaster() ISSendEoi(0x20)

#define IS_ICW1 0x11
#define IS_ICW4 0x01
#define IS_MASTER_CMD 0x20
#define IS_MASTER_DATA 0x21
#define IS_SLAVE_CMD 0xA0
#define IS_SLAVE_DATA 0xA1

#define IS_NOTIFICATION_BUSY 256
#define IS_NOTIFICATION_ERR 257

typedef struct ISNotificationSubscriber {
	Boolean active, busy, err;
	UInt16 irq, response; // Response can be read by Interrupt Service

	Void(*OnTrigger)(UInt16 irq);
} Attribute((packed)) ISNotificationSubscriber;

Void ISAddNotification(ISNotificationSubscriber* irq);
Void ISLegacyRemap(Void);
Void ISSendEoi(UInt8 irq);
Void ISMaskIrq(UInt8 irq);
Void ISUnMaskIrq(UInt8 irq);
