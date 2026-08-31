#include "eyedropper.h"

#include "paint.h"
#include "language.h"

#include "eyedropper_icon.h"

const char* Eyedropper::getName(Paint& paint) {
    return STR_EYEDROPPER.c_str();
}

void Eyedropper::update(Paint& paint) {
    if (!paint.reverseScreens) {
        if (keysD & KEY_B) {
            u16 color = paint.selectedColorSub;
            paint.selectedColorSub = paint.selectedColor;
            paint.selectedColor = color;
            paint.updateDrawColors = true;
        }
    }
}

void Eyedropper::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, eyedropper_iconBitmap, pixelBufferMain);
}

void Eyedropper::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = 0;
    int yOffset = 0;
    paint.drawBButton(x + xOffset, y + yOffset, pixelBufferMain);
}