#pragma once
typedef struct _UncStr
{
	char stub[0x10];
	int len;
	wchar_t str[1];
} *pUncStr;


std::string RPMString(DWORD64 address)
{
	if (!(void*)address)
		return std::string("BOT");

	std::string nameString;
	char name[30];
	memcpy(name, (void*)address, 30);
	for (int i = 0; i < sizeof(name); i++) {
		if (!name[i])
			break;
		if ((int)name[i] >= 32 && (int)name[i] <= 126)
			nameString += name[i];
		else
			break;
	}
	return nameString;
}