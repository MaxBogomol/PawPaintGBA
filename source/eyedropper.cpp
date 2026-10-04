#include "eyedropper.h"

#include "paint.h"
#include "language.h"

#include "eyedropper_icon.h"

const char* Eyedropper::getName(Paint& paint) {
    return STR_EYEDROPPER.c_str();
}

void Eyedropper::setup(Paint& paint) {
    updateDrawCursor = false;
}

void Eyedropper::update(Paint& paint) {
    if (keysD & KEY_B) {
        u16 color = paint.secondColor;
        paint.secondColor = paint.selectedColor;
        paint.selectedColor = color;
        if (!paint.reverseScreens) paint.updateDrawColors = true;
    }

    if (paint.reverseScreens) {
        if ((keysR & KEY_LEFT) && (paint.cursorX - 1 >= 0)) {
            paint.cursorX--;
            updateDrawCursor = true;
        }
        if ((keysR & KEY_RIGHT) && (paint.cursorX + 1 < SCREEN_WIDTH)) {
            paint.cursorX++;
            updateDrawCursor = true;
        }
        if ((keysR & KEY_UP) && (paint.cursorY - 1 >= 0)) {
            paint.cursorY--;
            updateDrawCursor = true;
        }
        if ((keysR & KEY_DOWN) && (paint.cursorY + 1 < SCREEN_HEIGHT)) {
            paint.cursorY++;
            updateDrawCursor = true;
        }
        if (keysH & KEY_A) {
            u16 color = paint.getPixel(paint.cursorX, paint.cursorY, pixelBufferCanvas);
            if (((color >> 15) & 1) == 0) color = blackColor;
            paint.selectedColor = color;
            paint.updateDrawColors = true;
        }

        if (updateDrawCursor) {
            drawCursor(paint);
            updateDrawCursor = false;
        }
    }
}

void Eyedropper::open(Paint& paint) {
    updateDrawCursor = false;
}

void Eyedropper::reverse(Paint& paint) {
    updateDrawCursor = true;
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
    paint.blendLayers(paint.cursorXOld - 2, paint.cursorYOld - 2, 5, 5);
    if (!clear) {
        paint.drawSquareOutline(paint.cursorX - 2, paint.cursorY - 2, 5, 5, pixelBufferMain, blackColor);
        paint.drawSquareOutline(paint.cursorX - 1, paint.cursorY - 1, 3, 3, pixelBufferMain, whiteColor);
    }

    paint.cursorXOld = paint.cursorX;
    paint.cursorYOld = paint.cursorY;
}

void Eyedropper::drawCursor(Paint& paint) {
    drawCursor(paint, false);
}