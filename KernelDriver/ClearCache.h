#pragma once
#include "hook.h"
#include <ntdef.h>
#include "skCrypter.h"



namespace cleaner {
	namespace PiDDB {
		PVOID g_KernelBase = NULL;
		ULONG g_KernelSize = 0;


		BOOLEAN printPiDDBTable(PRTL_AVL_TABLE table) {
			
			//DbgPrint("PiDDBCacheTable: %p", table);
			//DbgPrint("PiDDBCacheTable->NumberGenericTableElements: %d", table->NumberGenericTableElements);
			//DbgPrint("First child: %p", table->BalancedRoot.RightChild);

			UINT_PTR entry_address = reinterpret_cast<UINT_PTR>(table->BalancedRoot.RightChild) + sizeof(RTL_BALANCED_LINKS);
			PiDDBCacheEntry* first_entry = reinterpret_cast<PiDDBCacheEntry*>(entry_address);

			//DbgPrint("Name: %ws", first_entry->DriverName.Buffer);
			//DbgPrint("Timestamp: %X", first_entry->TimeDateStamp);

			ULONG count = 0;

			for (PLIST_ENTRY link = first_entry->List.Flink; link != first_entry->List.Blink; link = link->Flink, count++)
			{
				PiDDBCacheEntry* cache_entry = reinterpret_cast<PiDDBCacheEntry*>(link);

				//DbgPrint("list_id[%lu] name: %wZ \t\t stamp: %x",
					count,
					cache_entry->DriverName,
					cache_entry->TimeDateStamp;
			}

			return TRUE;

		}

		PVOID resolve_relative_address(_In_ PVOID Instruction, _In_ ULONG OffsetOffset, _In_ ULONG InstructionSize)
		{
			ULONG_PTR Instr = (ULONG_PTR)Instruction;
			LONG RipOffset = *(PLONG)(Instr + OffsetOffset);
			PVOID ResolvedAddr = (PVOID)(Instr + InstructionSize + RipOffset);

			return ResolvedAddr;
		}

		NTSTATUS pattern_scan(IN PCUCHAR pattern, IN UCHAR wildcard, IN ULONG_PTR len, IN const VOID* base, IN ULONG_PTR size, OUT PVOID* ppFound)
		{
			ASSERT(ppFound != NULL && pattern != NULL && base != NULL);
			if (ppFound == NULL || pattern == NULL || base == NULL)
				return STATUS_INVALID_PARAMETER;

			for (ULONG_PTR i = 0; i < size - len; i++)
			{
				BOOLEAN found = TRUE;
				for (ULONG_PTR j = 0; j < len; j++)
				{
					if (pattern[j] != wildcard && pattern[j] != ((PCUCHAR)base)[i + j])
					{
						found = FALSE;
						break;
					}
				}

				if (found != FALSE)
				{
					*ppFound = (PUCHAR)base + i;
					return STATUS_SUCCESS;
				}
			}

			return STATUS_NOT_FOUND;
		}

		PVOID get_kernel_base(OUT PULONG pSize)
		{
			NTSTATUS status = STATUS_SUCCESS;
			ULONG bytes = 0;
			PRTL_PROCESS_MODULES pMods = NULL;
			PVOID checkPtr = NULL;
			UNICODE_STRING routineName;

			// Already found
			if (g_KernelBase != NULL)
			{
				if (pSize)
					*pSize = g_KernelSize;
				return g_KernelBase;
			}

			RtlUnicodeStringInit(&routineName, L"NtOpenFile");

			checkPtr = MmGetSystemRoutineAddress(&routineName);
			if (checkPtr == NULL)
				return NULL;


			status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

			if (bytes == 0)
				return NULL;

			pMods = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x454E4F45); // 'ENON'
			RtlZeroMemory(pMods, bytes);

			status = ZwQuerySystemInformation(SystemModuleInformation, pMods, bytes, &bytes);

			if (NT_SUCCESS(status))
			{
				PRTL_PROCESS_MODULE_INFORMATION pMod = pMods->Modules;

				for (ULONG i = 0; i < pMods->NumberOfModules; i++)
				{
					// System routine is inside module
					if (checkPtr >= pMod[i].ImageBase &&
						checkPtr < (PVOID)((PUCHAR)pMod[i].ImageBase + pMod[i].ImageSize))
					{
						g_KernelBase = pMod[i].ImageBase;
						g_KernelSize = pMod[i].ImageSize;
						if (pSize)
							*pSize = g_KernelSize;
						break;
					}
				}
			}

