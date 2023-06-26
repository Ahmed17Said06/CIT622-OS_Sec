#!/usr/bin/python3  

from __future__ import print_function
from bcc import BPF, DEBUG_PREPROCESSOR


bpf_text = """
#include <uapi/linux/ptrace.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/types.h>

struct data_t {
    u32 pid;  // PID as in the userspace 
    u32 ppid; // Parent PID as in the userspace
    u32 uid;
    char comm[TASK_COMM_LEN];

    u64 start;
    u32 len;
    u64 prot;
};

#define READ_KERN(ptr) ({                    \
  typeof(ptr) _val;                          \
  __builtin_memset(&_val, 0, sizeof(_val));  \
  bpf_probe_read(&_val, sizeof(_val), &ptr); \
  _val;                                      \
})
BPF_PERF_OUTPUT(events);

int __do_mprotect_pkey(struct pt_regs *ctx,u64 start, u32 len, u64 prot)
{
    u32 uid = bpf_get_current_uid_gid() & 0xffffffff;

    struct data_t data = { .uid = uid, .start = start };
    struct task_struct *task;

    data.len = READ_KERN(len);
    data.prot = READ_KERN(prot);

    data.pid = bpf_get_current_pid_tgid() >> 32;

    bpf_get_current_comm(&data.comm, sizeof(data.comm));

    task = (struct task_struct *)bpf_get_current_task();
    data.ppid = task->real_parent->tgid;
    events.perf_submit(ctx, &data, sizeof(data));
    return 0;
}

"""


b = BPF(text=bpf_text, debug=0)
b.attach_kprobe(event="do_mprotect_pkey", fn_name="__do_mprotect_pkey")


def print_event(cpu, data, size):
    event = b["events"].event(data)
    if ((event.prot == 6) or (event.prot == 7) or (event.prot == 4)):
        print(event.pid, event.ppid, event.uid, event.comm.decode(),
              hex(event.start), event.len, event.prot, sep='\t')


b["events"].open_perf_buffer(print_event)

print("*********\__eBPF based mprotect observation started__/***********")
print("PID", "PPID", "UID", "command", "        startAddr", "length", "protFlags", sep='\t')

while 1:
    try:
        b.perf_buffer_poll()
    except KeyboardInterrupt:
        exit()