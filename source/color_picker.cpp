#include "color_picker.h"

#include "paint.h"
#include "language.h"

#include "color_picker_icon.h"

const char* ColorPicker::getName(Paint& paint) {
    return STR_COLOR_PICKER.c_str();
}

void ColorPicker::setup(Paint& paint) {
    line = 0;
    hue = 0;
    hueOld = 0;
    colorX = 0;
    colorY = 0;
    colorXOld = 0;
    colorYOld = 0;
    selectedColor = blackColor;
    newSelectedColor = blackColor;
    active = false;
    updateDrawTool = true;
    updatePicker = false;
    updateHue = false;
    updateSelected = false;
    updateNewSelected = false;
}

void ColorPicker::update(Paint& paint) {
    if (!paint.reverseScreens) {
        if (!active) {
            if (keysD & KEY_A) {
                active = true;

                paint.clearBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pixelBufferMain);

                drawHue(paint);
                drawOutlines(paint);

                updatePicker = true;
                updateHue = true;
                updateSelected = true;
                updateNewSelected = true;
            }
        } else {
            bool setNewColor = false;

            if (keysD & KEY_A) {
                selectedColor = newSelectedColor;
                paint.selectedColor = newSelectedColor;
                updateSelected = true;
            }

            if (keysH & KEY_B) {
                if (keysR & KEY_UP) {
                    hue--;
                    if (hue < 0) hue = 359;
                    updatePicker = true;
                    updateHue = true;
                    updateNewSelected = true;
                    setNewColor = true;
                }
                if (keysR & KEY_DOWN) {
                    hue++;
                    if (hue >= 360) hue = 0;
                    updatePicker = true;
                    updateHue = true;
                    updateNewSelected = true;
                    setNewColor = true;
                }
            } else {
                if ((keysR & KEY_LEFT) && (colorX - 1 >= 0)) {
                    colorX--;
                    updatePicker = true;
                    updateNewSelected = true;
                    setNewColor = true;
                }
                if ((keysR & KEY_RIGHT) && (colorX + 1 < 32)) {
                    colorX++;
                    updatePicker = true;
                    updateNewSelected = true;
                    setNewColor = true;
                }
                if ((keysR & KEY_UP) && (colorY - 1 >= 0)) {
                    colorY--;
                    updatePicker = true;
                    updateNewSelected = true;
                    setNewColor = true;
                }
                if ((keysR & KEY_DOWN) && (colorY + 1 < 32)) {
                    colorY++;
                    updatePicker = true;
                    updateNewSelected = true;
                    setNewColor = true;
                }
            }

            if (setNewColor) {
                newSelectedColor = paint.HSVtoRGB(hue, colorX * 8, 255 - (colorY * 8));
            }

            if (keysD & KEY_SELECT) {
                active = false;
                paint.updateDrawAll = true;
            }
        }

        if (updatePicker) {
            drawPicker(paint);
            drawPickerPointers(paint);
            updatePicker = false;
        }

        if (updateHue) {
            drawHuePointer(paint);
            updateHue = false;
        }

        if (updateSelected) {
            drawSelectedColor(paint);
            updateSelected = false;
        }

        if (updateNewSelected) {
            drawNewSelectedColor(paint);
            updateNewSelected = false;
        }
    }
}

void ColorPicker::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void ColorPicker::open(Paint& paint) {
    selectedColor = paint.selectedColor;
    newSelectedColor = paint.selectedColor;

    HSV hsv = paint.RGBtoHSV(selectedColor);
    hue = hsv.h;
    colorX = hsv.s / 8;
    colorY = (255 - hsv.v) / 8;
    hueOld = hue;
    colorXOld = colorX;
    colorYOld = colorY;

    updateDrawTool = true;
}

void ColorPicker::close(Paint& paint) {
    if (active) {
        paint.updateDrawAll = true;
    }

    active = false;
}

void ColorPicker::reverse(Paint& paint) {
    if (active) {
        paint.updateDrawAll = true;
    }

    active = false;
}

void ColorPicker::redraw(Paint& paint) {
    updateDrawTool = true;
    if (active) {
        updatePicker = true;
        updateHue = true;
        updateSelected = true;
        updateNewSelected = true;
    }
}

void ColorPicker::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, color_picker_iconBitmap, buffer);
}

void ColorPicker::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
}

void ColorPicker::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 64, pixelBufferMain);

    string moveString = string((line == 0) ? ">" : "") + STR_COLOR_PICKER_COLOR + ": " + ((line == 0 && active) ? "+" : "-"); 
    paint.drawText(3, yOffset, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
}

void ColorPicker::drawPicker(Paint& paint) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            u16 color = paint.HSVtoRGB(hue, x * 8, 255 - (y * 8));
            paint.drawSquare(x * 4 + 56, y * 4 + 16, 4, 4, pixelBufferMain, color);
        }
    }
}

void ColorPicker::drawHue(Paint& paint) {
    for (int y = 0; y < 120; y++) {
        u16 color = paint.HSVtoRGB(y, 255, 255);
        paint.drawSquare(201, y + 20, 4, 1, pixelBufferMain, color);
    }
    for (int y = 0; y < 120; y++) {
        u16 color = paint.HSVtoRGB(y + 120, 255, 255);
        paint.drawSquare(213, y + 20, 4, 1, pixelBufferMain, color);
    }
    for (int y = 0; y < 120; y++) {
        u16 color = paint.HSVtoRGB(y + 240, 255, 255);
        paint.drawSquare(225, y + 20, 4, 1, pixelBufferMain, color);
    }
}