			if (pMods)
				ExFreePoolWithTag(pMods, 0x454E4F45); // 'ENON'


			return g_KernelBase;
		}


		NTSTATUS scan_section(IN PCCHAR section, IN PCUCHAR pattern, IN UCHAR wildcard, IN ULONG_PTR len, OUT PVOID* ppFound)
		{
			ASSERT(ppFound != NULL);
			if (ppFound == NULL)
				return STATUS_INVALID_PARAMETER;

			PVOID base = get_kernel_base(NULL);
			if (!base)
				return STATUS_NOT_FOUND;


			PIMAGE_NT_HEADERS64 pHdr = (PIMAGE_NT_HEADERS64)RtlImageNtHeader(base);
			if (!pHdr)
				return STATUS_INVALID_IMAGE_FORMAT;

			PIMAGE_SECTION_HEADER pFirstSection = (PIMAGE_SECTION_HEADER)(pHdr + 1);
			for (PIMAGE_SECTION_HEADER pSection = pFirstSection; pSection < pFirstSection + pHdr->FileHeader.NumberOfSections; pSection++)
			{
				ANSI_STRING s1, s2;
				RtlInitAnsiString(&s1, section);
				RtlInitAnsiString(&s2, (PCCHAR)pSection->Name);
				if (RtlCompareString(&s1, &s2, TRUE) == 0)
				{
					PVOID ptr = NULL;
					NTSTATUS status = pattern_scan(pattern, wildcard, len, (PUCHAR)base + pSection->VirtualAddress, pSection->Misc.VirtualSize, &ptr);
					if (NT_SUCCESS(status))
						*(PULONG)ppFound = (ULONG)((PUCHAR)ptr - (PUCHAR)base);

					return status;
				}
			}

			return STATUS_NOT_FOUND;
		}

		BOOLEAN LocatePiDDB(PERESOURCE* lock, PRTL_AVL_TABLE* table)
		{
			//UCHAR PiDDBLockPtr_sig[] = "\x48\x8D\x0D\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x48\x8B\x0D\xCC\xCC\xCC\xCC\x33\xDB";
			//UCHAR PiDTablePtr_sig[] = "\x48\x8D\x0D\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x3D\xCC\xCC\xCC\xCC\x0F\x83\xCC\xCC\xCC\xCC";
			UCHAR PiDDBLockPtr_sig[] = "\x48\x8D\x0D\xCC\xCC\xCC\xCC\xE8\xCC\xCC\xCC\xCC\x4C\x8B\x8C";
			UCHAR PiDTablePtr_sig[] = "\x66\x03\xD2\x48\x8D\x0D";

			PVOID PiDDBLockPtr = NULL;
			if (!NT_SUCCESS(scan_section("PAGE", PiDDBLockPtr_sig, 0xCC, sizeof(PiDDBLockPtr_sig) - 1, (&PiDDBLockPtr)))) {
				//DbgPrint("--------LOCKPTR NOT FOUND-----------");
				return FALSE;
			}

			RtlZeroMemory(PiDDBLockPtr_sig, sizeof(PiDDBLockPtr_sig) - 1);

			PVOID PiDTablePtr = NULL;
			if (!NT_SUCCESS(scan_section("PAGE", PiDTablePtr_sig, 0xCC, sizeof(PiDTablePtr_sig) - 1, (&PiDTablePtr)))) {
				//DbgPrint("--------TABLEPTR NOT FOUND-----------");
				return FALSE;
			}

			RtlZeroMemory(PiDTablePtr_sig, sizeof(PiDTablePtr_sig) - 1);


			UINT64 RealPtrPIDLock = NULL;

			RealPtrPIDLock = (UINT64)g_KernelBase + (UINT64)PiDDBLockPtr;


			*lock = (PERESOURCE)resolve_relative_address((PVOID)RealPtrPIDLock, 3, 7);

			UINT64 RealPtrPIDTable = NULL;

			RealPtrPIDTable = (UINT64)g_KernelBase + (UINT64)PiDTablePtr;

			*table = (PRTL_AVL_TABLE)(resolve_relative_address((PVOID)RealPtrPIDTable, 6, 10));

			//if (!printPiDDBTable(*table)) DbgPrint("shit");

			return TRUE;
		}

