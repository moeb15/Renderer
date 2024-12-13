#pragma once
#include <queue>
#include <bitset>

namespace Yassin
{
	class Input
	{
		friend class Window;

	private:
		class MouseEvent
		{
		public:
			enum class Type
			{
				LMBPress,
				LMBRelease,
				RMBPress,
				RMBRelease,
				WheelUp,
				WheelDown,
				Move,
				Enter,
				Leave,
				Invalid
			};

		private:
			Type m_Type;
			bool m_LMB;
			bool m_RMB;
			int x;
			int y;

		public:

			Type GetType() const
			{
				return m_Type;
			}

			bool LeftMousePress() const
			{
				return m_Type == Type::LMBPress;
			}

			bool LeftMouseRelease() const
			{
				return m_Type == Type::LMBRelease;
			}

			bool RightMousePress() const
			{
				return m_Type == Type::RMBPress;
			}

			bool RightMouseRelease() const
			{
				return m_Type == Type::RMBRelease;
			}

			bool MouseWheelUp() const
			{
				return m_Type == Type::WheelUp;
			}

			bool MouseWheelDown() const
			{
				return m_Type == Type::WheelDown;
			}

			bool MouseMove() const
			{
				return m_Type == Type::Move;
			}

			bool MouseEnter() const
			{
				return m_Type == Type::Enter;
			}

			bool MouseLeave() const
			{
				return m_Type == Type::Leave;
			}

			bool IsValid() const
			{
				return m_Type != Type::Invalid;
			}

			int GetPosX() const
			{
				return x;
			}

			int GetPosY() const
			{
				return y;
			}

			std::pair<int, int> GetPosition() const
			{
				return std::make_pair(x, y);
			}

		public:
			MouseEvent() :
				m_Type(Type::Invalid),
				m_LMB(false),
				m_RMB(false),
				x(0),
				y(0)
			{}


			MouseEvent(Type type, bool lmb, bool rmb, int xpos, int ypos) :
				m_Type(type),
				m_LMB(lmb),
				m_RMB(rmb),
				x(xpos),
				y(ypos)
			{}
		};

		class KeyEvent
		{
		public:
			enum class Type
			{
				Press,
				Release,
				Invalid
			};

		private:
			Type m_Type;
			unsigned char m_KeyCode;

		public:
			KeyEvent() :
				m_Type(Type::Invalid),
				m_KeyCode(0)
			{}

			KeyEvent(Type type, unsigned char code) :
				m_Type(type),
				m_KeyCode(code)
			{}

			bool IsPress() const
			{
				return m_Type == Type::Press;
			}

			bool IsRelease() const
			{
				return m_Type == Type::Release;
			}

			bool IsValid() const
			{
				return m_Type != Type::Invalid;
			}

			unsigned char GetCode() const
			{
				return m_KeyCode;
			}

		};

	public:
		Input() = default;

		// Key Event
		bool IsKeyDown(unsigned char keycode) const;
		KeyEvent ReadKey();
		bool IsKeyEmpty() const;
		void FlushKey();

		// char event stuff
		char ReadChar();
		bool IsCharEmpty() const;
		void FlushChar();
		void FlushKeyEvents();

		// auto repeat control
		void EnableAutoRepeat();
		void DisableAutoRepeat();
		bool IsAutoRepeateEnabled() const;

		// Mouse event
		std::pair<int, int> GetPos() const { return std::pair<int, int>(m_MouseX, m_MouseY); }
		int GetPosY() const { return m_MouseY; }
		int GetPosX() const { return m_MouseX; }
		bool LMBPressed() const { return lmbPress; }
		bool RMBPressed() const { return rmbPress; }
		bool IsInWindow() const { return inWindow; }
		MouseEvent ReadMouse();
		bool IsEmpty() const { return m_MouseBuffer.empty(); }

		void FlushMouse();

	private:
		void OnMouseMove(int x, int y);
		void OnMouseLeave();
		void OnMouseEnter();
		void OnLMBPressed(int x, int y);
		void OnLMBReleased(int x, int y);
		void OnRMBPressed(int x, int y);
		void OnRMBReleased(int x, int y);
		void OnWheelUp(int x, int y);
		void OnWheelDown(int x, int y);
		void OnWheelDelta(int x, int y, int delta);

	private:
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(char character);
		void ClearState();

		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);

	private:
		static constexpr unsigned int nKeys = 256;
		static constexpr unsigned int bufferSize = 16;
		bool autoRepeat = false;
		std::bitset<nKeys> m_KeyStates;
		std::queue<KeyEvent> m_KeyBuffer;
		std::queue<char> m_CharBuffer;

		int m_MouseX;
		int m_MouseY;
		bool inWindow = false;
		bool lmbPress = false;
		bool rmbPress = false;
		int wheelDeltaCarry = 0;
		std::queue<MouseEvent> m_MouseBuffer;
	};

	template<typename T>
	inline void Input::TrimBuffer(std::queue<T>& buffer)
	{
		if (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}

}
