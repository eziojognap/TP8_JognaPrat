/***************************************************************************************************
 * @file    pantalla.h
 * @author  eziojognap
 * @date
 * @brief
 **************************************************************************************************/

/************************************* C++ compilar como C ****************************************/
#ifdef __cplusplus /*Esta definición se usa para compilar en c codigo que es de c++*/
extern "C" {
#endif

/************************************* INCLUDES ***************************************************/
#include<stdint.h>

/************************************* MACROS AND DEFINES *****************************************/

// Definiciones de bits asociados a cada segmento para construir los digitos

#define SEGMENT_A (1 << 0)
#define SEGMENT_B (1 << 1)
#define SEGMENT_C (1 << 2)
#define SEGMENT_D (1 << 3)
#define SEGMENT_E (1 << 4)
#define SEGMENT_F (1 << 5)
#define SEGMENT_G (1 << 6)
#define SEGMENT_P (1 << 7)


/************************************* TYPEDEFS ***************************************************/
typedef struct display_estructura * display_puntero;

typedef void(*display_screen_off_puntero)(void);

typedef void(*display_segments_on_puntero)(uint8_t segments);

typedef void(*display_digit_on_puntero)(uint8_t digit);

typedef struct display_driver_estructura {

    display_screen_off_puntero ScreenTurnOff;
    display_segments_on_puntero SegmentsTurnOn;
    display_digit_on_puntero DigitTurnOn;

} const * const display_driver_puntero;
/************************************* EXPORTED VARIABLES *****************************************/

/************************************* GLOBAL FUNCTION PROTOTYPES *********************************/
display_puntero DisplayCreate(uint8_t digits, display_driver_puntero driver);

void DisplayWriteBCD(display_puntero display, uint8_t * number, uint8_t size);

void DisplayRefresh(display_puntero display);

/************************************* FIN DEL COMPILADOR DE C++ **********************************/
#ifdef __cplusplus /*Con esto se cierra la la def condicional para compilar en c codigo que es de  \
                      c++*/
}
#endif