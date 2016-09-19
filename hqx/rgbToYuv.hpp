#pragma once

#include <stdint.h>
#include <cmath>

namespace hqx
{

const uint32_t RgbMask = 0xffffff00;
const uint32_t RgbShift = 8;

const int Ymask = 0xff000000; const int Yshift = 24;
const int Umask = 0x00ff0000; const int Ushift = 16;
const int Vmask = 0x0000ff00; const int Vshift = 8;
const int Amask = 0x000000ff;

void initialize();

uint32_t GetYuv(uint32_t rgb);

bool Diff(uint32_t c1, uint32_t c2, uint32_t trY, uint32_t trU, uint32_t trV, uint32_t trA);

const uint32_t MaskAlpha    = 0x000000ff;
const uint32_t MaskGreen    = 0x00ff0000;
const uint32_t MaskRedBlue  = 0xff00ff00;

uint32_t MixColours(uint32_t w1, uint32_t w2, uint32_t w3, uint32_t c1, uint32_t c2, uint32_t c3);
uint32_t MixColours(uint32_t w1, uint32_t w2, uint32_t c1, uint32_t c2);

uint32_t Mix3To1(uint32_t c1, uint32_t c2);
uint32_t Mix2To1To1(uint32_t c1, uint32_t c2, uint32_t c3);
uint32_t Mix7To1(uint32_t c1, uint32_t c2);
uint32_t Mix2To7To7(uint32_t c1, uint32_t c2, uint32_t c3);
uint32_t MixEven(uint32_t c1, uint32_t c2);
uint32_t Mix5To2To1(uint32_t c1, uint32_t c2, uint32_t c3);
uint32_t Mix6To1To1(uint32_t c1, uint32_t c2, uint32_t c3);
uint32_t Mix5To3(uint32_t c1, uint32_t c2);
uint32_t Mix2To3To3(uint32_t c1, uint32_t c2, uint32_t c3);
uint32_t Mix14To1To1(uint32_t c1, uint32_t c2, uint32_t c3);

}
