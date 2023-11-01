#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"

static KernelTcb_t  sTask_list[MAX_TASK_NUM];   // 1000 = Tcb * 64
static KernelTcb_t* sCurrent_tcb;
static KernelTcb_t* sNext_tcb;
static uint32_t sAllocated_tcb_index;
static uint32_t sCurrent_tcb_index;

static KernelTcb_t* Scheduler_round_robin_algorithm(void);
static void Save_context(void);
static void Restore_context(void);

void Kernel_task_init(void)
{
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;

    for(uint32_t i = 0 ; i < MAX_TASK_NUM ; i++)
    {
        sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
        sTask_list[i].sp = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;

        sTask_list[i].sp -= sizeof(KernelTaskContext_t);
        KernelTaskContext_t* ctx = (KernelTaskContext_t*)sTask_list[i].sp;
        ctx->pc = 0;
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

static KernelTcb_t* Scheduler_round_robin_algorithm(void)
{
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index;

    return &sTask_list[sCurrent_tcb_index];
}

void Kernel_task_scheduler(void)
{
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_round_robin_algorithm();

    disable_irq();
    Kernel_task_context_switching();
    enable_irq();
}

__attribute__ ((naked)) void Kernel_task_context_switching(void)
{
    __asm__ ("B Save_context");
    __asm__ ("B Restore_context");
}

static __attribute__ ((naked)) void Save_context(void)
{
    // save current task context into the current task tack
    __asm__("PUSH {lr}");   // PUSH LR to Stack
    __asm__("PUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}"); // PUSH r0~r12 to Stack
    __asm__("MRS r0, cpsr"); // Get cpsr to r0
    __asm__("PUSH {r0}"); // Push cpsr

    // save current task stack pointer into the current TCB
    __asm__("LDR r0, =sCurrent_tcb"); // Get sCurrent_tcb
    __asm__("LDR r0, [r0]"); // Get sCurrent_tcb's pointer
    __asm__("STMIA r0!, {sp}"); // Store Stacks(16) at sCurrnet_tcb
    /* STM: Store Multiple regiter(0~15) <-> LDM: Loade Multiple register
       IA: Ascending, EA: Descending */
}

static __attribute__ ((naked)) void Restore_context(void)
{
    // restore next task stack pointer from the next TCB
    __asm__("LDR r0, =sNext_tcb");  // Get Next_tcb
    __asm__("LDR r0, [r0]");
    __asm__("LDMIA r0!, {sp}"); // r0에 저장된 sNext_tcb에 있는 값 순서대로 sp를 Pop 해준다.

    // restore next task context from the next task stack
    __asm__("POP {r0}"); 
    __asm__("MSR cpsr, r0"); // Restore spsr to cpsr
    __asm__("POP {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__("POP {pc}"); // Retstore PC
}

void Kernel_task_start(void)
{
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_context();
}

uint32_t Kernel_task_get_current_task_id(void)
{
    return sCurrent_tcb_index;
}