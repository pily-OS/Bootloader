// VGA 텍스트 모드의 화면 표준 크기 (가로 80칸, 세로 25줄)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VIDEO_MEMORY (char *)0xB8000

// VGA 컬러 상수
#define COLOR_BLACK       0x00
#define COLOR_LIGHT_GREEN 0x0A
#define COLOR_YELLOW      0x0E
#define COLOR_LIGHT_CYAN  0x0B
#define COLOR_LIGHT_RED   0x0C

// 현재 화면의 커서 위치를 기억할 전역 변수
int cursor_x = 0;
int cursor_y = 0;
int current_fg_color = COLOR_LIGHT_GREEN;
int current_bg_color = 0x00;

// ============================================
// 속성 바이트 생성 함수
// ============================================
char make_color_attribute(int fg, int bg) {
    return (char)((bg << 4) | fg);
}

// ============================================
// 색상 변경 함수
// ============================================
void set_text_color(int fg, int bg) {
    current_fg_color = fg;
    current_bg_color = bg;
}

void reset_colors(void) {
    current_fg_color = COLOR_LIGHT_GREEN;
    current_bg_color = 0x00;
}

// 화면 전체를 공백으로 깨끗하게 비우는 함수
void clear_screen(void) {
    char *video = VIDEO_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = make_color_attribute(current_fg_color, current_bg_color);
    }
    cursor_x = 0;
    cursor_y = 0;
}

// 화면을 한 줄 위로 올리는 스크롤 함수
void scroll_screen(void) {
    char *video = VIDEO_MEMORY;

    // 1줄 (80글자 x 2바이트) 1줄 부터 24줄까지 0부터 23줄로 복사
    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i++) {
        video[i] = video[i + VGA_WIDTH * 2];
    }

    // 마지막 줄 공백으로 채우기
    int last_line_offset = (VGA_HEIGHT - 1) * VGA_WIDTH * 2;
    for (int i = last_line_offset; i < VGA_HEIGHT * VGA_WIDTH * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = make_color_attribute(current_fg_color, current_bg_color);
    }
}

// 백스페이스 함수
void backspace(void) {
    char *video = VIDEO_MEMORY;
    
    if (cursor_x == 0 && cursor_y == 0) {
        return;
    }
    
    if (cursor_x > 0) {
        cursor_x--;
    } else {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }
    
    int offset = (cursor_y * VGA_WIDTH + cursor_x) * 2;
    video[offset] = ' ';
    video[offset + 1] = make_color_attribute(current_fg_color, current_bg_color);
}

// 문자를 하나씩 출력하는 함수
void putchar(char c) {
    char *video = VIDEO_MEMORY;
    
    if (c == '\b') {
        backspace();
        return;
    }
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            scroll_screen();
            cursor_y = VGA_HEIGHT - 1;
        }
        return;
    }
    
    if (c == '\r') {
        cursor_x = 0;
        return;
    }
    
    int offset = (cursor_y * VGA_WIDTH + cursor_x) * 2;
    video[offset] = c;
    video[offset + 1] = make_color_attribute(current_fg_color, current_bg_color);
    
    cursor_x++;
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            scroll_screen();
            cursor_y = VGA_HEIGHT - 1;
        }
    }
}

// 문자열을 넘겨받아 putchar를 연속으로 호출
void kprint(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        putchar(str[i]);
        i++;
    }
}

// ============================================
// ============================================
// [Phase 2-1] GDT 관련 코드 (여기부터 추가!)
// ============================================
// ============================================

// 어셈블리에서 정의한 함수 선언
extern void gdt_load(void);

// GDT 초기화 함수
void gdt_init(void) {
    // 현재: GRUB의 GDT 사용
    // 우리가 만든 GDT를 CPU에 로드
    gdt_load();
    
    kprint("[GDT] Initialized\n");
}

// ============================================
// ============================================
// 커널 메인 함수
// ============================================
// ============================================

void kernel_main(void) {
    clear_screen();
    
    // 로고 출력 (연두색)
    set_text_color(COLOR_LIGHT_GREEN, 0x00);
    kprint("  _____  _____ _      __     __       ____   ____  \n");
    kprint(" |  __ \\|_   _| |     \\ \\   / /      / __ \\ / ____| \n");
    kprint(" | |__) | | | | |      \\ \\_/ /_____| |  | | (___   \n");
    kprint(" |  ___/  | | | |       \\   /|______| |  | |\\___ \\  \n");
    kprint(" | |     _| |_| |____    | |        | |__| |____) |\n");
    kprint(" |_|    |_____|______|   |_|         \\____/|_____/ \n");
    kprint("\n");
    
    // 구분선 (노란색)
    set_text_color(COLOR_YELLOW, 0x00);
    kprint("====================================================\n");
    
    // 부팅 메시지 (밝은 파란색)
    set_text_color(COLOR_LIGHT_CYAN, 0x00);
    kprint("  Booting Completed Successfully.\n");
    
    // 구분선 (노란색)
    set_text_color(COLOR_YELLOW, 0x00);
    kprint("====================================================\n");
    reset_colors();
    kprint("\n");
    
    //  [Phase 2-1] GDT 초기화 - 여기서 호출!
    gdt_init();
    
    kprint("\n");
    
    // 환영 메시지 (빨간색)
    set_text_color(COLOR_LIGHT_RED, 0x00);
    kprint("Welcome to pily-OS Kernel - Phase 2!\n");
    reset_colors();
    
    kprint("\nSystem ready.\n");
}
