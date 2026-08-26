#include "eraser.h"

#include "paint.h"
#include "language.h"

#include "eraser_icon.h"
#include "eraser_square_icon.h"
#include "eraser_circle_icon.h"
#include "eraser_dot_icon.h"
#include "eraser_square_noise_icon.h"
#include "eraser_circle_noise_icon.h"
#include "eraser_dot_noise_icon.h"

const char* Eraser::getName(Paint& paint) {
    return STR_ERASER.c_str();
}

void Eraser::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    const unsigned int* iconSprite = eraser_square_iconBitmap;
    switch (type) {
    	case 0: iconSprite = eraser_square_iconBitmap; break;
    	case 1: iconSprite = eraser_circle_iconBitmap; break;
		case 2: iconSprite = eraser_dot_iconBitmap; break;
        case 3: iconSprite = eraser_square_noise_iconBitmap; break;
    	case 4: iconSprite = eraser_circle_noise_iconBitmap; break;
		case 5: iconSprite = eraser_dot_noise_iconBitmap; break;
    }
    paint.drawSprite(x, y, 16, 16, iconSprite, buffer);
}

u16 Eraser::getSelectedColor(Paint& paint) {
    return alphaColor;
}