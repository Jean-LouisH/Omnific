#include "API.hpp"

void Lilliputian::API::run()
{
	this->lilliputian.initialize();
	this->lilliputian.run();
	this->lilliputian.shutdown();
}