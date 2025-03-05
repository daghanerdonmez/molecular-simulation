//
//  config.h
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#ifndef config_h
#define config_h

#define OUTPUT_RESULTS false
#define GRAPHICS_ON true

// 0 for single simulation
// 1 for simulation network
// If simulation network is chosen, applied boundary is automatically cylinder boundary,
#define MODE 1

#define TIME_TO_RUN 5000
#define DT 1e-2
#define NUMBER_OF_ITERATIONS TIME_TO_RUN/DT
#define ITERATIONS_PER_FRAME 10

#define PARTICLE_COUNT 1000
#define D 79.4e-12 // diffusion coefficient

#define GRAPHICS_ZOOM_MULTIPLIER 1e3

// ---------------------------- SINGLE SIMULATION CONFIGURATION BEGIN-------------------------------

#define SINGLE_RECEIVER_COUNT 1 // I only handle the 0 or 1 case right now
#define SINGLE_RECEIVER_RADIUS 1e-4
#define SINGLE_RECEIVER_X 3e-4
#define SINGLE_RECEIVER_Y 0
#define SINGLE_RECEIVER_Z 0

// 0 for no boundary
// 1 for box boundary
// 2 for cylinder boundary
#define SINGLE_APPLIED_BOUNDARY 2

#define SINGLE_BOX_BOUNDARY_Z 2.5e-4
#define SINGLE_BOX_BOUNDARY_Y 1e-4
#define SINGLE_BOX_BOUNDARY_X 2.5e-4

#define SINGLE_CYLINDER_ORIENTATION 0 // Central axis of cylinder along: 0=X 1=Y 2=Z
#define SINGLE_CYLINDER_R 3e-4
#define SINGLE_CYLINDER_Z 0 // 0 for infinite height cylinder

// FLOW
#define SINGLE_FLOW_X 0
#define SINGLE_FLOW_Y 0
#define SINGLE_FLOW_Z 0


// ---------------------------- SINGLE SIMULATION CONFIGURATION END-------------------------------

// -----------------------------------------------------------------------------------------------

// ---------------------------- SIMULATION NETWORK CONFIGURATION BEGIN----------------------------



// ---------------------------- SIMULATION NETWORK CONFIGURATION END------------------------------


#define DEBUG_CHECKPOINT_PRINTS false

#endif /* config_h */
