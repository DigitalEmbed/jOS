//! jOS Version 2.0b
/*!
  This code file was written by Jorge Henrique Moreira Santana and is under
  the GNU GPLv3 license. All legal rights are reserved.

  Permissions of this copyleft license are conditioned on making available
  complete source code of licensed works and modifications under the same
  license or the GNU GPLv3. Copyright and license notices must be preserved.
  Contributors provide an express grant of patent rights. However, a larger
  work using the licensed work through interfaces provided by the licensed
  work may be distributed under different terms and without source code for
  the larger work.

  * Permissions:
    -> Commercial use;
    -> Modification;
    -> Distribuition;
    -> Patent Use;
    -> Private Use;

  * Limitations:
    -> Liability;
    -> Warranty;

  * Conditions:
    -> License and copyright notice;
    -> Disclose source;
    -> State changes;
    -> Same license (library);

  For more informations, check the LICENSE document. If you want to use a
  commercial product without having to provide the source code, send an email
  to jorge_henrique_123@hotmail.com to talk.
*/

#ifndef System_H
#define System_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "./Configs.h"
#include <stdint.h>
#include <stdio.h>

extern const uint8_t ui8TickMS;                                                             /*!< extern const 8-bit integer type variable. */

void vTaskTimerConfiguration(void (*vSchedulerInterruption)(void));                         /*!< void type function. */
void vSemaphoreTimerConfiguration(void (*vSemaphoresInterruption)(void));                   /*!< void type function. */
void vSystemSleepConfiguration(void);                                                       /*!< void type function. */
void vSystemSleep(void);                                                                    /*!< void type function. */
void vEnableWatchdog(void);                                                                 /*!< void type function. */
void vDisableWatchdog(void);                                                                /*!< void type function. */
void vRestartSystem(void);                                                                  /*!< void type function. */

#ifdef __cplusplus
  }
#endif

#endif
