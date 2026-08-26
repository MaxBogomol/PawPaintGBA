#pragma once

#include "tool.h"

class Brush : public Tool {
    public:
        int type = 0;
        int line = 0;
        int squareSize = 1;
        int circleDiameter = 1;
        int dotRadius = 1;
        int noiseXSize = 2;
        int noiseYSize = 2;
        int noiseXShift = 0;
        int noiseYShift = 0;
        int noiseXOffset = 0;
        int noiseYOffset = 0;
        int cursorX = 0;
        int cursorY = 0;
        int cursorXOld = 0;
        int cursorYOld = 0;
        bool active = false;
        bool activeNoise = false;
        bool updateDrawTool = true;
        bool updateDrawCursor = false;

        virtual ~Brush() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void updateTool(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawLine(Paint& paint, int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        virtual void drawTool(Paint& paint);
        virtual void drawCursor(Paint& paint, bool clear);
        virtual void drawCursor(Paint& paint);

        virtual const char* getTypeName(Paint& paint, int type);

        virtual u16 *getSelectedLayer(Paint& paint);
        virtual u16 getSelectedColor(Paint& paint);
};