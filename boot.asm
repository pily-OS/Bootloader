; 멀티부트 1 사양 표준 매직 넘버 세팅
MULTIBOOT_MAGIC    equ 0x1BADB002
MULTIBOOT_FLAGS    equ 0x00000003 ; 페이지 정렬 및 메모리 정보 요청
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

section .multiboot
align 4
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

section .text
global _start
extern kernel_main      ; C언어에 있는 kernel_main 함수를 가져다 씁니다.

_start:
    cli                 ; 인터럽트를 잠시 끕니다.
    mov esp, stack_top  ; C언어가 함수를 호출할 수 있도록 스택 포인터 설정
    
    call kernel_main    ; C언어 커널 메인 함수 호출

.hang:
    hlt                 ; CPU를 절전/대기 상태로 둠
    jmp .hang           ; 만약 깨어나도 다시 대기 (무한 루프)

section .bss
align 16
stack_bottom:
    resb 16384          ; C언어가 쓸 16KB 크기의 스택 공간 확보
stack_top:
