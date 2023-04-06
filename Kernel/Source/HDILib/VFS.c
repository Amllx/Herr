#include <HDILib/HDI.h>
#include <HDILib/VFS.h>
#include <DT/Result.h>
#include <GraphicsLib/Terminal.h>

static Int32 HDIAttributeVirtualID(void);

static HDIContext* 	 g_Interface = NULL;
static HDIDirectory* g_RootDirectory = NULL;
static HDIDirectory* g_LastCreatedDir = NULL;
static Int32			 g_VirtualIds = 0;

typedef enum {
	HDI_STATUS_USABLE = 0,
	HDI_STATUS_COUNT,
} HDI_STATUS;

static Int32 HDIAttributeVirtualID(void) {
	if (g_VirtualIds > HDI_MAX_IDS) return HDI_UNATTRIBUTED;
	++g_VirtualIds;

	ConsoleLog("%s %i %n", "Virtual ID: ", g_VirtualIds - 1);
	return g_VirtualIds - 1;
}

Void SetCurrentVirtualIO(HDIContext* interface) {
	g_Interface = interface;
	Check(NotNull(g_Interface), "Interface is NULL! (VFS)");
}

PtrFileHandle CreateVirtualHandle(void) {
	if (!HDIEnabled()) return NULL;

	PtrFileHandle handle = MemAlloc(sizeof(FileHandle));
	handle->__id = HDIAttributeVirtualID();

	if (IsNull(g_RootDirectory)) {
		g_RootDirectory = MemAlloc(sizeof(HDIDirectory));
		Check(NotNull(g_RootDirectory), "g_RootDirectory is NULL!");
		
		g_RootDirectory->bLocked = True;
		g_RootDirectory->iId = handle->__id;
		g_RootDirectory->iWhere = HDI_LBA_START;
		g_RootDirectory->iStatus = HDI_STATUS_USABLE;
		g_RootDirectory->Root = NULL;
		g_RootDirectory->Child = NULL;

		g_LastCreatedDir = g_RootDirectory;
	}

	if (IsNull(g_LastCreatedDir)) {
		g_LastCreatedDir = MemAlloc(sizeof(HDIDirectory));
		Check(NotNull(g_LastCreatedDir), "g_LastCreatedDir is NULL!");
		
		g_LastCreatedDir->bLocked = True;
		g_LastCreatedDir->iId = handle->__id;
		g_LastCreatedDir->iStatus = HDI_STATUS_USABLE;
		g_LastCreatedDir->Root = g_RootDirectory;
		g_LastCreatedDir->iWhere = g_RootDirectory->iWhere + sizeof(HDIDirectory);
		g_LastCreatedDir->Child = NULL;

		g_LastCreatedDir = g_RootDirectory;
	} else {
		if (NotNull(g_LastCreatedDir->Child)) {
			handle->__id = g_LastCreatedDir->iId;
			ConsoleLog("Found handle.");
			return handle;
		}

		g_LastCreatedDir->Child = MemAlloc(sizeof(HDIDirectory));
		Check(NotNull(g_LastCreatedDir->Child), "g_LastCreatedDir->Child is NULL!");

		g_LastCreatedDir->Child->bLocked = True;
		g_LastCreatedDir->Child->iId = handle->__id;
		g_LastCreatedDir->Child->iStatus = HDI_STATUS_USABLE;
		g_LastCreatedDir->Child->Root = g_RootDirectory;
		g_LastCreatedDir->iWhere = g_LastCreatedDir->Root->iWhere + sizeof(HDIDirectory);
		g_LastCreatedDir->Child->Child = NULL;

		g_LastCreatedDir = g_LastCreatedDir->Child;

	}

	ConsoleLog(NotNull(handle) ? "New Handle created!\n" : "Handle is NULL!!!\n");
	return handle;
}

UInt16* VirtualRead(PtrFileHandle FileHandle, UInt16* strName) {
	if (FileHandle && strName) {
		HDIFile* delegate = NULL;
		HDIDirectory* dir = g_RootDirectory;

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

		Check(g_Interface != NULL, "VFS: Interface is NULL!");

		UInt16* file = NULL;
		if (HDIHasInterfaceFlag(g_Interface, HDI_DONT_NEED_COMPLEX_LAYOUT)) {
			Check(g_Interface->fReadEx, "AHCI I/O is NULL!");
			file = g_Interface->fReadEx(delegate->iStart, delegate->szFile);
			ConsoleLog("%s %s %n", "Done At:", " VirtualReadEx");
		} else {
			Check(g_Interface->fRead, "PIO I/O is NULL!");
			file = g_Interface->fRead(g_Interface, delegate->iStart, delegate->szFile);
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

	if (FileHandle && g_RootDirectory) {
		HDIFile* delegate = NULL;
		HDIDirectory* dir = g_RootDirectory;

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

		Check(g_Interface != NULL, "VFS: Interface is NULL!\n");

		if (HDIHasInterfaceFlag(g_Interface, HDI_DONT_NEED_COMPLEX_LAYOUT)) {
			Check(g_Interface->fWriteEx, "AHCI I/O is NULL!");
			g_Interface->fWriteEx(buffer, delegate->iStart);
			ConsoleLog("%s %s %n", "Done At:", " VirtualWriteEx");
		} else {
			Check(g_Interface->fWrite, "PIO I/O is NULL!");
			g_Interface->fWrite(g_Interface, delegate->iStart, delegate->szFile, buffer);
			ConsoleLog("%s %s %n", "Done At:", " VirtualWrite");
		}

		delegate->iStatus = 0;

		return True;
	}

	return False;
}
