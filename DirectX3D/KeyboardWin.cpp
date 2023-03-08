#include "KeyboardWin.h"

/// <summary>
/// Getting next key event from queue of events
/// </summary>
/// <returns>Next event from qeue</returns>
KeyEvent WinKeyboard::GetNextEvent() noexcept
{
	if (eventQueue_.size() > 0u)
	{
		KeyEvent event = eventQueue_.front();
		eventQueue_.pop();
		return event;
	}
	return KeyEvent();
}

/// <summary>
/// Gets next character from queue if the queue is not empty
/// </summary>
/// <returns>Next charcter from queue</returns>
char WinKeyboard::ReadNextChar() noexcept
{
	if (charQueue_.size() > 0u)
	{
		char character = charQueue_.front();
		charQueue_.pop();
		return character;
	}
	return 0;
};


void WinKeyboard::DeleteAll() noexcept
{
	DelChar();
	DelKey();
};

/// <summary>
/// If some key is pressed, then we update Event and Bitset buffer
/// </summary>
/// <param name="key">Pressed keyboard character</param>
void WinKeyboard::OnKeyPressEvent(unsigned char key) noexcept
{
	keybitset_[key] = true;
	KeyEvent event = KeyEvent(KeyState::PRESSED, key);
	eventQueue_.push(std::move(event));
	CutBuffer(eventQueue_);
};

/// <summary>
/// If some key is released, then we update Event and Bitset buffer
/// </summary>
/// <param name="key">Released keyboard charcter</param>
void WinKeyboard::OnKeyReleaseEvent(unsigned char key) noexcept
{
	keybitset_[key] = false;
	KeyEvent event = KeyEvent(KeyState::RELEASED, key);
	eventQueue_.push(std::move(event));
	CutBuffer(eventQueue_);
};

/// <summary>
///  Updating queue of characters
/// </summary>
/// <param name="ch">Character to be pushed</param>
void WinKeyboard::OnCharEvent(char ch) noexcept
{
	charQueue_.push(ch);
	CutBuffer(charQueue_);
};