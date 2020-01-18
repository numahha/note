#include <stdio.h>
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef _MSC_VER
#pragma warning(disable:4244 4305)  /* for VC++, no precision loss complaints */
#endif

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "physical_parameter.h"

#define OUTPUT_LENGTH 50.0 //[s]

dWorldID world;
dSpaceID space;
dBodyID body;
dGeomID Bform, ground;
dJointGroupID contactgroup;


/* set viewpoint */
static void start()
{
  static float xyz[3] = {0.0f,0.0f,4.0f};
  static float hpr[3] = {0.000f,-90.0000f,0.0000f};
  dsSetViewpoint (xyz,hpr);
}


/* put body in simlulation */
static void createBody()
{
  dMass m;
  dQuaternion q;
  
  //body definition
  body = dBodyCreate(world);
  dMassSetParameters(&m,
		     B_MASS,
		     B_COM_X, B_COM_Y, B_COM_Z,
		     B_INTM_11, B_INTM_22, B_INTM_33, B_INTM_12, B_INTM_13, B_INTM_23);
  dBodySetMass(body, &m);
  Bform = dCreateBox(space,
		     B_LENGTH, B_WIDTH, B_HEIGHT);
  dGeomSetBody(Bform, body);


  //initial condition
  dBodySetPosition  (body,
		     B_POS_X, B_POS_Y, B_POS_Z);
  dQSetIdentity(q);//hishi 後で3-1-3オイラーとかに変える．
  dBodySetQuaternion(body, q);
  dBodySetLinearVel (body,
		     B_LINVEL_X, B_LINVEL_Y, B_LINVEL_Z);
  dBodySetAngularVel(body,
		     B_ANGVEL_X, B_ANGVEL_Y, B_ANGVEL_Z);
}


/* drawing function */
void drawBody()
{
  double size[3];
  
  dsSetColor(1,1,0);
  dsSetTexture(DS_WOOD);
  dGeomBoxGetLengths(Bform, size);
  dsDrawBox(dBodyGetPosition(body), dBodyGetRotation(body), size);
}


/* get body information */
void getBodyInformation(double p_output[],
			double v_output[],
			double rotation_output[],
			double omega_output[])
{
  const dReal *pp = dBodyGetPosition(body);
  const dReal *vp = dBodyGetLinearVel(body);
  const dReal *rp = dBodyGetRotation(body);
  const dReal *op = dBodyGetAngularVel(body);

  p_output[0] = (double)pp[0];
  p_output[1] = (double)pp[1];
  p_output[2] = (double)pp[2];

  v_output[0] = (double)vp[0];
  v_output[1] = (double)vp[1];
  v_output[2] = (double)vp[2];

  rotation_output[0]  = (double)rp[0]; //r11
  rotation_output[1]  = (double)rp[1]; //r12
  rotation_output[2]  = (double)rp[2]; //r13
  rotation_output[3]  = (double)rp[3]; //0...blank
  rotation_output[4]  = (double)rp[4]; //r21
  rotation_output[5]  = (double)rp[5]; //r22
  rotation_output[6]  = (double)rp[6]; //r23
  rotation_output[7]  = (double)rp[7]; //0...blank
  rotation_output[8]  = (double)rp[8]; //r31
  rotation_output[9]  = (double)rp[9]; //r32
  rotation_output[10] = (double)rp[10];//r33
  rotation_output[11] = (double)rp[11];//0...blank

  omega_output[0] = (double)op[0];
  omega_output[1] = (double)op[1];
  omega_output[2] = (double)op[2];
}


/* for output data */
void fileOutputFunc( double time_input,
		     double p_input[],
		     double v_input[],
		     double rotation_input[],
		     double omega_input[])
{
  static int init_file_flag = 1;

  char filename[50] = "ode_output.csv";
  FILE *fp;

  //write output data file
  if(!init_file_flag){
    fp = fopen( filename, "a" );
    fprintf(fp,"%f,"      ,time_input);                                                //f1
    fprintf(fp,"%f,%f,%f,",p_input[0]       , p_input[1]       , p_input[2]);       //f2
    fprintf(fp,"%f,%f,%f,",v_input[0]       , v_input[1]       , v_input[2]);       //f3
    fprintf(fp,"%f,%f,%f,",rotation_input[0], rotation_input[1], rotation_input[2]);//f4
    fprintf(fp,"%f,%f,%f,",rotation_input[4], rotation_input[5], rotation_input[6]);
    fprintf(fp,"%f,%f,%f,",rotation_input[8], rotation_input[9], rotation_input[10]);
    fprintf(fp,"%f,%f,%f,",omega_input[0]   , omega_input[1]   , omega_input[2]);   //f5
    fprintf(fp,"\n");
    fclose(fp);
  }


  //open output data file
  if(init_file_flag){
    fp = fopen( filename, "w" );
    if(fp==NULL){
      printf("output file open error!\n");
      exit(1);
    }

    fprintf(fp,"time,");                //f1
    fprintf(fp,"p1,p2,p3,");            //f2
    fprintf(fp,"v1,v2,v3,");            //f3
    fprintf(fp,"r11,r12,r13,");         //f4
    fprintf(fp,"r11,r12,r13,");
    fprintf(fp,"r11,r12,r13,");
    fprintf(fp,"omega1,omega2,omega3,");//f5
    fprintf(fp,"\n");

    fclose(fp);
    init_file_flag  = 0;
  }
}


/* simulation loop */
static void simLoop (int pause)
{
  static double total_t=0.;
  double body_p[3], body_v[3], body_rotation[12], body_omega[3];

  // for output data
  if( OUTPUT_LENGTH > total_t ){
    getBodyInformation(body_p, body_v, body_rotation, body_omega);
    fileOutputFunc(total_t, body_p, body_v, body_rotation, body_omega);
  }

  //one step time evolution
  dWorldStep(world,STEPSIZE);

  //snap shot
  drawBody();

  printf("total_t = %f\n",total_t);
  total_t += STEPSIZE;
}


/* main */
int main (int argc, char **argv)
{
  /*************** Setup Pointers to drawstuff Callback Functions ***************/
  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = 0;
  fn.stop = 0;
  fn.path_to_textures = "./drawstuff/textures";

  /*************** Create World ***************/
  dInitODE2(0);
  world = dWorldCreate();
  space = dHashSpaceCreate(0);
  contactgroup = dJointGroupCreate(0);
  dWorldSetGravity(world,0,0,GRAVITY);
  //dWorldSetERP(world, WORLD_ERP);//デフォルト値を使う
  //dWorldSetCFM(world, WORLD_CFM);//デフォルト値を使う

  /*************** Create Body ***************/
  createBody();

  /*************** Run Simulation ***************/
  dsSimulationLoop (argc,argv,352,288,&fn);

  /*************** Close Simulation ***************/
  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  dCloseODE();
  return 0;
}
