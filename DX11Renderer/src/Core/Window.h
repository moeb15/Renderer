#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <optional>
#include <memory>
#include "Core/Input.h"
#include "Renderer/Renderer.h"

namespace Yassin
{
	class Window
	{
	public:
		Window(int width, int height, wchar_t* name);
		~Window();
		
		void Init();
		void ToggleCursor();

		inline Renderer& GetRenderer() { return *m_Renderer; }
		inline Input& GetInput() { return m_Input; }

		static std::optional<int> ProcessMessages();

	private:
		static LRESULT CALLBACK MessageHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK MessageHandlerInt(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		int m_Width;
		int m_Height;
		wchar_t* m_Name;
		HWND m_HWND;
		HINSTANCE m_HINST;
		std::unique_ptr<Renderer> m_Renderer;
		bool m_Fullscreen;
		Input m_Input;
		BOOL m_CursorVisible = TRUE;
	};
}
