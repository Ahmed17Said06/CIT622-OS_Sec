#define __USE_MISC
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>


// Example assembly code snippet
void myAssemblyCode() {
    asm("mov $0, %eax"); // Example instruction: move 0 to the EAX register
}

int main() {
    size_t pageSize = getpagesize();
    void* buffer = mmap(NULL, pageSize * 2, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Align the buffer address to the nearest page boundary
    void* alignedBuffer = (void*)((size_t)buffer + (pageSize - 1) & ~(pageSize - 1));

    // Mark the memory region as non-executable
    //if (mprotect(alignedBuffer, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
    if (mprotect(alignedBuffer, pageSize,  PROT_WRITE | PROT_READ) == -1) {        
        perror("mprotect");
        exit(1);
    }

    // Copy the assembly code to the allocated memory region
    memcpy(alignedBuffer, myAssemblyCode, pageSize);

    // Attempt to execute code from the protected memory region
    void (*func)() = (void (*)())alignedBuffer;
    func();

    // Clean up and free the allocated memory
    munmap(buffer, pageSize * 2);

    return 0;
}