		BOOLEAN clean_piddbcachetable() {
			PERESOURCE PiDDBLock = NULL;
			PRTL_AVL_TABLE PiDDBCacheTable = NULL;
			if (!LocatePiDDB(&PiDDBLock, &PiDDBCacheTable) && PiDDBLock == NULL && PiDDBCacheTable == NULL) {
				//DbgPrint("Error locating lock or table");
				return FALSE;
			}

			// build a lookup entry

			PIDCacheobj lookupEntry;

			// this should work :D
			UNICODE_STRING DriverName = RTL_CONSTANT_STRING(L"iqvw64e.sys");
			// removed *DriverName no need for it
			lookupEntry.DriverName = DriverName;
			lookupEntry.TimeDateStamp = 0x5284EAC3; // intel_driver TimeStamp.
			// aquire the ddb lock
			ExAcquireResourceExclusiveLite(PiDDBLock, TRUE);

			// search our entry in the table

			// maybe something will bsod here.
			PIDCacheobj* pFoundEntry = (PIDCacheobj*)RtlLookupElementGenericTableAvl(PiDDBCacheTable, &lookupEntry);

			if (pFoundEntry == NULL)
			{
				// release the ddb resource lock
				ExReleaseResourceLite(PiDDBLock);
				return FALSE;
			}
			else
			{
				// first, unlink from the list
				RemoveEntryList(&pFoundEntry->List);
				// then delete the element from the avl table
				RtlDeleteElementGenericTableAvl(PiDDBCacheTable, pFoundEntry);

				// release the ddb resource lock
				ExReleaseResourceLite(PiDDBLock);
			}

			//DbgPrint("-------------FINISHED LIST -------------");
			//printPiDDBTable(PiDDBCacheTable);
			return TRUE;
		}
	}

	namespace MmUnloadedDrivers { 
		PMM_UNLOADED_DRIVER MmUnloadedDrivers;
		ERESOURCE PsLoadedModuleResource;

		PULONG MmLastUnloadedDriver;

		EXTERN_C PVOID ResolveRelativeAddress(_In_ PVOID Instruction, _In_ ULONG OffsetOffset, _In_ ULONG InstructionSize) {
			ULONG_PTR Instr = (ULONG_PTR)Instruction;
			LONG RipOffset = *(PLONG)(Instr + OffsetOffset);
			PVOID ResolvedAddr = (PVOID)(Instr + InstructionSize + RipOffset);

			return ResolvedAddr;
		}

		BOOLEAN bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
		{
			for (; *szMask; ++szMask, ++pData, ++bMask)
				if (*szMask == 'x' && *pData != *bMask)
					return 0;

			return (*szMask) == 0;
		}

		UINT64 FindPattern(UINT64 dwAddress, UINT64 dwLen, BYTE* bMask, char* szMask)
		{
			for (UINT64 i = 0; i < dwLen; i++)
				if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
					return (UINT64)(dwAddress + i);

			return 0;
		}
		
		NTSTATUS FindMmDriverData(VOID){
			/*
			 *	nt!MmLocateUnloadedDriver:
			 *	fffff801`51c70394 4c8b15a57e1500  mov     r10,qword ptr [nt!MmUnloadedDrivers (fffff801`51dc8240)]
			 *	fffff801`51c7039b 4c8bc9          mov     r9 ,rcx
			 */
			PVOID MmUnloadedDriversInstr = (PVOID)FindPattern((UINT64)cleaner::PiDDB::g_KernelBase, cleaner::PiDDB::g_KernelSize,
				(BYTE*)"\x4C\x8B\x00\x00\x00\x00\x00\x4C\x8B\xC9\x4D\x85\x00\x74", "xx?????xxxxx?x"
			);

			/*
			 *	nt!MiRememberUnloadedDriver+0x59:
			 *	fffff801`5201a4c5 8b057ddddaff    mov     eax,dword ptr [nt!MmLastUnloadedDriver (fffff801`51dc8248)]
			 *	fffff801`5201a4cb 83f832          cmp     eax,32h
			*/
			PVOID MmLastUnloadedDriverInstr = (PVOID)FindPattern((UINT64)cleaner::PiDDB::g_KernelBase, cleaner::PiDDB::g_KernelSize,
				(BYTE*)"\x8B\x05\x00\x00\x00\x00\x83\xF8\x32",
				"xx????xxx"
			);

			if (MmUnloadedDriversInstr == NULL || MmLastUnloadedDriverInstr == NULL)
			{
				return STATUS_NOT_FOUND;
			}

			MmUnloadedDrivers = *(PMM_UNLOADED_DRIVER*)ResolveRelativeAddress(MmUnloadedDriversInstr, 3, 7);
			MmLastUnloadedDriver = (PULONG)ResolveRelativeAddress(MmLastUnloadedDriverInstr, 2, 6);
			//DbgPrint("MmUnloadedDrivers ModuleEnd: %x", MmUnloadedDrivers->ModuleEnd);
			//DbgPrint("MmUnloadedDrivers ModuleStart: %x", MmUnloadedDrivers->ModuleStart);
			//DbgPrint("MmUnloadedDrivers Name: %wZ", MmUnloadedDrivers->Name);
			//DbgPrint("MmUnloadedDrivers UnloadTime: %x", MmUnloadedDrivers->UnloadTime);

			//DbgPrint("MmUnloadedDrivers Addr: %x", MmUnloadedDrivers);
			//DbgPrint("MmLastUnloadedDriver Addr: %x", MmLastUnloadedDriver);
			return STATUS_SUCCESS;
		}

		BOOLEAN IsUnloadedDriverEntryEmpty( _In_ PMM_UNLOADED_DRIVER Entry ) {
			if (Entry->Name.MaximumLength == 0 ||
				Entry->Name.Length == 0 ||
				Entry->Name.Buffer == NULL)
			{
				return TRUE;
			}

			return FALSE;
		}

		BOOLEAN IsMmUnloadedDriversFilled(VOID) {
			for (ULONG Index = 0; Index < MM_UNLOADED_DRIVERS_SIZE; ++Index)
			{
				PMM_UNLOADED_DRIVER Entry = &MmUnloadedDrivers[Index];
				if (IsUnloadedDriverEntryEmpty(Entry))
				{
					return FALSE;
				}
			}

			return TRUE;
		}

		NTSTATUS ClearUnloadedDriver(_In_ PUNICODE_STRING DriverName, _In_ BOOLEAN AccquireResource) {
			if (AccquireResource)
			{
				ExAcquireResourceExclusiveLite(&PsLoadedModuleResource, TRUE);
			}

			BOOLEAN Modified = FALSE;
			BOOLEAN Filled = IsMmUnloadedDriversFilled();

			for (ULONG Index = 0; Index < MM_UNLOADED_DRIVERS_SIZE; ++Index)
			{
				PMM_UNLOADED_DRIVER Entry = &MmUnloadedDrivers[Index];
				//DbgPrint("Entry %i: %wZ\t%x", Index, Entry->Name, Entry->UnloadTime);
				if (Modified)
				{
					//
					// Shift back all entries after modified one.
					//
					PMM_UNLOADED_DRIVER PrevEntry = &MmUnloadedDrivers[Index - 1];
					RtlCopyMemory(PrevEntry, Entry, sizeof(MM_UNLOADED_DRIVER));

					//
					// Zero last entry.
					//
					if (Index == MM_UNLOADED_DRIVERS_SIZE - 1)
					{
						RtlFillMemory(Entry, sizeof(MM_UNLOADED_DRIVER), 0);
					}
				}
				else if (RtlEqualUnicodeString(DriverName, &Entry->Name, TRUE))
				{
					//DbgPrint("\n\nERASING DRIVER ENTRY: %S", Entry->Name);
					//
					// Erase driver entry.
					//
					PVOID BufferPool = Entry->Name.Buffer;
					RtlFillMemory(Entry, sizeof(MM_UNLOADED_DRIVER), 0);
					ExFreePoolWithTag(BufferPool, 'TDmM');

					//
					// Because we are erasing last entry we want to set MmLastUnloadedDriver to 49
					// if list have been already filled.
					//
					*MmLastUnloadedDriver = (Filled ? MM_UNLOADED_DRIVERS_SIZE : *MmLastUnloadedDriver) - 1;
					Modified = TRUE;
				}
			}

			if (Modified)
			{
				ULONG64 PreviousTime = 0;

				//
				// Make UnloadTime look right.
				//
				for (LONG Index = MM_UNLOADED_DRIVERS_SIZE - 2; Index >= 0; --Index)
				{
					PMM_UNLOADED_DRIVER Entry = &MmUnloadedDrivers[Index];
					if (IsUnloadedDriverEntryEmpty(Entry))
					{
						continue;
					}

					if (PreviousTime != 0 && Entry->UnloadTime > PreviousTime)
					{
						//
						// Decrease by random value here maybe.
						//
						Entry->UnloadTime = PreviousTime - 100;
					}

					PreviousTime = Entry->UnloadTime;
				}

				//
				// Clear remaining entries.
				//
				ClearUnloadedDriver(DriverName, FALSE);
			}

			if (AccquireResource)
			{
				ExReleaseResourceLite(&PsLoadedModuleResource);
			}

			return Modified ? STATUS_SUCCESS : STATUS_NOT_FOUND;
		}
	}

	//https://www.unknowncheats.me/forum/anti-cheat-bypass/428976-removing-mapped-driver-systembigpoolinformation.html
	/*
	namespace BigPool {
		bool FindPoolTable(UINT64* pPoolBigPageTable, UINT64* pPoolBigPageTableSize)
		{
			auto Pat1 = skCrypt("\x48\x8B\x15\x00\x00\x00\x00\x4C\x8D\x0D\x00\x00\x00\x00\x4C");
			auto Mask1 = skCrypt("xxx????xxx????x");
			PVOID ExProtectPoolExCallInstructionsAddress = (PVOID)MmUnloadedDrivers::FindPattern((UINT64)cleaner::PiDDB::g_KernelBase, cleaner::PiDDB::g_KernelSize, (BYTE*)Pat1.decrypt(), Mask1.decrypt());
			Pat1.clear();
			Mask1.clear();

			if (!ExProtectPoolExCallInstructionsAddress)
				return false;

			PVOID ExProtectPoolExAddress = MmUnloadedDrivers::ResolveRelativeAddress(ExProtectPoolExCallInstructionsAddress, 3, 5);

			if (!ExProtectPoolExAddress)
				return false;

			PVOID PoolBigPageTableInstructionAddress = (PVOID)((ULONG64)ExProtectPoolExAddress + 0x95);
			*pPoolBigPageTable = (UINT64)MmUnloadedDrivers::ResolveRelativeAddress(PoolBigPageTableInstructionAddress, 3, 7);

			PVOID PoolBigPageTableSizeInstructionAddress = (PVOID)((ULONG64)ExProtectPoolExAddress + 0x8E);
			*pPoolBigPageTableSize = (UINT64)MmUnloadedDrivers::ResolveRelativeAddress(PoolBigPageTableSizeInstructionAddress, 3, 7);

			return true;
		}

		bool RemoveFromBigPool(UINT64 Address)
		{
			UINT64 pPoolBigPageTable = 0;
			UINT64 pPoolBigPageTableSize = 0;

			if (FindPoolTable(&pPoolBigPageTable, &pPoolBigPageTableSize))
			{
				PPOOL_TRACKER_BIG_PAGES PoolBigPageTable = 0;
				RtlCopyMemory(&PoolBigPageTable, (PVOID)pPoolBigPageTable, 8);
				SIZE_T PoolBigPageTableSize = 0;
				RtlCopyMemory(&PoolBigPageTableSize, (PVOID)pPoolBigPageTableSize, 8);

				for (int i = 0; i < PoolBigPageTableSize; i++)
				{
					if (PoolBigPageTable[i].Va == Address || PoolBigPageTable[i].Va == (Address + 0x1))
					{
						PoolBigPageTable[i].Va = 0x1;
						PoolBigPageTable[i].NumberOfBytes = 0x0;
						return true;
					}
				}

				return true;
			}
			DbgPrint("ERROR: FindingPoolTable\n");
			return false;
		}
	}
	*/
}


