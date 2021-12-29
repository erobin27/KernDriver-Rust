#include "ClearCache.h"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING reg_path)
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(reg_path);
	//set hook
	if (!nullhook::call_kernel_function(&nullhook::hook_handler))DbgPrint("ERROR: Hooking\n");
	//clear PiDDB
	cleaner::PiDDB::clean_piddbcachetable();
	//clear mmUnloadedDrivers
	cleaner::MmUnloadedDrivers::FindMmDriverData();
	UNICODE_STRING DriverName = RTL_CONSTANT_STRING(L"iqvw64e.sys"); //iqvw64e.sys(kdmapper): 0x5284EAC3
	cleaner::MmUnloadedDrivers::ClearUnloadedDriver(&DriverName, TRUE);

	//clear bigpool
	//cleaner::BigPool::RemoveFromBigPool();

	return STATUS_SUCCESS;
}