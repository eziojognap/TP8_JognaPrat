/***************************************************************************************************
 * @file    pantalla.c
 * @brief
 **************************************************************************************************/

/************************************* INCLUDES ***************************************************/
#include "pantalla.h"
#include <string.h>

/************************************* EXTERN VARIABLES *******************************************/

/************************************* PRIVATE MACROS AND DEFINES *********************************/
#ifndef DISPLAY_MAX_DIGITS
#define DISPLAY_MAX_DIGITS 8
#endif

/************************************* PRIVATE TYPEDEFS *******************************************/
struct display_estructura {
    uint8_t digits;
    uint8_t active_digit;
    uint8_t memory[DISPLAY_MAX_DIGITS];
    struct display_driver_estructura driver[1];
};

/************************************* PRIVATE VARIABLES ******************************************/
static const uint8_t IMAGES[] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,
                SEGMENT_B | SEGMENT_C,
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,
                SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_B | SEGMENT_C,
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,
};

/************************************* GLOBAL VARIABLES *******************************************/

/************************************* STATIC FUNCTION PROTOTYPES *********************************/
static display_puntero DisplayAllocate(void);

/************************************* STATIC FUNCTIONS *******************************************/
static display_puntero DisplayAllocate(void) {
    static struct display_estructura instances[1] = {0};

    return &instances[0];
}
/************************************* GLOBAL FUNCTIONS *******************************************/
display_puntero DisplayCreate(uint8_t digits, display_driver_puntero driver) {
    display_puntero display = DisplayAllocate();
    
    if (display) {
        display->digits = digits;
        display->active_digit = digits - 1;
        memcpy(display->driver, driver, sizeof(display->driver));
        memset(display->memory, 0, sizeof(display->memory));
        display->driver->ScreenTurnOff();
    }

    return display;
}

void DisplayWriteBCD(display_puntero display, uint8_t * number, uint8_t size) {
    memset(display->memory, 0, sizeof(display->memory));
    for(int index = 0; index < size; index++){
        if(index >= display -> digits) break;
        display->memory[index] = IMAGES[number[index]];
    }
}

void DisplayRefresh(display_puntero display){
    display->driver->ScreenTurnOff();
    display->active_digit = (display->active_digit + 1) % display->digits;
    display->driver->SegmentsTurnOn(display->memory[display->active_digit]);
    display->driver->DigitTurnOn(display->active_digit);
}