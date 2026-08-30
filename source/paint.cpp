#include "paint.h"

#include <string>

#include "language.h"
#include "pawscript.h"

#include "pawscript_font_ascii.h"
#include "pawscript_font_latin.h"
#include "pawscript_font_latin_extended.h"
#include "pawscript_font_greek.h"
#include "pawscript_font_greek_extended.h"
#include "pawscript_font_cyrillic.h"
#include "pawscript_font_cyrillic_extended.h"
#include "pawscript_font_hebrew.h"
#include "paint_icon.h"
#include "paint_monochrome_icon.h"
#include "buttons_icon.h"

void Paint::setup() {
    firstFrameTool = true;
    updateLayers = false;

    selectedTheme = 0;
    selectedIcon = 0;
    selectedLayer = 0;
    selectedTool = 0;
    selectedColor = blackColor;
    selectedColorSub = whiteColor;
    reverseScreens = false;

    updateDrawAll = false;
    updateDrawTools = true;
    updateDrawColors = true;
    updateDrawHints = true;
    updateDrawPaintName = true;
    updateDrawPaintIcon = true;

    setRepeat(10, 2);

    readSelectedLanguage();
    setPaintName(STR_UNNAMED.c_str());
}

void Paint::setupVideo() {
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    irqInit();
    irqEnable(IRQ_VBLANK);
}

void Paint::setupLayers() {
    clearBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pixelBufferMain);
    clearBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pixelBufferCanvas, whiteColor);
}

void Paint::setupTools() {
    brush.setup(*this);
    eraser.setup(*this);
    eyedropper.setup(*this);
    colorPicker.setup(*this);
    saving.setup(*this);
    settings.setup(*this);
    info.setup(*this);

    tools.push_back(&brush);
    tools.push_back(&eraser);
    tools.push_back(&eyedropper);
    tools.push_back(&colorPicker);
    tools.push_back(&saving);
    tools.push_back(&settings);
    tools.push_back(&info);
}

void Paint::updateInputs() {
    scanKeys();
    keysD = keysDown();
    keysH = keysHeld();
    keysR = keysDownRepeat();
    keysU = keysUp();
}

void Paint::updateTools() {
    int selectedToolOld = selectedTool;
    bool toolChanged = false;

    if (keysD & KEY_START) {
        reverseScreens = !reverseScreens;
        if (reverseScreens) {
            dmaCopy(pixelBufferCanvas, pixelBufferMain, sizeof(pixelBufferCanvas));
        } else {
            updateDrawAll = true;
        }
        tools[selectedTool]->reverse(*this);
    }

    if (!reverseScreens) {
        if (keysD & KEY_L) {
            selectedTool--;
            if (selectedTool < 0) selectedTool = tools.size() - 1;
            toolChanged = true;
        }
        if (keysD & KEY_R) {
            selectedTool++;
            if (selectedTool > (int) tools.size() - 1) selectedTool = 0;
            toolChanged = true;
        }
    }

    if (toolChanged) {
        tools[selectedToolOld]->close(*this);
        tools[selectedTool]->open(*this);
        updateDrawTools = true;
        updateDrawHints = true;
    }

    if (firstFrameTool) {
        tools[selectedTool]->open(*this);
        firstFrameTool = false;
    }

    if (updateDrawAll) {
        clearBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pixelBufferMain);
        updateDrawTools = true;
        updateDrawColors = true;
        updateDrawHints = true;
        updateDrawPaintName = true;
        updateDrawPaintIcon = true;
        updateDrawAll = false;
        tools[selectedTool]->redraw(*this);
    }

    tools[selectedTool]->update(*this);
}

void Paint::updateVideo() {
    if (!reverseScreens) {
        if (updateDrawTools) {
            drawTools();
            updateDrawTools = false;
        }

        if (updateDrawColors) {
            drawColors();
            updateDrawColors = false;
        }

        if (updateDrawHints) {
            drawHints();
            updateDrawHints = false;
        }

        if (updateDrawPaintName) {
            drawPaintName();
            updateDrawPaintName = false;
        }

        if (updateDrawPaintIcon) {
            drawPaintIcon();
            updateDrawPaintIcon = false;
        }

        tools[selectedTool]->updateTool(*this);
    }

    VBlankIntrWait();
    dmaCopy(pixelBufferMain, videoMemory, sizeof(pixelBufferMain));
}

