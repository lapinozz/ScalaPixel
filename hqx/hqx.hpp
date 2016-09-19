#pragma once

#include "rgbToYuv.hpp"

namespace hqx
{

void Scale4(uint32_t* sp, uint32_t* dp, int32_t Xres, int32_t Yres, uint32_t trY = 48, uint32_t trU = 7, uint32_t trV = 6, uint32_t trA = 0, bool wrapX = false, bool wrapY = false);
void Scale3(uint32_t* sp, uint32_t* dp, int32_t Xres, int32_t Yres, uint32_t trY = 48, uint32_t trU = 7, uint32_t trV = 6, uint32_t trA = 0, bool wrapX = false, bool wrapY = false);
void Scale2(uint32_t* sp, uint32_t* dp, int32_t Xres, int32_t Yres, uint32_t trY = 48, uint32_t trU = 7, uint32_t trV = 6, uint32_t trA = 0, bool wrapX = false, bool wrapY = false);

}
