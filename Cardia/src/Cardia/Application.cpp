#include "cdpch.hpp"
#include "Cardia/Application.hpp"
#include "Cardia/Renderer/Renderer2D.hpp"
#include "Cardia/Scripting/ScriptEngine.hpp"

#include <GLFW/glfw3.h>
#include <Cardia/Project/AssetsManager.hpp>

namespace Cardia
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		cdCoreAssert(!s_Instance, "Application already exists");
		s_Instance = this;
		m_Window->setEventCallback([this](Event& e)
		{
			EventDispatcher dispatcher(e);
			dispatcher.dispatch<WindowCloseEvent>(CD_BIND_EVENT_FN(Application::onWinClose));
			m_ImGuiLayer->OnEvent(e);
			OnEvent(e);
		});

		m_ImGuiLayer = std::make_unique<ImGuiLayer>(m_Renderer);
	}

	Application::~Application() = default;

	void Application::Run()
	{
		Renderer2D::init();

		float time = 0.0f;
		while (m_Running)
		{
			Time::m_DeltaTime = static_cast<float>(glfwGetTime()) - time;
			time += Time::m_DeltaTime.seconds();
			m_Window->onUpdate();

			OnUpdate();

			if (const auto commandBuffer = m_Renderer.Begin())
			{
				m_Renderer.BeginRenderPass(m_Renderer.GetSwapChain().GetRenderPass());

				OnRender(commandBuffer);

				m_Renderer.EndRenderPass();


				m_ImGuiLayer->Begin();

				OnImGuiDraw();
				m_ImGuiLayer->Render(commandBuffer);

				m_ImGuiLayer->End();


				m_Renderer.End();
			}

			AssetsManager::Instance().CollectionRoutine(Time::m_DeltaTime);

		}
		Renderer2D::quit();
	}

	bool Application::onWinClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
