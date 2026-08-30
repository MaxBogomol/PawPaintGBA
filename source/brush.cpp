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
    active = false;
    activeNoise = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Brush::update(Paint& paint) {
    if (!paint.reverseScreens) {
        if (keysD & KEY_A) {
            if (line >= 2) {
                active = !active;
                activeNoise = false;
                updateDrawTool = true;
            }
        }

        int maxLine = 2;
        if (type >= 3) maxLine = 5;

        if (!active) {
            if ((keysD & KEY_UP) && (line - 1 >= 0)) {
                line--;
                updateDrawTool = true;
            }
            if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
                line++;
                updateDrawTool = true;
            }
        }

        switch (line) {
            case 0: {
                if (keysD & KEY_LEFT) {
                    type--;
                    if (type < 0) type = 5;
                    updateDrawTool = true;
                    paint.updateDrawTools = true;
                }
                if (keysD & KEY_RIGHT) {
                    type++;
                    if (type > 5) type = 0;
                    updateDrawTool = true;
                    paint.updateDrawTools = true;
                }
                break;
            }
            case 1: {
                switch (type) {
                    case 0:
                    case 3: {
                        if ((keysR & KEY_LEFT) && (squareSize - 1 >= 1)) {
                            squareSize--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (squareSize + 1 <= 64)) {
                            squareSize++;
                            updateDrawTool = true;
                        }
                        break;
                    }
                    case 1:
                    case 4: {
                        if ((keysR & KEY_LEFT) && (circleDiameter - 1 >= 1)) {
                            circleDiameter--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (circleDiameter + 1 <= 64)) {
                            circleDiameter++;
                            updateDrawTool = true;
                        }
                        break;
                    }
                    case 2:
                    case 5: {
                        if ((keysR & KEY_LEFT) && (dotRadius - 1 >= 1)) {
                            dotRadius--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (dotRadius + 1 <= 32)) {
                            dotRadius++;
                            updateDrawTool = true;
                        }
                        break;
                    }
                }
                break;
            }
        }

        if (type >= 3) {
            if (active) {
                if ((keysD & KEY_UP) || (keysD & KEY_DOWN)) {
                    activeNoise = !activeNoise;
                    updateDrawTool = true;
                }

                switch (line) {
                    case 2: {
                        if (!activeNoise) {
                            if ((keysR & KEY_LEFT) && (noiseXSize - 1 >= 1)) {
                                noiseXSize--;
                                updateDrawTool = true;
                            }
                            if ((keysR & KEY_RIGHT) && (noiseXSize + 1 <= 16)) {
                                noiseXSize++;
                                updateDrawTool = true;
                            }
                        } else {
                            if ((keysR & KEY_LEFT) && (noiseYSize - 1 >= 1)) {
                                noiseYSize--;
                                updateDrawTool = true;
                            }
                            if ((keysR & KEY_RIGHT) && (noiseYSize + 1 <= 16)) {
                                noiseYSize++;
                                updateDrawTool = true;
                            }
                        }
                        break;
                    }
                    case 3: {
                        if (!activeNoise) {
                            if ((keysR & KEY_LEFT) && (noiseXShift - 1 >= 0)) {
                                noiseXShift--;
                                updateDrawTool = true;
                            }
                            if ((keysR & KEY_RIGHT) && (noiseXShift + 1 < 16)) {
                                noiseXShift++;
                                updateDrawTool = true;
                            }
                        } else {
                            if ((keysR & KEY_LEFT) && (noiseYShift - 1 >= 0)) {
                                noiseYShift--;
                                updateDrawTool = true;
                            }
                            if ((keysR & KEY_RIGHT) && (noiseYShift + 1 < 16)) {
                                noiseYShift++;
                                updateDrawTool = true;
                            }
                        }
                        break;
                    }
                    case 4: {
                        if (!activeNoise) {
                            if ((keysR & KEY_LEFT) && (noiseXOffset - 1 >= 0)) {
                                noiseXOffset--;
                                updateDrawTool = true;
                            }
                            if ((keysR & KEY_RIGHT) && (noiseXOffset + 1 < 16)) {
                                noiseXOffset++;
                                updateDrawTool = true;
                            }
                        } else {
                            if ((keysR & KEY_LEFT) && (noiseYOffset - 1 >= 0)) {
                                noiseYOffset--;
                                updateDrawTool = true;
                            }
                            if ((keysR & KEY_RIGHT) && (noiseYOffset + 1 < 16)) {
                                noiseYOffset++;
                                updateDrawTool = true;
                            }
                        }
                        break;
                    }
                }
            }
        }
    } else {
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
            paint.updateLayersEnable();
            drawLine(paint, paint.cursorX, paint.cursorY, paint.cursorX, paint.cursorY, getSelectedLayer(paint), getSelectedColor(paint));
            paint.updateLayersDisable();
        }

        if (updateDrawCursor) {
        drawCursor(paint);
        updateDrawCursor = false;
    }
    }
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
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 62, pixelBufferMain);

    active = false;
    drawCursor(paint);
}

