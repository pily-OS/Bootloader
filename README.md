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
