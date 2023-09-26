#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"

static KernelTcb_t  sTask_list[MAX_TASK_NUM];   // 1000 = Tcb * 64
static uint32_t     sAllocated_tcb_index;

void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;

    for(uint32_t i = 0; i < MAX_TASK_NUM; i++)
    {
        sTask_list[i].stack_base =  (uint8_t*)(TASK_STACK_START + (i*USR_TASK_STACK_SIZE)); // stack 1M lolol
        sTask_list[i].sp =  (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE  - 4;  // sp + padding
        
        sTask_list[i].sp -= sizeof(KernelTaskContext_t);      
        KernelTaskContext_t * ctx = (KernelTaskContext_t*)sTask_list[i].sp;
        ctx->pc =0;
        ctx->spsr = ARM_MODE_BIT_SYS;
    }
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc)
{
    KernelTcb_t* new_tcb = &sTask_list[sAllocated_tcb_index++]; // set next_tcb 

    if(sAllocated_tcb_index>MAX_TASK_NUM)
    {
        return NOT_ENOUGH_TASK_NUM;
    }

    KernelTaskContext_t* ctx = (KernelTaskContext_t*)new_tcb->sp; // get StackPointer of new tcb
    ctx->pc=(uint32_t)startFunc; // new_tcb's context.pc = startFunc

    return (sAllocated_tcb_index - 1);
}