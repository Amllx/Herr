#include <HILib/Intel/InterruptService.h>

extern Void ISPicWait(Void);

Void ISLegacyRemap(Void) {
    Out8(IS_MASTER_CMD, IS_ICW1);
    ISPicWait();
    Out8(IS_SLAVE_CMD, IS_ICW1);
    ISPicWait();

    Out8(IS_MASTER_CMD, 0x20);
    ISPicWait();
    Out8(IS_SLAVE_CMD, 0x28);
    ISPicWait();

    Out8(IS_MASTER_CMD, 0x4);
    ISPicWait();
    Out8(IS_SLAVE_CMD, 0x2);
    ISPicWait();

    Out8(IS_MASTER_CMD, 1);
    Out8(IS_SLAVE_CMD, 1);
    ISPicWait();

    Out8(IS_MASTER_CMD, 0);
    ISPicWait();

    Out8(IS_SLAVE_CMD, 0);
    ISPicWait();
}

static ISNotificationSubscriber* gListeners[256];
static UInt8 gListenerCount = 0;

Void ISAddNotification(ISNotificationSubscriber* irq) {
	if (irq == NULL) return;

	if (gListenerCount >= 255) return;
	if (irq->irq >= 255) return;

	gListeners[irq->irq] = irq;
	++gListenerCount;
}

void ISSendEoi(UInt8 irq) {
    if (irq >= 40) Out8(IS_MASTER_CMD, IS_MASTER_CMD);

    Out8(IS_SLAVE_CMD, IS_MASTER_CMD);

	// Notify the listener
	ISNotificationSubscriber* notification = gListeners[irq];

	if (notification != NULL && notification->active) {
		if (!notification->busy && !notification->err)
			notification->OnTrigger(irq);
		else if (notification->busy)
			notification->OnTrigger(IS_NOTIFICATION_BUSY);
		else if (notification->err)
			notification->OnTrigger(IS_NOTIFICATION_ERR);
	}
}

void ISMaskIrq(UInt8 irq) {
    UInt16 port = 0;
    UInt8 value = 0;

    if(irq < 8) { // Check irq index
        port = IS_MASTER_DATA;
    } else {
        port = IS_SLAVE_DATA;
        irq -= 8;
    }

    value = In8(port) | (1 << irq);
    Out8(port, value);
}

void ISUnMaskIrq(UInt8 irq) {
    UInt16 port = 0;
    UInt8 value = 0;

    if(irq < 8) {
        port = IS_MASTER_DATA;
    } else {
        port = IS_SLAVE_DATA;
        irq -= 8;
    }

    value = (In8(port) & ~(1 << irq));
    Out8(port, value);
}