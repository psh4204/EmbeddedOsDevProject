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
        LDR R0, =0x10000000 @ SYS_ID
        LDR R1, [R0]

    @ dummy_handler 레이블  
    dummy_handler:
        B   .

.end