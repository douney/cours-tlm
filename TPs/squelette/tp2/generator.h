#ifndef Generator_H
#define Generator_H

#include "ensitlm.h"

SC_MODULE(Generator) {
	ensitlm::target_socket<Generator> target;


	SC_HAS_PROCESS(Generator);
	Generator(sc_core::sc_module_name name, unsigned int size);

	~Generator();

private:


public:

};

#endif
