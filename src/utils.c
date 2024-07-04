#include "utils.h"
#include <stdbool.h>

#define DEBOUNCE_INTERVAL 300

static const int io_periph[] = {GPIOA, GPIOA, GPIOA, GPIOC, GPIOA, GPIOC, GPIOC};
static const int io_pin[] = {GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_13, GPIO_PIN_0, GPIO_PIN_14, GPIO_PIN_15};

/**
 * @param[in] ch One of the 7 enumerations in utils.h
 * @returns 1 if button ch is pressed, 0 otherwise
 * */
int Get_Button(int ch)
{
    return (int)(gpio_input_bit_get(io_periph[ch], io_pin[ch]));
}

/**
 * @returns 1 if button BOOT0 ch is pressed, 0 otherwise
 * */
int Get_BOOT0(void)
{
    return (int)(gpio_input_bit_get(GPIOA, GPIO_PIN_8));
}


// bool debounce(int ch)
// {
//     static uint64_t last_time = 0;
//     uint64_t current_time = get_timer_value();
//     if (current_time - last_time < DEBOUNCE_INTERVAL)
//     {
//         return false;
//     }
//     last_time = current_time;
//     return true;
// }