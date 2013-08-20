/**
 * @file main.h
 *
 * @author take
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <windows.h>
#include <Winbase.h>
#include <time.h>
#include "photon.h"

struct ThreadParams{
    PPMRenderer* renderer;
    int w,h;
    clock_t start;
};

void observe(LPVOID params);

#endif /* MAIN_H_ */
