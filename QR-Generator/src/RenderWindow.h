#pragma once
#include <array>
#include <utility>
#include <functional>

#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"

#define IMGUI_WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar

extern bool g_WindowResized;

class RenderWindow
{
public:
	GLFWwindow* m_Window = nullptr;
private:
	inline void Swap()       const noexcept { glfwSwapBuffers(m_Window);    }
	inline void Clear()      const noexcept { glClear(GL_COLOR_BUFFER_BIT); }
	inline void PollEvents() const noexcept { glfwPollEvents();             }
	inline void WaitEvents() const noexcept { glfwWaitEvents();             }

	bool ImGuiInit(const char* iniFileName = nullptr) const noexcept;
	void ImGuiStartFrame() const noexcept;
	void ImGuiRender() const noexcept;
	void ImGuiSetTheme() const noexcept;
public:
	RenderWindow(int width = 1600, int height = 920, const char* title = "Default", GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL) noexcept;
	~RenderWindow() noexcept;

	// loop
	inline bool IsOpen()     const noexcept { return !glfwWindowShouldClose(m_Window); }
	inline void StartFrame() const noexcept { ImGuiStartFrame(); Clear();              }
	inline void EndFrame()   const noexcept { ImGuiRender(); PollEvents(); Swap();     }
	inline ImVec2 Pos()  const noexcept { int x = 0, y = 0; glfwGetWindowPos(m_Window, &x, &y); return { (float)x, (float)y }; }
	inline ImVec2 Size() const noexcept { const ImGuiIO& io = ImGui::GetIO(); return { io.DisplaySize.x, io.DisplaySize.y };   }
	void Show() noexcept;
};