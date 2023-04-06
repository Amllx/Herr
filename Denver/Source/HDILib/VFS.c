#include <HDILib/HDI.h>
#include <HDILib/VFS.h>
#include <DTLib/Result.h>
#include <GraphicsLib/Terminal.h>

static Int32 HDIAttributeVirtualID(void);

static HDIContext* 	 kInterface = NULL;
static HDIDirectory* kRootDirectory = NULL;
static HDIDirectory* kLastCreatedDir = NULL;
static Int32 kVirtualIds = 0;

typedef enum {
	HDI_STATUS_USABLE = 0,
	HDI_STATUS_COUNT,
} HDI_STATUS;

static Int32 HDIAttributeVirtualID(void) {
	if (kVirtualIds > HDI_MAX_IDS) return HDI_UNATTRIBUTED;
	++kVirtualIds;

	ConsoleLog("%s %i %n", "Virtual ID: ", kVirtualIds - 1);
	return kVirtualIds - 1;
}

Void SetCurrentVirtualIO(HDIContext* interface) {
	kInterface = interface;
	Check(NotNull(kInterface), "Interface is NULL! (VFS)");
}

PtrFileHandle CreateVirtualHandle(void) {
	if (!HDIEnabled()) return NULL;

	PtrFileHandle handle = MemAlloc(sizeof(FileHandle));
	handle->__id = HDIAttributeVirtualID();

	if (IsNull(kRootDirectory)) {
		kRootDirectory = MemAlloc(sizeof(HDIDirectory));
		Check(NotNull(kRootDirectory), "kRootDirectory is NULL!");
		
		kRootDirectory->bLocked = True;
		kRootDirectory->iId = handle->__id;
		kRootDirectory->iWhere = HDI_LBA_START;
		kRootDirectory->iStatus = HDI_STATUS_USABLE;
		kRootDirectory->Root = NULL;
		kRootDirectory->Child = NULL;

		kLastCreatedDir = kRootDirectory;
	}

	if (IsNull(kLastCreatedDir)) {
		kLastCreatedDir = MemAlloc(sizeof(HDIDirectory));
		Check(NotNull(kLastCreatedDir), "kLastCreatedDir is NULL!");
		
		kLastCreatedDir->bLocked = True;
		kLastCreatedDir->iId = handle->__id;
		kLastCreatedDir->iStatus = HDI_STATUS_USABLE;
		kLastCreatedDir->Root = kRootDirectory;
		kLastCreatedDir->iWhere = kRootDirectory->iWhere + sizeof(HDIDirectory);
		kLastCreatedDir->Child = NULL;

		kLastCreatedDir = kRootDirectory;
	} else {
		if (NotNull(kLastCreatedDir->Child)) {
			handle->__id = kLastCreatedDir->iId;
			ConsoleLog("Found handle.");
			return handle;
		}

		kLastCreatedDir->Child = MemAlloc(sizeof(HDIDirectory));
		Check(NotNull(kLastCreatedDir->Child), "kLastCreatedDir->Child is NULL!");

		kLastCreatedDir->Child->bLocked = True;
		kLastCreatedDir->Child->iId = handle->__id;
		kLastCreatedDir->Child->iStatus = HDI_STATUS_USABLE;
		kLastCreatedDir->Child->Root = kRootDirectory;
		kLastCreatedDir->iWhere = kLastCreatedDir->Root->iWhere + sizeof(HDIDirectory);
		kLastCreatedDir->Child->Child = NULL;

		kLastCreatedDir = kLastCreatedDir->Child;

	}

	ConsoleLog(NotNull(handle) ? "New Handle created!\n" : "Handle is NULL!!!\n");
	return handle;
}

UInt16* VirtualRead(PtrFileHandle FileHandle, UInt16* strName) {
	if (FileHandle && strName) {
		HDIFile* delegate = NULL;
		HDIDirectory* dir = kRootDirectory;

		while (NotNull(dir) && FileHandle->__id != dir->iId) {
			dir = dir->Child;
		}
		
		Result = HDI_NOT_FOUND;
		if (IsNull(dir)) return NULL; // NOT FOUND

		for (SizeT i = 0; i < HDI_MAX_FILES; i++) {
			if (StringCompareWide(strName, dir->Files[i]->strFileName, 256)) {
				delegate = (HDIFile*)dir->Files[i];
				break;
			}
		}
		
		if (IsNull(delegate)) return NULL;

		Result = HDI_USED;
		if (delegate->iStatus == -1) return NULL;

		delegate->iStatus = -1;

		Check(kInterface != NULL, "VFS: Interface is NULL!");

		UInt16* file = NULL;
		if (HDIHasInterfaceFlag(kInterface, HDI_DONT_NEED_COMPLEX_LAYOUT)) {
			Check(kInterface->fReadEx, "AHCI I/O is NULL!");
			file = kInterface->fReadEx(delegate->iStart, delegate->szFile);
			ConsoleLog("%s %s %n", "Done At:", " VirtualReadEx");
		} else {
			Check(kInterface->fRead, "PIO I/O is NULL!");
			file = kInterface->fRead(kInterface, delegate->iStart, delegate->szFile);
			ConsoleLog("%s %s %n", "Done At:", " VirtualRead");
		}

		delegate->iStatus = 0;
		
		Result = ERR_SUCCESS;
		return file;
	}

	return NULL;
}

Boolean VirtualWrite(PtrFileHandle FileHandle, UInt16* strName, UInt16* buffer) {
	if (!strName) return False;
	if (!buffer) return False;

	if (WideStringLength(strName) > 256) return False;
	if (WideStringLength(buffer) <= 2) return False;

	if (FileHandle && kRootDirectory) {
		HDIFile* delegate = NULL;
		HDIDirectory* dir = kRootDirectory;

		while (FileHandle->__id != dir->iId) {
			if (IsNull(dir)) break;
			dir = dir->Child;
		}
		
		if (IsNull(dir)) return HDI_NOT_FOUND; // NOT FOUND

		ConsoleLog("{VFS} Found the directory...\n");

		for (SizeT i = 0; i < HDI_MAX_FILES; i++) {
			if (dir->Files[i]->szFile == 0) {
				if (NotNull(dir->Files[i]) || dir->Files[i]->iStatus == -1) continue;

				delegate = dir->Files[i] = MemAlloc(sizeof(struct HDIFile));

				break;
			}
		}

		if (IsNull(delegate)) return False;
		ConsoleLog("{VFS} Found the delegate...\n");
	
		delegate->szFile = WideStringLength(buffer);
		CopyMem(strName, delegate->strFileName, delegate->szFile);

		delegate->iStatus = -1;

		Check(kInterface != NULL, "VFS: Interface is NULL!\n");

		if (HDIHasInterfaceFlag(kInterface, HDI_DONT_NEED_COMPLEX_LAYOUT)) {
			Check(kInterface->fWriteEx, "AHCI I/O is NULL!");
			kInterface->fWriteEx(buffer, delegate->iStart);
			ConsoleLog("%s %s %n", "Done At:", " VirtualWriteEx");
		} else {
			Check(kInterface->fWrite, "PIO I/O is NULL!");
			kInterface->fWrite(kInterface, delegate->iStart, delegate->szFile, buffer);
			ConsoleLog("%s %s %n", "Done At:", " VirtualWrite");
		}

		delegate->iStatus = 0;

		return True;
	}

	return False;
}
