#pragma once
#include <memory>
#include <string>
auto convertWStringToBuffer(const std::wstring& source)->std::unique_ptr<wchar_t[]>;