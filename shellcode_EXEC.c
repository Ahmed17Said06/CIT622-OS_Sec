#define __USE_MISC
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>

// Function to execute the shellcode
void execute_shellcode() {
    // Define your shellcode here
    // This is just an example shellcode that injects the execution of '/bin/sh' in the .text section
    unsigned char shellcode[] = "\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\xbf\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xef\x08\x57\x48\x89\xe7\xb0\x3b\x0f\x05";
    
    // Allocate a memory page with read and write permissions
    void *mem = mmap(NULL, sizeof(shellcode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    // Copy the shellcode to the allocated memory page
    memcpy(mem, shellcode, sizeof(shellcode));
    
    // Change the memory page permissions to execute
    if (mprotect(mem, sizeof(shellcode), PROT_READ | PROT_EXEC | PROT_WRITE) == -1) {
        perror("mprotect");
        return;
    }
    
    // Call the shellcode as a function
    void (*func)() = (void (*)())mem;
    func();
    
    // Deallocate the memory page
    if (munmap(mem, sizeof(shellcode)) == -1) {
        perror("munmap");
        return;
    }
}

int main() {
    printf("Executing shellcode...\n");
    execute_shellcode();
    printf("Shellcode execution completed.\n");
    
    return 0;
}
