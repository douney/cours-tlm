#include "ensitlm.h"
#include "native_wrapper.h"

/*
 * The C compiler does a bit of magic on the main() function. Trick it
 * by changing the name to something else.
 */
#define main __start

/* extern "C" is needed since the software is compiled in C and
 * is linked against native_wrapper.cpp, which is compiled in C++.
 */
extern "C" int main();
extern "C" void interrupt_handler();

extern "C" void hal_write32(uint32_t addr, uint32_t data) {
	NativeWrapper *instance = NativeWrapper::get_instance();
	instance->hal_write32(addr, data);
}

extern "C" unsigned int hal_read32(uint32_t addr) {
	NativeWrapper *instance = NativeWrapper::get_instance();
	return instance->hal_read32(addr);
}

extern "C" void hal_cpu_relax() {
	NativeWrapper *instance = NativeWrapper::get_instance();
	instance->hal_cpu_relax();
}

extern "C" void hal_wait_for_irq() {
	NativeWrapper *instance = NativeWrapper::get_instance();
	instance->hal_wait_for_irq();
}

/* To keep it simple, the soft wrapper is a singleton, we can
 * call its methods in a simple manner, using
 * NativeWrapper::get_instance()->method_name()
 */
NativeWrapper * NativeWrapper::get_instance() {
	static NativeWrapper * instance = NULL;
	if (!instance)
		instance = new NativeWrapper("native_wrapper");
	return instance;
}

NativeWrapper::NativeWrapper(sc_core::sc_module_name name) 
	: sc_module(name)
	, irq("irq")
	, interrupt(false)
{
	SC_METHOD(interrupt_handler_internal);
	sensitive << irq;
}

void NativeWrapper::hal_write32(unsigned int addr, unsigned int data)
{
	socket.write(addr, data);
}

unsigned int NativeWrapper::hal_read32(unsigned int addr)
{
	unsigned int data;
	socket.read(addr, data);
	return data;
}

void NativeWrapper::hal_cpu_relax()
{
	wait(1, sc_core::SC_MS);
}

void NativeWrapper::hal_wait_for_irq()
{
	if (!interrupt) 
		wait(interrupt_event);
	interrupt = false;
}

void NativeWrapper::compute()
{
	abort(); // TODO
}

void NativeWrapper::interrupt_handler_internal()
{
	this->interrupt = true; 
	interrupt_event.notify();
	this->compute();
}
