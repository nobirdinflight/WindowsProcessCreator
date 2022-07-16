#include "ProcessWrapper.h"
#include <Windows.h>
#include <memory>
uint32_t createProcess(const std::wstring& path)
{
	auto appname = std::make_unique<wchar_t[]>(path.size() + 1);
	std::copy(path.begin(), path.end(), appname.get());
	STARTUPINFOW startup_info{ .cb = sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION process_infomation{};
	CreateProcessW(appname.get(), nullptr, nullptr, nullptr, false, NULL, nullptr, nullptr, &startup_info, &process_infomation);
	CloseHandle(process_infomation.hProcess);
	CloseHandle(process_infomation.hThread);
	return uint32_t();
}
