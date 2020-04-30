typedef int (*functionPtr)(int, int);

void workerTask();
void RegisterSumFuncPtr(functionPtr func);
void RegisterMulFuncPtr(functionPtr func);