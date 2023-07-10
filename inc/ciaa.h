/***************************************************************************************************
 * @file    ciaa.H
 * @author  eziojognap
 * @date
 * @brief
 **************************************************************************************************/

/************************************* C++ compilar como C
 * ****************************************/
#ifdef __cplusplus /*Esta definición se usa para compilar en c codigo que es  \
                      de c++*/
extern "C" {
#endif

/************************************* INCLUDES
 * ***************************************************/
#include "digital.h"
#include "pantalla.h"
#include <stdbool.h>

/************************************* MACROS AND DEFINES
 * *****************************************/

/************************************* TYPEDEFS
 * ***************************************************/

typedef struct board_estructura {
  digital_input_puntero set_time;
  digital_input_puntero set_alarm;
  digital_input_puntero decrement;
  digital_input_puntero increment;
  digital_input_puntero accept;
  digital_input_puntero cancel;

  digital_output_puntero buzzer;

  display_puntero display;

} const *const board_puntero;

/************************************* EXPORTED VARIABLES
 * *****************************************/

/************************************* GLOBAL FUNCTION PROTOTYPES
 * *********************************/

board_puntero BoardCreate(void);

void SisTick_Init(uint16_t ticks);

#ifdef __cplusplus /*Con esto se cierra la la def condicional para compilar en \
                      c codigo que es de c++*/
}
#endif