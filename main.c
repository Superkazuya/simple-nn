/* Dirrrrrrrrrrrrrrrrrrty! */
#include <math.h>
#include <stdlib.h>

#define ETA 0.1
#define ALPHA 0.9
#define N_IN 2
#define N_OUT 2
#define L 3  //LAYER NUMBER
#define NMAXNEURON 5
#define ERR_THRESH 1
#define ITER_THRESH 1000

#define FILENAME "weight.h"

#define NEURO(X) (1/(1+exp(-X)))
#define BP_COEFF(X) (X*(1-X))
#define DENEURO(X) (BP_COEFF(NEURO(X)))

const unsigned int neuron_num[L] = {N_IN, 5, N_OUT};

//convention: weight[i][j][k] : i :current layer, j :j th neuron in next layer, k: k th neuron in current layer 
int weight[L-1][NMAXNEURON][NMAXNEURON + 1];
int output[L][NMAXNEURON];

int input [N_IN];
int target[N_OUT];

int error[L-1][NMAXNEURON] = {0};

static inline void adjust_weight();
static inline void calc_forward();
static void init_weight();
static void store_weight();  


inline void
calc_forward()
{
  unsigned int i, j, k;
  unsigned int sum, err = 0;
  for(i = 0; i < L-1; i++) //for all layers
    for(j = 0; j < neuron_num[i+1]; j++) // for every neuron in next layer
    {
      sum = weight[i][j][0];
      for(k = 0; k < neuron_num[i]; i++) // for every neuron in this layer
	sum+=weight[i][j][k+1]*output[i][k];
      output[i+1][j] = NEURO(sum);
    }
}

inline void
adjust_weight()
{
  unsigned int i, j, k;
  int o, err;
  i = L-2; //last weight
  for(j = 0; j < neuron_num[i+1]; j++) 
  {
    o = output[i+1][j];
    err = error[i][j] = BP_COEFF(o)*(target[j] - o); //negative error
    for(k = 0; k < neuron_num[i]; i++) //current layer
    {
      weight[i][j][k+1] *= ALPHA;
      weight[i][j][k+1] += ETA*err*output[i][j];
    }
    weight[i][j][0] *= ALPHA;
    weight[i][j][0] += ETA*err;
  }

  //Error back-propagate
  for(i = L-3; i >=0 ; i--) //Start from the 2nd last weight
  {
    for(k = 0; k < neuron_num[i]; i++) //to current layer
    {
      err = 0;
      for(j = 0; j < neuron_num[i+1]; j++) //Propagate from next layer
	err += weight[i+1][j][k+1]*error[i+1][j];
      o = output[i+1][k];
      err *= BP_COEFF(o);
      error[i][k] = err;
      for(j = 0; j < neuron_num[i+1]; j++)
      {
	weight[i][j][k+1] *= ALPHA;
	weight[i][j][k+1] += ETA*err*output[i][j];
      }
    }
    for(j = 0; j < neuron_num[i+1]; j++)
    {
      weight[i][j][0] *= ALPHA;
      weight[i][j][0] += ETA*err;
    }

  }
}

void
init_weight()
{
  unsigned int i, j, k;
  for(i = 0; i < L-1; i++)
    for(j = 0; j < neuron_num[i+1]; j++)
      for(k = 0; k < neuron_num[i]+1; k++)
	weight[i][j][k] = rand()/RAND_MAX*10000;
}

void
store_weight()
{
  unsigned int i, j, k;
  FILE* fp = fopen(FILENAME, "w+");
  fprintf(fp, "weight[%d][%d][%d] = \n", L-1, NMAXNEURON, NMAXNEURON);
  for(i = 0; i < L-1; i++)
  {
    fprintf(fp, "{ ");
    for(j = 0; j < NMAXNEURON; j++)
    {
      fprintf(fp, "{ ");
      for(k = 0; k < NMAXNEURON; k++)
	fprintf(fp, " %d,  ", weight[i][j][k]);
      fprintf(fp, " %d  }\n", weight[i][j][NMAXNEURON]);
    }
    fprintf(fp, " }\n ");
  }
  fclose(fp);
}

}
int
main()
{
  unsigned int i, j, k;
  for(i = 0; i < N_IN; i++)
    input[i] = 0; 
  for(i = 0; i < N_IN; i++)
    output
  calc_forward()
  while(
  return(EXIT_SUCCESS);
}
