#pragma once

#include <string>

using namespace std;

#define STRING(what, def) extern string STR_##what;
#include "language.inl"
#undef STRING

bool readLanguage(const char* path);