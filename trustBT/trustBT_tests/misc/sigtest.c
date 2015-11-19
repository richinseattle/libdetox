#include <stdio.h>
#include <libfastbt.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifndef __USE_GNU         // Deal with current ucontext ugliness.  The current
#define __USE_GNU         // mess of asm/ucontext.h & sys/ucontext.h and their
#include <sys/ucontext.h> // mutual exclusion on more recent versions of gcc
#undef __USE_GNU          // dictates this ugly hack.
#else
#include <sys/ucontext.h>
#endif

static void seghandler(unsigned int sn , siginfo_t  si , struct ucontext *sc)
{
  int *gregs = sc->uc_mcontext.gregs;
  printf(" Signal number = %d (%d), status = %d\n", si.si_signo, sn, si.si_status);
  switch(si.si_code) {
    case 1: printf(" SI code = %d (Address not mapped to object)\n", si.si_code);
      break;
    case 2: printf(" SI code = %d (Invalid permissions for mapped object)\n",si.si_code);
      break;
    default: printf(" SI code = %d (Unknown SI Code)\n",si.si_code);
      break;
  }

  printf(" Intruction pointer = 0x%08x \n", gregs[REG_EIP]);
  printf(" Fault addr = 0x%p \n",si.si_addr);
  printf(" trap = 0x%08x \n", gregs[REG_TRAPNO]);
  printf(" err  = 0x%08x \n", gregs[REG_ERR]);
  printf("*** Registers ***\n");
  printf("    EAX: 0x%08x\n", gregs[REG_EAX]);
  printf("    EBX: 0x%08x\n", gregs[REG_EBX]);
  printf("    ECX: 0x%08x\n", gregs[REG_ECX]);
  printf("    EDX: 0x%08x\n", gregs[REG_EDX]);
  printf("    EDI: 0x%08x\n", gregs[REG_EDI]);
  printf("    ESI: 0x%08x\n", gregs[REG_ESI]);
  printf("    EBP: 0x%08x\n", gregs[REG_EBP]);
  printf("    ESP: 0x%08x\n", gregs[REG_ESP]);
  printf("     SS: 0x%08x\n", gregs[REG_SS]);
  printf(" EFLAGS: 0x%08x\n", gregs[REG_EFL]);

  exit(139);
}


int main(int argc, char **argv)
{

  	struct sigaction m;
	volatile int i = 1;

	printf("before start transaction\n");

	fbt_init(NULL);
	fbt_start_transaction(&fbt_commit_transaction);

  	memset(&m, 0, sizeof(struct sigaction));
  	m.sa_flags = SA_SIGINFO;
  	m.sa_sigaction = (void *)seghandler;

	//if(sigaction(SIGTERM,&m,(struct sigaction *)NULL) != 0)
	//if(sigaction(SIGSEGV,&m,(struct sigaction *)NULL) != 0)
	if(sigaction(SIGINT,&m,(struct sigaction *)NULL) != 0)
	{
		printf("sigaction failed\n");
		exit(0);
	}

	printf("start loop\n");

	while(i) {
	}
	//if(raise(SIGTERM) !=0) printf("sending signal failed\n");

	fbt_commit_transaction();

	printf("after loop\n");
	return 0;
}


