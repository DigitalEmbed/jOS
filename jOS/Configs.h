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

#ifndef jOS_Configs_H
#define jOS_Configs_H

#ifdef __cplusplus
  extern "C" {
#endif

//! Configuration: HAL
/*!
  Set the hal system here.
  Availables:
    - ArduinoAVR
    - HALru
*/
#define   HALru

//! Configuration: Task Manager
/*!
  Configure the jOS system task manager here.
*/
#define   AMOUNT_OF_TASKS                     8
#define   MINIMAL_THREAD_PRIORITY             100
#define   SIZE_OF_TASK_NAME                   7

//! Configuration: Semaphores Manager
/*!
  Configure the jOS system semaphore manager here.
*/
#define   AMOUNT_OF_SEMAPHORES                3
#define   AMOUNT_OF_SWITCHES                  8

#ifdef __cplusplus
  }
#endif

#endif