#include <chrono>
#include <iostream>
#include "timer.h"

timer::timer()
{
	start = std::chrono::high_resolution_clock::now();
}

timer::~timer()
{
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	std::cout << "Process duration =  " << duration.count() << " second" << std::endl;
}
