#ifndef TIMER_H
#define TIMER_H

#include <time.h>

class timer {
private:
    clock_t start_time;
public:
    typedef double result_type;

    void start() {
        start_time = clock();
    }

    result_type stop() {
        return 1000000000. * ((clock() - start_time) / double(CLOCKS_PER_SEC));
    }
};

#endif
