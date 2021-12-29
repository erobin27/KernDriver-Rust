#pragma once
#include "includes.h"

extern uintptr_t base_address;
extern std::uint32_t process_id;
typedef struct _NULL_MEMORY
{
	void* buffer_address;
	UINT_PTR address;
	ULONGLONG size;
	ULONG pid;
	BOOLEAN write;
	BOOLEAN read;
	BOOLEAN req_base;
	void* output;
	const char* module_name;
	ULONG64 base_address;
}NULL_MEMORY;

class Memory;
class Memory {
public:
	std::uint32_t get_process_id(std::string_view process_name);
	ULONG64 get_module_base_address(const char* module_name);
	template <class T>
	T Read(UINT_PTR read_address);
	std::string ReadString(UINT_PTR read_address, SIZE_T size);
	template<typename S>
	bool write(UINT_PTR write_address, const S& value);

private:
	template<typename ... Arg>
	uint64_t call_hook(const Arg ... args);
	struct HandleDisposer;
	bool write_memory(UINT_PTR write_address, UINT_PTR source_address, SIZE_T write_size);
};

template <class T>
inline T Memory::Read(UINT_PTR read_address) {
	T response{};
	NULL_MEMORY instructions;
	instructions.pid = process_id;
	instructions.size = sizeof(T);
	instructions.address = read_address;
	instructions.read = TRUE;
	instructions.write = FALSE;
	instructions.req_base = FALSE;
	instructions.output = &response;
	call_hook(&instructions);

	return response;
}

inline std::string Memory::ReadString(UINT_PTR read_address, SIZE_T size) {
	std::string response{};
	NULL_MEMORY instructions;
	instructions.pid = process_id;
	instructions.size = size;
	instructions.address = read_address;
	instructions.read = TRUE;
	instructions.write = FALSE;
	instructions.req_base = FALSE;
	instructions.output = &response;
	call_hook(&instructions);

	return response;
}

template<typename S>
inline bool Memory::write(UINT_PTR write_address, const S& value)
{
	return write_memory(write_address, (UINT_PTR)&value, sizeof(S));
}
extern Memory* mem;