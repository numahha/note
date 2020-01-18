#include <stdio.h>
#include "physical_parameter.h"

#define R_NUM 3
#define SIMULATION_LENGTH 30.0 // [s]


void eulerEquation( double omega_input[],
		    double domega_output[] )
{
  // domega/dt = - omega X I * omega
  domega_output[0] = (B_INTM_22-B_INTM_33) * omega_input[1] * omega_input[2] / B_INTM_11;
  domega_output[1] = (B_INTM_33-B_INTM_11) * omega_input[2] * omega_input[0] / B_INTM_22;
  domega_output[2] = (B_INTM_11-B_INTM_22) * omega_input[0] * omega_input[1] / B_INTM_33;
}


void rotationEquation( double rotation_input[],
		       double omega_input[],
		       double drotation_output[] )
{
  // dR/dt = R * [omega X] 
  int i, j, k;
  double x;
  double omega_matrix[R_NUM*R_NUM];

  omega_matrix[0] =              0.;   omega_matrix[1] = -omega_input[2];   omega_matrix[2] =  omega_input[1];
  omega_matrix[3] =  omega_input[2];   omega_matrix[4] =              0.;   omega_matrix[5] = -omega_input[0];
  omega_matrix[6] = -omega_input[1];   omega_matrix[7] =  omega_input[0];   omega_matrix[8] =              0.;

  for(i=0;i<R_NUM;i++){
    for(j=0;j<R_NUM;j++){
      x=0.;
      for(k=0;k<R_NUM;k++) x += rotation_input[R_NUM*i+k] * omega_matrix[R_NUM*k+j];
      drotation_output[i*R_NUM+j] = x;
    }
  } 
}


void rungeStep( double omega_input[],
		double rotation_input[],
		double nextOmega_output[],
		double nextRotation_output[] )
{
  int i;
  double temp_omega[R_NUM]         , omega_k1[R_NUM]         , omega_k2[R_NUM]         , omega_k3[R_NUM]         , omega_k4[R_NUM];
  double temp_rotation[R_NUM*R_NUM], rotation_k1[R_NUM*R_NUM], rotation_k2[R_NUM*R_NUM], rotation_k3[R_NUM*R_NUM], rotation_k4[R_NUM*R_NUM];

  //k1
  eulerEquation   ( omega_input                , omega_k1    );
  rotationEquation( rotation_input, omega_input, rotation_k1 );

  //k2
  for(i=0;i<R_NUM;i++)       temp_omega   [i]  =  omega_input   [i] + omega_k1   [i]*STEPSIZE/2.;
  for(i=0;i<R_NUM*R_NUM;i++) temp_rotation[i]  =  rotation_input[i] + rotation_k1[i]*STEPSIZE/2.;
  eulerEquation   ( temp_omega               , omega_k2 );
  rotationEquation( temp_rotation, temp_omega, rotation_k2 );

  //k3
  for(i=0;i<R_NUM;i++)       temp_omega   [i]  =  omega_input   [i] + omega_k2   [i]*STEPSIZE/2.;
  for(i=0;i<R_NUM*R_NUM;i++) temp_rotation[i]  =  rotation_input[i] + rotation_k2[i]*STEPSIZE/2.;
  eulerEquation   ( temp_omega               , omega_k3    );
  rotationEquation( temp_rotation, temp_omega, rotation_k3 );

  //k4
  for(i=0;i<R_NUM;i++)       temp_omega   [i]  =  omega_input   [i] + omega_k3   [i]*STEPSIZE;
  for(i=0;i<R_NUM*R_NUM;i++) temp_rotation[i]  =  rotation_input[i] + rotation_k3[i]*STEPSIZE;
  eulerEquation   ( temp_omega               , omega_k4    );
  rotationEquation( temp_rotation, temp_omega, rotation_k4 );

  // next step
  for(i=0;i<R_NUM;i++)       nextOmega_output   [i]  =  omega_input   [i]  +  ( omega_k1   [i] + omega_k2   [i]*2. + omega_k3   [i]*2. + omega_k4   [i] ) * (STEPSIZE/6.);
  for(i=0;i<R_NUM*R_NUM;i++) nextRotation_output[i]  =  rotation_input[i]  +  ( rotation_k1[i] + rotation_k2[i]*2. + rotation_k3[i]*2. + rotation_k4[i] ) * (STEPSIZE/6.);
}


