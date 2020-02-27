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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <EmbeddedTools.h>

extern const uint8_t __ui8TaskTickMS;                                                         /*!< extern const 8-bit integer type variable. */
extern const uint8_t __ui8SemaphoreTickMS;                                                    /*!< extern const 8-bit integer type variable. */

void System_taskTimerConfiguration(void (*vfSchedulerInterruption)(void));                    /*!< void type function. */
void System_semaphoreTimerConfiguration(void (*vfSemaphoresInterruption)(void));              /*!< void type function. */
void System_sleepConfiguration(void);                                                         /*!< void type function. */
void System_sleep(void);                                                                      /*!< void type function. */
void System_restart(void);                                                                    /*!< void type function. */
void System_enableInterrupts(void);                                                           /*!< void type function. */
void System_enableHardwareWatchdog(void);                                                     /*!< void type function. */
void System_disableHardwareWatchdog(void);                                                    /*!< void type function. */

#ifdef __cplusplus
  }
#endif

#endif
