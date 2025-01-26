#include "Core/Window.h"
#include <iostream>
#include "imgui.h"
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
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(m_HWND);

		LOGICAL_PROCESSOR_RELATIONSHIP rel = RelationProcessorCore;
		DWORD len = 0;
		GetLogicalProcessorInformationEx(rel, nullptr, &len);
		if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			std::cerr << "Error determining buffer size.\n";
			return;
		}

		std::vector<char> buffer(len);

		if(!GetLogicalProcessorInformationEx(
			rel, 
			reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data()),
			&len))
		{
			std::cerr << "Error retreiving processor information.\n";
			return;
		}

		unsigned int numCores = 0;
		char* ptr = buffer.data();
		while(ptr < buffer.data() + len)
		{
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);
			if(info->Relationship == RelationProcessorCore)
			{
				numCores++;
			}
			ptr += info->Size;
		}

		m_Renderer = std::make_unique<Renderer>();
		m_Renderer->Init(m_Width, m_Height, m_HWND, m_Fullscreen, numCores);

		static bool raw_input_initialized = false;
		if(!raw_input_initialized)
		{
			RAWINPUTDEVICE rid;
			rid.usUsagePage = 0x01;
			rid.usUsage = 0x02;
			rid.dwFlags = 0;
			rid.hwndTarget = 0;

			if(!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			{
				DWORD error = GetLastError();
				return;
			}
			raw_input_initialized = true;
		}
	}

	void Window::ToggleCursor()
	{
		SetCursorPos(m_Width / 2, m_Height / 2);
		m_CursorVisible = !m_CursorVisible;
		ShowCursor(m_CursorVisible);
	}

	std::optional<int> Window::ProcessMessages()
	{
		MSG msg;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) return (int)msg.wParam;

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
				if(wParam == VK_F1)
				{
					m_Input.ToggleProcessRawMouseMove();
					ToggleCursor();
				}
				if (wParam == VK_F2)
				{
					RendererContext::ToggleRasterizerState();
				}
				if (wParam == VK_F3)
				{
					m_Renderer->TogglePostProcessing();
				}
				if (wParam == VK_F4)
				{
					m_Renderer->ToggleBoundingVolumes();
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
				if (ImGui::GetIO().WantCaptureMouse && m_CursorVisible) break;
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

			case WM_INPUT:
			{
				UINT dataSize = 0;
				GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
					RID_INPUT,
					NULL,
					&dataSize,
					sizeof(RAWINPUTHEADER));

				if(dataSize > 0)
				{
					std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
					if(GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam),
						RID_INPUT,
						rawData.get(),
						&dataSize,
						sizeof(RAWINPUTHEADER)) == dataSize)
					{
						RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
						if(raw->header.dwType == RIM_TYPEMOUSE)
						{ 
							m_Input.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
						}
					}
				}
				break;
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}