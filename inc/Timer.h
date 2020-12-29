/*
 * timer-custom.h
 *
 *  Created on: May 1, 2018
 *      Author: simulator
 */

#ifndef INCLUDES_TIMER_H_
#define INCLUDES_TIMER_H_

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define CLOCKID CLOCK_MONOTONIC
#define SIGNAL_ID SIGRTMIN

class TimerThread
{
 public:
 	void start_timer(timer_t *,int, float, void (*)());
 	void stop_timer(timer_t *);
	
	typedef struct timer_config_data_t {
	 timer_t timerid;
	 int  sec;
	 long  msec;

	}timer_config_data_t;

	timer_config_data_t timer_config[10];
	~TimerThread();
};


#endif /* INCLUDES_TIMER_H_ */
