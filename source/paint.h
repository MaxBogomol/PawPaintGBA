#pragma once

#include <gba.h>
#include <vector>

using namespace std;

#include "defines.h"
#include "brush.h"
#include "eraser.h"
#include "eyedropper.h"
#include "color_picker.h"
#include "saving.h"
#include "settings.h"
#include "info.h"

inline Brush brush;
inline Eraser eraser;
inline Eyedropper eyedropper;
inline ColorPicker colorPicker;
inline Saving saving;
inline Settings settings;
inline Info info;

class Paint {
    private:
        bool firstFrameTool = true;

        bool updateLayers = false;

        vector<Tool*> tools;

        const char* paintName = "Unnamed";

    public:
        bool fileSystemInit = false;

        int cursorX = 0;
        int cursorY = 0;
        int cursorXOld = 0;
        int cursorYOld = 0;

        bool reverseScreens = false;

        int selectedLanguage = 0;
        int selectedTheme = 0;
        int selectedIcon = 0;
        int selectedLayer = 0;
        int selectedTool = 0;
        u16 selectedColor = blackColor;
        u16 secondColor = whiteColor;

        bool updateDrawAll = false;
        bool updateDrawTools = true;
        bool updateDrawColors = true;
        bool updateDrawHints = true;
        bool updateDrawPaintName = true;
        bool updateDrawPaintIcon = true;

        void setup();
        void setupVideo();
        void setupLayers();
        void setupTools();

        void updateInputs();
        void updateTools();
        void updateVideo();

        void drawTools();
        void drawColors();
        void drawHints();
        void drawPaintName();
        void drawPaintIcon();

        const char* getPaintName();
        void setPaintName(const char* name);

        u16 getThemeColor(int theme);
        u16 getSelectedThemeColor();

        const unsigned int* getIconSprite(int icon);
        const unsigned int* getSelectedIconSprite();

        const char* getLanguageCode(int language);
        const char* getSelectedLanguageCode();

        u16 getSelectedColor();
        u16 getSecondColor();

        int getToolYOffset();
        int getToolsYOffset();
        int getToolsButtonsOffset();

        void clearBuffer(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void clearBuffer(int x0, int y0, int x1, int y1, u16* buffer);
        void blendLayers(int x, int y);
        void blendLayers(int x0, int y0, int x1, int y1);

        void updateLayersEnable();
        void updateLayersDisable();

        u16 getPixel(int x, int y, u16* buffer);

        void drawPixel(int x, int y, u16* buffer, u16 color);
        void drawSquare(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawSquareOutline(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawSquareNoise(int x0, int y0, int x1, int y1, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset);
        void drawCircleRadius(int xc, int yc, int r, u16* buffer, u16 color);
        void drawCircleRadiusNoise(int xc, int yc, int r, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset);
        void drawCircleDiameter(int xc, int yc, int d, u16* buffer, u16 color);
        void drawCircleDiameterNoise(int xc, int yc, int d, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset);
        void drawLine(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawChar(int x, int y, u32 c, u16* buffer, u16 color);
        void drawText(int x, int y, const char* text, u16* buffer, u16 color);
        void drawCharOutline(int x, int y, u32 c, u16* buffer, u16 color, u16 outlineColor);
        void drawTextOutline(int x, int y, const char* text, u16* buffer, u16 color, u16 outlineColor);
        void drawSprite(int x0, int y0, int x1, int y1, int xShift, int yShift, int xSize, int ySize, const unsigned int* spriteBitmap, u16* buffer);
        void drawSprite(int x0, int y0, int x1, int y1, const unsigned int* spriteBitmap, u16* buffer);

        void drawUpButton(int x, int y, u16* buffer);
        void drawRightButton(int x, int y, u16* buffer);
        void drawDownButton(int x, int y, u16* buffer);
        void drawLeftButton(int x, int y, u16* buffer);
        void drawNoneButton(int x, int y, u16* buffer);
        void drawUpDownButton(int x, int y, u16* buffer);
        void drawLeftRightButton(int x, int y, u16* buffer);
        void drawAllButton(int x, int y, u16* buffer);
        void drawBButton(int x, int y, u16* buffer);
        void drawAButton(int x, int y, u16* buffer);
        void drawLButton(int x, int y, u16* buffer);
        void drawRButton(int x, int y, u16* buffer);
        void drawStartButton(int x, int y, u16* buffer);
        void drawSelectButton(int x, int y, u16* buffer);

        u16 blendColors(u16 src, u16 dst);
        u16 HSVtoRGB(int h, int s, int v);
        u16 HSVtoRGB(HSV hsv);
        HSV RGBtoHSV(u16 color);

        const char* intToChars(int val);
        u32 decodeChar(const char** s);
        int getCharLength(u32 c);
        int getTextLength(const char* text);

        int getDitherThreshold(int x, int y, int xSize, int ySize, int xShift, int yShift);

        bool readSelectedLanguage();
};