#include <gba.h>

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