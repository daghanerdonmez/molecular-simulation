//
//  config.h

#ifndef config_h
#define config_h

#define OUTPUT_RESULTS true
#define GRAPHICS_ON true

// 0 for single simulation
// 1 for simulation network
// If simulation network is chosen, applied boundary is automatically cylinder boundary
#define MODE 1
#define BULKMODE false

#define TIME_TO_RUN 5000
#define DT 0.01
#define NUMBER_OF_ITERATIONS (int)(TIME_TO_RUN/DT)
#define ITERATIONS_PER_FRAME 100

#define D 7.94e-11 // diffusion coefficient

#define GRAPHICS_ZOOM_MULTIPLIER 1e+02

#define FLOW_VALUE 0.001

#endif /* config_h */
