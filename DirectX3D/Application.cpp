#include "Application.h"
#include <iomanip>

/// <summary>
/// Wrpas the message processing and whole aplication loop
/// </summary>
/// <returns>exit code</returns>
int Application::Run()
{
	while (true)
	{
		//Message processing without bocking running application whene there is no message in queue
		std::optional<int> code = Window::ProcessMessageLoop();
		if(code.has_value())
			return code.value();
		ProcessFrame(); // update frame in loop
	}
}

/// <summary>
/// Function processing one frame in graphics
/// </summary>
void Application::ProcessFrame()
{
	float frame_duration = GetTimeFromLastWithoutUpdate();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << frame_duration << " !";
	SetWindowTextA(window.handle_, oss.str().c_str());
}

/// <summary>
/// Conting time from last frame with updating previous_time_stamp to current one
/// </summary>
/// <returns>duration of the frame</returns>
float Application::GetTimeFromLast()
{
	duration<float> frame_time_stamp = steady_clock::now() - previous_time_stamp_;
	previous_time_stamp_ = steady_clock::now();
	return frame_time_stamp.count();
}

/// <summary>
/// Conting time from last frame without updating previous_time_stamp to current one
/// </summary>
/// <returns>duration of the frame</returns>
float Application::GetTimeFromLastWithoutUpdate()
{
	duration<float> frame_time_stamp = steady_clock::now() - previous_time_stamp_;
	return frame_time_stamp.count();
}
