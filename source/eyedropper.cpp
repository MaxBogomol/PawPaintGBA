#include "eyedropper.h"

#include "paint.h"
#include "language.h"

#include "eyedropper_icon.h"

const char* Eyedropper::getName(Paint& paint) {
    return STR_EYEDROPPER.c_str();
}

void Eyedropper::setup(Paint& paint) {
    line = 0;
    cursorX = 0;
    cursorY = 0;
    cursorXOld = 0;
    cursorYOld = 0;
    active = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Eyedropper::update(Paint& paint) {
    if (keysD & KEY_B) {
        u16 color = paint.selectedColorSub;
        paint.selectedColorSub = paint.selectedColor;
        paint.selectedColor = color;
        if (!paint.reverseScreens) paint.updateDrawColors = true;
    }

    if (paint.reverseScreens) {
        if ((keysR & KEY_LEFT) && (cursorX - 1 >= 0)) {
            cursorX--;
            updateDrawCursor = true;
        }
        if ((keysR & KEY_RIGHT) && (cursorX + 1 < SCREEN_WIDTH)) {
            cursorX++;
            updateDrawCursor = true;
        }
        if ((keysR & KEY_UP) && (cursorY - 1 >= 0)) {
            cursorY--;
            updateDrawCursor = true;
        }
        if ((keysR & KEY_DOWN) && (cursorY + 1 < SCREEN_HEIGHT)) {
            cursorY++;
            updateDrawCursor = true;
        }
        if (keysH & KEY_A) {
            u16 color = paint.getPixel(cursorX, cursorY, pixelBufferCanvas);
            if (((color >> 15) & 1) == 0) color = blackColor;
            paint.selectedColor = color;
            paint.updateDrawColors = true;
        }
    }

    if (updateDrawCursor) {
        drawCursor(paint);
        updateDrawCursor = false;
    }
}

void Eyedropper::open(Paint& paint) {
    line = 0;
    active = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Eyedropper::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 14, pixelBufferMain);

    active = false;
    drawCursor(paint);
}

void Eyedropper::reverse(Paint& paint) {
    updateDrawCursor = true;
}

void Eyedropper::redraw(Paint& paint) {
    updateDrawTool = true;
}

void Eyedropper::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, eyedropper_iconBitmap, pixelBufferMain);
}

void Eyedropper::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    paint.drawBButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
}

void Eyedropper::drawCursor(Paint& paint, bool clear) {
    paint.blendLayers(cursorXOld - 2, cursorYOld - 2, 5, 5);
    if (!clear) {
        paint.drawSquareOutline(cursorX - 2, cursorY - 2, 5, 5, pixelBufferMain, blackColor);
        paint.drawSquareOutline(cursorX - 1, cursorY - 1, 3, 3, pixelBufferMain, whiteColor);
    }

    cursorXOld = cursorX;
    cursorYOld = cursorY;
}

void Eyedropper::drawCursor(Paint& paint) {
    drawCursor(paint, false);
}