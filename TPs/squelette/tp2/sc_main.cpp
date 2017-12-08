
#include "bus.h"
#include "generator.h"
#include "ensitlm.h"
#include "memory.h"
#include "LCDC.h"
#include "const.h"
#include "ROM.h"


int sc_main(int argc, char **argv) {

	Generator generator("Generator1");
	Memory memory("Memory1", MEMORY_SIZE);
	Bus bus("Bus");
	LCDC lcd("LCD", sc_core::sc_time(1.0 / 25.0, sc_core::SC_SEC));
	ROM rom("Rom");
	sc_core::sc_signal<bool> sig("Signal");

	bus.map(memory.target, MEMORY_BASE, MEMORY_SIZE);
	bus.map(lcd.target_socket, LCD_REG_BASE, LCD_REG_SIZE);
	bus.map(rom.socket, ROM_BASE, ROM_SIZE);

	lcd.initiator_socket.bind(bus.target);
	bus.initiator.bind(lcd.target_socket);
	bus.initiator.bind(rom.socket);

	generator.display_int(sig);
	lcd.display_int(sig);

	generator.initiator.bind(bus.target);

	bus.initiator.bind(memory.target);
		
	sc_core::sc_start();
	return 0;
}