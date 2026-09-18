# Bootloader
A 64-bit CLI OS kernel based on x86-64 architecture, featuring a custom boot setup with Multiboot (GRUB).

## 🛠️ Development Environment & Toolchain
* **Host OS:** Windows 11 with **WSL2 (Ubuntu 22.04 LTS)**
* **IDE:** Visual Studio Code (Remote - WSL extension)
* **Compiler:** GNU GCC & NASM (Netwide Assembler)
* **Emulator:** QEMU (x86_64 system emulator)

## 🚀 How to Run
Simply clone the repository and run the automated build via Makefile:
```bash
# Clone the repository
git clone <your-repository-url>
cd my_64bit_os

# Build and launch the OS with QEMU
make run
```

## 📌 Upcoming Milestones (향후 개발 목표)

- [ ] **Phase 1: Advanced VGA Screen Driver (화면 출력 드라이버 고도화)**
  * Implement standard character output functions with escape sequence support (`\n` newline, `\b` backspace for character deletion).
  * 텍스트 개행(`\n`) 및 글자 지우기(`\b`, Backspace)가 작동하는 안정적인 커널 전용 출력 함수(기초 `kprintf`) 구현.
  
- [ ] **Phase 2: 64-bit Long Mode Transition & Paging (64비트 롱 모드 전환 및 페이징 설계)**
  * Initialize hierarchical 4-level page tables (PML4, PDPT, PD, PT) to bridge x86 (32-bit) to x86-64 (64-bit Long Mode).
  * CPU를 32비트 보호 모드에서 진짜 64비트 롱 모드로 업그레이드하기 위한 4단계 계층형 페이징 구조 설계 및 메모리 매핑.