void ColorPicker::drawSelectedColor(Paint& paint) {
    paint.drawSquare(20, 80, 16, 16, pixelBufferMain, selectedColor);
    clearSelectedColor(paint);

    int r = (selectedColor) & 31;
    int g = (selectedColor >> 5) & 31;
    int b = (selectedColor >> 10) & 31;

    string colorString = string("") + paint.intToChars(r) + " " + paint.intToChars(g) + " " + paint.intToChars(b); 
    paint.drawTextOutline(4, 148, colorString.c_str(), pixelBufferMain, blackColor, whiteColor);
}

void ColorPicker::drawNewSelectedColor(Paint& paint) {
    paint.drawSquare(20, 64, 16, 16, pixelBufferMain, newSelectedColor);
    clearNewSelectedColor(paint);

    int r = (newSelectedColor) & 31;
    int g = (newSelectedColor >> 5) & 31;
    int b = (newSelectedColor >> 10) & 31;

    string colorString = string("") + paint.intToChars(r) + " " + paint.intToChars(g) + " " + paint.intToChars(b); 
    paint.drawTextOutline(4, 4, colorString.c_str(), pixelBufferMain, blackColor, whiteColor);
}

void ColorPicker::drawOutlines(Paint& paint) {
    paint.drawSquareOutline(19, 63, 18, 34, pixelBufferMain, blackColor);
    paint.drawSquareOutline(55, 15, 130, 130, pixelBufferMain, blackColor);
    paint.drawSquareOutline(200, 19, 6, 122, pixelBufferMain, blackColor);
    paint.drawSquareOutline(212, 19, 6, 122, pixelBufferMain, blackColor);
    paint.drawSquareOutline(224, 19, 6, 122, pixelBufferMain, blackColor);
}

void ColorPicker::drawPickerPointers(Paint& paint) {
    clearPickerPointers(paint);

    paint.drawSquare(colorX * 4 + 55, 8, 6, 6, pixelBufferMain, blackColor);
    paint.drawSquare(colorX * 4 + 56, 9, 4, 4, pixelBufferMain, whiteColor);

    paint.drawSquare(colorX * 4 + 55, 146, 6, 6, pixelBufferMain, blackColor);
    paint.drawSquare(colorX * 4 + 56, 147, 4, 4, pixelBufferMain, whiteColor);

    paint.drawSquare(48, colorY * 4 + 15, 6, 6, pixelBufferMain, blackColor);
    paint.drawSquare(49, colorY * 4 + 16, 4, 4, pixelBufferMain, whiteColor);

    paint.drawSquare(186, colorY * 4 + 15, 6, 6, pixelBufferMain, blackColor);
    paint.drawSquare(187, colorY * 4 + 16, 4, 4, pixelBufferMain, whiteColor);

    paint.drawSquareOutline(colorX * 4 + 55, colorY * 4 + 15, 6, 6, pixelBufferMain, blackColor);

    colorXOld = colorX;
    colorYOld = colorY;
}

void ColorPicker::drawHuePointer(Paint& paint) {
    clearHuePointer(paint);

    int x = 0;
    int y = hue;
    if (hue >= 120 && hue < 240) {
        y = y - 120;
        x = x + 12;
    }
    if (hue >= 240) {
        y = y - 240;
        x = x + 36;
    }
    paint.drawSquareOutline(x + 195, y + 19, 4, 3, pixelBufferMain, blackColor);
    paint.drawSquare(x + 195 + 1, y + 20, 2, 1, pixelBufferMain, whiteColor);
    if (hue >= 120 && hue < 240) {
        paint.drawSquareOutline(x + 195 + 12, y + 19, 4, 3, pixelBufferMain, blackColor);
        paint.drawSquare(x + 195 + 1 + 12, y + 20, 2, 1, pixelBufferMain, whiteColor);
    }

    hueOld = hue;
}

void ColorPicker::clearSelectedColor(Paint& paint) {
    paint.clearBuffer(3, 147, 48, 10, pixelBufferMain);
}

void ColorPicker::clearNewSelectedColor(Paint& paint) {
    paint.clearBuffer(3, 3, 48, 10, pixelBufferMain);
}

void ColorPicker::clearPickerPointers(Paint& paint) {
    paint.clearBuffer(colorXOld * 4 + 55, 8, 6, 6, pixelBufferMain);
    paint.clearBuffer(colorXOld * 4 + 55, 146, 6, 6, pixelBufferMain);

    paint.clearBuffer(48, colorYOld * 4 + 15, 6, 6, pixelBufferMain);
    paint.clearBuffer(186, colorYOld * 4 + 15, 6, 6, pixelBufferMain);
}

void ColorPicker::clearHuePointer(Paint& paint) {
    int x = 0;
    int y = hueOld;
    if (hueOld >= 120 && hueOld < 240) {
        y = y - 120;
        x = x + 12;
    }
    if (hueOld >= 240) {
        y = y - 240;
        x = x + 36;
    }
    paint.clearBuffer(x + 195, y + 19, 4, 3, pixelBufferMain);
    if (hue >= 120 && hue < 240) {
        paint.clearBuffer(x + 195 + 12, y + 19, 4, 3, pixelBufferMain);
    }
}