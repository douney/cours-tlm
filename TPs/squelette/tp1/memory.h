
#ifndef MEMORY_H
#define MEMORY_H

#include "ensitlm.h"

struct Memory : sc_core::sc_module {
	ensitlm::data_t size;
	ensitlm::data_t *storage;
	ensitlm::target_socket<Memory> target;



	tlm::tlm_response_status write(const ensitlm::addr_t &a,
	                               const ensitlm::data_t &d);

	tlm::tlm_response_status read(const ensitlm::addr_t &a,
	                              /* */ ensitlm::data_t &d);
	
	SC_HAS_PROCESS(Memory);
	Memory(sc_core::sc_module_name name, ensitlm::data_t size);
	~Memory();

};

#endif