#ifndef REGISTER_ALLOCATOR_H
#define REGISTER_ALLOCATOR_H

// The total number of registers
#define N_REGS 4
// R3 is reserved
#define STACK_REG 3

#include <iostream>
#include <stdlib.h>

// The register allocator manages Register objects.
#include "register.h"

using namespace std;

class Register_Allocator
{
public:
	Register_Allocator();
	~Register_Allocator();

	// Get an unused register.
	Register *allocate_register();

	// Free a previously allocated register.
	void deallocate_register(Register *r);

private:
	// The register set we can allocate from.
	Register *register_set[N_REGS];

	// Internal error routine.
	void freeing_unallocated_register(Register *reg);
};
#endif
