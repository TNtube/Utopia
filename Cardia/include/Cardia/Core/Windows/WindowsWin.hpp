#pragma once

#include "Cardia/Core/Window.hpp"
#include "Cardia/Renderer/Renderer.hpp"

#include <GLFW/glfw3.h>


namespace Cardia
{
	struct WinData
	{
		std::string Title;
		int Width, Height;
		bool VSync = false;
		bool ShouldInvalidateSwapchain = false;
		bool ShouldUpdateFullscreenMode = false;
		bool IsFullscreen = false;
		bool HasFocus = true;

		std::function<void(Event&)> EventCallback;
	};

	class WindowsWin : public Window
	{
	public:
		explicit WindowsWin(const WinProperties& properties);
		~WindowsWin() override;

		void OnUpdate() override;
		inline int GetWidth() const override { return m_Data.Width; }
		inline int GetHeight() const override { return m_Data.Height; }
		inline Vector2i GetSize() const override { return {m_Data.Width, m_Data.Height}; }
		inline VkExtent2D GetExtent() const override { return VkExtent2D {static_cast<unsigned>(m_Data.Width), static_cast<unsigned>(m_Data.Height)}; }

		inline bool ShouldInvalidateSwapchain() const override { return m_Data.ShouldInvalidateSwapchain; }
		inline void SwapchainInvalidated() override { m_Data.ShouldInvalidateSwapchain = false; }


		inline void SetEventCallback(const std::function<void(Event&)>& callback) override { m_Data.EventCallback = callback; }

		void SetFullscreen(bool state) override;
		void UpdateFullscreenMode() override;
		bool IsFullscreen() const override;
		bool IsFocused() const override;

		void SetVSync(bool state) override;
		bool IsVSync() const override;

		inline void* GetNativeWindow() const override { return m_Window; }

	private:
		void Init(const WinProperties& properties);
		void Quit();
		
		static bool s_isGlfwInit; // Can only initialize glfw once.
		GLFWwindow* m_Window;
		WinData m_Data;
	};
}
