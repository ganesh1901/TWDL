/*
 * timer.c
 *
 *  Created on: May 1, 2018
 *      Author: simulator
 *     when it is integrated to the Excalibur API's  blocking and unblocking not working
 *     only start and stop timer is working
 */

#include <Timer.h>


void TimerThread::start_timer(timer_t *timerid, int timeout, float timeout_usec, void (*callback)())

{

	struct sigevent sev0;

	memset(&sev0,0x00,sizeof sev0);
	sev0.sigev_notify = SIGEV_THREAD;
	sev0.sigev_notify_function = callback;
	sev0.sigev_signo = SIGNAL_ID;
	sev0.sigev_value.sival_ptr = timerid;
	sev0.sigev_notify_attributes = NULL;

	struct itimerspec its0;
	memset(&its0,0x00,sizeof its0);
	/* Timeout for timer*/


	its0.it_value.tv_sec = timeout;
	its0.it_value.tv_nsec =timeout_usec*1000000;

	its0.it_interval.tv_sec = timeout;
	its0.it_interval.tv_nsec = timeout_usec*1000000;


	if( (*timerid) == NULL  )
	{
	    if (timer_create(CLOCKID, &sev0, timerid) == -1){
		    perror("timer_create");
	    }
	    printf("timer ID  is 0x%ld \n", (long ) *timerid);
	    if (timer_settime(*timerid, 0, &its0, NULL) == -1)
		    perror("timer_settime1");
	}
	else{
	    printf(" Timer already Running \n" );
	}

}

void TimerThread::stop_timer(timer_t *timerid)
{
	 struct itimerspec its0;
	if(*timerid == NULL )
	{
		printf(" Already --- stoped \n");
		return;
	}
	else
	{
		printf("Stopping Timer == 0x%ld \n", (long)*timerid);
		memset((void*)&its0,0,sizeof(its0));
		if(timer_settime(*timerid,0,&its0,NULL) == -1)
			printf("Unable to Stop Timer ...! \n");
		if(timer_delete(*timerid))
			printf("unable to delete timer ...! \n");
		*timerid = NULL;
	}
}

TimerThread::~TimerThread(){
	printf(" At timer class destructor \n");
}
