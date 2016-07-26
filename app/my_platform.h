#ifndef __MY_PLATFORM_H
#define __MY_PLATFORM_H

#include "api.h"
#include "debug.h"
#include "app_cfg.h"

#include "stm32f10x.h"
#include "stm32_eval.h"

#ifdef __cplusplus
extern "C"
{
#endif

void delay_1us(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t get_random(void);
void soft_reset(void);

#ifdef __cplusplus
}

#endif

#endif
