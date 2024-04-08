This project delves into the exciting realm of eBPF and its potential to revolutionize OS security. The growing hype surrounding this technology is well-deserved, as it offers numerous advantages across various domains, including kernel development, telecommunications, and security.
Specifically, this research project focuses on memory protection mechanisms, with a keen emphasis on DEP/NX. These mechanisms play a critical role in preventing the execution of malicious code. However, attackers continuously find ways to bypass these protections. The goal of my project is to harness the power of eBPF to effectively detect and thwart such bypass attempts.
By leveraging the flexibility and efficiency of eBPF, I aim to analyze specific system call events within the OS kernel. This analysis will enable the identification of anomalies that may indicate attempts to bypass DEP/NX. By doing so, we can significantly enhance the overall security posture of operating systems and safeguard against malicious activities.

Our PoC/Demo consists of eBPF BCC-based code that does the observation inside the kernel for the mprotect() system call, and 2 other c code files that work to bypass the NX one of them just bypass the NX and execute a mov assembly instruction while the other one do shellcode injection, and open a shell terminal.
It’s noticed that our eBPF program detect those malicious activity very well, and it’s also noticed that on a fully fledged OS like ubuntu and while making almost all the programs installed of it working no other programs are using this kind of Memory Permission Manipulation. 

Working Enviroment:
Ubuntu 22.04 LTS VM and should work fine on any other Linux or Unix based OS.
eBPF is kernel-based feature and it exists in all modern OS kernels, so you just need to ensure that the OS you use supports eBPF. However, you will need to install the eBPF dependencies like LLVM, libbpf, and kernel headers, also you need to install BCC and to make sure it is working.
eBPF should be working also on windows but this project didn’t take into consideration windows, and it was only tested on Linux.

Files
•	“eBPF_NX.py”: is the BCC python program that wraps the eBPF c-based program. Must run it with sudo or as a root.
•	“NX_pyPass.c”: c code that allocates and aligns memory within the process and then manipulate its permissions to execute a move instruction, if you are inside, it’s code didn’t use the mprotect or the execution permission the code will crash with segmentation fault due to the NX, but with mprotect it bypasses the NX and the code execute the move instruction normally and exists. This code demonstrates a technique commonly used in code injection attacks. It allows the execution of arbitrary assembly code within the context of the running program. While the example code itself does not contain any malicious instructions, it provides a foundation for potentially dangerous operations.
•	“shellcode_EXEC.c”: same as the above c code, but this time it executes real shell code that manipulates registers and it executes a shell through “/bin/bash”. This works as a base to an attack and can be combined with other techniques like buffer overflow and/or code injection, and it could have serious consequences. 
•	Other files: are logs of strace and objdump based .asm file that contains the system calls and disassembly.
