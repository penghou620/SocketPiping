#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <sys/time.h>

#define CLOCKS_PER_SEC 8000000000. // 800 Mhz machine

__inline__ uint64_t rdtsc() {
   uint32_t lo, hi;
   __asm__ __volatile__ (
   "xorl %%eax, %%eax \n   cpuid"
   ::: "%rax", "%rbx", "%rcx", "%rdx");
   __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
   return (uint64_t)hi << 32 | lo;
}

uint64_t tsc;

void start_timer() {
	tsc = rdtsc();
}

double end_timer() {
	uint64_t d = rdtsc() - tsc;
	return ((double)d) / CLOCKS_PER_SEC / 2;
}

struct timeval s, e;

void start_timeofday() {
	gettimeofday(&s, NULL);
}

double end_timeofday() {
	gettimeofday(&e, NULL);
	double time = (((double)e.tv_sec) + ((double)e.tv_usec) * 0.0000001) - 
				  (((double)s.tv_sec) + ((double)s.tv_usec) * 0.0000001);
	return time;
}

#endif