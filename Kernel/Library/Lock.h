#pragma once

#include <System.h>

typedef Int32 SpinLock;

#define MakeSpinLock(Name) volatile SpinLock Name ## _Locked


#define SyncCompareAndSwap(TheLock) sync_bool_compare_and_swap(TheLock, 0, 1)
#define SyncSynchronize() sync_synchronize()

#define LockCurrentThread(Name) \
	while (!SyncCompareAndSwap(& Name ## _Locked, 0, 1)); \
	SyncSynchronize();

#define UnlockCurrentThread(Name) \
	SyncSynchronize(); \
	Name##_Locked = 0;

