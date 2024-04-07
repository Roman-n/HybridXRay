#pragma once

enum class ELocalization
{
    EN,
    RU
};

struct SEnglishStringToRussify
{
    const char* String;
};

struct SLocalizedString
{
    const char* StringEN = nullptr;
    const char* StringRU = nullptr;
};
