#pragma once

#include "tool.h"

class Eyedropper : public Tool {
    public:
        bool updateDrawCursor = false;

        virtual ~Eyedropper() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void reverse(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;
        virtual void drawHints(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawCursor(Paint& paint, bool clear);
        virtual void drawCursor(Paint& paint);
};