#ifndef PULSEIN_H
#define PULSEIN_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

extern TIM_HandleTypeDef htim2;
#define PULSEIN_GPIO_PIN        GPIO_PIN_0    
#define PULSEIN_TIM_FREQ_MHZ    1               
#define PULSEIN_GPIO_PORT       GPIOA

void PulseIn_Init(void);
uint32_t pluseIn(void);
uint32_t PulseIn_Read(void);
uint32_t PulseIn_ReadTimeout(uint32_t timeout_us);
void PulseIn_SetTimeout(uint32_t timeout_us);
bool PulseIn_IsReady(void);

#endif // PULSEIN_H