void Paint::drawTools() {
    clearBuffer(0, 0, SCREEN_WIDTH, getToolsYOffset(), pixelBufferMain);

    int i = 0;
    int j = 0;
    for (int t = 0; t < (int) tools.size(); t++) {
        tools[t]->drawIcon(*this, 4 + (i * 18), 3 + (j * 18), pixelBufferMain);
        if (t == selectedTool) drawSquareOutline(3 + (i * 18), 2 + (j * 18), 18, 18, pixelBufferMain, blackColor);
        i++;
        if (i >= 13) {
            i = 0;
            j++;
        }
    }

    string toolString = string(STR_TOOL) + ": " + tools[selectedTool]->getName(*this); 
    drawText(3, getToolYOffset(), toolString.c_str(), pixelBufferMain, blackColor);
}

void Paint::drawColors() {
    clearBuffer(2, SCREEN_HEIGHT - 36, 90, 34, pixelBufferMain);

    drawSquareOutline(2, SCREEN_HEIGHT - 36, 18, 34, pixelBufferMain, blackColor);

    drawSquare(3, SCREEN_HEIGHT - 35, 16, 16, pixelBufferMain, selectedColor);
    int r = (selectedColor) & 31;
    int g = (selectedColor >> 5) & 31;
    int b = (selectedColor >> 10) & 31;
    string colorString = string("RGB: ") + intToChars(r) + " " + intToChars(g) + " " + intToChars(b); 
    drawText(21, SCREEN_HEIGHT - 31, colorString.c_str(), pixelBufferMain, blackColor);

    drawSquare(3, SCREEN_HEIGHT - 19, 16, 16, pixelBufferMain, selectedColorSub);
    int rs = (selectedColorSub) & 31;
    int gs = (selectedColorSub >> 5) & 31;
    int bs = (selectedColorSub >> 10) & 31;
    string colorSubString = string("RGB: ") + intToChars(rs) + " " + intToChars(gs) + " " + intToChars(bs); 
    drawText(21, SCREEN_HEIGHT - 15, colorSubString.c_str(), pixelBufferMain, blackColor);
}

void Paint::drawHints() {
    int xOffset = 100;
    int yOffset = SCREEN_HEIGHT - 34;
    clearBuffer(xOffset, yOffset, 80, 30, pixelBufferMain);
    drawLButton(xOffset, yOffset, pixelBufferMain);
    drawRButton(xOffset += 10, yOffset, pixelBufferMain);
    drawStartButton(xOffset += 10, yOffset, pixelBufferMain);
    xOffset = 100;
    yOffset += 10;
    tools[selectedTool]->drawHints(*this, xOffset, yOffset, pixelBufferMain);
}

void Paint::drawPaintName() {
    clearBuffer(0, SCREEN_HEIGHT - 49, SCREEN_WIDTH, 12, pixelBufferMain);
    drawText(3, SCREEN_HEIGHT - 46, getPaintName(), pixelBufferMain, blackColor);
}

void Paint::drawPaintIcon() {
    clearBuffer(SCREEN_WIDTH - 32 - 3, SCREEN_HEIGHT - 32 - 3, 32, 32, pixelBufferMain);
    drawSprite(SCREEN_WIDTH - 32 - 3, SCREEN_HEIGHT - 32 - 3, 32, 32, getSelectedIconSprite(), pixelBufferMain);
}

void Paint::blendLayers(int x, int y) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        pixelBufferMain[x + (y * SCREEN_WIDTH)] = pixelBufferCanvas[x + (y * SCREEN_WIDTH)];
    }
}

void Paint::updateLayersEnable() {
    updateLayers = true;
}

void Paint::updateLayersDisable() {
    updateLayers = false;
}

u16 Paint::getSelectedColor() {
	return selectedColor;
}

u16 Paint::getPixel(int x, int y, u16* buffer) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		return buffer[x + (y * SCREEN_WIDTH)];
	}
    return blackColor;
}

void Paint::drawPixel(int x, int y, u16* buffer, u16 color) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		if (getPixel(x, y, buffer) != color) {
            buffer[x + (y * SCREEN_WIDTH)] = color;
		    if (updateLayers) blendLayers(x, y);
        }
	}
}

void Paint::drawSquare(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
	for (int x = 0; x < x1; x++) {
		for (int y = 0; y < y1; y++) {
			drawPixel(x0 + x, y0 + y, buffer, color);
		}
	}
}

void Paint::drawSquareOutline(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    x1 = x1 - 1;
    y1 = y1 - 1;
	drawLine(x0, y0, x0 + x1, y0, buffer, color);
    drawLine(x0, y0 + y1, x0 + x1, y0 + y1, buffer, color);
    drawLine(x0, y0, x0, y0 + y1, buffer, color);
    drawLine(x0 + x1, y0, x0 + x1, y0 + y1, buffer, color);
}

