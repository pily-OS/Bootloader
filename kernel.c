// VGA 텍스트 모드의 화면 표준 크기 (가로 80칸, 세로 25줄)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VIDEO_MEMORY (char *)0xB8000

// 현재 화면의 커서 위치를 기억할 전역 변수
int cursor_x = 0;
int cursor_y = 0;

// 화면 전체를 공백으로 깨끗하게 비우는 함수
void clear_screen(void) {
    char *video = VIDEO_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video[i] = ' ';     // 공백 문자 채우기
        video[i+1] = 0x07;  // 검은 배경에 흰 글씨 속성
    }
    cursor_x = 0;
    cursor_y = 0;
}

// 문자를 하나씩 분석해서 화면에 그리는 핵심 드라이버 함수
void putchar(char c) {
    char *video = VIDEO_MEMORY;

    // 1. 줄 바꿈 문자('\n')를 만났을 때의 처리
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } 
    // 2. 일반 문자일 때의 처리
    else {
        // 현재 x, y 좌표를 기반으로 1차원 비디오 메모리 주소(Offset) 계산
        int offset = (cursor_y * VGA_WIDTH + cursor_x) * 2;
        video[offset] = c;
        video[offset + 1] = 0x0A; // 검은 배경에 '연두색' 글씨 지정!

        cursor_x++;
    }

    // 가로 한 줄(80칸)이 꽉 찼다면 자동으로 다음 줄로 넘기기
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // [참고] 만약 cursor_y가 25줄을 넘어가면 화면을 위로 올리는 '스크롤' 로직이 추후 필요합니다.
}

// 문자열을 넘겨받아 putchar를 연속으로 호출하는 커널 전용 출력 함수
void kprint(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        putchar(str[i]);
        i++;
    }
}

// 커널 메인 함수
void kernel_main(void) {
    clear_screen(); 

    
    kprint("  _____  _____ _      __     __       ____   ____  \n");
    kprint(" |  __ \\|_   _| |     \\ \\   / /      / __ \\ / ____| \n");
    kprint(" | |__) | | | | |      \\ \\_/ /_____| |  | | (___   \n");
    kprint(" |  ___/  | | | |       \\   /|______| |  | |\\___ \\  \n");
    kprint(" | |     _| |_| |____    | |        | |__| |____) |\n");
    kprint(" |_|    |_____|______|   |_|         \\____/|_____/ \n");
    kprint("\n");
    kprint("====================================================\n");
    kprint("  Booting Completed Successfully.\n");
    kprint("====================================================\n");
    kprint("\n");
    
    kprint("Welcome to pily-OS Kernel!\n");
    
}


