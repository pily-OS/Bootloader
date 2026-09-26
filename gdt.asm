; GDT 테이블 정의 (32비트 NASM 어셈블리)
section .data
    align 8
    gdt_start:
        ; Index 0: NULL Descriptor (8바이트 = 0x0000000000000000)
        dd 0x00000000  ; 처음 4바이트
        dd 0x00000000  ; 다음 4바이트
        
        ; Index 1: Kernel Code (32-bit)
        ; Descriptor: 0x00cf9a000000ffff
        dd 0x0000ffff  ; 처음 4바이트 (Limit 하위 16비트 + Base 하위 16비트)
        dd 0x00cf9a00  ; 다음 4바이트 (Base 상위 16비트 + Attributes + Limit 상위 4비트)
        
        ; Index 2: Kernel Data (32-bit)
        ; Descriptor: 0x00cf92000000ffff
        dd 0x0000ffff  ; 처음 4바이트
        dd 0x00cf9200  ; 다음 4바이트
    
    gdt_end:

    ; GDT 포인터 (GDTR 레지스터에 로드할 값)
    gdt_pointer:
        dw gdt_end - gdt_start - 1  ; GDT 크기 - 1 (16비트)
        dd gdt_start                 ; GDT 시작 주소 (32비트)

; GDT 로드 함수
global gdt_load
gdt_load:
    lgdt [gdt_pointer]  ; GDTR 레지스터에 GDT 포인터 로드
    ret
