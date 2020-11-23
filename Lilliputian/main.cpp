#include "Engine.hpp"

#undef main

int main(int argc, char* argv[])
{
	Lilliputian::Engine lilliputian(argc, argv);
	lilliputian.run();
	return 0;
}