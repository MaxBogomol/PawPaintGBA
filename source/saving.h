#pragma once

#include "tool.h"

class Saving : public Tool {
    public:
        int line = 0;
        int lineOffset = 0;
        bool loading = false;
        bool saving = false;
        bool savingExport = false;
        int doneTimer = 0;
        const char* pawName = "Unnamed";
        bool updateDrawTool = true;

        virtual ~Saving() {} 

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
};