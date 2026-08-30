#pragma once

#include <gba.h>
#include <vector>

using namespace std;

#include "brush.h"
#include "eraser.h"
#include "eyedropper.h"
#include "settings.h"
#include "info.h"

#define ARGB16(a,r,g,b) (((a) << 15) | (r) | ((g) << 5) | ((b) << 10))

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

u16* const videoMemory = (u16*) 0x06000000;

inline u16 alphaColor = ARGB16(0, 0, 0, 0);
inline u16 whiteColor = ARGB16(1, 31, 31, 31);
inline u16 blackColor = ARGB16(1, 0, 0, 0);
inline u16 grayColor = ARGB16(1, 15, 15, 15);
inline u16 redColor = ARGB16(1, 31, 0, 0);
inline u16 greenColor = ARGB16(1, 0, 31, 0);
inline u16 blueColor = ARGB16(1, 0, 0, 31);
inline u16 pinkColor = ARGB16(1, 31, 0, 31);

inline u16 pinkFoxThemeColor = ARGB16(1, 31, 24, 25); //#fec8cf
inline u16 maidThemeColor = ARGB16(1, 6, 5, 7); //#2e2939
inline u16 aceThemeColor = ARGB16(1, 20, 6, 20); //#a231a2

inline EWRAM_DATA u16 pixelBufferMain[SCREEN_WIDTH * SCREEN_HEIGHT];
inline EWRAM_DATA u16 pixelBufferCanvas[SCREEN_WIDTH * SCREEN_HEIGHT];

inline int keysD;
inline int keysH;
inline int keysR;
inline int keysU;

inline Brush brush;
inline Eraser eraser;
inline Eyedropper eyedropper;
inline Settings settings;
inline Info info;

inline const char* paintVerstion = "v0.1";

inline int maxLanguages = 6;
inline int maxPaintThemes = 4;
inline int maxPaintIcons = 2;

inline const char* languageCodes[6] = {
    "en_us",
    "ru_ru",
    "be_by",
    "be_tar",
    "be_by_latn",
    "be_tar_latn"
};

struct HSV {
    int h;
    int s;
    int v;
};

class Paint {
    private:
        bool firstFrameTool = true;
        bool updateLayers = false;

        const char* paintName = "Unnamed";

    public:
        int selectedLanguage = 0;
        int selectedTheme = 0;
        int selectedIcon = 0;
        int selectedLayer = 0;
        int selectedTool = 0;
        u16 selectedColor = blackColor;
        u16 selectedColorSub = whiteColor;
        bool reverseScreens = false;

        bool updateDrawAll = false;
        bool updateDrawSelectedColor = false;
        bool updateDrawTools = true;
        bool updateDrawColors = true;
        bool updateDrawHints = true;
        bool updateDrawPaintName = true;
        bool updateDrawPaintIcon = true;

        bool fileSystemInit = false;

        int cursorX = 0;
        int cursorY = 0;
        int cursorXOld = 0;
        int cursorYOld = 0;

        vector<Tool*> tools;

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

        void blendLayers(int x, int y);
        void updateLayersEnable();
        void updateLayersDisable();

        u16 getSelectedColor();

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
        u32 decodeChar(const char** s);
        int getCharLength(u32 c);
        int getTextLength(const char* text);
        void drawChar(int x, int y, u32 c, u16* buffer, u16 color);
        void drawText(int x, int y, const char* text, u16* buffer, u16 color);
        void drawCharOutline(int x, int y, u32 c, u16* buffer, u16 color, u16 outlineColor);
        void drawTextOutline(int x, int y, const char* text, u16* buffer, u16 color, u16 outlineColor);
        void drawSprite(int x0, int y0, int x1, int y1, int xShift, int yShift, int xSize, int ySize, const unsigned int* spriteBitmap, u16* buffer);
        void drawSprite(int x0, int y0, int x1, int y1, const unsigned int* spriteBitmap, u16* buffer);

        u16 blendColors(u16 src, u16 dst);
        u16 HSVtoRGB(int h, int s, int v);
        u16 HSVtoRGB(HSV hsv);
        HSV RGBtoHSV(u16 color);
        int getDitherThreshold(int x, int y, int xSize, int ySize, int xShift, int yShift);
        const char* intToChars(int val);

        void setPaintName(const char* name);
        const char* getPaintName();

        void clearBuffer(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void clearBuffer(int x0, int y0, int x1, int y1, u16* buffer);

        int getToolYOffset();
        int getToolsYOffset();
        int getToolsButtonsOffset();

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

        u16 getThemeColor(int theme);
        u16 getSelectedThemeColor();

        const unsigned int* getIconSprite(int icon);
        const unsigned int* getSelectedIconSprite();

        const char* getLanguageCode(int language);
        const char* getSelectedLanguageCode();

        bool readSelectedLanguage();
};