#ifndef MATH_H
#define MATH_H

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#undef M_PI_2
#define M_PI_2 (M_PI * 2.0)

#undef M_PI_H
#define M_PI_H (M_PI * 0.5)

#define RSDK_PI (3.1415927f)

#define MEM_ZERO(x)  memset(&(x), 0, sizeof((x)))
#define MEM_ZEROP(x) memset((x), 0, sizeof(*(x)))

#if RETRO_PLATFORM != RETRO_PSP
extern int sinM7LookupTable[0x200];
extern int cosM7LookupTable[0x200];

extern int sin512LookupTable[0x200];
extern int cos512LookupTable[0x200];

extern int sin256LookupTable[0x100];
extern int cos256LookupTable[0x100];

extern byte arcTan256LookupTable[0x100 * 0x100];
#endif

// Setup Angles
void CalculateTrigAngles();

#if RETRO_PLATFORM == RETRO_PSP
#include <math.h>

inline int ClampInt(int val, int minVal, int maxVal)
{
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

inline int SafeFloatToInt(float val, int minVal, int maxVal)
{
    if (!isfinite(val)) return 0;
    int result = (int)val;
    return ClampInt(result, minVal, maxVal);
}

inline int Sin512(int angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    if (angle == 0x00 || angle == 0x100)
        return 0;
    else if (angle == 0x80)
        return 0x200;
    else if (angle == 0x180)
        return -0x200;
    return SafeFloatToInt(sinf((angle / 256.0f) * RSDK_PI) * 512.0f, -0x200, 0x200);
}

inline int Cos512(int angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    if (angle == 0x00)
        return 0x200;
    else if (angle == 0x80 || angle == 0x180)
        return 0;
    else if (angle == 0x100)
        return -0x200;
    return SafeFloatToInt(cosf((angle / 256.0f) * RSDK_PI) * 512.0f, -0x200, 0x200);
}

inline int Sin256(int angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return (Sin512(angle * 2) >> 1);
}

inline int Cos256(int angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return (Cos512(angle * 2) >> 1);
}

inline int SinM7(int angle)
{
    angle &= 0x1FF;
    if (angle == 0x00 || angle == 0x100)
        return 0;
    else if (angle == 0x80)
        return 0x1000;
    else if (angle == 0x180)
        return -0x1000;
    return SafeFloatToInt(sinf((angle / 256.0f) * RSDK_PI) * 4096.0f, -0x1000, 0x1000);
}

inline int CosM7(int angle)
{
    angle &= 0x1FF;
    if (angle == 0x00)
        return 0x1000;
    else if (angle == 0x80 || angle == 0x180)
        return 0;
    else if (angle == 0x100)
        return -0x1000;
    return SafeFloatToInt(cosf((angle / 256.0f) * RSDK_PI) * 4096.0f, -0x1000, 0x1000);
}

#else

inline int Sin512(int angle)
{
    if (angle < 0)
        angle = 0x200 - angle;

    angle &= 0x1FF;
    return sin512LookupTable[angle];
}

inline int Cos512(int angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    return cos512LookupTable[angle];
}

inline int Sin256(int angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return sin256LookupTable[angle];
}

inline int Cos256(int angle)
{
    if (angle < 0)
        angle = 0x100 - angle;
    angle &= 0xFF;
    return cos256LookupTable[angle];
}

inline int SinM7(int angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    return sinM7LookupTable[angle];
}

inline int CosM7(int angle)
{
    if (angle < 0)
        angle = 0x200 - angle;
    angle &= 0x1FF;
    return cosM7LookupTable[angle];
}

#endif

// Get Arc Tan value
byte ArcTanLookup(int X, int Y);

inline double DegreesToRad(float degrees) { return (M_PI / 180) * degrees; }

#endif // !MATH_H
