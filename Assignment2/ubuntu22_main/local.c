/*------------------------------------------------------------------------*/
// Copyright (C) 2019-2020, Armin Biere, Johannes Kepler University Linz
// Copyright (C) 2020, .... [ put your name here ] ....
/*------------------------------------------------------------------------*/

/*task 7: based on progress.c, others have additional name part to separate the basis*/
#define program "local"
#define CACHE_LINE_SIZE 64

static const char *usage =
        "usage: " program " <workers> <operations>\n"
        "(operations logarithmically in base 10)\n";

/*------------------------------------------------------------------------*/

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

/*------------------------------------------------------------------------*/

static unsigned num_workers;
static unsigned f_work = 0;

static uint64_t total_operations;

/*------------------------------------------------------------------------*/

// Error and verbose messages.

static void
die (const char *fmt, ...)
{
    fprintf (stderr, "%s: error: ", program);
    va_list ap;
    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
    va_end (ap);
    fputc ('\n', stderr);
    exit (1);
}

static void
msg (const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    vprintf (fmt, ap);
    va_end (ap);
    fputc ('\n', stdout);
    fflush (stdout);
}

/*========================================================================*/

// This part is the real 'meat' of the exercise:

static volatile bool go;
static volatile uint64_t global_result;

//pthread_mutex_t go_mutex; //for the fix in 'run'

typedef struct worker worker;

struct worker
{
    uint64_t operations;
    uint64_t localsum;
    pthread_t thread;
    //char padding[12]; // to put 2 structs into cache line
    char padding[CACHE_LINE_SIZE - sizeof(pthread_t) - 2*sizeof(uint64_t)];
};

static worker *workers;

static void* monitor()
{
    while (f_work < num_workers) {
        usleep(10000);
        for (unsigned i = 0; i<num_workers; i++)
        {
          double percent = (double) workers[i].localsum / (double) workers[i].operations;
          printf("--- prog[%u]: %ld%% ---",i, (uint64_t)(percent*100));
        }      
        printf("\r");
        fflush(stdout);
    }
}

static void *
run (void *ptr)
{
    //FIX of "possible data race" form helgrind
    /*bool tempgo;
    
    pthread_mutex_lock(&go_mutex);
      tempgo=go;
    pthread_mutex_unlock(&go_mutex);
      
    worker *worker = ptr;
    
    while (!tempgo)
    {
      pthread_mutex_lock(&go_mutex);
      tempgo=go;
      pthread_mutex_unlock(&go_mutex);
    }*/
    
    worker *worker = ptr;
    while (!go)
        ;    
    const uint64_t operations = worker->operations; 
    for (uint64_t i = 0; i < operations; i++)
    {   
        worker->localsum++;
    }
    __sync_add_and_fetch(&global_result,worker->localsum);
    __sync_add_and_fetch(&f_work,1);
    return 0;
}

/*========================================================================*/

// Get current wall-clock and process time in seconds.

static double
wall_clock_time ()
{
    double res = 0;
    struct timeval tv;
    if (!gettimeofday (&tv, 0))
        res = 1e-6 * tv.tv_usec, res += tv.tv_sec;
    return res;
}

static double
process_time ()
{
    struct rusage u;
    if (getrusage (RUSAGE_SELF, &u))
        return 0;
    double res = u.ru_utime.tv_sec + 1e-6 * u.ru_utime.tv_usec;
    res += u.ru_stime.tv_sec + 1e-6 * u.ru_stime.tv_usec;
    return res;
}

/*------------------------------------------------------------------------*/

int
main (int argc, char **argv)
{
    //pthread_mutex_init(&go_mutex,NULL); //part of 'run' fix

    if (argc != 3)
    {
        fprintf (stderr, usage, program);
        exit (1);
    }
    int tmp = atoi (argv[1]);
    if (tmp <= 0)
        die ("non-positive number of workers");
    num_workers = tmp;
    tmp = atoi (argv[2]);
    if (tmp > 20)
        die ("operations exponent too large");
    total_operations = 1;
    while (tmp--)
        total_operations *= 10;

    msg ("testing %s version", program);
    msg ("initializing %u workers", num_workers);
    msg ("executing %" PRIu64 " operations in total", total_operations);
    uint64_t rest = total_operations % num_workers;
    uint64_t operations_per_worker = total_operations / num_workers;
    msg ("executing %" PRIu64 " operations per worker", operations_per_worker);

    workers = calloc (num_workers, sizeof *workers);

    for (unsigned i = 0; i < num_workers; i++)
    {
        worker *worker = workers + i;
        worker->operations = operations_per_worker;
        if (rest)
        {
            worker->operations++;
            rest--;
        }
    }

    pthread_t m_thread;

    if (pthread_create(&m_thread,0,monitor,NULL))
        die("Exception: no monitor thread!");

    for (unsigned i = 0; i < num_workers; i++)
        if (pthread_create (&workers[i].thread, 0, run, workers + i))
            die ("failed to create worker thread %u", i);

    double w = wall_clock_time ();
    double p = process_time ();

    //pthread_mutex_lock(&go_mutex); //related to 'run' fix
    go = true;
    //pthread_mutex_unlock(&go_mutex);

    for (unsigned i = 0; i < num_workers; i++)
        if (pthread_join (workers[i].thread, 0))
            die ("failed to join worker thread %u", i);

    if (pthread_join(m_thread,0))
        die("Exception: monitor thread failed to join!");

    free (workers);

    if (global_result == total_operations)
        msg ("SUCCESS: result is %" PRIu64 " as expected", total_operations);
    else
        msg ("ERROR: result is %" PRIu64 " but expected %" PRIu64,
             global_result, total_operations);

    w = wall_clock_time () - w;
    p = process_time () - p;

    msg ("used %.3f seconds wall-clock and %.3f process time", w, p);
    msg ("utilization %.0f%%, %.1f million operations per second",
         (w ? 100 * p / w : 0), w ? total_operations / 1e6 / w : 0);
         
    //pthread_mutex_destroy(&go_mutex); //part of 'run fix

    return 0;
}
