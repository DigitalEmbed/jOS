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

#ifndef __JOS_H__
#define __JOS_H__

#ifdef __cplusplus
  extern "C" {
#endif

#define __JOS_ACTIVED__

//! Standard C Libs
/*!
  Path to standard c libraries.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! Path: EmbeddedTools
/*!
  Path to EmbeddedTools library.
*/
#include <EmbeddedTools.h>

//! Path: Tools
/*!
  Path to jOS system task manager libraries.
*/
#include "./Task.h"
#include "./Scheduler.h"
#include "./Coroutines.h"
#include "./Switch.h"
#include "./Semaphores.h"

//! Path: System
/*!
  Path to jOS system libraries.
*/
#include "./System.h"

#ifdef __cplusplus
  }
#endif

#endif
