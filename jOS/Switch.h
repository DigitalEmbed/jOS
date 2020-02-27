//! jOS Version 3.0b
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

#ifndef __SWITCH_H__
  #define __SWITCH_H__

  #include "./Configs.h"

  #if defined(__SWITCHES_MANAGER_ENABLE__) && defined(__AMOUNT_OF_SWITCHES__) && (__AMOUNT_OF_SWITCHES__ > 0)

    #ifdef __cplusplus
      extern "C" {
    #endif

    #include <stdio.h>
    #include <stdint.h>

    //! Type Enumeration: Switch Satus
    /*!
      This typedef is for facilitate the use of this library.
    */
    typedef enum{
      SWITCH_STATUS_ERROR_NOT_INITIALIZED = 0,
      SWITCH_STATUS_INITIALIZED,
      SWITCH_STATUS_TURNED_OFF,
      SWITCH_STATUS_TURNED_ON
    } switch_status_t;

    //! Type Definition: switch_t
    /*!
      This typedef exist for organization purpose. This type is equivalent of a 8-bit unsigned integer.
    */
    typedef uint8_t switch_t;

    switch_status_t newSwitch(switch_t* swSwitch);                  /*!< switch_status_t integer type function. */
    switch_status_t Switch_getStatus(switch_t swSwitch);            /*!< switch_status_t integer type function. */
    void Switch_turnOn(switch_t swSwitch);                          /*!< switch_status_t integer type function. */
    void Switch_turnOff(switch_t swSwitch);                         /*!< switch_status_t integer type function. */

    #ifdef __cplusplus
      }
    #endif

  #endif
#endif
