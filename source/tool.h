#pragma once

#include <cstdint>

typedef uint16_t u16;

class Paint;

class Tool {
    public:
        virtual ~Tool() {} 

        virtual const char* getName(Paint& paint) {
            return "Tool";
        }
        
        virtual void setup(Paint& paint) {}
        virtual void update(Paint& paint) {}
        virtual void updateTool(Paint& paint) {}
        virtual void open(Paint& paint) {}
        virtual void close(Paint& paint) {}

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) {}
};