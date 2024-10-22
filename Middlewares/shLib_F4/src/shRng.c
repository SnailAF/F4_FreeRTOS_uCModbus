#include "shRng.h"

static void RNG_Init(void)
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_RNG);
    LL_RNG_Enable(RNG);
}

uint32_t RNG_GetRandNum(void)
{
    static uint8_t isInit = 0;
    uint32_t rand;
    
    if(!isInit)
    {
        RNG_Init();
        isInit = 1;
    }
    
    while(!LL_RNG_IsActiveFlag_DRDY(RNG));
    rand = LL_RNG_ReadRandData32(RNG);
    return rand;
}
