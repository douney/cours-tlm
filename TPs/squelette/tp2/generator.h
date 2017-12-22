
#ifndef GENERATOR_H
#define GENERATOR_H

#include "ensitlm.h"

struct Generator : sc_core::sc_module {
	sc_core::sc_in<bool> irq;
	ensitlm::initiator_socket<Generator> initiator;
	bool irq_received;
	// Variable qui permet de gérer le défilement de l'image
	int decalage;

	void thread(void);
	void animation_thread(void);
	void irq_handler(void);

	SC_CTOR(Generator);
};

#endif