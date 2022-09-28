#include "cdpch.hpp"
#include "Cardia/Application.hpp"
#include "Cardia/Renderer/Renderer2D.hpp"
#include "Cardia/Renderer/RenderAPI.hpp"
#include "Cardia/Scripting/ScriptEngine.hpp"

#include <GLFW/glfw3.h>

namespace Cardia
{
	Application* Application::s_Instance = nullptr;

	Application::Application() : m_DeltaTime()
	{
		cdCoreAssert(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->setEventCallback([this](Event& e)
		{
			EventDispatcher dispatcher(e);
			dispatcher.dispatch<WindowCloseEvent>(CD_BIND_EVENT_FN(Application::onWinClose));
			m_ImGuiLayer->onEvent(e);
			onEvent(e);
		});

		m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		ScriptEngine::init();
	}

	Application::~Application()
	{
		ScriptEngine::shutdown();
	}

	void Application::Run()
	{
		RenderAPI::init();
		Renderer2D::init();

		float time = 0.0f;
		while (m_Running)
		{
			Time::m_DeltaTime = static_cast<float>(glfwGetTime()) - time;
			time += Time::m_DeltaTime.seconds();

			onUpdate();

			m_ImGuiLayer->Begin();
			onImGuiDraw();
			m_ImGuiLayer->End();

			m_Window->onUpdate();

		}
		Renderer2D::quit();
	}

	bool Application::onWinClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
