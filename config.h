#define N_LAY 3
#define N_MAX 20
#define N_IN  2
#define N_OUT 2

#define NEURO(X) (1/(1+exp(-(X))))
#define FILENAME "data.xml"

const unsigned int neuron_num[N_LAY] = {N_IN, 20, N_OUT};

