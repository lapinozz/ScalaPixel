#include "hqx.hpp"

namespace hqx
{

void Scale4(uint32_t* sp, uint32_t* dp, int32_t Xres, int32_t Yres, uint32_t trY, uint32_t trU, uint32_t trV, uint32_t trA, bool wrapX, bool wrapY)
{
    //Don't shift trA, as it uses shift right instead of a mask for comparisons.
    trY <<= 2 * 8;
    trU <<= 1 * 8;
    int32_t dpL = Xres * 4;

    int32_t prevline, nextline;
    uint32_t w[9];

    for (int32_t j = 0; j < Yres; j++)
    {
        if (j > 0)
        {
            prevline = -Xres;
        }
        else
        {
            if (wrapY)
            {
                prevline = Xres * (Yres - 1);
            }
            else
            {
                prevline = 0;
            }
        }
        if (j < Yres - 1)
        {
            nextline = Xres;
        }
        else
        {
            if (wrapY)
            {
                nextline = -(Xres * (Yres - 1));
            }
            else
            {
                nextline = 0;
            }
        }

        for (int32_t i = 0; i < Xres; i++)
        {
            w[1] = *(sp + prevline);
            w[4] = *sp;
            w[7] = *(sp + nextline);

            if (i > 0)
            {
                w[0] = *(sp + prevline - 1);
                w[3] = *(sp - 1);
                w[6] = *(sp + nextline - 1);
            }
            else
            {
                if (wrapX)
                {
                    w[0] = *(sp + prevline + Xres - 1);
                    w[3] = *(sp + Xres - 1);
                    w[6] = *(sp + nextline + Xres - 1);
                }
                else
                {
                    w[0] = w[1];
                    w[3] = w[4];
                    w[6] = w[7];
                }
            }

            if (i < Xres - 1)
            {
                w[2] = *(sp + prevline + 1);
                w[5] = *(sp + 1);
                w[8] = *(sp + nextline + 1);
            }
            else
            {
                if (wrapX)
                {
                    w[2] = *(sp + prevline - Xres + 1);
                    w[5] = *(sp - Xres + 1);
                    w[8] = *(sp + nextline - Xres + 1);
                }
                else
                {
                    w[2] = w[1];
                    w[5] = w[4];
                    w[8] = w[7];
                }
            }

            int32_t pattern = 0;
            int32_t flag = 1;

            for (int32_t k = 0; k < 9; k++)
            {
                if (k == 4) continue;

                if (w[k] != w[4])
                {
                    if (Diff(w[4], w[k], trY, trU, trV, trA))
                        pattern |= flag;
                }
                flag <<= 1;
            }

            switch (pattern)
            {
            case 0:
            case 1:
            case 4:
            case 32:
            case 128:
            case 5:
            case 132:
            case 160:
            case 33:
            case 129:
            case 36:
            case 133:
            case 164:
            case 161:
            case 37:
            case 165:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 2:
            case 34:
            case 130:
            case 162:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 16:
            case 17:
            case 48:
            case 49:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 64:
            case 65:
            case 68:
            case 69:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 8:
            case 12:
            case 136:
            case 140:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 3:
            case 35:
            case 131:
            case 163:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 6:
            case 38:
            case 134:
            case 166:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 20:
            case 21:
            case 52:
            case 53:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 144:
            case 145:
            case 176:
            case 177:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 192:
            case 193:
            case 196:
            case 197:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 96:
            case 97:
            case 100:
            case 101:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 40:
            case 44:
            case 168:
            case 172:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 9:
            case 13:
            case 137:
            case 141:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 18:
            case 50:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 80:
            case 81:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 72:
            case 76:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 10:
            case 138:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + 1) = w[4];
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 66:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 24:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 7:
            case 39:
            case 135:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 148:
            case 149:
            case 180:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 224:
            case 228:
            case 225:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 41:
            case 169:
            case 45:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 22:
            case 54:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 208:
            case 209:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 104:
            case 108:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 11:
            case 139:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 19:
            case 51:
            {
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[3]);
                    *(dp + 1) = Mix7To1(w[4], w[3]);
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp) = Mix3To1(w[4], w[1]);
                    *(dp + 1) = Mix3To1(w[1], w[4]);
                    *(dp + 2) = Mix5To3(w[1], w[5]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                    *(dp + dpL + 3) = Mix2To1To1(w[5], w[4], w[1]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 146:
            case 178:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                    *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                }
                else
                {
                    *(dp + 2) = Mix2To1To1(w[1], w[4], w[5]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                    *(dp + dpL + 3) = Mix5To3(w[5], w[1]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                break;
            }
            case 84:
            case 85:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + 3) = Mix5To3(w[4], w[1]);
                    *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + 3) = Mix3To1(w[5], w[4]);
                    *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                    *(dp + dpL + dpL + 3) = Mix5To3(w[5], w[7]);
                    *(dp + dpL + dpL + dpL + 2) = Mix2To1To1(w[7], w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 112:
            case 113:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                    *(dp + dpL + dpL + 3) = Mix2To1To1(w[5], w[4], w[7]);
                    *(dp + dpL + dpL + dpL) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[7], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                break;
            }
            case 200:
            case 204:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix2To1To1(w[3], w[4], w[7]);
                    *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 73:
            case 77:
            {
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[1]);
                    *(dp + dpL) = Mix7To1(w[4], w[1]);
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp) = Mix3To1(w[4], w[3]);
                    *(dp + dpL) = Mix3To1(w[3], w[4]);
                    *(dp + dpL + dpL) = Mix5To3(w[3], w[7]);
                    *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix2To1To1(w[7], w[4], w[3]);
                }
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 42:
            case 170:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                    *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = Mix2To1To1(w[1], w[4], w[3]);
                    *(dp + dpL) = Mix5To3(w[3], w[1]);
                    *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                    *(dp + dpL + dpL) = Mix3To1(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = Mix3To1(w[4], w[3]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 14:
            case 142:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + 2) = Mix7To1(w[4], w[5]);
                    *(dp + 3) = Mix5To3(w[4], w[5]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = Mix5To3(w[1], w[3]);
                    *(dp + 2) = Mix3To1(w[1], w[4]);
                    *(dp + 3) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix2To1To1(w[3], w[4], w[1]);
                    *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                }
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 67:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 70:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 28:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 152:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 194:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 98:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 56:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 25:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 26:
            case 31:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 82:
            case 214:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 88:
            case 248:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                break;
            }
            case 74:
            case 107:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 27:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 86:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 216:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 106:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 30:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 210:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 120:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 75:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 29:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 198:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 184:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 99:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 57:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 71:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 156:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 226:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 60:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 195:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 102:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 153:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 58:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 83:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 92:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 202:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 78:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 154:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 114:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                break;
            }
            case 89:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 90:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 55:
            case 23:
            {
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[3]);
                    *(dp + 1) = Mix7To1(w[4], w[3]);
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp) = Mix3To1(w[4], w[1]);
                    *(dp + 1) = Mix3To1(w[1], w[4]);
                    *(dp + 2) = Mix5To3(w[1], w[5]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                    *(dp + dpL + 3) = Mix2To1To1(w[5], w[4], w[1]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 182:
            case 150:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = w[4];
                    *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                }
                else
                {
                    *(dp + 2) = Mix2To1To1(w[1], w[4], w[5]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                    *(dp + dpL + 3) = Mix5To3(w[5], w[1]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                break;
            }
            case 213:
            case 212:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + 3) = Mix5To3(w[4], w[1]);
                    *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + 3) = Mix3To1(w[5], w[4]);
                    *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                    *(dp + dpL + dpL + 3) = Mix5To3(w[5], w[7]);
                    *(dp + dpL + dpL + dpL + 2) = Mix2To1To1(w[7], w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 241:
            case 240:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                    *(dp + dpL + dpL + 3) = Mix2To1To1(w[5], w[4], w[7]);
                    *(dp + dpL + dpL + dpL) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[7], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                break;
            }
            case 236:
            case 232:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix2To1To1(w[3], w[4], w[7]);
                    *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 109:
            case 105:
            {
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[1]);
                    *(dp + dpL) = Mix7To1(w[4], w[1]);
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp) = Mix3To1(w[4], w[3]);
                    *(dp + dpL) = Mix3To1(w[3], w[4]);
                    *(dp + dpL + dpL) = Mix5To3(w[3], w[7]);
                    *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix2To1To1(w[7], w[4], w[3]);
                }
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 171:
            case 43:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                    *(dp + dpL + 1) = w[4];
                    *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = Mix2To1To1(w[1], w[4], w[3]);
                    *(dp + dpL) = Mix5To3(w[3], w[1]);
                    *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                    *(dp + dpL + dpL) = Mix3To1(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = Mix3To1(w[4], w[3]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 143:
            case 15:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + 2) = Mix7To1(w[4], w[5]);
                    *(dp + 3) = Mix5To3(w[4], w[5]);
                    *(dp + dpL) = w[4];
                    *(dp + dpL + 1) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = Mix5To3(w[1], w[3]);
                    *(dp + 2) = Mix3To1(w[1], w[4]);
                    *(dp + 3) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix2To1To1(w[3], w[4], w[1]);
                    *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                }
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 124:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 203:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 62:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 211:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 118:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 217:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 110:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 155:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 188:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 185:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 61:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 157:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 103:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 227:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 230:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 199:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 220:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                break;
            }
            case 158:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 234:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 242:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                break;
            }
            case 59:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 121:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 87:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 79:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 122:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 94:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL + 2) = w[4];
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 218:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                break;
            }
            case 91:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL + 1) = w[4];
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 229:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 167:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 173:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 181:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 186:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 115:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                break;
            }
            case 93:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 206:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 205:
            case 201:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                    *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 174:
            case 46:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[0]);
                    *(dp + 1) = Mix3To1(w[4], w[0]);
                    *(dp + dpL) = Mix3To1(w[4], w[0]);
                    *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                    *(dp + 1) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix3To1(w[4], w[3]);
                    *(dp + dpL + 1) = w[4];
                }
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 179:
            case 147:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = Mix3To1(w[4], w[2]);
                    *(dp + 3) = Mix5To3(w[4], w[2]);
                    *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                    *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                }
                else
                {
                    *(dp + 2) = Mix3To1(w[4], w[1]);
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 117:
            case 116:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                }
                else
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                break;
            }
            case 189:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 231:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 126:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 219:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 125:
            {
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[1]);
                    *(dp + dpL) = Mix7To1(w[4], w[1]);
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp) = Mix3To1(w[4], w[3]);
                    *(dp + dpL) = Mix3To1(w[3], w[4]);
                    *(dp + dpL + dpL) = Mix5To3(w[3], w[7]);
                    *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix2To1To1(w[7], w[4], w[3]);
                }
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 221:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + 3) = Mix5To3(w[4], w[1]);
                    *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix3To1(w[4], w[5]);
                    *(dp + dpL + 3) = Mix3To1(w[5], w[4]);
                    *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                    *(dp + dpL + dpL + 3) = Mix5To3(w[5], w[7]);
                    *(dp + dpL + dpL + dpL + 2) = Mix2To1To1(w[7], w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 207:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + 2) = Mix7To1(w[4], w[5]);
                    *(dp + 3) = Mix5To3(w[4], w[5]);
                    *(dp + dpL) = w[4];
                    *(dp + dpL + 1) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = Mix5To3(w[1], w[3]);
                    *(dp + 2) = Mix3To1(w[1], w[4]);
                    *(dp + 3) = Mix3To1(w[4], w[1]);
                    *(dp + dpL) = Mix2To1To1(w[3], w[4], w[1]);
                    *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                }
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 238:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                }
                else
                {
                    *(dp + dpL + dpL) = Mix2To1To1(w[3], w[4], w[7]);
                    *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = Mix3To1(w[4], w[7]);
                }
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 190:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = w[4];
                    *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                }
                else
                {
                    *(dp + 2) = Mix2To1To1(w[1], w[4], w[5]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                    *(dp + dpL + 3) = Mix5To3(w[5], w[1]);
                    *(dp + dpL + dpL + 3) = Mix3To1(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = Mix3To1(w[4], w[5]);
                }
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                break;
            }
            case 187:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                    *(dp + dpL + 1) = w[4];
                    *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = Mix2To1To1(w[1], w[4], w[3]);
                    *(dp + dpL) = Mix5To3(w[3], w[1]);
                    *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                    *(dp + dpL + dpL) = Mix3To1(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = Mix3To1(w[4], w[3]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 243:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                    *(dp + dpL + dpL + 3) = Mix2To1To1(w[5], w[4], w[7]);
                    *(dp + dpL + dpL + dpL) = Mix3To1(w[4], w[7]);
                    *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[7], w[5]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                break;
            }
            case 119:
            {
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp) = Mix5To3(w[4], w[3]);
                    *(dp + 1) = Mix7To1(w[4], w[3]);
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 2) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp) = Mix3To1(w[4], w[1]);
                    *(dp + 1) = Mix3To1(w[1], w[4]);
                    *(dp + 2) = Mix5To3(w[1], w[5]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                    *(dp + dpL + 3) = Mix2To1To1(w[5], w[4], w[1]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 237:
            case 233:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[5]);
                *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix6To1To1(w[4], w[5], w[1]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[1]);
                *(dp + dpL + dpL) = w[4];
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                }
                *(dp + dpL + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 175:
            case 47:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                }
                *(dp + 1) = w[4];
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = w[4];
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix6To1To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                break;
            }
            case 183:
            case 151:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + 3) = w[4];
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 1) = Mix6To1To1(w[4], w[3], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[3]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 245:
            case 244:
            {
                *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[3]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[1]);
                *(dp + dpL + 1) = Mix6To1To1(w[4], w[3], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = w[4];
                *(dp + dpL + dpL + 3) = w[4];
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 250:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                break;
            }
            case 123:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 95:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 222:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 252:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix5To2To1(w[4], w[1], w[0]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                *(dp + dpL + dpL + 3) = w[4];
                *(dp + dpL + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 249:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To2To1(w[4], w[1], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = w[4];
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                }
                *(dp + dpL + dpL + dpL + 1) = w[4];
                break;
            }
            case 235:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix5To2To1(w[4], w[5], w[2]);
                *(dp + dpL + dpL) = w[4];
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                }
                *(dp + dpL + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 111:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                }
                *(dp + 1) = w[4];
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = w[4];
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix5To2To1(w[4], w[5], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 63:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                }
                *(dp + 1) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = w[4];
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To2To1(w[4], w[7], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 159:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                }
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + 3) = w[4];
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To2To1(w[4], w[7], w[6]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 215:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + 3) = w[4];
                *(dp + dpL + dpL) = Mix5To2To1(w[4], w[3], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 246:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix5To2To1(w[4], w[3], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = w[4];
                *(dp + dpL + dpL + 3) = w[4];
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 254:
            {
                *(dp) = Mix5To3(w[4], w[0]);
                *(dp + 1) = Mix3To1(w[4], w[0]);
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = Mix3To1(w[4], w[0]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[0]);
                *(dp + dpL + 2) = w[4];
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                *(dp + dpL + dpL + 3) = w[4];
                *(dp + dpL + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 253:
            {
                *(dp) = Mix5To3(w[4], w[1]);
                *(dp + 1) = Mix5To3(w[4], w[1]);
                *(dp + 2) = Mix5To3(w[4], w[1]);
                *(dp + 3) = Mix5To3(w[4], w[1]);
                *(dp + dpL) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 2) = Mix7To1(w[4], w[1]);
                *(dp + dpL + 3) = Mix7To1(w[4], w[1]);
                *(dp + dpL + dpL) = w[4];
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                *(dp + dpL + dpL + 3) = w[4];
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                }
                *(dp + dpL + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 251:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = Mix3To1(w[4], w[2]);
                *(dp + 3) = Mix5To3(w[4], w[2]);
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[2]);
                *(dp + dpL + 3) = Mix3To1(w[4], w[2]);
                *(dp + dpL + dpL) = w[4];
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                }
                *(dp + dpL + dpL + dpL + 1) = w[4];
                break;
            }
            case 239:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                }
                *(dp + 1) = w[4];
                *(dp + 2) = Mix7To1(w[4], w[5]);
                *(dp + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL) = w[4];
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + 3) = Mix5To3(w[4], w[5]);
                *(dp + dpL + dpL) = w[4];
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                }
                *(dp + dpL + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + dpL + 2) = Mix7To1(w[4], w[5]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[5]);
                break;
            }
            case 127:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                }
                *(dp + 1) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 2) = w[4];
                    *(dp + 3) = w[4];
                    *(dp + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + 2) = MixEven(w[1], w[4]);
                    *(dp + 3) = MixEven(w[1], w[5]);
                    *(dp + dpL + 3) = MixEven(w[5], w[4]);
                }
                *(dp + dpL) = w[4];
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL) = w[4];
                    *(dp + dpL + dpL + dpL + 1) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL) = MixEven(w[3], w[4]);
                    *(dp + dpL + dpL + dpL) = MixEven(w[7], w[3]);
                    *(dp + dpL + dpL + dpL + 1) = MixEven(w[7], w[4]);
                }
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[8]);
                *(dp + dpL + dpL + 3) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[8]);
                break;
            }
            case 191:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                }
                *(dp + 1) = w[4];
                *(dp + 2) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                }
                *(dp + dpL) = w[4];
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + 3) = w[4];
                *(dp + dpL + dpL) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 2) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + 3) = Mix7To1(w[4], w[7]);
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 1) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 2) = Mix5To3(w[4], w[7]);
                *(dp + dpL + dpL + dpL + 3) = Mix5To3(w[4], w[7]);
                break;
            }
            case 223:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                    *(dp + 1) = w[4];
                    *(dp + dpL) = w[4];
                }
                else
                {
                    *(dp) = MixEven(w[1], w[3]);
                    *(dp + 1) = MixEven(w[1], w[4]);
                    *(dp + dpL) = MixEven(w[3], w[4]);
                }
                *(dp + 2) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                }
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + 3) = w[4];
                *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[6]);
                *(dp + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + 3) = w[4];
                    *(dp + dpL + dpL + dpL + 2) = w[4];
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + 3) = MixEven(w[5], w[4]);
                    *(dp + dpL + dpL + dpL + 2) = MixEven(w[7], w[4]);
                    *(dp + dpL + dpL + dpL + 3) = MixEven(w[7], w[5]);
                }
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[6]);
                *(dp + dpL + dpL + dpL + 1) = Mix3To1(w[4], w[6]);
                break;
            }
            case 247:
            {
                *(dp) = Mix5To3(w[4], w[3]);
                *(dp + 1) = Mix7To1(w[4], w[3]);
                *(dp + 2) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                }
                *(dp + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + 3) = w[4];
                *(dp + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + 2) = w[4];
                *(dp + dpL + dpL + 3) = w[4];
                *(dp + dpL + dpL + dpL) = Mix5To3(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 1) = Mix7To1(w[4], w[3]);
                *(dp + dpL + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            case 255:
            {
                if (Diff(w[3], w[1], trY, trU, trV, trA))
                {
                    *dp = w[4];
                }
                else
                {
                    *(dp) = Mix2To1To1(w[4], w[1], w[3]);
                }
                *(dp + 1) = w[4];
                *(dp + 2) = w[4];
                if (Diff(w[1], w[5], trY, trU, trV, trA))
                {
                    *(dp + 3) = w[4];
                }
                else
                {
                    *(dp + 3) = Mix2To1To1(w[4], w[1], w[5]);
                }
                *(dp + dpL) = w[4];
                *(dp + dpL + 1) = w[4];
                *(dp + dpL + 2) = w[4];
                *(dp + dpL + 3) = w[4];
                *(dp + dpL + dpL) = w[4];
                *(dp + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + 2) = w[4];
                *(dp + dpL + dpL + 3) = w[4];
                if (Diff(w[7], w[3], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                }
                *(dp + dpL + dpL + dpL + 1) = w[4];
                *(dp + dpL + dpL + dpL + 2) = w[4];
                if (Diff(w[5], w[7], trY, trU, trV, trA))
                {
                    *(dp + dpL + dpL + dpL + 3) = w[4];
                }
                else
                {
                    *(dp + dpL + dpL + dpL + 3) = Mix2To1To1(w[4], w[7], w[5]);
                }
                break;
            }
            }
            sp++;
            dp += 4;
        }
        dp += (dpL * 3);
    }
}

