// VGA 텍스트 모드의 화면 크기는 가로 80, 세로 25입니다.
// 주소 0xB8000에 [문자 1바이트 + 색상 1바이트] 쌍으로 글자를 적으면 화면에 나옵니다.
void kernel_main(void) {
    char *video_memory = (char *) 0xB8000;
    
    // 화면 맨 첫 줄에 출력할 메시지
    const char *str = "Welcome to My 64-bit OS Kernel!";
    
    // 화면을 깨끗하게 청소 (공백문자로 채우기)
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';     // 문자 공간
        video_memory[i+1] = 0x07;  // 색상 공간 (검은 배경에 흰 글씨)
    }

    // 메시지 출력하기
    int i = 0;
    while (str[i] != '\0') {
        video_memory[i * 2] = str[i];      // 문자 쓰기
        video_memory[i * 2 + 1] = 0x0A;  // 색상 쓰기 (검은 배경에 연두색 글씨!)
        i++;
    }
}
