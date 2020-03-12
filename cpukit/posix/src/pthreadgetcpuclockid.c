/**
 * @file
 *
 * @ingroup POSIXAPI
 *
 * @brief Returns Clock ID for CPU time clock of the thread 
 */

/*
 *  20.1.6 Accessing a Thread CPU-time Clock, P1003.4b/Draft 8, p. 58
 */

/*
 *  COPYRIGHT (c) 1989-2007,2020.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

#include <rtems/score/threadimpl.h>

int pthread_getcpuclockid(
  pthread_t    thread,
  clockid_t   *clock_id
)
{
  Thread_Control               *the_thread;
  ISR_lock_Context              lock_context;

  if ( clock_id == NULL ) {
    return EINVAL;
  }

  the_thread = _Thread_Get( thread, &lock_context );

  if ( the_thread == NULL ) {
    return ESRCH;
  }

  _Thread_State_acquire_critical( the_thread, &lock_context );

  *clock_id = the_thread->Object.id;

  _Thread_State_release( the_thread, &lock_context );

  return 0;
}
