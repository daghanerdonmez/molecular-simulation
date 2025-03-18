//
//  oldconfig.h
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#ifndef oldconfig_h
#define oldconfig_h

#define PARTICLE_COUNT 1000

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


#endif /* oldconfig_h */
