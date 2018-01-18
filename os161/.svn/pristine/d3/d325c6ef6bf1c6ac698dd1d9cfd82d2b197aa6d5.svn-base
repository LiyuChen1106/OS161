#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <machine/trapframe.h>
#include <kern/callno.h>
#include <syscall.h>
#include <kern/unistd.h>
#include <thread.h>
#include <curthread.h>
#include <addrspace.h>
#include <vm.h>
#include <lib.h>

int
sys__time(time_t *seconds, unsigned long *nanoseconds, int *retval)
{


	if (seconds == MIPS_KSEG0)
		return EFAULT;
	if (seconds == 0x40000000)
		return EFAULT;
	if (nanoseconds == MIPS_KSEG0)
		return EFAULT;
	if (nanoseconds == 0x40000000)
		return EFAULT;



	// local copy of seconds and nanoseconds
	time_t local_seconds;
	unsigned long local_nanoseconds;
	int result;

	// get the time from the kernel
	gettime(&local_seconds, &local_nanoseconds);

	if (seconds == NULL && nanoseconds == NULL) {

		//kprintf("both pointers are null, second is %d \n", local_seconds);
		*retval = local_seconds;
		assert(curspl == 0);
		return 0;
	}

	if (seconds == NULL) {

		// copy out to the user space
		result = copyout(&local_nanoseconds, (userptr_t) nanoseconds, sizeof(unsigned long));
		if (result) {
			*retval = -1;
			assert(curspl == 0);
			return result;
		}

		kprintf("second pointer is null, second is %d \n", local_seconds);
		*retval = local_seconds;
		return 0;
	}

	if (nanoseconds == NULL) {

		// copy out to the user space
		result = copyout(&local_seconds, (userptr_t) seconds, sizeof(time_t));
		if (result) {
			*retval = -1;
			assert(curspl == 0);
			return result;
		}

		kprintf("nanoseconds pointer is null, second is %d \n", local_seconds);
		*retval = local_seconds;
		assert(curspl == 0);
		return 0;
	}

	// copy out to the user space
	result = copyout(&local_seconds, (userptr_t) seconds, sizeof(time_t));
	if (result) {
		*retval = -1;
		assert(curspl == 0);
		return result;
	}

	// copy out to the user space
	result = copyout(&local_nanoseconds, (userptr_t) nanoseconds, sizeof(unsigned long));
	if (result) {
		*retval = -1;
		assert(curspl == 0);
		return result;
	}

	*retval = *seconds;
	assert(curspl == 0);

	return 0;
}
