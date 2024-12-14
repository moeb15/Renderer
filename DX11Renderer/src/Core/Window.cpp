#include "Core/Window.h"
#include <iostream>
#include "imgui.h"
#define IMGUI_IMPL_API
#include <backends/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);                // Use ImGui::GetCurrentContext()

namespace Yassin
{
	Window::Window(int width, int height, wchar_t* name) :
		m_Width(width),
		m_Height(height),
		m_Name(name)
	{
	}

	Window::~Window()
	{
		ImGui_ImplWin32_Shutdown();
		DestroyWindow(m_HWND);
	}

	void Window::Init()
	{
		m_HINST = GetModuleHandle(NULL);
		WNDCLASSEX wc = {};
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = MessageHandlerSetup;
		wc.hInstance = m_HINST;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszClassName = m_Name;
		wc.lpszMenuName = nullptr;
		wc.cbSize = sizeof(WNDCLASSEX);

		if (!RegisterClassEx(&wc)) return;

		m_Fullscreen = (m_Width == GetSystemMetrics(SM_CXSCREEN) && m_Height == GetSystemMetrics(SM_CYSCREEN));

		m_HWND = CreateWindowEx(
			WS_EX_APPWINDOW,
			m_Name,
			L"DX11Renderer",
			WS_CLIPCHILDREN | WS_POPUP | WS_CLIPSIBLINGS,
			0,
			0,
			m_Width,
			m_Height,
			nullptr,
			nullptr,
			m_HINST,
			this);

		if (!m_HWND)
		{
			DWORD error = GetLastError();
			std::cerr << error << std::endl;
			return;
		}

		BOOL res = ShowWindow(m_HWND, SW_SHOW);
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(m_HWND);

		m_Renderer = std::make_unique<Renderer>();
		m_Renderer->Init(m_Width, m_Height, m_HWND, m_Fullscreen);
	}

	std::optional<int> Window::ProcessMessages()
	{
		MSG msg;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) return msg.wParam;

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		return {};
	}

	LRESULT Window::MessageHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWindow = static_cast<Window*>(pCreate->lpCreateParams);

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::MessageHandlerInt));

			return pWindow->MessageHandler(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::MessageHandlerInt(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWindow->MessageHandler(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

		switch (msg)
		{
			case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}

			case WM_KEYDOWN:
			{
				if (ImGui::GetIO().WantCaptureKeyboard) break;

				if (wParam == VK_ESCAPE)
				{
					PostQuitMessage(0);
					return 0;
				}

				m_Input.OnKeyPressed(static_cast<unsigned char>(wParam));
				break;
			}

			case WM_SYSKEYDOWN:
			{
				if (ImGui::GetIO().WantCaptureKeyboard) break;

				m_Input.OnKeyPressed(static_cast<unsigned char>(wParam));
				break;
			}

			case WM_KEYUP:
			{
				m_Input.OnKeyReleased(static_cast<unsigned char>(wParam));
				break;
			}

			case WM_CHAR:
			{
				m_Input.OnChar(static_cast<unsigned char>(wParam));
				break;
			}

			case WM_MOUSEMOVE:
			{
				if (ImGui::GetIO().WantCaptureMouse) break;
				const POINTS pt = MAKEPOINTS(lParam);

				// in client region -> log move, and log enter + capture mouse
				if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
				{
					m_Input.OnMouseMove(pt.x, pt.y);
					if (!m_Input.IsInWindow())
					{
						SetCapture(m_HWND);
						m_Input.OnMouseEnter();
					}
				}
				// not in client -> log move / maintain capture if button down
				else
				{
					if (wParam & (MK_LBUTTON | MK_RBUTTON))
					{
						m_Input.OnMouseMove(pt.x, pt.y);
					}
					// button up -> release capture / log event for leaving
					else
					{
						ReleaseCapture();
						m_Input.OnMouseLeave();
					}
				}

				break;
			}

			case WM_LBUTTONDOWN:
			{
				if (ImGui::GetIO().WantCaptureMouse) break;

				const POINTS pt = MAKEPOINTS(lParam);
				m_Input.OnLMBPressed(pt.x, pt.y);
				break;
			}

			case WM_RBUTTONDOWN:
			{
				if (ImGui::GetIO().WantCaptureMouse) break;

				const POINTS pt = MAKEPOINTS(lParam);
				m_Input.OnRMBPressed(pt.x, pt.y);
				break;
			}

			case WM_LBUTTONUP:
			{
				if (ImGui::GetIO().WantCaptureMouse) break;

				const POINTS pt = MAKEPOINTS(lParam);
				m_Input.OnLMBReleased(pt.x, pt.y);
				break;
			}

			case WM_RBUTTONUP:
			{
				if (ImGui::GetIO().WantCaptureMouse) break;

				const POINTS pt = MAKEPOINTS(lParam);
				m_Input.OnRMBReleased(pt.x, pt.y);
				break;
			}


			case WM_MOUSEWHEEL:
			{
				if (ImGui::GetIO().WantCaptureMouse) break;

				const POINTS pt = MAKEPOINTS(lParam);
				const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				m_Input.OnWheelDelta(pt.x, pt.y, delta);
				break;
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}