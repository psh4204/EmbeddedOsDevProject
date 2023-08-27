#include "ARMv7AR.h"
#include "MemoryMap.h"

.text                       @ AREA .text 섹션
    .code 32                @ CODE32. 32 비트라는 뜻
    
    @ EXPORT. extern. 전역 변수.
    .global vector_start
    .global vetor_end

    @ vector_start 레이블
    vector_start:
        LDR PC, reset_handler_addr                      @ PC에 reset_handler_addr 주소 로드
        LDR PC, undef_handler_addr
        LDR PC, svc_handler_addr
        LDR PC, pftch_abt_handler_addr
        LDR PC, data_abt_handler_addr
        B   .                                           @ 현재 PC로 Branch(점프)/ 무한 루프
        LDR PC, irq_handler_addr
        LDR PC, fiq_handler_addr
        
        reset_handler_addr:     .word reset_handler     @ reset_handler_addr레이블. reset_handler이름으로  .word 크기 메모리 할당
        undef_handler_addr:     .word dummy_handler
        svc_handler_addr:       .word dummy_handler
        pftch_abt_handler_addr: .word dummy_handler
        data_abt_handler_addr:  .word dummy_handler
        irq_handler_addr:       .word dummy_handler
        fiq_handler_addr:       .word dummy_handler
    vetor_end:
    
    @ reset_hadler 레이블
    reset_handler:
    MRS r0, cpsr                        @ Move Register <- Current Status. 
    BIC r1, r0, #0x1F                   @ r1 = (r0 & ~#0x1F) (r0에서 0x1F를 클리어). PSR에서 Mode영역 Reset               
    ORR r1, r1, #ARM_MODE_BIT_SVC       @ r1 = (r1 | SVC 모드)
    LDR sp, =SVC_STACK_TOP              @ sp = SVC 스택 최상단 주소로

    MRS r0, cpsr                        @ 다시 r0 = CPSR
    BIC r1, r0, #0x1F                   @ PSR Mode Reset
    ORR r1, r1, #ARM_MODE_BIT_IRQ       @ IRQ모드 Set
    LDR sp, =IRQ_STACK_TOP              @ sp = IRQ 스택 최상단 주소로

    MRS r0, cpsr                        @ 다시 r0 = CPSR
    BIC r1, r0, #0x1F                   @ PSR Mode Reset
    ORR r1, r1, #ARM_MODE_BIT_FIQ       @ IRQ모드 Set
    LDR sp, =FIQ_STACK_TOP              @ sp = FIQ 스택 최상단 주소로

    MRS r0, cpsr                        @ 다시 r0 = CPSR
    BIC r1, r0, #0x1F                   @ PSR Mode Reset
    ORR r1, r1, #ARM_MODE_BIT_ABT       @ ABT모드 Set
    MSR cpsr, r1                        @ cpsr <- r1
    LDR sp, =ABT_STACK_TOP              @ sp = ABT 스택 최상단 주소로

    MRS r0, cpsr                        @ 다시 r0 = CPSR (ABT)
    BIC r1, r0, #0x1F                   @ PSR Mode Reset
    ORR r1, r1, #ARM_MODE_BIT_UND       @ UND모드 Set
    MSR cpsr, r1                        @ cpsr <- r1
    LDR sp, =UND_STACK_TOP              @ sp = UND 스택 최상단 주소로

    MRS r0, cpsr                        @ 다시 r0 = CPSR (UND)
    BIC r1, r0, #0x1F                   @ PSR Mode Reset
    ORR r1, r1, #ARM_MODE_BIT_SYS       @ SYS모드 Set (USR SYS)
    MSR cpsr, r1                        @ cpsr <- r1
    LDR sp, =USRSYS_STACK_TOP           @ sp = USRSYS 스택 최상단 주소로

    @ dummy_handler 레이블  
    dummy_handler:
        B   .

.end