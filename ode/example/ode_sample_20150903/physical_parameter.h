#ifndef _INC_PHYIS_PARAM
#define _INC_PHYIS_PARAM


//simulator parameter 
//#define WORLD_ERP 0.0
//#define WORLD_ERP 0.0

//world definition 
#define GRAVITY  0.0 // [kg/(s^2)]
#define STEPSIZE 0.1 // [s]

//geometry defition
#define B_LENGTH 5.0 // [m] length of box-shaped body
#define B_WIDTH  3.0 // [m] width 
#define B_HEIGHT 1.0 // [m] height 

//physical parameter definition
#define B_MASS     1.0 // [kg] total mass of body
#define B_COM_X    0.0 // [m]  1st component of position vector of center of mass in BODY coordinate
#define B_COM_Y    0.0 // [m]  2nd component 
#define B_COM_Z    0.0 // [m]  3rd component 
#define B_INTM_11  (B_HEIGHT*B_HEIGHT + B_WIDTH *B_WIDTH ) // [kg*(m^2)] (1,1)-th component of moment of inertia in BODY coordinate
#define B_INTM_22  (B_LENGTH*B_LENGTH + B_HEIGHT*B_HEIGHT) // [kg*(m^2)] (2,2)-th component
#define B_INTM_33  (B_WIDTH *B_WIDTH  + B_LENGTH*B_LENGTH) // [kg*(m^2)] (3,3)-th component
#define B_INTM_12  0.0 // [kg*(m^2)] (1,2)-th component
#define B_INTM_13  0.0 // [kg*(m^2)] (1,3)-th component
#define B_INTM_23  0.0 // [kg*(m^2)] (2,3)-th component

//initial condition of body
#define B_POS_X       0.0 // [m]  1st component of position vector of center of mass in WORLD coordinate
#define B_POS_Y       0.0 // [m]  2nd component 
#define B_POS_Z       0.0 // [m]  3rd component 
#define B_LINVEL_X    0.0 // [m/s]  1st component of linear velocity vector of center of mass in WORLD coordinate
#define B_LINVEL_Y    0.0 // [m/s]  2nd component 
#define B_LINVEL_Z    0.0 // [m/s]  3rd component 
#define B_ANGVEL_X    1.0 // [rad/s]  1st component of angular velocity vector of center of mass in WORLD coordinate
#define B_ANGVEL_Y    0.0 // [rad/s]  2nd component 
#define B_ANGVEL_Z    1.0 // [rad/s]  3rd component 


#endif
