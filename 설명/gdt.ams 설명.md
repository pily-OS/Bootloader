; GDT 테이블 정의 (32비트 NASM 어셈블리)
section .data

설명:
section .data: 초기화된 데이터 섹션 (읽기 전용)
GDT는 메모리에 고정된 값으로 저장되므로 .data 사용
(.text는 코드, .bss는 초기화 안 된 변수)

align 8
    gdt_start:

설명:
align 8: 8바이트 경계에 정렬
GDT 디스크립터는 8바이트씩이므로 정렬 필수
메모리 주소가 8의 배수여야 함 (효율성)
gdt_start:: 라벨 (GDT 시작 주소)
링커가 이 레이블을 기억했다가 뒤에서 사용

; Index 0: NULL Descriptor (8바이트 = 0x0000000000000000)
dd 0x00000000  ; 처음 4바이트
dd 0x00000000  ; 다음 4바이트

설명:
dd: Define Doubleword (4바이트 = 32비트)
NULL 디스크립터는 CPU 규칙
x86 CPU가 요구하는 첫 번째 엔트리
항상 0으로 설정
사용하면 General Protection Fault 발생

메모리 주소: gdt_start
  ┌─────────────────┐
  │  0x00000000     │ ← 첫 4바이트
  │  0x00000000     │ ← 다음 4바이트
  └─────────────────┘ 총 8바이트 (Index 0)

; Index 1: Kernel Code (32-bit)
; Descriptor: 0x00cf9a000000ffff
dd 0x0000ffff  ; 처음 4바이트
dd 0x00cf9a00  ; 다음 4바이트

64비트 값 분석:

0x00cf9a000000ffff
└─────────┬─────────┘
          │
      디스크립터 (한 줄로 표현)

나누면:
┌─ 상위 32비트 ─┬─ 하위 32비트 ─┐
│  0x00cf9a00  │  0x0000ffff   │
└──────────────┴───────────────┘
각 부분 의미:
하위 32비트: 0x0000ffff
  Bytes 0-1: 0xffff = Limit (64KB, 4KB 단위 = 4GB)
  Bytes 2-3: 0x0000 = Base Address (24비트 중 하위 16비트)

상위 32비트: 0x00cf9a00
  Byte 4:    0x00 = Base Address (24비트 중 상위 8비트)
  Byte 5:    0x9a = Attributes
             10011010
             ││││││││
             │││││││└─ Type bit 0 (Code=1)
             ││││││└── Type bit 1 (Code/Data=0)
             │││││└─── Type bit 2 (Direction=0)
             ││││└──── Type bit 3 (Readable=1)
             │││└───── Accessed bit (=0)
             ││└────── DPL bit 0 (Ring=0)
             │└─────── DPL bit 1 (Ring=0)
             └──────── P (Present=1) ← 이게 제일 중요!
  Byte 6:    0xcf = Granularity + Limit 상위 4비트
             11001111
             ││││││││
             │││││││└─ Limit bit 12
             ││││││└── Limit bit 13
             │││││└─── Limit bit 14
             ││││└──── Limit bit 15
             │││└───── Reserved
             ││└────── Default Op Size (1=32-bit)
             │└─────── Granular (1=4KB 단위)
             └──────── Reserved
  Byte 7:    0x00 = Base Address (상위 8비트)

실제 값:
Base Address (32비트 세그먼트 기본주소):
  0x00 (Byte 7) + 0x00 (Byte 4) + 0x0000 (Bytes 2-3)
  = 0x00000000 ← 메모리 맨 처음

Limit (세그먼트 크기):
  0xf (Byte 6 상위 4비트) + 0xffff (Bytes 0-1)
  = 0xfffff ← 4KB 단위로 = 4GB

권한 (Privilege Level):
  0x9a Byte에서 DPL = 00 ← Ring 0 (커널)

타입:
  0x9a = 10011010
  → Type=1010 (코드 세그먼트, 읽기 가능)


커널 데이터 디스크립터:
        ; Index 2: Kernel Data (32-bit)
        ; Descriptor: 0x00cf92000000ffff
        dd 0x0000ffff
        dd 0x00cf9200
커널 코드와의 차이:

코드:  0x00cf9a000000ffff  (0x9a)
데이터: 0x00cf92000000ffff  (0x92)
                   ││
                   └─ Type = 0010 (데이터, 읽기/쓰기 가능)
다른 것은 모두 같음 (Base, Limit, Ring 0)

GDT 끝 마킹
gdt_end:
GDT 끝을 표시하는 라벨
뒤에서 gdt_end - gdt_start - 1로 GDT 크기 계산

GDT 포인터 (GDTR용)
gdt_pointer:
        dw gdt_end - gdt_start - 1  ; 16비트: GDT 크기
        dd gdt_start                 ; 32비트: GDT 주소

GDTR 레지스터 포맷:
GDTR 레지스터 (48비트)
├─ Bytes 0-1 (16비트): GDT 크기 - 1
│  예: 3개 엔트리 = 3×8 - 1 = 23 (0x17)
│
└─ Bytes 2-5 (32비트): GDT 시작 주소
   예: 0x00000000

이 코드에서:
gdt_end - gdt_start - 1
= 24바이트 - 0바이트 - 1
= 23 (0x17)
← 3개 엔트리 × 8바이트 - 1

GDT 로드 함수
global gdt_load
gdt_load:
    lgdt [gdt_pointer]  ; GDTR 레지스터에 GDT 포인터 로드
    ret

global gdt_load: 다른 파일(kernel.c)에서 호출 가능하게 공개
lgdt [gdt_pointer]: GDTR 레지스터에 로드
CPU가 "이제부터 이 GDT를 사용해"라고 알림
[gdt_pointer] = 메모리 주소 (gdt_pointer가 있는 곳)
ret: C 함수로 복귀

최종 메모리 구조

메모리:
┌──────────────────────────────────┐
│  gdt_start (주소: gdt_start)     │
├──────────────────────────────────┤
│  0x00000000 0x00000000           │ ← Index 0: NULL
│  (8바이트)                        │
├──────────────────────────────────┤
│  0x0000ffff 0x00cf9a00           │ ← Index 1: Code
│  (8바이트)                        │
├──────────────────────────────────┤
│  0x0000ffff 0x00cf9200           │ ← Index 2: Data
│  (8바이트)                        │
├──────────────────────────────────┤
│  gdt_end (끝)                    │
└──────────────────────────────────┘

GDTR 레지스터에 로드되는 값:
┌────┬──────────────────┐
│ 23 │  gdt_start addr  │ (48비트)
└────┴──────────────────┘
