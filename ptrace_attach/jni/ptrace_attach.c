#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/syscall.h>

int long_size = sizeof(long);

static void reverse(char *str){
    int i, j;
    char temp;
    for(i = 0, j = strlen(str) -2; i <= j; i++, j--){
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

static void getdata(pid_t pid, long addr, char *str, int len) {
    char *laddr;
    int i, j;
    union u{
        long val;
        char chars[long_size];
    } data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j){
        data.val = ptrace(PTRACE_PEEKDATA, pid, (addr + i*4), NULL);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0){
        data.val = ptrace(PTRACE_PEEKDATA, pid, (addr + i*4), NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
}

static void putdata(pid_t pid, long addr, char *str, int len){
    char *laddr;
    int i, j;
    union u {
        long val;
        char chars[long_size];
    } data;

    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j){
        memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKEDATA, pid, (void*)(addr + i*4), data.val);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0){
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, pid, (void*)(addr + i*4), data.val);
    }
}

static long getSysCallNo(int pid, struct pt_regs *regs){
    long scno = 0;
    // struct pt_regs regs;
    ptrace(PTRACE_GETREGS, pid, NULL, regs);
    scno = ptrace(PTRACE_PEEKTEXT, pid, (void *)(regs->ARM_pc -4), NULL);
    if(scno == 0){
        return 0;
    }
    if(scno == 0xef000000) {
        scno = regs->ARM_r7;
    } else {
        if((scno & 0x0ff00000) != 0x0f900000) {
            return -1;
        }
        scno &= 0x000fffff;
    }
    return scno;
}

static void tracePro(int pid){
    printf("Attached success: %d.\n", pid);
    long scno = 0;
    long regv = 0;
    struct pt_regs regs;
    char *str;

    scno = getSysCallNo(pid, &regs);
    // TODO ... getdata 和 putdata 的字符串处理逻辑有问题.
    if(scno == __NR_write){
        printf("Call __NR_write. \n");
        str = (char*)calloc(1, (regs.ARM_r2+1) * sizeof(char));
        printf("start getdata. \n");
        getdata(pid, regs.ARM_r1, str, regs.ARM_r2);
        printf("end getdata: %s.\n", str);
        reverse(str);
        printf("start putdata. \n");
        putdata(pid, regs.ARM_r1, str, regs.ARM_r2);
        printf("end putdata. \n");
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: %s <pid to be traced>\n", argv[0]);
        return 1;
    }

    pid_t traced_process;
    int status;
    traced_process = atoi(argv[1]);
    if(0 != ptrace(PTRACE_ATTACH, traced_process, NULL, NULL)) {
        printf("Trace process failed: %d.\n", errno);
        return 1;
    }
    while(1){
        wait(&status);
        if(WIFEXITED(status)){
            break;
        }
        tracePro(traced_process);
        ptrace(PTRACE_SYSCALL, traced_process, NULL, NULL);
    }

    ptrace(PTRACE_DETACH, traced_process, NULL, NULL);

    return 0;
}
