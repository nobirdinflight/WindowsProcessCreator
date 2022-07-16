#include "ProcessWrapper.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include "Utils.h"
uint32_t createProcessDefault(const std::wstring& path)
{
	uint32_t error_code = ERROR_SUCCESS;
	auto appname = convertWStringToBuffer(path);
	STARTUPINFOW startup_info{ .cb = sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION process_infomation{};
	CreateProcessW(appname.get(), nullptr, nullptr, nullptr, false, NULL, nullptr, nullptr, &startup_info, &process_infomation);
	CloseHandle(process_infomation.hProcess);
	CloseHandle(process_infomation.hThread);
	return error_code;
}


uint32_t createProcessWithSettings(const std::wstring& path, size_t parent_pid)
{
	uint32_t error_code = ERROR_SUCCESS;
	auto appname = convertWStringToBuffer(path);
	STARTUPINFOEXW startup_info_ex{ sizeof(STARTUPINFOEXW) };
	size_t attribute_list_size{ 0 };
	InitializeProcThreadAttributeList(NULL, 1, 0, &attribute_list_size);
	auto attribute_list_pointer = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attribute_list_size);
	if (InitializeProcThreadAttributeList(attribute_list_pointer, 1, 0, &attribute_list_size)) {
		if (auto process = OpenProcess(PROCESS_ALL_ACCESS, false, parent_pid))
		{
			if (UpdateProcThreadAttribute(attribute_list_pointer, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &process, sizeof(HANDLE), NULL, NULL))
			{
				startup_info_ex.lpAttributeList = attribute_list_pointer;
				PROCESS_INFORMATION process_infomation{};
				if (CreateProcessW(appname.get(), nullptr, nullptr, nullptr, false, EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr, &startup_info_ex.StartupInfo, &process_infomation))
				{
					CloseHandle(process_infomation.hProcess);
					CloseHandle(process_infomation.hThread);
					return error_code;
				}
				error_code = GetLastError();
				return error_code;
			}
			error_code = GetLastError();
			CloseHandle(process);
			return error_code;
		}
	}

	error_code = GetLastError();
	return error_code;
}
uint32_t enumeratePIDs(std::vector<size_t>& pids)
{
	uint32_t error_code = ERROR_SUCCESS;
	if (auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0))
	{
		PROCESSENTRY32W process_entry{ .dwSize = sizeof(PROCESSENTRY32W) };
		if (Process32FirstW(snapshot, &process_entry))
		{
			do
			{
				pids.push_back(process_entry.th32ProcessID);
			} while (Process32NextW(snapshot, &process_entry));
		}
		error_code = GetLastError();
		CloseHandle(snapshot);
		return error_code;
	}
	return error_code;
}

size_t getCurrentPID()
{
	return GetCurrentProcessId();
}

uint32_t getProcessImagePathFromPID(const size_t pid, std::wstring& path)
{
	uint32_t error_code = ERROR_SUCCESS;
	auto path_buffer = std::make_unique<wchar_t[]>(MAX_PATH);
	if (auto process = OpenProcess(PROCESS_ALL_ACCESS, false, pid))
	{
		unsigned long length = MAX_PATH;
		if (QueryFullProcessImageNameW(process, 0, path_buffer.get(), &length))
		{
			path = path_buffer.get();
		}
		CloseHandle(process);
	}
	return uint32_t();
}

