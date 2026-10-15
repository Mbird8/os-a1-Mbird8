#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/uproc.h"
#include "user/user.h"

static struct uproc procs[NPROC];   // static: keep it off the user stack
static char *states[] = { "unused", "used", "sleeping", "runnable",
                          "running", "zombie" };

int
main(void)
{
  int n = getprocs(procs, NPROC);
  if(n < 0){
    fprintf(2, "ps: getprocs failed\n");
    exit(1);
  }
  printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");
  for(int i = 0; i < n; i++){
    int s = procs[i].state;
    printf("%d\t%d\t%s\t%lu\t%s\n", procs[i].pid, procs[i].ppid,
           (s >= 0 && s < 6) ? states[s] : "?", procs[i].sz, procs[i].name);
  }
  exit(0);
}