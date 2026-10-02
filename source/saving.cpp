#include "saving.h"

#include "paint.h"
#include "language.h"

#include "saving_icon.h"
#include "saving_error_icon.h"
#include "saving_loading_icon.h"
#include "saving_done_icon.h"

const char* Saving::getName(Paint& paint) {
    return STR_SAVING.c_str();
}

void Saving::setup(Paint& paint) {
    line = 0;
    loading = false;
    saving = false;
    updateDrawTool = true;
}

void Saving::update(Paint& paint) {
    if (!paint.reverseScreens) {
        int maxLine = 3;

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

        if (keysD & KEY_A) {
            switch (line) {
                case 0: {
                    paint.clearBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pixelBufferCanvas, paint.secondColor);
                }
            }
        }

        if (doneTimer > 0)  {
            doneTimer--;
            paint.updateDrawTools = true;
        }
    }
}

void Saving::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Saving::open(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Saving::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 3 * 13 + 3, pixelBufferMain);
    doneTimer = 0;
}

void Saving::redraw(Paint& paint) {
    updateDrawTool = true;
}

void Saving::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    const unsigned int* iconSprite = saving_iconBitmap;
    if (loading || saving) {
        iconSprite = saving_loading_iconBitmap;
    } else if (doneTimer > 0) {
        iconSprite = saving_done_iconBitmap;
    }
    paint.drawSprite(x, y, 16, 16, iconSprite, buffer);
}

void Saving::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    paint.drawUpDownButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
}

void Saving::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 3 * 13 + 3, pixelBufferMain);

    yOffset -= 13;

    string newString = string((line == 0) ? ">" : "") + STR_SAVING_NEW;
    paint.drawText(3, yOffset += 13, newString.c_str(), pixelBufferMain, blackColor);

    string loadString = string((line == 1) ? ">" : "") + STR_SAVING_LOAD;
    paint.drawText(3, yOffset += 13, loadString.c_str(), pixelBufferMain, grayColor);

    string saveString = string((line == 2) ? ">" : "") + STR_SAVING_SAVE;
    paint.drawText(3, yOffset += 13, saveString.c_str(), pixelBufferMain, grayColor);
}