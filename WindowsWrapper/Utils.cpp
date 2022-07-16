#include "Utils.h"

auto convertWStringToBuffer(const std::wstring& source) -> std::unique_ptr<wchar_t[]>
{
	auto buffer = std::make_unique<wchar_t[]>(source.size() + 1);
	std::copy(source.cbegin(), source.cend(), buffer.get());
	return buffer;
}
