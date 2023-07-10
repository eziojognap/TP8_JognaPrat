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

/* === Private variable declarations
 * =========================================================== */
static bool buzzer;

/* === Private function declarations
 * =========================================================== */
void Alarma_ON(clock_puntero reloj) { buzzer = true; }
/* === Public variable definitions
 * ============================================================= */

/* === Private variable definitions
 * ============================================================ */

/* === Private function implementation
 * ========================================================= */

/* === Public function implementation
 * ========================================================= */
int main(void) {

  clock_puntero reloj = ClockCreate(100, Alarma_ON);
  board_puntero board = BoardCreate();
  //  modo = SIN_CONFIGURAR;

  // SisTick_Init(1000);
  // DisplayFlashDigits(board->display, 0, 3, 200);

  while (true) {
    if (DigitalInputHasActivated(board->accept) == true) {
      DisplayWriteBCD(board->display, (uint8_t[]){1, 2, 3, 4}, 4);
      DisplayRefresh(board->display);
    }

    if (DigitalInputHasActivated(board->cancel) == true) {
      // DisplayWriteBCD(board->display, NULL, 0);
      // DisplayRefresh(board->display);
    }

    if (DigitalInputHasActivated(board->set_time) == true) {
      // DisplayWriteBCD(board->display, (uint8_t[]){1, 1, 1, 1}, 4);
      // DisplayRefresh(board->display);
    }

    if (DigitalInputHasActivated(board->set_alarm) == true) {
      // DisplayWriteBCD(board->display, (uint8_t[]){2, 2, 2, 2}, 4);
    }

    if (DigitalInputHasActivated(board->decrement) == true) {
      // DisplayWriteBCD(board->display, (uint8_t[]){5, 6, 7, 8}, 4);
    }

    if (DigitalInputHasActivated(board->increment) == true) {
      // DisplayWriteBCD(board->display, (uint8_t[]){9, 9, 9, 9}, 4);
    }

    // DisplayRefresh(board->display);
  }
  void Systick_Handler(void) {
    static bool last_value = false;
    bool current_value;

    DisplayRefresh(board->display);
    current_value = ClockNewTick(reloj);

    if (current_value != last_value) {
      DisplayToggleDot(board->display, 1);
      last_value = current_value;
    }
  }
}

/* === End of documentation
 * ==================================================================== */

/** @} End of module definition for doxygen */