double rotationEnergy( double omega_input[] )
{
  // T = omega * I * omega / 2
  double ret_val;
  ret_val  = B_INTM_11 * omega_input[0] * omega_input[0] / 2.;
  ret_val += B_INTM_22 * omega_input[1] * omega_input[1] / 2.;
  ret_val += B_INTM_33 * omega_input[2] * omega_input[2] / 2.;
  return ret_val;
}


void angularMomentumInWorldCoordinate( double rotation_input[],
				       double omega_input[],
				       double ell_output[])
{
  // L = R * I * omega
  int i,j;
  double x;
  double ell_in_body_coordinate[R_NUM];

  ell_in_body_coordinate[0] = B_INTM_11 * omega_input[0];
  ell_in_body_coordinate[1] = B_INTM_22 * omega_input[1];
  ell_in_body_coordinate[2] = B_INTM_33 * omega_input[2];

  for(i=0;i<R_NUM;i++){
    x=0.;
    for(j=0;j<R_NUM;j++) x += rotation_input[R_NUM*i+j] * ell_in_body_coordinate[j];
    ell_output[i] = x;
  }
}


void fileOutputFunc( FILE *fp_input,
		     double t_input,
		     double omega_input[],
		     double rotation_input[],
		     double ell_input[],
		     double energy_input)
{
  static int init_flag=1;

  if(init_flag){
    fprintf(fp_input,"time,");                            //f1
    fprintf(fp_input,"omega1,omega2,omega3,");            //f2
    fprintf(fp_input,"rotation11,rotation12,rotation13,");//f3
    fprintf(fp_input,"rotation21,rotation22,rotation23,");
    fprintf(fp_input,"rotation31,rotation32,rotation33,");
    fprintf(fp_input,"ell1,ell2,ell3,");                  //f4
    fprintf(fp_input,"energy,\n");                        //f5
    init_flag=0;
  }

  fprintf(fp_input,"%f,"      ,t_input);                                              //f1
  fprintf(fp_input,"%f,%f,%f,",omega_input[0]   ,omega_input[1]   ,omega_input[2]);   //f2
  fprintf(fp_input,"%f,%f,%f,",rotation_input[0],rotation_input[1],rotation_input[2]);//f3
  fprintf(fp_input,"%f,%f,%f,",rotation_input[3],rotation_input[4],rotation_input[5]);
  fprintf(fp_input,"%f,%f,%f,",rotation_input[6],rotation_input[7],rotation_input[8]);
  fprintf(fp_input,"%f,%f,%f,",ell_input[0]     ,ell_input[1]     ,ell_input[2]);     //f4
  fprintf(fp_input,"%f,\n"    ,energy_input);                                         //f5
}


int main()
{
  int i;
  double total_t = 0.;
  double omega[R_NUM],          nextOmega[R_NUM];
  double rotation[R_NUM*R_NUM], nextRotation[R_NUM*R_NUM];
  double ell[R_NUM];
  double energy;

  char filename[50] = "runge_output.csv";
  FILE *fp;


  // --1-- file open
  fp = fopen(filename,"w");
  if(fp==NULL){
    printf("file open error!\n");
    return 1;
  }


  // --2-- set initila condition
  omega[0]  = B_ANGVEL_X;
  omega[1]  = B_ANGVEL_Y;
  omega[2]  = B_ANGVEL_Z;
  rotation[0] = 1.0; rotation[1] = 0.0; rotation[2] = 0.0;
  rotation[3] = 0.0; rotation[4] = 1.0; rotation[5] = 0.0;
  rotation[6] = 0.0; rotation[7] = 0.0; rotation[8] = 1.0;


  // --3-- simulation run
  while(1){
    if(total_t>=SIMULATION_LENGTH) break;

    energy = rotationEnergy( omega );
    angularMomentumInWorldCoordinate( rotation, omega, ell);

    fileOutputFunc(fp, total_t, omega, rotation, ell, energy);

    rungeStep( omega    , rotation,
	       nextOmega, nextRotation);

    for(i=0;i<R_NUM;i++)       omega   [i] = nextOmega   [i];
    for(i=0;i<R_NUM*R_NUM;i++) rotation[i] = nextRotation[i];
    
    total_t += STEPSIZE;    
  }


  // --4-- file close
  fclose(fp);

  return 0;
}
