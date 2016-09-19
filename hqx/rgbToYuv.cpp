#include "rgbToYuv.hpp"

namespace hqx
{

uint32_t lookupTable[0x1000000];

void initialize()
{
    uint8_t* lP = (uint8_t*)lookupTable;
    for (uint32_t i = 0; i < 0x1000000; ++i)
    {
        float r = (i & 0xff0000) >> 16;
        float g = (i & 0x00ff00) >> 8;
        float b = (i & 0x0000ff);

        lP++; //Skip alpha byte
        *(lP++) = (uint8_t)((int32_t)(-.169 * r - .331 * g + .5 * b) + 128);
        *(lP++) = (uint8_t)((int32_t)(.5 * r - .419 * g - .081 * b) + 128);
        *(lP++) = (uint8_t)(.299 * r + .587 * g + .114 * b);
    }
}

uint32_t GetYuv(uint32_t rgb)
{
    return lookupTable[(rgb & RgbMask) >> RgbShift];
}

bool Diff(uint32_t c1, uint32_t c2, uint32_t trY, uint32_t trU, uint32_t trV, uint32_t trA)
{
    uint32_t YUV1 = GetYuv(c1);
    uint32_t YUV2 = GetYuv(c2);

    return
    ((std::abs((int32_t)(YUV1 >> Yshift) - (int32_t)(YUV2 >> Yshift)) > trY) ||
    (std::abs((int32_t)(YUV1 >> Ushift)  - (int32_t)(YUV2 >> Ushift)) > trU) ||
    (std::abs((int32_t)(YUV1 >> Vshift)  - (int32_t)(YUV2 >> Vshift)) > trV) ||
    (std::abs((int32_t)(c1 & Amask)      - (int32_t)(c2 & Amask)) > trA));
}

uint32_t MixColours(uint32_t w1, uint32_t w2, uint32_t w3, uint32_t c1, uint32_t c2, uint32_t c3)
{
    uint32_t totalPartsColour = 0;
    uint32_t totalPartsAlpha = 0;

    uint32_t totalGreen = 0;
    uint32_t totalRedBlue = 0;
    uint32_t totalAlpha = 0;

    uint32_t weights[3] = {w1, w2, w3};
    uint32_t colours[3] = {c1, c2, c3};

    for (int i = 0; i < 3; i++)
    {
        uint32_t weighting = weights[i];
        uint32_t colour = colours[i];

        if (weighting > 0)
        {
            uint32_t alpha = (colour & MaskAlpha) * weighting;

            totalPartsAlpha += weighting;
            if(alpha != 0)
            {
                totalAlpha += alpha;

                totalPartsColour += weighting;
//                totalGreen += (colour & MaskGreen) * weighting;
                totalGreen += ((colour & MaskGreen) >> RgbShift) * weighting;
//                totalRedBlue += (colour & MaskRedBlue) * weighting;
                totalRedBlue += ((colour & MaskRedBlue) >> RgbShift) * weighting;
            }
        }
    }

    totalAlpha /= totalPartsAlpha;
    totalAlpha;// <<= AlphaShift;

    if(totalPartsColour > 0)
    {
        totalGreen /= totalPartsColour;
        totalGreen <<= RgbShift;
        totalGreen &= MaskGreen;

        totalRedBlue /= totalPartsColour;
        totalRedBlue <<= RgbShift;
        totalRedBlue &= MaskRedBlue;
    }

    return totalAlpha | totalGreen | totalRedBlue;
}

uint32_t MixColours(uint32_t w1, uint32_t w2, uint32_t c1, uint32_t c2)
{
    return MixColours(w1, w2, 0, c1, c2, 0);
}

uint32_t Mix3To1(uint32_t c1, uint32_t c2)
{
    return MixColours(3, 1, c1, c2);
}

uint32_t Mix2To1To1(uint32_t c1, uint32_t c2, uint32_t c3)
{
    return MixColours(2, 1, 1, c1, c2, c3);
}

uint32_t Mix7To1(uint32_t c1, uint32_t c2)
{
    return MixColours(7, 1, c1, c2);
}

uint32_t Mix2To7To7(uint32_t c1, uint32_t c2, uint32_t c3)
{
    return MixColours(2, 7, 7, c1, c2, c3);
}

uint32_t MixEven(uint32_t c1, uint32_t c2)
{
    return MixColours(1, 1, c1, c2);
}

uint32_t Mix5To2To1(uint32_t c1, uint32_t c2, uint32_t c3)
{
    return MixColours(5, 2, 1, c1, c2, c3);
}

uint32_t Mix6To1To1(uint32_t c1, uint32_t c2, uint32_t c3)
{
    return MixColours(6, 1, 1, c1, c2, c3);
}

uint32_t Mix5To3(uint32_t c1, uint32_t c2)
{
    return MixColours(5, 3, c1, c2);
}

uint32_t Mix2To3To3(uint32_t c1, uint32_t c2, uint32_t c3)
{
    return MixColours(2, 3, 3, c1, c2, c3);
}

uint32_t Mix14To1To1(uint32_t c1, uint32_t c2, uint32_t c3)
{
    return MixColours(14, 1, 1, c1, c2, c3);
}

}
