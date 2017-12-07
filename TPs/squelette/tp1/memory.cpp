
#include "memory.h"

using namespace std;

Memory::Memory(sc_core::sc_module_name name, ensitlm::data_t size) 
	: sc_module(name), size(size) {
		storage = new ensitlm::data_t[size];
	}

Memory::~Memory() {
	delete [] storage;
}

tlm::tlm_response_status Memory::write(const ensitlm::addr_t &a,
                                       const ensitlm::data_t &d) {

	if (a / 4 > this->size) {
		return tlm::TLM_ADDRESS_ERROR_RESPONSE ;
	}

	cout << name() << " (write) " <<  ": " << d << " at @" << a << endl;
	this->storage[a / 4] = d;
	return tlm::TLM_OK_RESPONSE;
}

tlm::tlm_response_status Memory::read(const ensitlm::addr_t &a,
                                      /* */ ensitlm::data_t &d) {
	d = this->storage[a / 4];
	return tlm::TLM_OK_RESPONSE;
}