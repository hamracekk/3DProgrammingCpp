#include "WinMouse.h"

/// <summary>
/// Gets next event from queue of mouse events
/// </summary>
/// <returns>events from queue</returns>
MouseEvent WinMouse::GetNextEvent() noexcept
{
	if (eventQueue_.size() > 0)
	{
		MouseEvent event = eventQueue_.front();
		eventQueue_.pop();
		return event;
	}
	return MouseEvent(); // NOSTATE event in default constructor
};

/// <summary>
/// Creates MOVE mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnMouseMoveEvent(int x_pos, int y_pos) noexcept
{
	x_position_ = x_pos;
	y_position_ = y_pos;
	MouseEvent event(MouseState::MOUSEMOVE, x_pos, y_pos, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
};

/// <summary>
/// Creates LEFTPRESSED mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnLeftPressedEvent(int x_pos, int y_pos) noexcept
{
	left_pressed_ = true;
	MouseEvent event(MouseState::LEFTPRESSED, x_pos, y_pos, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
};

/// <summary>
/// Creates MOVE mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnRightPressedEvent(int x_pos, int y_pos) noexcept
{
	right_pressed_ = true;
	MouseEvent event(MouseState::RIGHTPRESSED, x_pos, y_pos, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
}

/// <summary>
/// Creates LEFTRELEASED mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnLeftReleaseEvent(int x_pos, int y_pos) noexcept
{
	left_pressed_ = false;
	MouseEvent event(MouseState::LEFTRELEASED, x_pos, y_pos, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
}

/// <summary>
/// Creates RIGHTRELEASED mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnRightReleaseEvent(int x_pos, int y_pos) noexcept
{
	right_pressed_ = false;
	MouseEvent event(MouseState::RIGHTRELEASED, x_position_, y_position_, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
}

/// <summary>
/// Creates SCROLLUP mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnScrollUpEvent(int x_pos, int y_pos) noexcept
{
	MouseEvent event(MouseState::SCROLLUP, x_position_, y_position_, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
}

/// <summary>
/// Creates SCROLLDOWN mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnScrollDownEvent(int x_pos, int y_pos) noexcept
{
	MouseEvent event(MouseState::SCROLLDOWN, x_position_, y_position_, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
};

/// <summary>
/// Creates ENTERWINDOW mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnEnterWindowEvent() noexcept
{
	in_window_ = true;
	MouseEvent event(MouseState::ENTERWINDOW, x_position_, y_position_, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
}

/// <summary>
/// Creates LEAVEWINDOW mouse event and enqueue it to queue of events
/// </summary>
/// <param name="x_pos">Current x position of mouse</param>
/// <param name="y_pos">Current y position of mouse</param>
void WinMouse::OnLeaveWindowEvent() noexcept
{
	in_window_ = false;
	MouseEvent event(MouseState::LEAVEWINDOW, x_position_, y_position_, left_pressed_, right_pressed_);
	eventQueue_.push(std::move(event));
	CutBuffer();
}

/// <summary>
/// Trins the buffer if it overflows the fixed maximum size
/// </summary>
void WinMouse::CutBuffer()
{
	while (eventQueue_.size() > buffer_size_)
		eventQueue_.pop();
}
