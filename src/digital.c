/***************************************************************************************************
 * @file    DIGITAL.c
 * @author  eziojognap
 * @date
 * @brief
 **************************************************************************************************/

/************************************* INCLUDES
 * ***************************************************/

#include "digital.h"
#include "chip.h"
#include <stdbool.h>

/************************************* EXTERN VARIABLES
 * *******************************************/

/************************************* PRIVATE MACROS AND DEFINES
 * *********************************/

#ifndef INSTANCIAS_OUTPUT
#define INSTANCIAS_OUTPUT 2
#endif

#ifndef INSTANCIAS_INPUT
#define INSTANCIAS_INPUT 6
#endif

/************************************* PRIVATE TYPEDEFS
 * *******************************************/
/// estructura para almacenar el descriptor de una salida digital
struct digital_output_estructura {
  uint8_t port; ///< puerto GPIO de la salida digital
  uint8_t pin;  ///< terminal del puerto GPIO de la salida digital
  bool alocado;
};

/// estructura para almacenar el descriptor de una entrada digital
struct digital_input_estructura {
  uint8_t port; ///< puerto GPIO de la entrada digital
  uint8_t pin;  ///< terminal del puerto GPIO de la entrada digital
  bool alocado;
  bool invertido;
  bool estado_anterior;
};
/************************************* DECLARACION DE FUNCIONES INTERNAS
 * **************************/
digital_output_puntero DigitalOutputAllocated(void);
digital_input_puntero DigitalInputAllocated(void);

/************************************* STATIC VARIABLES
 * *******************************************/

/************************************* GLOBAL VARIABLES
 * *******************************************/

/************************************* STATIC FUNCTION PROTOTYPES
 * *********************************/

/************************************* STATIC FUNCTIONS
 * *******************************************/

/************************************* GLOBAL FUNCTIONS
 * *******************************************/

/* Salidas */

/// @brief Es una funcion parecida a malloc
/// @param
/// @return me devuelve el valor de un puntero donde hay un espacio de memoria
/// disponible
digital_output_puntero DigitalOutputAllocated(void) {

  static struct digital_output_estructura instancias[INSTANCIAS_OUTPUT] = {0};

  digital_output_puntero valor_puntero = NULL;

  for (int i = 0; i < INSTANCIAS_OUTPUT; i++) {
    if (!instancias[i].alocado) {   // si no esta ocupado entra a la condicion
      instancias[i].alocado = true; // como lo voy a ocupar, lo pongo en ocupado
      valor_puntero = &instancias[i];
      break; // sale del bucle
    }
  }
  return valor_puntero;
}

digital_output_puntero DigitalOutputCreate(uint8_t port, uint8_t pin) {

  digital_output_puntero puntero_salida = DigitalOutputAllocated();

  if (puntero_salida) { // si es null no hace nada.
    puntero_salida->port = port;
    puntero_salida->pin = pin;
    // No necesito poner que esta alocado porque ya lo hace la funcion
    // DigitalOutputAllocated
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, puntero_salida->port,
                          puntero_salida->pin, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, puntero_salida->port,
                        puntero_salida->pin, true);
  }

  return puntero_salida;
}

void DigitalOutputActivate(digital_output_puntero output) {
  Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, true);
}

void DigitalOutputDeactivate(digital_output_puntero output) {
  Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->port, output->pin, false);
}

void DigitalOutputToggle(digital_output_puntero output) {
  Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, output->port, output->pin);
}

/* Entradas */

digital_input_puntero DigitalInputAllocated(void) {

  static struct digital_input_estructura instancias[INSTANCIAS_INPUT] = {0};

  digital_input_puntero valor_puntero = NULL;

  for (int i = 0; i < INSTANCIAS_INPUT; i++) {
    if (!instancias[i].alocado) {   // si no esta ocupado entra a la condicion
      instancias[i].alocado = true; // como lo voy a ocupar, lo pongo en ocupado
      valor_puntero = &instancias[i];
      break; // sale del bucle
    }
  }
  return valor_puntero;
}

digital_input_puntero DigitalInputCreate(uint8_t port, uint8_t pin,
                                         bool logic) {

  digital_input_puntero puntero_entrada = DigitalInputAllocated();

  if (puntero_entrada) { // si es null no hace nada.
    puntero_entrada->port = port;
    puntero_entrada->pin = pin;
    puntero_entrada->invertido = logic;
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, puntero_entrada->port,
                        puntero_entrada->pin, false);
  }

  return puntero_entrada;
}

void DigitalInputActivate(digital_input_puntero Input) {
  Chip_GPIO_SetPinState(LPC_GPIO_PORT, Input->port, Input->pin, true);
}

void DigitalInputDeactivate(digital_input_puntero Input) {
  Chip_GPIO_SetPinState(LPC_GPIO_PORT, Input->port, Input->pin, false);
}

/// @brief Analiza si tiene logica Negativa o Positiva y devuelve un valor en
/// logica Positiva
/// @param input
/// @return
bool DigitalInputGetState(digital_input_puntero input) {
  //   bool valor_input, valor_bit, valor;
  //   valor_input = input->invertido;
  //   valor_bit = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, input->port,
  //   input->pin);

  //   // Hace un XOR entre el valor de la entrada y el valor del bit del GPIO
  //   valor = valor_input ^ valor_bit;
  //   return valor;
  return input->invertido ^
         Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, input->port, input->pin);
}

bool DigitalInputHasChange(digital_input_puntero input) {
  bool estado_actual = DigitalInputGetState(input);
  if ((estado_actual) && (!(input->estado_anterior))) { // Si son distintos
    return true;
  } else { // Si son iguales
    return false;
  }
  input->estado_anterior = estado_actual;
}

bool DigitalInputHasActivated(digital_input_puntero input) {
  bool estado_actual = DigitalInputGetState(input);
  if (estado_actual == true && input->estado_anterior == false) {
    return true;
  } else {
    return false;
  }
  input->estado_anterior = estado_actual;
}
