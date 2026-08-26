#include "paint.h"

#include <string>

void Paint::setupVideo() {
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
    irqInit();
    irqEnable(IRQ_VBLANK);
}

void Paint::setupLayers() {
    clearBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pixelBufferMain, whiteColor);
    clearBuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pixelBufferCanvas, whiteColor);
}

void Paint::updateVideo() {
    drawCircleDiameter(20, 20, 10, pixelBufferMain, blackColor);
    VBlankIntrWait();
    dmaCopy(pixelBufferMain, vid_mem, sizeof(pixelBufferMain));
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
		    //if (updateSubLayers) blendSubLayers(x, y);
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

void Paint::clearBuffer(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    for (int x = 0; x < x1; x++) {
        for (int y = 0; y < y1; y++) {
            drawPixel(x0 + x, y0 + y, buffer, color);
        }
    }
}

void Paint::clearBuffer(int x0, int y0, int x1, int y1, u16* buffer) {
    //clearBuffer(x0, y0, x1, y1, buffer, getSelectedThemeColor());
}