#pragma once
#ifndef _SERIAL_EXT_H_
#define _SERIAL_EXT_H_

#include <cstdint>
#include <type_traits>

enum SaveUse : uint8_t { kSave, kGetSaveSize };
template <SaveUse Use> struct generic_save;
template <> struct generic_save<kSave> { using type = void*; };
template <> struct generic_save<kGetSaveSize> { using type = uintptr_t; };
template <SaveUse Use> using generic_save_t = typename generic_save<Use>::type;

template <SaveUse Use, typename T>
auto genericSave(T& toSave, generic_save_t<Use>& offset) -> std::enable_if_t<Use == kSave, generic_save_t<Use>>
{
	return offset + sizeof(T);
}
template <SaveUse Use, typename T>
constexpr auto genericSave(const T& toSave, const generic_save_t<Use> offset) -> std::enable_if_t<Use == kGetSaveSize, generic_save_t<Use>>
{
	return sizeof(T);
}

#endif // _SERIAL_EXT_H_
