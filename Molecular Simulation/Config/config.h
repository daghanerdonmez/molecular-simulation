//
//  config.h
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#ifndef config_h
#define config_h

#define NUMBER_OF_ITERATIONS 10000
#define ITERATIONS_PER_FRAME 5

#define PARTICLE_COUNT 1000
#define BROWNIAN_STD_DEV 0.003

#define RECEIVER_COUNT 1 // I only handle the 1 case right now
#define RECEIVER_RADIUS 0.2
#define RECEIVER_X 0.5
#define RECEIVER_Y 0
#define RECEIVER_Z 0

// 0 for no boundary
// 1 for box boundary
// 2 for cylinder boundary
#define APPLIED_BOUNDARY 0

#define BOX_BOUNDARY_Z 0.5
#define BOX_BOUNDARY_Y 0.5
#define BOX_BOUNDARY_X 0.5

#define CYLINDER_ORIENTATION 0 // Central axis of cylinder along: 0=X 1=Y 2=Z
#define CYLINDER_R 0.3
#define CYLINDER_Z 0 // 0 for infinite height cylinder

#endif /* config_h */
