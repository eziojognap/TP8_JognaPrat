/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions
 * =============================================================== */

#include "ciaa.h"
#include "digital.h"
#include "reloj.h"
#include <stdbool.h>
#include <stddef.h>

/* === Macros definitions
 * ====================================================================== */

/* === Private data type declarations
 * ========================================================== */
typedef enum {
  SIN_CONFIGURAR,
  MOSTRANDO_HORA,
  AJUSTANDO_MINUTOS_ACTUAL,
  AJUSTANDO_HORAS_ACTUAL,
  AJUSTANDO_MINUTOS_ALARMA,
  AJUSTANDO_HORAS_ALARMA,
} modo_t;

/* === Private variable declarations
 * =========================================================== */
static bool buzzer;
static modo_t modo;
static board_puntero board;
static clock_puntero reloj;

/* === Private function declarations
 * =========================================================== */
void Alarma_ON(clock_puntero reloj);
void CambiarModo(modo_t valor);
void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]);
void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]);

/* === Public variable definitions
 * ============================================================= */

/* === Private variable definitions
 * ============================================================ */
static const uint8_t LIMITE_HORAS[] = {2, 4};
static const uint8_t LIMITE_MINUTOS[] = {6, 0};

/* === Private function implementation
 * ========================================================= */

/* === Public function implementation
 * ========================================================= */
void CambiarModo(modo_t valor) {
  modo = valor;
  switch (modo) {
  case SIN_CONFIGURAR:
    DisplayFlashDigits(board->display, 0, 3, 200);
    break;
  case MOSTRANDO_HORA:
    DisplayFlashDigits(board->display, 0, 0, 0);
    break;
  case AJUSTANDO_MINUTOS_ACTUAL:
    DisplayFlashDigits(board->display, 0, 1, 200);
    break;
  case AJUSTANDO_HORAS_ACTUAL:
    DisplayFlashDigits(board->display, 2, 3, 200);
    break;
  case AJUSTANDO_MINUTOS_ALARMA:
    DisplayFlashDigits(board->display, 0, 1, 200);
    break;
  case AJUSTANDO_HORAS_ALARMA:
    DisplayFlashDigits(board->display, 2, 3, 200);
    DisplayDotOn(board->display, 0);
    DisplayDotOn(board->display, 1);
    DisplayDotOn(board->display, 2);
    DisplayDotOn(board->display, 3);
    break;
  default:
    break;
  }
}

void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
  numero[1]++;
  if (numero[1] == 10) {
    numero[1] = 0;
    numero[0]++;
  }
  if ((numero[0] == limite[0]) && (numero[1] == limite[1])) {
    numero[1] = 0;
    numero[0] = 0;
  }
}

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
  numero[1]--;
  if (numero[1] > 9) {
    numero[1] = 9;
    if (numero[0] > 0) {
      numero[0]--;
    } else {
      if (limite[0] == 2) {
        numero[0] = limite[0];
      } else {
        numero[0] = limite[0] - 1;
      }
      if (limite[1] != 0) {
        numero[1] = limite[1] - 1;
      } else {
        numero[1] = 9;
      }
    }
  }
}

void Alarma_ON(clock_puntero reloj) {
  DigitalOutputActivate(board->buzzer);
  buzzer = true;
}