void Scale3(uint32_t* sp, uint32_t* dp, int32_t Xres, int32_t Yres, uint32_t trY, uint32_t trU, uint32_t trV, uint32_t trA, bool wrapX, bool wrapY)
{
    //Don't shift trA, as it uses shift right instead of a mask for comparisons.
    trY <<= 2 * 8;
    trU <<= 1 * 8;
    int32_t dpL = Xres * 3;

    int32_t prevline, nextline;
    uint32_t w[9];

    for (int32_t j = 0; j < Yres; j++)
    {
        if (j > 0)
        {
            prevline = -Xres;
        }
        else
        {
            if (wrapY)
            {
                prevline = Xres * (Yres - 1);
            }
            else
            {
                prevline = 0;
            }
        }
        if (j < Yres - 1)
        {
            nextline = Xres;
        }
        else
        {
            if (wrapY)
            {
                nextline = -(Xres * (Yres - 1));
            }
            else
            {
                nextline = 0;
            }
        }

        for (int32_t i = 0; i < Xres; i++)
        {
            w[1] = *(sp + prevline);
            w[4] = *sp;
            w[7] = *(sp + nextline);

            if (i > 0)
            {
                w[0] = *(sp + prevline - 1);
                w[3] = *(sp - 1);
                w[6] = *(sp + nextline - 1);
            }
            else
            {
                if (wrapX)
                {
                    w[0] = *(sp + prevline + Xres - 1);
                    w[3] = *(sp + Xres - 1);
                    w[6] = *(sp + nextline + Xres - 1);
                }
                else
                {
                    w[0] = w[1];
                    w[3] = w[4];
                    w[6] = w[7];
                }
            }

            if (i < Xres - 1)
            {
                w[2] = *(sp + prevline + 1);
                w[5] = *(sp + 1);
                w[8] = *(sp + nextline + 1);
            }
            else
            {
                if (wrapX)
                {
                    w[2] = *(sp + prevline - Xres + 1);
                    w[5] = *(sp - Xres + 1);
                    w[8] = *(sp + nextline - Xres + 1);
                }
                else
                {
                    w[2] = w[1];
                    w[5] = w[4];
                    w[8] = w[7];
                }
            }

            int32_t pattern = 0;
            int32_t flag = 1;

            for (int32_t k = 0; k < 9; k++)
            {
                if (k == 4) continue;

                if (w[k] != w[4])
                {
                    if (Diff(w[4], w[k], trY, trU, trV, trA))
                        pattern |= flag;
                }
                flag <<= 1;
            }

            switch (pattern)
            {
                case 0:
                case 1:
                case 4:
                case 32:
                case 128:
                case 5:
                case 132:
                case 160:
                case 33:
                case 129:
                case 36:
                case 133:
                case 164:
                case 161:
                case 37:
                case 165:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 2:
                case 34:
                case 130:
                case 162:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 16:
                case 17:
                case 48:
                case 49:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 64:
                case 65:
                case 68:
                case 69:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 8:
                case 12:
                case 136:
                case 140:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 3:
                case 35:
                case 131:
                case 163:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 6:
                case 38:
                case 134:
                case 166:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 20:
                case 21:
                case 52:
                case 53:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 144:
                case 145:
                case 176:
                case 177:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 192:
                case 193:
                case 196:
                case 197:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 96:
                case 97:
                case 100:
                case 101:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 40:
                case 44:
                case 168:
                case 172:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 9:
                case 13:
                case 137:
                case 141:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 18:
                case 50:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 80:
                case 81:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 72:
                case 76:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 10:
                case 138:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 66:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 24:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 7:
                case 39:
                case 135:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 148:
                case 149:
                case 180:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 224:
                case 228:
                case 225:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 41:
                case 169:
                case 45:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 22:
                case 54:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 208:
                case 209:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 104:
                case 108:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 11:
                case 139:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 19:
                case 51:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[3]);
                            *(dp + 1) = w[4];
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[1], w[4]);
                            *(dp + 2) = MixEven(w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 146:
                case 178:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + 2) = MixEven(w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[5], w[4]);
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 84:
                case 85:
                    {
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[1]);
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[5], w[4]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = MixEven(w[5], w[7]);
                        }
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        break;
                    }
                case 112:
                case 113:
                    {
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                            *(dp + dpL + dpL + 2) = MixEven(w[5], w[7]);
                        }
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        break;
                    }
                case 200:
                case 204:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + dpL) = MixEven(w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 73:
                case 77:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix3To1(w[3], w[4]);
                            *(dp + dpL + dpL) = MixEven(w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 42:
                case 170:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp) = MixEven(w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = Mix3To1(w[3], w[4]);
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 14:
                case 142:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                            *(dp + 1) = w[4];
                            *(dp + 2) = Mix3To1(w[4], w[5]);
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = MixEven(w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[1], w[4]);
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 67:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 70:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 28:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 152:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 194:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 98:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 56:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 25:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 26:
                case 31:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 82:
                case 214:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 88:
                case 248:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 74:
                case 107:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 27:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 86:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 216:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 106:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 30:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 210:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 120:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 75:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 29:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 198:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 184:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 99:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 57:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 71:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 156:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 226:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 60:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 195:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 102:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 153:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 58:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 83:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 92:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 202:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 78:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 154:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 114:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 89:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 90:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 55:
                case 23:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[3]);
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[1], w[4]);
                            *(dp + 2) = MixEven(w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 182:
                case 150:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + 2) = MixEven(w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[5], w[4]);
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 213:
                case 212:
                    {
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[1]);
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[5], w[4]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = MixEven(w[5], w[7]);
                        }
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        break;
                    }
                case 241:
                case 240:
                    {
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                            *(dp + dpL + dpL + 2) = MixEven(w[5], w[7]);
                        }
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        break;
                    }
                case 236:
                case 232:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + dpL) = MixEven(w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 109:
                case 105:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix3To1(w[3], w[4]);
                            *(dp + dpL + dpL) = MixEven(w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 171:
                case 43:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp) = MixEven(w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = Mix3To1(w[3], w[4]);
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 143:
                case 15:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + 2) = Mix3To1(w[4], w[5]);
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = MixEven(w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[1], w[4]);
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 124:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 203:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 62:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 211:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 118:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 217:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 110:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 155:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 188:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 185:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 61:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 157:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 103:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 227:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 230:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 199:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 220:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 158:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 234:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 242:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 59:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 121:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 87:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 79:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 122:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 94:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 218:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 91:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 229:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 167:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 173:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 181:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 186:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 115:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 93:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 206:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 205:
                case 201:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 174:
                case 46:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 179:
                case 147:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 117:
                case 116:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 189:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 231:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 126:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 219:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 125:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix3To1(w[3], w[4]);
                            *(dp + dpL + dpL) = MixEven(w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 221:
                    {
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + 2) = Mix3To1(w[4], w[1]);
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[5], w[4]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = MixEven(w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        break;
                    }
                case 207:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + 2) = Mix3To1(w[4], w[5]);
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = MixEven(w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[1], w[4]);
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 238:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + dpL) = MixEven(w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 190:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + 2) = MixEven(w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[5], w[4]);
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 187:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp) = MixEven(w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = Mix3To1(w[3], w[4]);
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 243:
                    {
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix3To1(w[7], w[4]);
                            *(dp + dpL + dpL + 2) = MixEven(w[5], w[7]);
                        }
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        break;
                    }
                case 119:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp) = Mix3To1(w[4], w[3]);
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                            *(dp + 1) = Mix3To1(w[1], w[4]);
                            *(dp + 2) = MixEven(w[1], w[5]);
                            *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 237:
                case 233:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 175:
                case 47:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 183:
                case 151:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 245:
                case 244:
                    {
                        *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 250:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 123:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 95:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 222:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 252:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 249:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 235:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 111:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 63:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 159:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 215:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 246:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 254:
                    {
                        *(dp) = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 253:
                    {
                        *(dp) = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + 2) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 251:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                        }
                        *(dp + 2) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 2) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 239:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        *(dp + 2) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 127:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To7To7(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                            *(dp + dpL + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To7To7(w[4], w[7], w[3]);
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                        }
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 191:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 1) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + dpL + 2) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 223:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To7To7(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix7To1(w[4], w[3]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + 2) = w[4];
                            *(dp + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix7To1(w[4], w[1]);
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                            *(dp + dpL + 2) = Mix7To1(w[4], w[5]);
                        }
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 1) = w[4];
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 1) = Mix7To1(w[4], w[7]);
                            *(dp + dpL + dpL + 2) = Mix2To7To7(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 247:
                    {
                        *(dp) = Mix3To1(w[4], w[3]);
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        *(dp + dpL + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 255:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *(dp) = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = w[4];
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 2) = w[4];
                        }
                        else
                        {
                            *(dp + 2) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = w[4];
                        *(dp + dpL + 1) = w[4];
                        *(dp + dpL + 2) = w[4];
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + dpL + 1) = w[4];
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + dpL + 2) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + dpL + 2) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
            }
            sp++;
            dp += 3;
        }
        dp += (dpL * 2);
    }
}

