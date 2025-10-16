// PulseIn.c
#include "pulseIn.h"
#include "main.h"

static uint32_t pulse_start_time = 0;
static uint32_t pulse_end_time = 0;
static volatile uint8_t pulse_state = 0; 
static volatile uint8_t pulse_valid = 0;
static uint32_t timeout_us = 1000000;   

/**
 * @brief  ?????????
 * @param  None
 * @retval None
 */
void PulseIn_Init(void) {
    // ?????
    HAL_TIM_Base_Start(&htim2);
    
    // ???????
    pulse_start_time = 0;
    pulse_end_time = 0;
    pulse_state = 0;
    pulse_valid = 0;
    timeout_us = 1000000; // ????????
}

/**
 * @brief  ????:?????????
 * @param  None
 * @retval None
 */
static void PulseIn_Update(void) {
    static uint32_t last_update_time = 0;
    uint32_t current_time = __HAL_TIM_GET_COUNTER(&htim2);
    
    // ??????
    if ((current_time - last_update_time) > (timeout_us * PULSEIN_TIM_FREQ_MHZ)) {
        // ???????
        pulse_state = 0;
        pulse_valid = 0;
        last_update_time = current_time;
    }
    
    if(pulse_state == 0) { // ?????
        if(HAL_GPIO_ReadPin(PULSEIN_GPIO_PORT, PULSEIN_GPIO_PIN) == GPIO_PIN_SET) {
            pulse_start_time = current_time;
            pulse_state = 1; // ??????????
            last_update_time = current_time;
        }
    } else { // ?????
        if(HAL_GPIO_ReadPin(PULSEIN_GPIO_PORT, PULSEIN_GPIO_PIN) == GPIO_PIN_RESET) {
            pulse_end_time = current_time;
            pulse_state = 0; // ??????????
            pulse_valid = 1; // ??????
            last_update_time = current_time;
        }
    }
}

/**
 * @brief  ??????(???)
 * @param  None
 * @retval ??????(??),?????????0
 */
uint32_t pluseIn(void) {
    PulseIn_Update();
    
    if(pulse_valid) {
        uint32_t raw_duration;
        
        // ??????(?????????)
        if(pulse_end_time >= pulse_start_time) {
            raw_duration = pulse_end_time - pulse_start_time;
        } else {
            // ??????32????
            raw_duration = (0xFFFFFFFF - pulse_start_time) + pulse_end_time + 1;
        }
        
        // ?????
        uint32_t duration_us = raw_duration / PULSEIN_TIM_FREQ_MHZ;
        
        // ??????,???????
        pulse_valid = 0;
        
        return duration_us;
    }
    
    return 0;
}

/**
 * @brief  ??????(???)- ????????
 * @param  None
 * @retval ??????(??),?????????0
 */
uint32_t PulseIn_Read(void) {
    return pluseIn();
}

/**
 * @brief  ??????(???,???)
 * @param  timeout_us ????(??)
 * @retval ??????(??),??????0
 */
uint32_t PulseIn_ReadTimeout(uint32_t timeout_us) {
    uint32_t start_time = __HAL_TIM_GET_COUNTER(&htim2);
    uint32_t timeout_ticks = timeout_us * PULSEIN_TIM_FREQ_MHZ;
    
    while((__HAL_TIM_GET_COUNTER(&htim2) - start_time) < timeout_ticks) {
        uint32_t result = pluseIn();
        if(result > 0) {
            return result;
        }
        // ???????????CPU??
        // HAL_Delay(1);
    }
    
    return 0; // ??
}

/**
 * @brief  ??????
 * @param  timeout_us ????(??)
 * @retval None
 */
void PulseIn_SetTimeout(uint32_t timeout_us) {
    timeout_us = timeout_us;
}

/**
 * @brief  ????????????
 * @param  None
 * @retval true: ?????, false: ?????
 */
bool PulseIn_IsReady(void) {
    PulseIn_Update();
    return pulse_valid ? true : false;
}
