#ifndef SYNCH_H_
#define SYNCH_H_
void Kernel_sem_init(int32_t max);

bool Kernel_sem_test(void);

void Kernel_sem_release(void);
#endif