void Scale2(uint32_t* sp, uint32_t* dp, int32_t Xres, int32_t Yres, uint32_t trY, uint32_t trU, uint32_t trV, uint32_t trA, bool wrapX, bool wrapY)
{

    //Don't shift trA, as it uses shift right instead of a mask for comparisons.
    trY <<= 2 * 8;
    trU <<= 1 * 8;
    int32_t dpL = Xres * 2;

    int32_t prevline, nextline;
    uint32_t w[9];

    for (int32_t j = 0; j < Yres; j++)
    {
        if (j > 0)
        {
            prevline = -Xres;
        }
        else
        {
            if (wrapY)
            {
                prevline = Xres * (Yres - 1);
            }
            else
            {
                prevline = 0;
            }
        }
        if (j < Yres - 1)
        {
            nextline = Xres;
        }
        else
        {
            if (wrapY)
            {
                nextline = -(Xres * (Yres - 1));
            }
            else
            {
                nextline = 0;
            }
        }

        for (int32_t i = 0; i < Xres; i++)
        {
            w[1] = *(sp + prevline);
            w[4] = *sp;
            w[7] = *(sp + nextline);

            if (i > 0)
            {
                w[0] = *(sp + prevline - 1);
                w[3] = *(sp - 1);
                w[6] = *(sp + nextline - 1);
            }
            else
            {
                if (wrapX)
                {
                    w[0] = *(sp + prevline + Xres - 1);
                    w[3] = *(sp + Xres - 1);
                    w[6] = *(sp + nextline + Xres - 1);
                }
                else
                {
                    w[0] = w[1];
                    w[3] = w[4];
                    w[6] = w[7];
                }
            }

            if (i < Xres - 1)
            {
                w[2] = *(sp + prevline + 1);
                w[5] = *(sp + 1);
                w[8] = *(sp + nextline + 1);
            }
            else
            {
                if (wrapX)
                {
                    w[2] = *(sp + prevline - Xres + 1);
                    w[5] = *(sp - Xres + 1);
                    w[8] = *(sp + nextline - Xres + 1);
                }
                else
                {
                    w[2] = w[1];
                    w[5] = w[4];
                    w[8] = w[7];
                }
            }

            int32_t pattern = 0;
            int32_t flag = 1;

            for (int32_t k = 0; k < 9; k++)
            {
                if (k == 4) continue;

                if (w[k] != w[4])
                {
                    if (Diff(w[4], w[k], trY, trU, trV, trA))
                        pattern |= flag;
                }
                flag <<= 1;
            }

            switch (pattern)
            {
                case 0:
                case 1:
                case 4:
                case 32:
                case 128:
                case 5:
                case 132:
                case 160:
                case 33:
                case 129:
                case 36:
                case 133:
                case 164:
                case 161:
                case 37:
                case 165:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 2:
                case 34:
                case 130:
                case 162:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 16:
                case 17:
                case 48:
                case 49:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 64:
                case 65:
                case 68:
                case 69:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 8:
                case 12:
                case 136:
                case 140:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 3:
                case 35:
                case 131:
                case 163:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 6:
                case 38:
                case 134:
                case 166:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 20:
                case 21:
                case 52:
                case 53:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 144:
                case 145:
                case 176:
                case 177:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 192:
                case 193:
                case 196:
                case 197:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 96:
                case 97:
                case 100:
                case 101:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 40:
                case 44:
                case 168:
                case 172:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 9:
                case 13:
                case 137:
                case 141:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 18:
                case 50:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 80:
                case 81:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 72:
                case 76:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 10:
                case 138:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 66:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 24:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 7:
                case 39:
                case 135:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 148:
                case 149:
                case 180:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 224:
                case 228:
                case 225:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 41:
                case 169:
                case 45:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 22:
                case 54:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 208:
                case 209:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 104:
                case 108:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 11:
                case 139:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 19:
                case 51:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[3]);
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *dp = Mix5To2To1(w[4], w[1], w[3]);
                            *(dp + 1) = Mix2To3To3(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 146:
                case 178:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                            *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp + 1) = Mix2To3To3(w[4], w[1], w[5]);
                            *(dp + dpL + 1) = Mix5To2To1(w[4], w[5], w[7]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        break;
                    }
                case 84:
                case 85:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + 1) = Mix5To2To1(w[4], w[5], w[1]);
                            *(dp + dpL + 1) = Mix2To3To3(w[4], w[5], w[7]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        break;
                    }
                case 112:
                case 113:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix5To2To1(w[4], w[7], w[3]);
                            *(dp + dpL + 1) = Mix2To3To3(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 200:
                case 204:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                            *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To3To3(w[4], w[7], w[3]);
                            *(dp + dpL + 1) = Mix5To2To1(w[4], w[7], w[5]);
                        }
                        break;
                    }
                case 73:
                case 77:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *dp = Mix5To2To1(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix2To3To3(w[4], w[7], w[3]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 42:
                case 170:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                            *(dp + dpL) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *dp = Mix2To3To3(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 14:
                case 142:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                            *(dp + 1) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *dp = Mix2To3To3(w[4], w[3], w[1]);
                            *(dp + 1) = Mix5To2To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 67:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 70:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 28:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 152:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 194:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 98:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 56:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 25:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 26:
                case 31:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 82:
                case 214:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 88:
                case 248:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 74:
                case 107:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 27:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 86:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 216:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 106:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 30:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 210:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 120:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 75:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 29:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 198:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 184:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 99:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 57:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 71:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 156:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 226:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 60:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 195:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 102:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 153:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 58:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 83:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 92:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 202:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 78:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 154:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 114:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 89:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 90:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 55:
                case 23:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[3]);
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *dp = Mix5To2To1(w[4], w[1], w[3]);
                            *(dp + 1) = Mix2To3To3(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 182:
                case 150:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp + 1) = Mix2To3To3(w[4], w[1], w[5]);
                            *(dp + dpL + 1) = Mix5To2To1(w[4], w[5], w[7]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        break;
                    }
                case 213:
                case 212:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix5To2To1(w[4], w[5], w[1]);
                            *(dp + dpL + 1) = Mix2To3To3(w[4], w[5], w[7]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        break;
                    }
                case 241:
                case 240:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix5To2To1(w[4], w[7], w[3]);
                            *(dp + dpL + 1) = Mix2To3To3(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 236:
                case 232:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To3To3(w[4], w[7], w[3]);
                            *(dp + dpL + 1) = Mix5To2To1(w[4], w[7], w[5]);
                        }
                        break;
                    }
                case 109:
                case 105:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *dp = Mix5To2To1(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix2To3To3(w[4], w[7], w[3]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 171:
                case 43:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + dpL) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *dp = Mix2To3To3(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 143:
                case 15:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *dp = Mix2To3To3(w[4], w[3], w[1]);
                            *(dp + 1) = Mix5To2To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 124:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 203:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 62:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 211:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 118:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 217:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 110:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 155:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 188:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 185:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 61:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 157:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 103:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 227:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 230:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 199:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 220:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 158:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 234:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 242:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 59:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 121:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 87:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 79:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 122:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 94:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 218:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 91:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 229:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 167:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 173:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 181:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 186:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 115:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 93:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 206:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 205:
                case 201:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[6]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix6To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 174:
                case 46:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[0]);
                        }
                        else
                        {
                            *dp = Mix6To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 179:
                case 147:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[2]);
                        }
                        else
                        {
                            *(dp + 1) = Mix6To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 117:
                case 116:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix6To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 189:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 231:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 126:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 219:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 125:
                    {
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[1]);
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *dp = Mix5To2To1(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix2To3To3(w[4], w[7], w[3]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 221:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + 1) = Mix3To1(w[4], w[1]);
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix5To2To1(w[4], w[5], w[1]);
                            *(dp + dpL + 1) = Mix2To3To3(w[4], w[5], w[7]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        break;
                    }
                case 207:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + 1) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *dp = Mix2To3To3(w[4], w[3], w[1]);
                            *(dp + 1) = Mix5To2To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 238:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                            *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To3To3(w[4], w[7], w[3]);
                            *(dp + dpL + 1) = Mix5To2To1(w[4], w[7], w[5]);
                        }
                        break;
                    }
                case 190:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                            *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *(dp + 1) = Mix2To3To3(w[4], w[1], w[5]);
                            *(dp + dpL + 1) = Mix5To2To1(w[4], w[5], w[7]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 187:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                            *(dp + dpL) = Mix3To1(w[4], w[7]);
                        }
                        else
                        {
                            *dp = Mix2To3To3(w[4], w[3], w[1]);
                            *(dp + dpL) = Mix5To2To1(w[4], w[3], w[7]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 243:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = Mix3To1(w[4], w[3]);
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix5To2To1(w[4], w[7], w[3]);
                            *(dp + dpL + 1) = Mix2To3To3(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 119:
                    {
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *dp = Mix3To1(w[4], w[3]);
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *dp = Mix5To2To1(w[4], w[1], w[3]);
                            *(dp + 1) = Mix2To3To3(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 237:
                case 233:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix14To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 175:
                case 47:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix14To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        break;
                    }
                case 183:
                case 151:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix14To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 245:
                case 244:
                    {
                        *dp = Mix2To1To1(w[4], w[3], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix14To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 250:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 123:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 95:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 222:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 252:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix14To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 249:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix14To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 235:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix2To1To1(w[4], w[2], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix14To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 111:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix14To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[5]);
                        break;
                    }
                case 63:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix14To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix2To1To1(w[4], w[8], w[7]);
                        break;
                    }
                case 159:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix14To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 215:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix14To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix2To1To1(w[4], w[6], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 246:
                    {
                        *dp = Mix2To1To1(w[4], w[0], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix14To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 254:
                    {
                        *dp = Mix3To1(w[4], w[0]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix14To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 253:
                    {
                        *dp = Mix3To1(w[4], w[1]);
                        *(dp + 1) = Mix3To1(w[4], w[1]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix14To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix14To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 251:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[2]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix14To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 239:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix14To1To1(w[4], w[3], w[1]);
                        }
                        *(dp + 1) = Mix3To1(w[4], w[5]);
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix14To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[5]);
                        break;
                    }
                case 127:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix14To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix2To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix2To1To1(w[4], w[7], w[3]);
                        }
                        *(dp + dpL + 1) = Mix3To1(w[4], w[8]);
                        break;
                    }
                case 191:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix14To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix14To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[7]);
                        *(dp + dpL + 1) = Mix3To1(w[4], w[7]);
                        break;
                    }
                case 223:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix2To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix14To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[6]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix2To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 247:
                    {
                        *dp = Mix3To1(w[4], w[3]);
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix14To1To1(w[4], w[1], w[5]);
                        }
                        *(dp + dpL) = Mix3To1(w[4], w[3]);
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix14To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
                case 255:
                    {
                        if (Diff(w[3], w[1], trY, trU, trV, trA))
                        {
                            *dp = w[4];
                        }
                        else
                        {
                            *dp = Mix14To1To1(w[4], w[3], w[1]);
                        }
                        if (Diff(w[1], w[5], trY, trU, trV, trA))
                        {
                            *(dp + 1) = w[4];
                        }
                        else
                        {
                            *(dp + 1) = Mix14To1To1(w[4], w[1], w[5]);
                        }
                        if (Diff(w[7], w[3], trY, trU, trV, trA))
                        {
                            *(dp + dpL) = w[4];
                        }
                        else
                        {
                            *(dp + dpL) = Mix14To1To1(w[4], w[7], w[3]);
                        }
                        if (Diff(w[5], w[7], trY, trU, trV, trA))
                        {
                            *(dp + dpL + 1) = w[4];
                        }
                        else
                        {
                            *(dp + dpL + 1) = Mix14To1To1(w[4], w[5], w[7]);
                        }
                        break;
                    }
            }
            sp++;
            dp += 2;
        }
        dp += dpL;
    }
}

}
