ENTRY(_start)

SECTIONS
{
    /* GRUB 부트로더 커널을 로드하는 표준 주소 (1MB 이후) */
    . = 1M;

    .text : ALIGN(4K)
    {
        *(.multiboot) /* 멀티부트 헤더가 파일 맨 앞에 오도록 배치 */
        *(.text)
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    .data : ALIGN(4K)
    {
        *(.data)
    }

    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
