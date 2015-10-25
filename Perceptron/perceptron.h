#define PRC_TRAIN 					1
#define PRC_EXPORT_WEIGHTS 			2
#define PRC_EXPORT_WEIGHTS_PROGRESS 4
#define PRC_COMPUTE 				8
#define PRC_GEN_IW 					16
#define PRC_EXPORT_ERROR 			32

#define NUM_DATA 					2

void Train(char*, float*, int*, char*, char*, char*, int, const float);
void Compute(char*, float*, int);
int Neuron(const float*, const float*, const int);