void Paint::drawSquareNoise(int x0, int y0, int x1, int y1, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset) {
	for (int x = 0; x < x1; x++) {
		for (int y = 0; y < y1; y++) {
            int threshold = getDitherThreshold(x0 + x + xOffset, y0 + y + yOffset, xSize, ySize, xShift, yShift);
			if (threshold <= 0) drawPixel(x0 + x, y0 + y, buffer, color);
		}
	}
}

void Paint::drawCircleRadius(int xc, int yc, int r, u16* buffer, u16 color) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                drawPixel(xc + x, yc + y, buffer, color);
            }
        }
    }
}

void Paint::drawCircleRadiusNoise(int xc, int yc, int r, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                int threshold = getDitherThreshold(xc + x + xOffset, yc + y + yOffset, xSize, ySize, xShift, yShift);
                if (threshold <= 0) drawPixel(xc + x, yc + y, buffer, color);
            }
        }
    }
}

void Paint::drawCircleDiameter(int xc, int yc, int d, u16* buffer, u16 color) {
    int d2 = d * d;
    int offset = (d - 1);
    int center = d / 2; 
    for (int y = 0; y < d; y++) {
        for (int x = 0; x < d; x++) {
            int dx = 2 * x - offset;
            int dy = 2 * y - offset;
            if (dx * dx + dy * dy <= d2) {
                drawPixel(xc + x - center, yc + y - center, buffer, color);
            }
        }
    }
}

void Paint::drawCircleDiameterNoise(int xc, int yc, int d, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset) {
    int d2 = d * d;
    int offset = (d - 1);
    int center = d / 2; 
    for (int y = 0; y < d; y++) {
        for (int x = 0; x < d; x++) {
            int dx = 2 * x - offset;
            int dy = 2 * y - offset;
            if (dx * dx + dy * dy <= d2) {
                int threshold = getDitherThreshold(xc + x - center + xOffset, yc + y - center + yOffset, xSize, ySize, xShift, yShift);
                if (threshold <= 0) drawPixel(xc + x - center, yc + y - center, buffer, color);
            }
        }
    }
}

