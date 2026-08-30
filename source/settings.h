#pragma once

#include "tool.h"

class Settings : public Tool {
    public:
        int line = 0;
        bool updateTheme = false;
        bool updateDrawTool = true;

        virtual ~Settings() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void updateTool(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
        virtual void redraw(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;
        virtual void drawHints(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawTool(Paint& paint);

        virtual const char* getThemeName(Paint& paint, int theme);
        virtual const char* getIconName(Paint& paint, int icon);
};