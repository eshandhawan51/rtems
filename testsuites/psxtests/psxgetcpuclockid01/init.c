/*
 *  @file
 *  @brief Test suite for getcpuclockid methods
 */

/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (C) 2020 embedded brains GmbH, Eshan Dhawan
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <rtems.h>
#include <rtems/timespec.h>
#include <rtems/test.h>
#include <time.h>
#include <unistd.h>
#include <rtems/test.h>
#include <rtems/bspIo.h>

#include "tmacros.h"
#include "test_support.h"

const char rtems_test_name[] = "PSXGETCPUCLOCKID 1";

/* Forward declaration to avoid warnings */
void *POSIX_Init (void * argument);
void *test_pthread(void *);

static char buffer[512];

static const T_action actions[] = {
  T_report_hash_sha256,
  T_check_task_context,
  T_check_file_descriptors,
  T_check_rtems_barriers,
  T_check_rtems_extensions,
  T_check_rtems_message_queues,
  T_check_rtems_partitions,
  T_check_rtems_periods,
  T_check_rtems_regions,
  T_check_rtems_semaphores,
  T_check_rtems_tasks,
  T_check_rtems_timers,
  T_check_posix_keys
};

static const T_config config = {
  .name = "psxgetcpuclockid1",
  .buf = buffer,
  .buf_size = sizeof(buffer),
  .putchar = rtems_put_char,
  .verbosity = T_VERBOSE,
  .now = T_now_clock,
  .action_count = T_ARRAY_SIZE(actions),
  .actions = actions
};

/* test_pthread function begins */
void *test_pthread(void *arg)
{
  int i;
  /*do something*/
  for( i = 0; i < 200000; i++ );

  pthread_exit(0);
  return NULL;
}

/* init test function begins */ 
T_TEST_CASE(pthread_getcpuclockid)
{
  
  int r;
  clockid_t cid;
  pthread_t thread_id;
  struct timespec start_cpu, start_wall, finish;
  struct timespec time_passed_wall, time_passed_cpu ;
  bool result ;
  /* setting  the timespecs to zero */
  rtems_timespec_zero( &start_cpu );
  rtems_timespec_zero( &start_wall );
  rtems_timespec_zero( &finish );
  rtems_timespec_zero( &time_passed_wall );
  rtems_timespec_zero( &time_passed_cpu );
  
  /* getting the cpuclock_id */ 
  r = pthread_getcpuclockid( pthread_self(), &cid );
  /* Error checking for pthread_getcpuclockid() */ 
  T_quiet_psx_success(r);

  /* getting the start cpu time of the pthread */
  r = clock_gettime( cid, &start_cpu );
  /* Error checking for clock_gettime() */
  T_quiet_psx_success(r);

  /* starting the wall time */
  r = clock_gettime( CLOCK_MONOTONIC, &start_wall );
  /* Error checking clock_gettime */
  T_quiet_psx_success(r);

  /* starting the loop for .5 seconds of wall time passed */
  while (1) {
    /* getting the wall time */
  clock_gettime( CLOCK_MONOTONIC, &finish );
  /* calculating the time elapsed */  
  rtems_timespec_subtract( &start_wall, &finish, &time_passed_wall );  
  if(time_passed_wall.tv_nsec >= 500000000 || time_passed_wall.tv_sec >= 0.5 ){
     break;
    }
  }

  /* getting the end cpu clock time */
  r = clock_gettime( cid, &finish );
  /* Error checking for clock_gettime() */
  T_quiet_psx_success(r);

  /* calculating the cpu time elapsed */
  rtems_timespec_subtract( &start_cpu, &finish, &time_passed_cpu ); 

  /* comparing the wall and cpu times*/
  /* cpu time should be greater then the wall time */
  result = rtems_timespec_less_than(&time_passed_cpu, &time_passed_wall );
  /* checking the result */
  if( result )
  {
    printf( "CPU time elapsed is greater than the wall time\n" );
  }
  T_quiet_psx_success( result );
  
  /* creating a pthread to chech for ESRCH Exeption */

  r= pthread_create( &thread_id, NULL, test_pthread, NULL );
  /* Checking error in pthread_create() */
  T_quiet_psx_success(r);

  /* joining the pthread to make the thread_id invalid */
  r = pthread_join( thread_id, NULL );
  /* Checking for error in pthread_join() */
  T_quiet_psx_success(r);

  /* checking the ESRCH Exeption */
  r = pthread_getcpuclockid( thread_id, &cid );
  /* Checking for ESRCH exeption */
  T_quiet_psx_success (!(r == ESRCH));

}

void *POSIX_Init (void * argument)
{
  int exit_code;

  TEST_BEGIN();

  T_register();
  exit_code = T_main(&config);
  if (exit_code == 0) {
    TEST_END();
  }

 rtems_test_exit(exit_code);

}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS  2   
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
