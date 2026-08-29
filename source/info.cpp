#include "info.h"

#include "paint.h"
#include "language.h"

#include "info_icon.h"
#include "qrcode_github.h"
#include "qrcode_fluffyvillage.h"
#include "qrcode_discord.h"

const char* Info::getName(Paint& paint) {
    return STR_INFO.c_str();
}

void Info::setup(Paint& paint) {
    line = 0;
    page = 0;
    showPage = false;
    updateDrawTool = true;
}

void Info::update(Paint& paint) {
    int maxLine = 4;

    if (!showPage) {
        if ((keysD & KEY_UP) && (line - 1 >= 0)) {
            line--;
            updateDrawTool = true;
        }
        if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
            line++;
            updateDrawTool = true;
        }
        if (keysD & KEY_A) {
            page = line;
            showPage = true;
            updateDrawTool = true;
        }   
    } else {
        if ((keysD & KEY_A) || (keysD & KEY_B)) {
            showPage = false;
            updateDrawTool = true;
        }
    }
}

void Info::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Info::open(Paint& paint) {
    updateDrawTool = true;
}

void Info::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 92, pixelBufferMain);
}

void Info::redraw(Paint& paint) {
    updateDrawTool = true;
}

void Info::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, info_iconBitmap, pixelBufferMain);
}

void Info::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 91, pixelBufferMain);

    if (!showPage) {
        string aboutString = string((line == 0) ? ">" : "") + STR_INFO_ABOUT.c_str();
        paint.drawText(3, yOffset, aboutString.c_str(), pixelBufferMain, blackColor);
        paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

        string fluffyVillageString = string((line == 1) ? ">" : "") + "The Fluffy Village";
        paint.drawText(3, yOffset += 10, fluffyVillageString.c_str(), pixelBufferMain, blackColor);
        paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

        string gitHubString = string((line == 2) ? ">" : "") + "GitHub";
        paint.drawText(3, yOffset += 10, gitHubString.c_str(), pixelBufferMain, blackColor);
        paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

        string discordString = string((line == 3) ? ">" : "") + "Discord";
        paint.drawText(3, yOffset += 10, discordString.c_str(), pixelBufferMain, blackColor);
        paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
    } else {
        switch (page) {
            case 0: {
                string pageString = string(">") + STR_INFO_ABOUT.c_str();
                paint.drawText(3, yOffset, pageString.c_str(), pixelBufferMain, blackColor);
                paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

                string paintString = string("Paw Paint GBA") + " - " + paintVerstion;
                paint.drawText(3, yOffset += 10, paintString.c_str(), pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 10, STR_DESCRIPTION.c_str(), pixelBufferMain, blackColor);
                string byString = STR_INFO_BY + " MaxBogomol (Pink Joke)";
                paint.drawText(3, yOffset += 10, byString.c_str(), pixelBufferMain, blackColor);
                break;
            }
            case 1: {
                paint.drawText(3, yOffset, ">The Fluffy Village", pixelBufferMain, blackColor);
                paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

                paint.drawSprite(SCREEN_WIDTH - bOffset - 72, yOffset += 10, 72, 72, qrcode_fluffyvillageBitmap, pixelBufferMain);

                paint.drawText(3, yOffset, "https://fluffy-village.dev/", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 10, "pages/eng/creations/", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 10, "paw_paint.html", pixelBufferMain, blackColor);
                break;
            }
            case 2: {
                paint.drawText(3, yOffset, ">GitHub", pixelBufferMain, blackColor);
                paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

                paint.drawSprite(SCREEN_WIDTH - bOffset - 64, yOffset += 10, 64, 64, qrcode_githubBitmap, pixelBufferMain);

                paint.drawText(3, yOffset, "https://github.com/MaxBogomol/", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 10, "PawPaintGBA", pixelBufferMain, blackColor);
                break;
            }
            case 3: {
                paint.drawText(3, yOffset, ">Discord", pixelBufferMain, blackColor);
                paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

                paint.drawSprite(SCREEN_WIDTH - bOffset - 64, yOffset += 10, 64, 64, qrcode_discordBitmap, pixelBufferMain);

                paint.drawText(3, yOffset, "https://discord.fluffy-village", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 10, ".dev/", pixelBufferMain, blackColor);
                break;
            }
        }
    }
}