/*
NTSTATUS FindMmDriverData( VOID )
		{
PVOID MmUnloadedDriversInstr = NULL;
UCHAR MmUnloadedDrivers_sig[] = "\x4C\x8B\x15\xCC\xCC\xCC\xCC\x4C\x8B\xC9";
if (!NT_SUCCESS(cleaner::PiDDB::scan_section("PAGE", MmUnloadedDrivers_sig, 0xCC, sizeof(MmUnloadedDrivers_sig) - 1, (&MmUnloadedDriversInstr)))) {
	return FALSE;
}

PVOID MmLastUnloadedDriverInstr = NULL;
UCHAR MmLastUnloadedDriver_sig[] = "\x8B\x05\xCC\xCC\xCC\xCC\x83\xF8\x32";
if (!NT_SUCCESS(cleaner::PiDDB::scan_section("PAGE", MmLastUnloadedDriver_sig, 0xCC, sizeof(MmLastUnloadedDriver_sig) - 1, (&MmLastUnloadedDriverInstr)))) {
	return FALSE;
}

if (MmUnloadedDriversInstr == NULL || MmLastUnloadedDriverInstr == NULL)
{
	return STATUS_NOT_FOUND;
}

MmUnloadedDrivers = *(PMM_UNLOADED_DRIVER*)ResolveRelativeAddress(MmUnloadedDriversInstr, 3, 7);
MmLastUnloadedDriver = (PULONG)ResolveRelativeAddress(MmLastUnloadedDriverInstr, 2, 6);

return STATUS_SUCCESS;
		}
*/