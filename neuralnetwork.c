//convention: weight[i][n][k] : i :current layer, n :n th neuron in <n>ext layer, k: k th neuron in <c>urrent layer 
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define NEURO(X) (1/(1+exp(-(X))))
#define BP_COEFF(X) (X*(1-X))
#define DENEURO(X) (BP_COEFF(NEURO(X)))
#define RAND(X)   ((X)*((double)rand()/RAND_MAX))

#define N_LAY 3
#define N_MAX 20
#define N_IN  2
#define N_OUT 2

#define ETA 0.1
#define ERR_THRESH 0.0001
#define ITER_THRESH 100000

#define FILENAME "data.xml"

const unsigned int neuron_num[N_LAY] = {N_IN, 20, N_OUT};

double weight[N_LAY-1][N_MAX][N_MAX];

double output[N_LAY][N_MAX];
double thresh[N_LAY][N_MAX];

double input [N_IN];
double target[N_OUT];

double error[N_LAY][N_MAX];


//Declaration
/*
static unsigned int calc_forward_err(double  weight[N_LAY-1][N_MAX][N_MAX], double error[N_LAY][N_MAX], double thresh[N_LAY][N_MAX], double *input, double output[N_LAY][N_MAX], double* target);
static void         init_weight_n_thresh(double weight[N_LAY-1][N_MAX][N_MAX], double thresh[N_LAY][N_MAX])
static void	    store_weight_n_thresh(double weight[N_LAY-1][N_MAX][N_MAX], double thresh[N_LAY][N_MAX])
static void         updata_err(double  weight[N_LAY-1][N_MAX][N_MAX], double error[N_LAY][N_MAX], double output[N_LAY][N_MAX]);
static void         updata_weight_n_thresh(double  weight[N_LAY-1][N_MAX][N_MAX], double error[N_LAY][N_MAX], double thresh[N_LAY][N_MAX], double output[N_LAY][N_MAX]);
*/
static double	    calc_forward_err();
static void         init_weight_n_thresh();
static void	    store_weight_n_thresh();
static void         updata_err();
static void         updata_weight_n_thresh();




double
calc_forward_err()
{
  unsigned int i, n, k; 
  double sum, err, o;
  for(k = 0; k < N_IN; k++) //first layer
    output[0][k] = NEURO(input[k] - thresh[0][k]);
  for(i = 0; i < N_LAY-1; i++) //following layer
    for(n = 0; n < neuron_num[i+1]; n++)
    {
      sum = thresh[i+1][n];
      for(k = 0; k < neuron_num[i]; k++)
	sum += weight[i][n][k]*output[i][k];
      output[i+1][n] = NEURO(sum);
    }
  err = 0;
  for(k = 0; k < N_OUT; k++) //out layer, calc error
  {
    o = output[N_LAY-1][k];  //o : output of kth @ last layer
    sum = target[k] - o;    //sum -> e
    err += sum*sum;
    error[N_LAY-1][k] = sum*BP_COEFF(o);
  }
  err *= 0.5;
  return(err);
}

void
init_weight_n_thresh()
{
  unsigned int i, n, k;
  for(i = 0; i < N_LAY-1; i++)
    for(n = 0; n < neuron_num[i+1]; n++)
      for(k = 0; k < neuron_num[i]; k++)
	weight[i][n][k] = RAND(1.0);
  for(i = 0; i < N_LAY; i++)
    for(k = 0; k < neuron_num[i]; k++)
      thresh[i][k] = RAND(1.0);
}

void
store_weight_n_thresh()
{
  unsigned int i, n, k;
  FILE* fp = fopen(FILENAME, "w+");
  fprintf(fp, "weight[%d][%d][%d] = \n", N_LAY-1, N_MAX, N_MAX);
  fprintf(fp, "{ ");
  for(i = 0; i < N_LAY-1; i++)
  {
    fprintf(fp, "{ ");
    for(n = 0; n < N_MAX; n++)
    {
      fprintf(fp, "{ ");
      for(k = 0; k < N_MAX; k++)
	fprintf(fp, " %f,  ", weight[i][n][k]);
      fprintf(fp, " %f  },\n", weight[i][n][N_MAX]);
    }
    fprintf(fp, " },\n ");
  }
  fprintf(fp, " };\n ");

    //threshold
  fprintf(fp, "thresh[%d][%d] = \n", N_LAY, N_MAX);
  fprintf(fp, "{ ");
  for(i = 0; i < N_LAY; i++)
  {
    fprintf(fp, "{ ");
      for(k = 0; k < N_MAX-1; k++)
	fprintf(fp, " %f,  ", thresh[i][k]);
      fprintf(fp, " %f  },\n", thresh[i][N_MAX-1]);
  } 
  fprintf(fp, "};\n");
  fclose(fp);
}


void
updata_err()
{
  int i, n, k;
  double sum, o;
  for(i = N_LAY - 2; i >= 0 ; i--)
    for(k = 0; k < neuron_num[i]; k++)
    {
      sum = 0;
      for(n = 0; n < neuron_num[i+1]; n++)
	sum += weight[i][n][k]*error[i+1][n];
      o = output[i][k];
      error[i][k] = sum*BP_COEFF(o);
    }
}

void
updata_weight_n_thresh()
{
  unsigned int i, n, k;
  for(i = 0; i < N_LAY-1; i++)
    for(k = 0; k < neuron_num[i]; k++)
      for(n = 0; n < neuron_num[i+1]; n++)
	weight[i][n][k] += ETA*output[i][k]*error[i+1][n];
  for(i = 0; i < N_LAY; i++)
    for(k = 0; k < neuron_num[i]; k++)
      thresh[i][k] = ETA*error[i][k];
}


int
main()
{
  unsigned int i, j, k;
  unsigned int counter = 0;
  double err = ERR_THRESH+1;

  init_weight_n_thresh();

  while( err > ERR_THRESH && counter++ < ITER_THRESH)
  {
    err = 0;
    for(k = 0; k < 100; k++)
    {
      input[0] = k%2;
      input[1] = !input[0];
      target[0] = !input[0];
      target[1] = !input[1];
      err+= calc_forward_err();
      updata_err();
      updata_weight_n_thresh();
    }
  printf("%d interations, err = %f\n", counter, (err));
  }
  //store_weight_n_thresh();
  scanf("%lf %lf", input, input+1);
  printf("%lf %lf\n", input[0], input[1]);
  calc_forward_err();
  for(k=0; k < N_OUT; k++)
    printf("%lf\n", output[N_LAY-1][k]);

  return(EXIT_SUCCESS);

}
