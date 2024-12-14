#include "Input.h"
#include <Windows.h>
#include <assert.h>

namespace Yassin
{
	Input* Input::s_Instance = nullptr;

	Input::Input()
	{
		assert(s_Instance == nullptr);
		s_Instance = this;
	}

	bool Input::IsKeyDown(unsigned char keycode)
	{
		return s_Instance->m_KeyStates[keycode];
	}

	Input::KeyEvent Input::ReadKey()
	{
		if (m_KeyBuffer.size() > 0)
		{
			Input::KeyEvent e = m_KeyBuffer.front();
			m_KeyBuffer.pop();
			return e;
		}
		return KeyEvent();
	}

	bool Input::IsKeyEmpty() const
	{
		return m_KeyBuffer.empty();;
	}

	void Input::FlushKey()
	{
		m_KeyBuffer = std::queue<Input::KeyEvent>();
	}

	char Input::ReadChar()
	{
		if (m_CharBuffer.size() > 0)
		{
			unsigned char charCode = m_CharBuffer.front();
			m_CharBuffer.pop();
			return charCode;
		}
		return 0;
	}

	bool Input::IsCharEmpty() const
	{
		return m_CharBuffer.empty();;
	}

	void Input::FlushChar()
	{
		m_CharBuffer = std::queue<char>();
	}

	void Input::FlushKeyEvents()
	{
		FlushChar();
		FlushKey();
	}

	void Input::EnableAutoRepeat()
	{
		autoRepeat = true;
	}

	void Input::DisableAutoRepeat()
	{
		autoRepeat = false;
	}

	bool Input::IsAutoRepeateEnabled() const
	{
		return autoRepeat;
	}

	std::pair<long, long> Input::GetPosRaw()
	{
		if(s_Instance->m_RawDeltas.size() > 0)
		{
			RawDelta rD = s_Instance->m_RawDeltas.front();
			s_Instance->m_RawDeltas.pop();
			return std::pair<long, long>(rD.x, rD.y);
		}
		return std::pair<long, long>(0, 0);
	}

	Input::MouseEvent Input::ReadMouse()
	{
		if (m_MouseBuffer.size() > 0)
		{
			Input::MouseEvent e = m_MouseBuffer.front();
			m_MouseBuffer.pop();
			return e;
		}
		return MouseEvent();
	}

	void Input::FlushMouse()
	{
		m_MouseBuffer = std::queue<MouseEvent>();
		m_RawDeltas = std::queue<RawDelta>();
	}

	void Input::OnMouseMove(int x, int y)
	{
		m_MouseX = x;
		m_MouseY = y;
		MouseEvent e(MouseEvent::Type::Move, lmbPress, rmbPress, x, y);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnMouseMoveRaw(long x, long y)
	{
		m_MouseXRaw = x;
		m_MouseYRaw = y;
		RawDelta rD;
		rD.x = x;
		rD.y = y;
		m_RawDeltas.push(rD);
		TrimBuffer<RawDelta>(m_RawDeltas);
	}

	void Input::OnMouseLeave()
	{
		inWindow = false;
		MouseEvent e(MouseEvent::Type::Leave, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnMouseEnter()
	{
		inWindow = true;
		MouseEvent e(MouseEvent::Type::Enter, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnLMBPressed(int x, int y)
	{
		m_MouseX = x;
		m_MouseY = y;
		lmbPress = true;
		MouseEvent e(MouseEvent::Type::LMBPress, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnLMBReleased(int x, int y)
	{
		m_MouseX = x;
		m_MouseY = y;
		lmbPress = false;
		MouseEvent e(MouseEvent::Type::LMBRelease, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnRMBPressed(int x, int y)
	{
		m_MouseX = x;
		m_MouseY = y;
		rmbPress = true;
		MouseEvent e(MouseEvent::Type::RMBPress, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnRMBReleased(int x, int y)
	{
		m_MouseX = x;
		m_MouseY = y;
		rmbPress = false;
		MouseEvent e(MouseEvent::Type::RMBRelease, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnWheelUp(int x, int y)
	{
		m_MouseX = x;
		m_MouseY = y;
		MouseEvent e(MouseEvent::Type::WheelUp, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnWheelDown(int x, int y)
	{
		m_MouseX = x;
		m_MouseY = y;
		MouseEvent e(MouseEvent::Type::WheelDown, lmbPress, rmbPress, m_MouseX, m_MouseY);
		m_MouseBuffer.push(e);
		TrimBuffer<MouseEvent>(m_MouseBuffer);
	}

	void Input::OnWheelDelta(int x, int y, int delta)
	{
		wheelDeltaCarry += delta;
		while (wheelDeltaCarry >= WHEEL_DELTA)
		{
			wheelDeltaCarry -= WHEEL_DELTA;
			OnWheelUp(x, y);
		}

		while (wheelDeltaCarry <= WHEEL_DELTA)
		{
			wheelDeltaCarry += WHEEL_DELTA;
			OnWheelUp(x, y);
		}
	}

	void Input::OnKeyPressed(unsigned char keycode)
	{
		m_KeyStates[keycode] = true;
		Input::KeyEvent e = Input::KeyEvent(KeyEvent::Type::Press, keycode);
		m_KeyBuffer.push(e);
		TrimBuffer<Input::KeyEvent>(m_KeyBuffer);
	}

	void Input::OnKeyReleased(unsigned char keycode)
	{
		m_KeyStates[keycode] = false;
		Input::KeyEvent e = Input::KeyEvent(KeyEvent::Type::Release, keycode);
		m_KeyBuffer.push(e);
		TrimBuffer<Input::KeyEvent>(m_KeyBuffer);
	}

	void Input::OnChar(char character)
	{
		m_CharBuffer.push(character);
		TrimBuffer<char>(m_CharBuffer);
	}

	void Input::ClearState()
	{
		m_KeyStates.reset();
	}

}