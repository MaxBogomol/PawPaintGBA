#include "brush.h"

#include "paint.h"
#include "language.h"

#include "buttons_icon.h"
#include "brush_icon.h"
#include "brush_square_icon.h"
#include "brush_circle_icon.h"
#include "brush_dot_icon.h"
#include "brush_square_noise_icon.h"
#include "brush_circle_noise_icon.h"
#include "brush_dot_noise_icon.h"

const char* Brush::getName(Paint& paint) {
    return STR_BRUSH.c_str();
}

void Brush::setup(Paint& paint) {
    type = 0;
    line = 0;
    squareSize = 1;
    circleDiameter = 1;
    dotRadius = 1;
    noiseXSize = 2;
    noiseYSize = 2;
    noiseXShift = 0;
    noiseYShift = 0;
    noiseXOffset = 0;
    noiseYOffset = 0;
    cursorX = 0;
    cursorY = 0;
    cursorXOld = 0;
    cursorYOld = 0;
    active = false;
    activeNoise = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Brush::update(Paint& paint) {

}

void Brush::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Brush::open(Paint& paint) {
    line = 0;
    active = false;
    activeNoise = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Brush::close(Paint& paint) {
    //int yOffset = paint.getToolsYOffset();
    //paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 62, pixelBufferMain);

    active = false;
    drawCursor(paint);
}

void Brush::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    const unsigned int* iconSprite = brush_square_iconBitmap;
    switch (type) {
    	case 0: iconSprite = brush_square_iconBitmap; break;
    	case 1: iconSprite = brush_circle_iconBitmap; break;
		case 2: iconSprite = brush_dot_iconBitmap; break;
		case 3: iconSprite = brush_square_noise_iconBitmap; break;
    	case 4: iconSprite = brush_circle_noise_iconBitmap; break;
		case 5: iconSprite = brush_dot_noise_iconBitmap; break;
    }
    paint.drawSprite(x, y, 16, 16, iconSprite, buffer);
}

void Brush::drawLine(Paint& paint, int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        switch (type) {
            case 0: {
                int center = squareSize / 2;
                paint.drawSquare(x0 - center, y0 - center, squareSize, squareSize, buffer, color);
                break;
            }
            case 1: {
                paint.drawCircleDiameter(x0, y0, circleDiameter, buffer, color);
                break;
            }
            case 2: {
                paint.drawCircleRadius(x0, y0, dotRadius - 1, buffer, color);
                break;
            }
            case 3: {
                int center = squareSize / 2;
                paint.drawSquareNoise(x0 - center, y0 - center, squareSize, squareSize, buffer, color, noiseXSize, noiseYSize, noiseXShift, noiseYShift, noiseXOffset, noiseYOffset);
                break;
            }
            case 4: {
                paint.drawCircleDiameterNoise(x0, y0, circleDiameter, buffer, color, noiseXSize, noiseYSize, noiseXShift, noiseYShift, noiseXOffset, noiseYOffset);
                break;
            }
            case 5: {
                paint.drawCircleRadiusNoise(x0, y0, dotRadius - 1, buffer, color, noiseXSize, noiseYSize, noiseXShift, noiseYShift, noiseXOffset, noiseYOffset);
                break;
            }
        }

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

void Brush::drawTool(Paint& paint) {

}

void Brush::drawCursor(Paint& paint, bool clear) {
    int size = 0;
    switch (type) {
        case 0:
    	case 3: size = squareSize; break;
        case 1:
    	case 4: size = circleDiameter; break;
        case 2:
    	case 5: size = dotRadius * 2; break;
    }
    size = size + 2;


    //if (active && !clear) drawLine(paint, cursorX, cursorY, cursorX, cursorY, pixelBufferSub, getSelectedColor(paint));

    cursorXOld = cursorX;
    cursorYOld = cursorY;
}

void Brush::drawCursor(Paint& paint) {
    drawCursor(paint, false);
}

const char* Brush::getTypeName(Paint& paint, int type) {
	switch (type) {
    	case 0: return STR_BRUSH_TYPE_SQUARE.c_str();
    	case 1: return STR_BRUSH_TYPE_CIRLCE.c_str();
		case 2: return STR_BRUSH_TYPE_DOT.c_str();
		case 3: return STR_BRUSH_TYPE_SQUARE_NOISE.c_str();
    	case 4: return STR_BRUSH_TYPE_CIRLCE_NOISE.c_str();
		case 5: return STR_BRUSH_TYPE_DOT_NOISE.c_str();
    }
	return "Type";
}

u16 *Brush::getSelectedLayer(Paint& paint) {
    return pixelBufferMain;//paint.getSelectedLayer();
}

u16 Brush::getSelectedColor(Paint& paint) {
    return paint.getSelectedColor();
}