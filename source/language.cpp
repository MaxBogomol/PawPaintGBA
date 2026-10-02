#include "language.h"

#include <gba.h>

#define STRING(what, def) string STR_##what;
#include "language.inl"
#undef STRING

bool readLanguage() {
    #define STRING(what, def) STR_##what = def;
    #include "language.inl"
    #undef STRING

    return true;
}