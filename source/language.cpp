#include "language.h"

#include <gba.h>

#define STRING(what, def) string STR_##what;
#include "language.inl"
#undef STRING

string getString(FILE* &fp, const string &item, const string &defaultValue) {
    return defaultValue;
}

bool readLanguage(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        #define STRING(what, def) STR_##what = def;
        #include "language.inl"
        #undef STRING
        return false;
    }

    #define STRING(what, def) STR_##what = getString(fp, ""#what, def);
    #include "language.inl"
    #undef STRING

    return true;
}