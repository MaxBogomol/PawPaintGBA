#pragma once

#include "brush.h"

class Eraser : public Brush {
    public:
        virtual ~Eraser() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;

        virtual u16 getSelectedColor(Paint& paint) override;
};