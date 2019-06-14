//! jOS Version 1.0b
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

#ifndef Switch_h
#define Switch_h

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdio.h>
#include <inttypes.h>

//! Macros: Switch Satus
/*!
  This macros are for facilitate the use of this library.
*/
#define   ERROR_SWITCH_NOT_INITIALIZED      0
#define   SWITCH_INITIALIZED                1

#define   TURNED_OFF_SWITCH                 0
#define   TURNED_ON_SWITCH                  1

//! Type Definition: switch_t
/*!
  This typedef exist for organization purpose. This type is equivalent of a 8-bit unsigned integer.
*/
typedef uint8_t switch_t;

uint8_t ui8SwitchInit(switch_t* spSwitch);          /*!< 8-bits integer type function. */
void vTurnOnSwitch(switch_t sSwitch);               /*!< 8-bits integer type function. */
void vTurnOffSwitch(switch_t sSwitch);              /*!< 8-bits integer type function. */
uint8_t ui8GetSwitchStatus(switch_t sSwitch);       /*!< 8-bits integer type function. */

#ifdef __cplusplus
  }
#endif

#endif
