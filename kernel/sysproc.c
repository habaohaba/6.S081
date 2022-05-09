#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
    // lab pgtbl: your code here.
    // pgtbl of process
    pagetable_t pgtbl = myproc()->pagetable;
    // starting va of the first user page to check
    uint64 va;
    // number of pages to check
    int n;
    // ua to store results 
    uint64 ua;
    argaddr(0, &va);
    argint(1, &n);
    argaddr(2, &ua);
    // bitmask to store result
    uint64 bitmask = 0;
    uint64 cleaner = 0xFFFFFFFFFFFFFFBF;
    uint64 flag;
    if (n > 64)
        panic("more than 64 pages to scanned.");  
    for (int i = 0; i < n; i++){
        pte_t *pte = walk(pgtbl, va, 0);
        if ((*pte & PTE_V) && (*pte & PTE_A)){
            flag = (1L << i);
            bitmask = bitmask | flag;
            *pte = *pte & cleaner;
        }
        va += PGSIZE;
    }
    copyout(pgtbl, ua, (char *) &bitmask, 8);
    return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
