
#include "bus.h"
#include "generator.h"
#include "memory.h"
#include "ensitlm.h"



int sc_main(int argc, char **argv) {

	Generator generator("Generator1");
	Memory memory("Memory", 0xFF / 4);
	Bus bus("Bus");

	bus.map(memory.target, 0x10000000, 0xFF);

	generator.initiator.bind(bus.target);
	bus.initiator.bind(memory.target);

	
	sc_core::sc_start();
	return 0;
}