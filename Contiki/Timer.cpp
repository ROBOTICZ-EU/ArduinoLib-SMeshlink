/*
 * Copyright (c) 2011-2013, SmeshLink Technology Ltd.
 * All rights reserved.
 *
 * Timer.cpp
 *
 *      Author: smeshlink
 */

#include "Timer.h"

#ifdef __cplusplus
extern "C" {
#endif
void initialize();
#ifdef __cplusplus
} // extern "C"
#endif

Timer::Timer(clock_time_t time, bool autoReset, void (*tick)(void *ctx), void *context)
	: time(time), autoReset(autoReset), context(context), tick(tick) {
}

Timer::~Timer() {
	ctimer_stop(&_ctimer);
}

void Timer::start() {
	initialize();
	ctimer_set(&_ctimer, time, ctimer_callback, this);
}

void Timer::stop() {
	ctimer_stop(&_ctimer);
}

void Timer::ctimer_callback(void *ctx) {
	Timer *timer = (Timer *)ctx;
	timer->tick(timer->context);
	if (timer->autoReset)
		ctimer_reset(&timer->_ctimer);
}
void Timer::setTime(clock_time_t newtime)
{
	time=newtime;
	ctimer_set(&_ctimer, time, ctimer_callback, this);
}