void Paint::drawLine(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
		drawPixel(x0, y0, buffer, color);

        if (x0 == x1 && y0 == y1) break;
        
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

u32 Paint::decodeChar(const char** c) {
    const unsigned char* p = (const unsigned char*)*c;
    u32 code = 0;

    if (p[0] < 0x80) {
        code = p[0];
        *c += 1;
    } else if (p[0] < 0xE0) {
        code = ((p[0] & 0x1F) << 6) | (p[1] & 0x3F);
        *c += 2;
    } else if (p[0] < 0xF0) {
        code = ((p[0] & 0x0F) << 12) | ((p[1] & 0x3F) << 6) | (p[2] & 0x3F);
        *c += 3;
    }
    return code;
}

int Paint::getCharLength(u32 c) {
    int listSize = sizeof(pawscriptCharLengthList) / sizeof(pawscriptCharLengthList[0]);
    for (int i = 0; i < listSize; i++) {
        if (c == (u32) pawscriptCharLengthList[i][0]) return (int) pawscriptCharLengthList[i][1];
    }
    return 6;
}

int Paint::getTextLength(const char* text) {
    const char* textPtr = text;
    int l = 0;

    while (*textPtr) {
        u32 charCode = decodeChar(&textPtr);
        l += getCharLength(charCode);
    }
    return l;
}

void Paint::drawChar(int x, int y, u32 c, u16* buffer, u16 color) {
    const u16* pixels = (const u16*) pawscript_font_asciiBitmap;
    int index = (int) c;
    bool extended = false;

    if (c >= 0x00C0 && c <= 0x017F) {
        int latinSize = sizeof(pawscriptLatinList) / sizeof(pawscriptLatinList[0]);
        for (int i = 0; i < latinSize; i++) {
            if (c == pawscriptLatinList[i]) {
                index = i;
                break;
            }
        }
        int latinExtendedSize = sizeof(pawscriptLatinExtendedList) / sizeof(pawscriptLatinExtendedList[0]);
        for (int i = 0; i < latinExtendedSize; i++) {
            if (c == pawscriptLatinExtendedList[i]) {
                index = i;
                extended = true;
                break;
            }
        }
        
        if (extended) {
            pixels = (const u16*) pawscript_font_latin_extendedBitmap;
        } else {
            pixels = (const u16*) pawscript_font_latinBitmap;
        }
    }

    if (c >= 0x0370 && c <= 0x03FF) {
        int greekSize = sizeof(pawscriptGreekList) / sizeof(pawscriptGreekList[0]);
        for (int i = 0; i < greekSize; i++) {
            if (c == pawscriptGreekList[i]) {
                index = i;
                break;
            }
        }
        int greekExtendedSize = sizeof(pawscriptGreekExtendedList) / sizeof(pawscriptGreekExtendedList[0]);
        for (int i = 0; i < greekExtendedSize; i++) {
            if (c == pawscriptGreekExtendedList[i]) {
                index = i;
                extended = true;
                break;
            }
        }
        
        if (extended) {
            pixels = (const u16*) pawscript_font_greek_extendedBitmap;
        } else {
            pixels = (const u16*) pawscript_font_greekBitmap;
        }
    }

    if (c >= 0x0400 && c <= 0x04FF) {
        int cyrillicSize = sizeof(pawscriptCyrillicList) / sizeof(pawscriptCyrillicList[0]);
        for (int i = 0; i < cyrillicSize; i++) {
            if (c == pawscriptCyrillicList[i]) {
                index = i;
                break;
            }
        }
        int cyrillicExtendedSize = sizeof(pawscriptCyrillicExtendedList) / sizeof(pawscriptCyrillicExtendedList[0]);
        for (int i = 0; i < cyrillicExtendedSize; i++) {
            if (c == pawscriptCyrillicExtendedList[i]) {
                index = i;
                extended = true;
                break;
            }
        }

        if (extended) {
            pixels = (const u16*) pawscript_font_cyrillic_extendedBitmap;
        } else {
            pixels = (const u16*) pawscript_font_cyrillicBitmap;
        }
    }

    if (c >= 0x0590 && c <= 0x05FF) {
        int hebrewSize = sizeof(pawscriptHebrewList) / sizeof(pawscriptHebrewList[0]);
        for (int i = 0; i < hebrewSize; i++) {
            if (c == pawscriptHebrewList[i]) {
                index = i;
                break;
            }
        }

        pixels = (const u16*) pawscript_font_hebrewBitmap;
    }

    int xSize = extended ? 9 : 8;
    int ySize = extended ? 12 : 8;
    int spriteWidth = extended ? 144 : 128;
    int yOffset = extended ? 3 : 0;

    for (int row = 0; row < ySize; row++) {
        for (int col = 0; col < xSize; col++) {
            int xx = ((index % 16) * xSize) + col;
            int yy = ((index / 16) * ySize) + row;

            u16 pixel = pixels[xx + (yy * spriteWidth)];

            if (pixel & BIT(15)) { 
                int px = x + col;
                int py = y + row - yOffset;
                
                if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                    buffer[py * SCREEN_WIDTH + px] = color;
                }
            }
        }
    }
}

void Paint::drawText(int x, int y, const char* text, u16* buffer, u16 color) {
    const char* textPtr = text;

    while (*textPtr) {
        u32 charCode = decodeChar(&textPtr);

        drawChar(x, y, charCode, buffer, color);
        x += getCharLength(charCode);
    }
}

void Paint::drawCharOutline(int x, int y, u32 c, u16* buffer, u16 color, u16 outlineColor) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            drawChar(x + dx, y + dy, c, buffer, outlineColor);
        }
    }
    drawChar(x, y, c, buffer, color);
}

void Paint::drawTextOutline(int x, int y, const char* text, u16* buffer, u16 color, u16 outlineColor) {
    const char* textPtr = text;

    while (*textPtr) {
        u32 charCode = decodeChar(&textPtr);

        drawCharOutline(x, y, charCode, buffer, color, outlineColor);
        x += getCharLength(charCode);
    }
}

void Paint::drawSprite(int x0, int y0, int x1, int y1, int xShift, int yShift, int xSize, int ySize, const unsigned int* spriteBitmap, u16* buffer) {
    const u16* pixels = (const u16*) spriteBitmap;

    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            u16 color = pixels[x + xShift + ((y + yShift) * x1)];

            if (color & BIT(15)) {
                drawPixel(x0 + x, y0 + y, buffer, color);
            }
        }
    }
}

void Paint::drawSprite(int x0, int y0, int x1, int y1, const unsigned int* spriteBitmap, u16* buffer) {
    drawSprite(x0, y0, x1, y1, 0, 0, x1, y1, spriteBitmap, buffer);
}

u16 Paint::blendColors(u16 src, u16 dst) {
	u8 alpha = (dst >> 15) & 1;
	if (alpha == 1) {
		return dst;
	}
	return src;
}

