#pragma once

#include "tool.h"

class ColorPicker : public Tool {
    public:
        int line = 0;
        int hue = 0;
        int hueOld = 0;
        int colorX = 0;
        int colorY = 0;
        int colorXOld = 0;
        int colorYOld = 0;
        u16 selectedColor = 0;
        u16 newSelectedColor = 0;
        bool active = false;
        bool updateDrawTool = true;
        bool updatePicker = false;
        bool updateHue = false;
        bool updateSelected = false;
        bool updateNewSelected = false;

        virtual ~ColorPicker() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void updateTool(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
        virtual void reverse(Paint& paint) override;
        virtual void redraw(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;
        virtual void drawHints(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawTool(Paint& paint);
        virtual void drawPicker(Paint& paint);
        virtual void drawHue(Paint& paint);
        virtual void drawSelectedColor(Paint& paint);
        virtual void drawNewSelectedColor(Paint& paint);
        virtual void drawOutlines(Paint& paint);
        virtual void drawPickerPointers(Paint& paint);
        virtual void drawHuePointer(Paint& paint);

        virtual void clearSelectedColor(Paint& paint);
        virtual void clearNewSelectedColor(Paint& paint);
        virtual void clearPickerPointers(Paint& paint);
        virtual void clearHuePointer(Paint& paint);
};