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

#ifndef __COROUTINES_H__
#define __COROUTINES_H__

#ifdef __cplusplus
  extern "C" {
#endif

  #include "./Task.h"
  #include "./Scheduler.h"

  //! Macro: Coroutine Block Initializer
  /*!
    This macro initializes coroutine block in your void function.
  */
  #define coroutine\
    static uint8_t __ui8TaskYield = 1;\
    static uint16_t __ui16TaskPeriodBackup = 0;\
    __ui16TaskPeriodBackup = 0;\
    if (__ui8TaskYield != 1){\
      System_restart();\
    }\
    __ui8TaskYield = 0;\
    switch(__tCurrentTask->ui16Line){\
      case 0:

  //! Macro: Coroutine Block Finalizer
  /*!
    This macro finalizes a coroutine block.
  */
  #define end_coroutine\
    }\
    __ui8TaskYield = 1;\
    __tCurrentTask->ui16Line = __tCurrentTask->ui16StartLine;\
    if (__ui16TaskPeriodBackup != 0){\
      __tCurrentTask->ui16TimeCounter = __tCurrentTask->ui16Period;\
    }\

  //! Macro: Coroutine/Thread Context Changer
  /*!
    This macro pauses a coroutine block/thread in a void function.
  */
  #define Task_yield()\
    __tCurrentTask->ui16Line = __LINE__ ; __ui8TaskYield = 1 ; return ; case __LINE__: __ui8TaskYield = 0;

  //! Macro: Coroutine/Thread Pauser
  /*!
    This macro pauses a coroutine block/thread in a void function until a condition is satisfied.
    \param xCondition is the condition that will be verified.
  */
  #define Task_waitUntil(xCondition)\
    __tCurrentTask->ui16Line = __LINE__ ; case __LINE__: if ((xCondition)) {__ui8TaskYield = 1 ; return ;} __ui8TaskYield = 0;

  //! Macro: Coroutine/Thread Pauser
  /*!
    This macro pauses a coroutine block/thread in a void function until a condition isn't satisfied.
    \param xCondition is the condition that will be verified.
  */
  #define Task_waitFor(xCondition)\
    __tCurrentTask->ui16Line = __LINE__ ; case __LINE__: if (!(xCondition)) {__ui8TaskYield = 1 ; return ;} __ui8TaskYield = 0;

  //! Macro: Coroutine/Thread Pauser
  /*!
    This macro pauses a coroutine block/thread in a void function in a time period.
    \param ui16TimeMS is a 16-bits unsigned integer. It's the time period that the function will be paused.
  */
  #define Task_delayMS(ui16TimeMS)\
    __ui16TaskPeriodBackup = __tCurrentTask->ui16Period ;\
    Task_setPeriod(NULL, ((ui16TimeMS) < __ui8TaskTickMS ? __ui8TaskTickMS : (ui16TimeMS - __ui8TaskTickMS)));\
    __tCurrentTask->ui16Line = __LINE__ ; __ui8TaskYield = 1 ; return ; case __LINE__:\
    Task_setPeriod(NULL, __ui16TaskPeriodBackup);\
    __ui8TaskYield = 0;
  
  //! Macro: Once Block Initializer
  /*!
    This macro initialize a block that will be runned once time.
  */
  #define once\
    static uint8_t __ui8RunOnce = 0;\
    if (__ui8RunOnce == 0)

  //! Macro: Once Block Finalizer
  /*!
    This macro finalizes a once block.
  */
  #define end_once\
    __ui8RunOnce = 1;
  
  //! Macro: Thread Modifier
  /*!
    This macro simulates a function modifier. Basically, this modifier initializer a corountine blocks in function start.
  */
  #define THREAD\
    {\
      coroutine

  #define thread_loop\
    __tCurrentTask->ui16StartLine = __LINE__; case __LINE__:

  //! Macro: Thread Modifier
  /*!
    This macro finalizes a THREAD block.
  */
  #define END_THREAD\
      end_coroutine;\
    }

#ifdef __cplusplus
  }
#endif

#endif