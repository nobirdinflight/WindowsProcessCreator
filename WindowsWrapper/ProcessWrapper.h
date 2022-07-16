#pragma once
#include<cstdint>
#include<string>
#include <vector>
uint32_t createProcessDefault(const std::wstring& path);
uint32_t createProcessWithSettings(const std::wstring& path, size_t parent_pid);
uint32_t enumeratePIDs(std::vector<size_t>& pids);
size_t getCurrentPID();
uint32_t getProcessImagePathFromPID(size_t pid, std::wstring& path);