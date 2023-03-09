#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "WindowWrapper.h"
#include <chrono>

using namespace std::chrono;

/// <summary>
/// Class wrapping application logic
/// </summary>
class Application
{
public:
	//Constructing window inside of the application
	inline Application() : window(500, 500, "Some good stuff is here"), 
		                   previous_time_stamp_(steady_clock::now()) {};
	int Run(); // Start application loop
private:
	void ProcessFrame();
	float GetTimeFromLast();
	float GetTimeFromLastWithoutUpdate();
private:
	Window window;
	steady_clock::time_point previous_time_stamp_;
};

#endif // !APPLICATION_H_

