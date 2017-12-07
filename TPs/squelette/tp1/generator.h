
#ifndef MEMORY_H
#define MEMORY_H

#include "ensitlm.h"

struct Generator : sc_core::sc_module {

	ensitlm::initiator_socket<Generator> initiator;
	void thread(void);

	SC_CTOR(Generator);
	
};

#endif