int main(void) {
  uint8_t entrada[4];

  reloj = ClockCreate(30, Alarma_ON);
  board = BoardCreate();

  SisTick_Init(1000);
  CambiarModo(SIN_CONFIGURAR);

  while (true) {
    if (DigitalInputHasActivated(board->accept) == true) {
      if (buzzer == true) {
        DigitalOutputDeactivate(board->buzzer);
        ClockActivateSnoozeAlarm(reloj);
        buzzer = false;
      } else {
        if (modo == AJUSTANDO_MINUTOS_ACTUAL) {
          CambiarModo(AJUSTANDO_HORAS_ACTUAL);
        } else if (modo == AJUSTANDO_HORAS_ACTUAL) {
          ClockSetTime(reloj, entrada, sizeof(entrada));
          CambiarModo(MOSTRANDO_HORA);
        } else if (modo == AJUSTANDO_MINUTOS_ALARMA) {
          CambiarModo(AJUSTANDO_HORAS_ALARMA);
        } else if (modo == AJUSTANDO_HORAS_ALARMA) {
          ClockSetUpAlarm(reloj, entrada, sizeof(entrada));
          CambiarModo(MOSTRANDO_HORA);
          ClockAlarmActivate(reloj);
          DisplayDotOn(board->display, 3);
        }
      }
    }

    if (DigitalInputHasActivated(board->cancel) == true) {
      if (buzzer == true) {
        DigitalOutputDeactivate(board->buzzer);
        buzzer = false;
      } else {
        if (modo == MOSTRANDO_HORA) {
          ClockAlarmDeactivate(reloj);
          DisplayDotOff(board->display, 3);
        } else if (ClockGetTime(reloj, entrada, sizeof(entrada))) {
          CambiarModo(MOSTRANDO_HORA);
        } else {
          CambiarModo(SIN_CONFIGURAR);
        }
      }
    }

    if (DigitalInputHasActivated(board->set_time) == true) {
      CambiarModo(AJUSTANDO_MINUTOS_ACTUAL);
      ClockGetTime(reloj, entrada, sizeof(entrada));
      DisplayWriteBCD(board->display, entrada, sizeof(entrada));
    }

    if (DigitalInputHasActivated(board->set_alarm) == true) {
      CambiarModo(AJUSTANDO_MINUTOS_ALARMA);
      ClockGetAlarm(reloj, entrada, sizeof(entrada));
      DisplayWriteBCD(board->display, entrada, sizeof(entrada));
    }

    if (DigitalInputHasActivated(board->decrement) == true) {
      if (modo == AJUSTANDO_MINUTOS_ACTUAL) {
        DecrementarBCD(&entrada[2], LIMITE_MINUTOS);
      } else if (modo == AJUSTANDO_HORAS_ACTUAL) {
        DecrementarBCD(entrada, LIMITE_HORAS);
      } else if (modo == AJUSTANDO_MINUTOS_ALARMA) {
        DecrementarBCD(&entrada[2], LIMITE_MINUTOS);
      } else if (modo == AJUSTANDO_HORAS_ALARMA) {
        DecrementarBCD(entrada, LIMITE_HORAS);
      }
      DisplayWriteBCD(board->display, entrada, sizeof(entrada));
    }

    if (DigitalInputHasActivated(board->increment) == true) {
      if (modo == AJUSTANDO_MINUTOS_ACTUAL) {
        IncrementarBCD(&entrada[2], LIMITE_MINUTOS);
      } else if (modo == AJUSTANDO_HORAS_ACTUAL) {
        IncrementarBCD(entrada, LIMITE_HORAS);
      } else if (modo == AJUSTANDO_MINUTOS_ALARMA) {
        IncrementarBCD(&entrada[2], LIMITE_MINUTOS);
      } else if (modo == AJUSTANDO_HORAS_ALARMA) {
        IncrementarBCD(entrada, LIMITE_HORAS);
      }
      DisplayWriteBCD(board->display, entrada, sizeof(entrada));
    }

    // DisplayRefresh(board->display);
  }
}
void SysTick_Handler(void) {
  static bool last_value = false;
  bool current_value;
  uint8_t hora[4];

  DisplayRefresh(board->display);
  current_value = ClockNewTick(reloj);

  if (modo > AJUSTANDO_HORAS_ACTUAL) {
    DisplayDotOn(board->display, 0);
    DisplayDotOn(board->display, 1);
    DisplayDotOn(board->display, 2);
    DisplayDotOn(board->display, 3);
  }

  if (current_value != last_value) {
    last_value = current_value;

    if (modo <= MOSTRANDO_HORA) {
      ClockGetTime(reloj, hora, sizeof(hora));
      DisplayWriteBCD(board->display, hora, sizeof(hora));

      if (modo == MOSTRANDO_HORA) {
        DisplayToggleDot(board->display, 1);
      }
    }
  }
}

/* === End of documentation
 * ==================================================================== */

/** @} End of module definition for doxygen */