u16 Paint::HSVtoRGB(int h, int s, int v) {
	int r = 0, g = 0, b = 0;
    
    if (s == 0) {
        r = g = b = v;
    } else {
        int region = h / 60;
        int remainder = (h % 60) * 255 / 60;

        int p = (v * (255 - s)) >> 8;
        int q = (v * (255 - ((s * remainder) >> 8))) >> 8;
        int t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

        switch (region) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default: r = v; g = p; b = q; break;
        }
    }

    return ARGB16(1, r >> 3, g >> 3, b >> 3);
}

u16 Paint::HSVtoRGB(HSV hsv) {
    return HSVtoRGB(hsv.h, hsv.s, hsv.v);
}

HSV Paint::RGBtoHSV(u16 color) {
    int r5 = (color) & 0x1F;
    int g5 = (color >> 5) & 0x1F;
    int b5 = (color >> 10) & 0x1F;

    int r = (r5 * 255) / 31;
    int g = (g5 * 255) / 31;
    int b = (b5 * 255) / 31;

    int maxV = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
    int minV = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
    int delta = maxV - minV;

    HSV res;
    res.v = maxV;

    if (maxV == 0) {
        res.s = 0;
        res.h = 0;
        return res;
    }
    res.s = (255 * delta) / maxV;

    if (delta == 0) {
        res.h = 0;
    } else {
        if (maxV == r) {
            res.h = 60 * (g - b) / delta;
        } else if (maxV == g) {
            res.h = 120 + 60 * (b - r) / delta;
        } else {
            res.h = 240 + 60 * (r - g) / delta;
        }

        if (res.h < 0) res.h += 360;
    }

    return res;
}

int Paint::getDitherThreshold(int x, int y, int xSize, int ySize, int xShift, int yShift) {
    int xOffset = ((y / ySize) % 2 == 1) ? xShift : 0;
    int yOffset = ((x / xSize) % 2 == 1) ? yShift : 0;
    return ((x + xOffset) % xSize) + ((y + yOffset) % ySize);
}

const char* Paint::intToChars(int val) {
    static char buf[12];
    sprintf(buf, "%d", val);
    return buf;
}

void Paint::setPaintName(const char* name) {
    paintName = name;
}

const char* Paint::getPaintName() {
    return paintName;
}

void Paint::clearBuffer(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    for (int x = 0; x < x1; x++) {
        for (int y = 0; y < y1; y++) {
            drawPixel(x0 + x, y0 + y, buffer, color);
        }
    }
}

void Paint::clearBuffer(int x0, int y0, int x1, int y1, u16* buffer) {
    clearBuffer(x0, y0, x1, y1, buffer, getSelectedThemeColor());
}

int Paint::getToolYOffset() {
    return 24;
}

int Paint::getToolsYOffset() {
    return getToolYOffset() + 12;
}

int Paint::getToolsButtonsOffset() {
    return 8;
}

void Paint::drawUpButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 0, 0, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawRightButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawDownButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 16, 0, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawLeftButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawNoneButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 0, 8, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawUpDownButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 8, 8, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawLeftRightButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 16, 8, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawAllButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 24, 8, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawBButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 0, 24, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawAButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 8, 24, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawLButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 16, 16, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawRButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 24, 16, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawStartButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 16, 24, 8, 8, buttons_iconBitmap, buffer);
}

void Paint::drawSelectButton(int x, int y, u16* buffer) {
    drawSprite(x, y, 32, 32, 24, 24, 8, 8, buttons_iconBitmap, buffer);
}

u16 Paint::getThemeColor(int theme) {
    switch (theme) {
        case 0: return whiteColor; break;
        case 1: return pinkFoxThemeColor; break;
        case 2: return maidThemeColor; break;
        case 3: return aceThemeColor; break;
    }
	return whiteColor;
}

u16 Paint::getSelectedThemeColor() {
	return getThemeColor(selectedTheme);
}

const unsigned int* Paint::getIconSprite(int icon) {
    switch (icon) {
        case 0: return paint_iconBitmap; break;
        case 1: return paint_monochrome_iconBitmap; break;
    }
	return paint_iconBitmap;
}

const unsigned int* Paint::getSelectedIconSprite() {
	return getIconSprite(selectedIcon);
}

const char* Paint::getLanguageCode(int language) {
    if (language >= 0 && language < (int) sizeof(languageCodes)) {
        return languageCodes[language];
    }
	return "en_us";
}

const char* Paint::getSelectedLanguageCode() {
	return getLanguageCode(selectedLanguage);
}

bool Paint::readSelectedLanguage() {
    //string path = string("languagesPath") + "/" + getSelectedLanguageCode() + ".ini";
    return readLanguage("");
}