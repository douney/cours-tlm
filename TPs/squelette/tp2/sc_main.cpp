
#include "bus.h"
#include "generator.h"
#include "ensitlm.h"
#include "memory.h"
#include "LCDC.h"
#include "const.h"


int sc_main(int argc, char **argv) {

	Generator generator("Generator1");
	Memory memory("Memory1", MEMORY_SIZE);
	Bus bus("Bus");
	LCDC lcd("LCD", sc_core::sc_time(1.0 / 25.0, sc_core::SC_SEC));
	sc_core::sc_signal<bool> sig("Signal");

	bus.map(memory.target, 0x10000000, BUS_SIZE);

	lcd.initiator_socket.bind(bus.target);
	bus.initiator.bind(lcd.target_socket);

	generator.display_int(sig);
	lcd.display_int(sig);

	generator.initiator.bind(bus.target);

	bus.initiator.bind(memory.target);
	

	
	sc_core::sc_start();
	return 0;
}