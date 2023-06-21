.text                       /* text 섹션 시작 _ ' ; ' 주석이 안되면 이 주석을 쓰자 */
    .code 32                /* 32 비트라는 뜻 */

    .global vector_start    /* .gloabl 은 extern 이다. */
    .global vetor_end

    vector_start:           /* vector_start 레이블 */
        MOV     R0, R1      /* R1을 R0에 에 옮겨라 */
    vetor_end:              /* vector_end 레이블 */
        .space 1024, 0      /* 1024 바이트 만큼 0으로 채워라 */
.end                        /* text 섹션 끝 */
