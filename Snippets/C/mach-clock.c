#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif



struct timespec ts;

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
clock_serv_t cclock;
mach_timespec_t mts;
host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
clock_get_time(cclock, &mts);
mach_port_deallocate(mach_task_self(), cclock);
ts.tv_sec = mts.tv_sec;
ts.tv_nsec = mts.tv_nsec;

#else
clock_gettime(CLOCK_REALTIME, &ts);
#endif

uint64_t GetPIDTimeInNanoseconds(void)
{
    uint64_t        start;
    uint64_t        end;
    uint64_t        elapsed;
    uint64_t        elapsedNano;
    static mach_timebase_info_data_t    sTimebaseInfo;

    // Start the clock.

    start = mach_absolute_time();

    // Call getpid. This will produce inaccurate results because 
    // we're only making a single system call. For more accurate 
    // results you should call getpid multiple times and average 
    // the results.

    (void) getpid();

    // Stop the clock.

    end = mach_absolute_time();

    // Calculate the duration.

    elapsed = end - start;

    // Convert to nanoseconds.

    // If this is the first time we've run, get the timebase.
    // We can use denom == 0 to indicate that sTimebaseInfo is 
    // uninitialised because it makes no sense to have a zero 
    // denominator is a fraction.

    if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }

    // Do the maths. We hope that the multiplication doesn't 
    // overflow; the price you pay for working in fixed point.

    elapsedNano = elapsed * sTimebaseInfo.numer / sTimebaseInfo.denom;

    printf("multiplier %u / %u\n", sTimebaseInfo.numer, sTimebaseInfo.denom);
    return elapsedNano;
}



#ifdef __MACH__
#include <sys/time.h>
//clock_gettime is not implemented on OSX
int clock_gettime(int /*clk_id*/, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif