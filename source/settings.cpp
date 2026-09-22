#include "settings.h"

#include "paint.h"
#include "language.h"

#include "buttons_icon.h"
#include "settings_icon.h"

const char* Settings::getName(Paint& paint) {
    return STR_SETTINGS.c_str();
}

void Settings::setup(Paint& paint) {
    line = 0;
    updateTheme = false;
    updateDrawTool = true;
}

void Settings::update(Paint& paint) {
    if (updateTheme) {
        updateDrawTool = true;
        updateTheme = false;
    }

    int maxLine = 4;

    if ((keysD & KEY_UP) && (line - 1 >= 0)) {
        line--;
        updateDrawTool = true;
        paint.updateDrawHints = true;
    }
    if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
        line++;
        updateDrawTool = true;
        paint.updateDrawHints = true;
    }

    switch (line) {
        case 0: {
            if (keysD & KEY_LEFT) {
                paint.selectedTheme--;
                if (paint.selectedTheme < 0) paint.selectedTheme = maxPaintThemes - 1;
                paint.updateDrawAll = true;
                updateTheme = true;
            }
            if (keysD & KEY_RIGHT) {
                paint.selectedTheme++;
                if (paint.selectedTheme > maxPaintThemes - 1) paint.selectedTheme = 0;
                paint.updateDrawAll = true;
                updateTheme = true;
            }
            break;
        }
        case 1: {
            if (keysD & KEY_LEFT) {
                paint.selectedIcon--;
                if (paint.selectedIcon < 0) paint.selectedIcon = maxPaintIcons - 1;
                paint.updateDrawPaintIcon = true;
                updateDrawTool = true;
            }
            if (keysD & KEY_RIGHT) {
                paint.selectedIcon++;
                if (paint.selectedIcon > maxPaintIcons - 1) paint.selectedIcon = 0;
                paint.updateDrawPaintIcon = true;
                updateDrawTool = true;
            }
            break;
        }
        case 2: {
            if (keysD & KEY_LEFT) {
                paint.selectedLanguage--;
                if (paint.selectedLanguage < 0) paint.selectedLanguage = maxLanguages - 1;
                paint.updateDrawTools = true;
                updateTheme = true;
                paint.readSelectedLanguage();
            }
            if (keysD & KEY_RIGHT) {
                paint.selectedLanguage++;
                if (paint.selectedLanguage > maxLanguages - 1) paint.selectedLanguage = 0;
                paint.updateDrawTools = true;
                updateTheme = true;
                paint.readSelectedLanguage();
            }
            break;
        }
        case 3: {
            if (keysD & KEY_A) {
                setDefaultSetting(paint);
                paint.updateDrawAll = true;
                paint.readSelectedLanguage();
            }
            break;
        }
    }
}

void Settings::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Settings::open(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Settings::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 4 * 13 + 3, pixelBufferMain);
}

void Settings::redraw(Paint& paint) {
    updateDrawTool = true;
}

void Settings::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, settings_iconBitmap, pixelBufferMain);
}

void Settings::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    paint.drawUpDownButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    if (line < 3) {
        paint.drawLeftRightButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    } else {
        paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    }
}

void Settings::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 4 * 13 + 3, pixelBufferMain);

    string themeString = string((line == 0) ? ">" : "") + STR_SETTINGS_THEME + ": " + getThemeName(paint, paint.selectedTheme);
    paint.drawText(3, yOffset, themeString.c_str(), pixelBufferMain, blackColor);

    string iconString = string((line == 1) ? ">" : "") + STR_SETTINGS_ICON + ": " + getIconName(paint, paint.selectedIcon);
    paint.drawText(3, yOffset += 13, iconString.c_str(), pixelBufferMain, blackColor);

    string languageString = string((line == 2) ? ">" : "") + STR_SETTINGS_LANGUAGE + ": " + STR_LANGUAGE;
    paint.drawText(3, yOffset += 13, languageString.c_str(), pixelBufferMain, paint.fileSystemInit ? blackColor : grayColor);

    string defaultString = string((line == 3) ? ">" : "") + STR_SETTINGS_DEFAULT;
    paint.drawText(3, yOffset += 13, defaultString.c_str(), pixelBufferMain, blackColor);
}

const char* Settings::getThemeName(Paint& paint, int theme) {
    switch (theme) {
        case 0: return STR_THEME_WHITE.c_str(); break;
        case 1: return STR_THEME_PINK_FOX.c_str(); break;
        case 2: return STR_THEME_MAID.c_str(); break;
        case 3: return STR_THEME_ACE.c_str(); break;
    }
    return STR_THEME_WHITE.c_str();
}

const char* Settings::getIconName(Paint& paint, int icon) {
    switch (icon) {
        case 0: return STR_ICON_PRIDE.c_str(); break;
        case 1: return STR_ICON_MONOCHROME.c_str(); break;
    }
    return STR_ICON_PRIDE.c_str();
}

void Settings::setDefaultSetting(Paint& paint) {
    paint.selectedTheme = 0;
    paint.selectedIcon = 0;
    paint.selectedLanguage = 0;
}