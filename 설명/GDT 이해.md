1. x86 메모리 보호의 역사
이전 (8086 시대):
메모리 = 1MB 선형 주소공간
보호없음 (모든 프로그램이 모든 메모리 접근 가능)
ㄴ 한 프로그램이 다른 프로그램 메모리 변조 가능

X86 보호 모드 (386~):
메모리 = 세그먼트로 나눔
각 세그먼트 = 권한(Ring) + 크기 + 기본 주소

Ring 0: 커널(모든 권한)
Ring 1: 시스템 소프트웨어 (거의 안 씀)
Ring 2: 시스템 소프트웨어 (거의 안 씀)
Ring 3: 사용자 프로그램 (제한된 권한)
ㄴ 한 프로그램이 다른 메모리를 건드리려 하면 CPU가 자동으로 "페이지 폴트" 발생
//페이지 폴트: cpu가 접근하는 데이터나 프로그램 페이지가 현제 물리 메모리에 존재하지 않고 보조기억장치에 있을 때 발생하는 예외(인터럽트)

2. GDT(Global Descriptor Table)의 물리적 구조

GDT란?
1.내 커널이 인정하는 세그먼트 목록
2.메모리의 한 위치에 배열로 저장 
3.각 엔트리 = 8바이트(64비트)

구조:
GDT의 메모리 주소: 0x00000000 (임의)

┌─────────────────────────┐
│  Index 0: NULL (0~7)    │ ← 항상 NULL (CPU 규칙)
├─────────────────────────┤
│  Index 1: Code (8~15)   │ ← 커널 코드 세그먼트
├─────────────────────────┤
│  Index 2: Data (16~23)  │ ← 커널 데이터 세그먼트
├─────────────────────────┤
│  Index 3: Code (24~31)  │ ← 사용자 코드 세그먼트 (Ring 3)
├─────────────────────────┤
│  Index 4: Data (32~39)  │ ← 사용자 데이터 세그먼트 (Ring 3)
└─────────────────────────┘

각 엔트리의 인덱스 × 8 = 세그먼트 선택자(Selector)
예: Index 1 → Selector = 0x08 (1 × 8)

3. GDT 엔트리(디스크립터) 상세 구조

64비트 = 8바이트 디스크립터:

Byte 0-1: limit (하위 16비트)
Byte 2-4: Bass Address (24비트)
Byte 5: Type & Attributes
        Bit 7: Present (P) - 1=무효
        Bit 6-5: Privilege Level (DPL)
                 00=Ring 0 (커널)
                 11=Ring 3 (사용자)
        Bit 4: Descriptor Type (5)
               1=Code/Data, 0 = System
        Bit 3-0: Type
                 1000=Code (execute only)
                 0000=Data (read/write)
Byte 6: Granularity(세분성)
        Bit 7: Granular (G) - 1=4KB, 0=1Byte
        Bit 6: Default operation size (D/B)
               1=32-bit, 0=16-bit
        Bit 5-4: Reserved (예약된)
        Bit 3-0: Limit  (상위 4비트)
Byte 7: Base Address (상위 8비트)

디스크립터 = { 기본주소, 크기, 권한, 유형}
예: 커널 코드 세그먼트
    - 기본주소:0x00000000 (메모리 맨 처음)
    - 크기: 4GB (전체 메모리)
    - 권한: Ring 0 (커널)
    - 유형: 코드 (execute only) (오직 실행)

4. GRUB이 준 기본 GDT
현재 GRUB이 만들 GDT사용:
Index 0: NULL
Index 1: Kernel Code (32-bit)
Index 2: Kernel Data (32-bit)

32비트만 사용: 현제는 상관없음
64비트 롱모드 필요: X 64비트 세그먼트 필요

Index 0: NULL
Index 1: Kernel Code (32-bit)
Index 2: Kernel Data (32-bit)
Index 3: Kernel Code (64-bit) 
Index 4: Kernel Data (64-bit) 
Index 5: User Code (32-bit)    
Index 6: User Data (32-bit)    
