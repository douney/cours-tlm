
#include "generator.h"

using namespace std;
using namespace sc_core;

Generator::Generator(sc_module_name name) : sc_module(name) {
	SC_THREAD(thread);
}

void Generator::thread(void) {
	ensitlm::addr_t addr = 0x10000000;
	ensitlm::data_t d;

	for (ensitlm::data_t i = 0; i <= 0xFF / 4; ++i) {
		if (this->initiator.write(addr, i) == tlm::TLM_ADDRESS_ERROR_RESPONSE) {
			SC_REPORT_ERROR("TLM", "Adress Error");
		}
		addr += 4;
	}

	addr = 0x10000000;
	for (ensitlm::data_t i = 0; i <= 0xFF / 4; ++i) {
		if (this->initiator.read(addr, d) == tlm::TLM_ADDRESS_ERROR_RESPONSE) {
			SC_REPORT_ERROR("TLM", "Adress Error");
		}
		if (d != i) {
			SC_REPORT_ERROR("MEMORY", "Content error");
		}
		cout << "read : " << d << endl;
		addr += 4;
	}

}