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
            paint.updateDrawHints = true;
        }   
    } else {
        if (keysD & KEY_A) {
            showPage = false;
            updateDrawTool = true;
            paint.updateDrawPaintName = true;
            paint.updateDrawHints = true;
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
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 4 * 13 + 3, pixelBufferMain);
    paint.clearBuffer(SCREEN_WIDTH - bOffset - 72, yOffset + 10, 72, 72, pixelBufferMain);
    paint.updateDrawPaintName = true;
}

void Info::redraw(Paint& paint) {
    showPage = false;
    updateDrawTool = true;
}

void Info::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, info_iconBitmap, pixelBufferMain);
}

void Info::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    if (!showPage) paint.drawUpDownButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
}

void Info::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 4 * 13 + 3, pixelBufferMain);
    paint.clearBuffer(SCREEN_WIDTH - bOffset - 72, yOffset + 10, 72, 72, pixelBufferMain);

    if (!showPage) {
        string aboutString = string((line == 0) ? ">" : "") + STR_INFO_ABOUT.c_str();
        paint.drawText(3, yOffset, aboutString.c_str(), pixelBufferMain, blackColor);

        string fluffyVillageString = string((line == 1) ? ">" : "") + "The Fluffy Village";
        paint.drawText(3, yOffset += 13, fluffyVillageString.c_str(), pixelBufferMain, blackColor);

        string gitHubString = string((line == 2) ? ">" : "") + "GitHub";
        paint.drawText(3, yOffset += 13, gitHubString.c_str(), pixelBufferMain, blackColor);

        string discordString = string((line == 3) ? ">" : "") + "Discord";
        paint.drawText(3, yOffset += 13, discordString.c_str(), pixelBufferMain, blackColor);
    } else {
        switch (page) {
            case 0: {
                string pageString = string(">") + STR_INFO_ABOUT.c_str();
                paint.drawText(3, yOffset, pageString.c_str(), pixelBufferMain, blackColor);

                string paintString = string("Paw Paint GBA") + " - " + paintVerstion;
                paint.drawText(3, yOffset += 13, paintString.c_str(), pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 13, STR_DESCRIPTION.c_str(), pixelBufferMain, blackColor);
                string byString = STR_INFO_BY + " MaxBogomol (Pink Joke)";
                paint.drawText(3, yOffset += 13, byString.c_str(), pixelBufferMain, blackColor);
                break;
            }
            case 1: {
                paint.drawText(3, yOffset, ">The Fluffy Village", pixelBufferMain, blackColor);

                paint.drawSprite(SCREEN_WIDTH - bOffset - 72, yOffset += 13, 72, 72, qrcode_fluffyvillageBitmap, pixelBufferMain);

                paint.drawText(3, yOffset, "https://fluffy-village.dev/", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 13, "pages/eng/creations/", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 13, "paw_paint.html", pixelBufferMain, blackColor);
                break;
            }
            case 2: {
                paint.drawText(3, yOffset, ">GitHub", pixelBufferMain, blackColor);

                paint.drawSprite(SCREEN_WIDTH - bOffset - 64, yOffset += 13, 64, 64, qrcode_githubBitmap, pixelBufferMain);

                paint.drawText(3, yOffset, "https://github.com/MaxBogomol/", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 13, "PawPaintGBA", pixelBufferMain, blackColor);
                break;
            }
            case 3: {
                paint.drawText(3, yOffset, ">Discord", pixelBufferMain, blackColor);

                paint.drawSprite(SCREEN_WIDTH - bOffset - 64, yOffset += 13, 64, 64, qrcode_discordBitmap, pixelBufferMain);

                paint.drawText(3, yOffset, "https://discord.fluffy-village", pixelBufferMain, blackColor);
                paint.drawText(3, yOffset += 13, ".dev/", pixelBufferMain, blackColor);
                break;
            }
        }
    }
}