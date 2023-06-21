ENTRY(vector_start)
SECTIONS
{
    . =0x00;                    /* reset_vector */

    .text :                     /* .text section (program) */
    {
        *(vector_start)
        *(.text .rodata)
    }
    .data :                     /* .data section (global val) */
    {
        *(.data)
    }
    .bss :                      /* .bss section (zero init global val) */
    {
        *(.bss)
    }
}