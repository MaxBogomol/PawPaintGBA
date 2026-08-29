#pragma once

#include "tool.h"

class Eyedropper : public Tool {
    public:
        virtual ~Eyedropper() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void update(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;
};