void Brush::reverse(Paint& paint) {
    updateDrawCursor = true;
}

void Brush::redraw(Paint& paint) {
    updateDrawTool = true;
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
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 61, pixelBufferMain);

    string typeString = string((line == 0) ? ">" : "") + STR_BRUSH_TYPE + ": " + getTypeName(paint, type); 
    paint.drawText(3, yOffset, typeString.c_str(), pixelBufferMain, blackColor);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 8, yOffset, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);

    yOffset += 10;

    switch (type) {
        case 0:
    	case 3: {
            string sizeString = string((line == 1) ? ">" : "") + STR_BRUSH_SIZE + ": " + paint.intToChars(squareSize);
            paint.drawText(3, yOffset, sizeString.c_str(), pixelBufferMain, blackColor);
            paint.drawScrollBox(SCREEN_WIDTH - bOffset - 64, yOffset + 1, 64, squareSize - 1, pixelBufferMain);
            break;
        }
        case 1:
        case 4: {
            string diameterString = string((line == 1) ? ">" : "") + STR_BRUSH_DIAMETER + ": " + paint.intToChars(circleDiameter);
            paint.drawText(3, yOffset, diameterString.c_str(), pixelBufferMain, blackColor);
            paint.drawScrollBox(SCREEN_WIDTH - bOffset - 64, yOffset + 1, 64, circleDiameter - 1, pixelBufferMain);
            break;
        }
        case 2:
        case 5: {
            string radiusString = string((line == 1) ? ">" : "") + STR_BRUSH_RADIUS + ": " + paint.intToChars(dotRadius);
            paint.drawText(3, yOffset, radiusString.c_str(), pixelBufferMain, blackColor);
            paint.drawScrollBox(SCREEN_WIDTH - bOffset - 32, yOffset + 1, 32, dotRadius - 1, pixelBufferMain);
            break;
        }
    }

    if (type >= 3) {
        string noiseSizeString = string((line == 2 && !active) ? ">" : "") + STR_BRUSH_NOISE_SIZE + ": " + ((line == 2 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXSize) + " " + ((line == 2 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYSize);
        paint.drawText(3, yOffset += 10, noiseSizeString.c_str(), pixelBufferMain, blackColor);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 40, yOffset + 1, 16, noiseXSize - 1, pixelBufferMain);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 16, yOffset + 1, 16, noiseYSize - 1, pixelBufferMain);

        string noiseShiftString = string((line == 3 && !active) ? ">" : "") + STR_BRUSH_NOISE_SHIFT + ": " + ((line == 3 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXShift) + " " + ((line == 3 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYShift);
        paint.drawText(3, yOffset += 10, noiseShiftString.c_str(), pixelBufferMain, blackColor);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 40, yOffset + 1, 16, noiseXShift, pixelBufferMain);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 16, yOffset + 1, 16, noiseYShift, pixelBufferMain);

        string noiseOffsetString = string((line == 4 && !active) ? ">" : "") + STR_BRUSH_NOISE_OFFSET + ": " + ((line == 4 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXOffset) + " " + ((line == 4 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYOffset);
        paint.drawText(3, yOffset += 10, noiseOffsetString.c_str(), pixelBufferMain, blackColor);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 40, yOffset + 1, 16, noiseXOffset, pixelBufferMain);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 16, yOffset + 1, 16, noiseYOffset, pixelBufferMain);
    }
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
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            paint.blendLayers(paint.cursorXOld + x - (size / 2), paint.cursorYOld + y - (size / 2));
        }
    }

    if (!clear) drawLine(paint, paint.cursorX, paint.cursorY, paint.cursorX, paint.cursorY, pixelBufferMain, getSelectedColor(paint));

    paint.cursorXOld = paint.cursorX;
    paint.cursorYOld = paint.cursorY;
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
    return pixelBufferCanvas;//paint.getSelectedLayer();
}

u16 Brush::getSelectedColor(Paint& paint) {
    return paint.getSelectedColor();
}