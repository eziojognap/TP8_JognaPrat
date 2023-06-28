/***************************************************************************************************
 * @file    DIGITAL.H
 * @author  eziojognap
 * @date
 * @brief
 **************************************************************************************************/
#ifndef DIGITAL_H
#define DIGITAL_H
#endif

/************************************* C++ compilar como C
 * ****************************************/
#ifdef __cplusplus /*Esta definición se usa para compilar en c codigo que es  \
                      de c++*/
extern "C" {
#endif

/************************************* INCLUDES
 * ***************************************************/
#include <stdbool.h>
#include <stdint.h>

/************************************* MACROS AND DEFINES
 * *****************************************/

/************************************* TYPEDEFS
 * ***************************************************/
//! Referencia a un descriptor para gestionar una salida digital
typedef struct digital_output_estructura *digital_output_puntero;

//! Referencia a un descriptor para gestionar una entrada digital
typedef struct digital_input_estructura *digital_input_puntero;

/************************************* EXPORTED VARIABLES
 * *****************************************/

/************************************* GLOBAL FUNCTION PROTOTYPES
 * *********************************/

/* Salidas */

/// crea una salida
digital_output_puntero DigitalOutputCreate(uint8_t port, uint8_t pin);

/// @brief  activa una salida
/// @param output
void DigitalOutputActivate(digital_output_puntero output);

/// @brief desactiva una salida
/// @param output
void DigitalOutputDeactivate(digital_output_puntero output);

/// @brief cambia la salida
/// @param output
void DigitalOutputToggle(digital_output_puntero output);

/* Entradas */

/// crea una entrada
digital_input_puntero DigitalInputCreate(uint8_t port, uint8_t pin, bool logic);

void DigitalInputActivate(digital_input_puntero input);

void DigitalInputDeactivate(digital_input_puntero input);

bool DigitalInputGetState(digital_input_puntero input);

bool DigitalInputHasChanged(digital_input_puntero input);

#ifdef __cplusplus /*Con esto se cierra la la def condicional para compilar en \
                      c codigo que es de c++*/
}
#endif