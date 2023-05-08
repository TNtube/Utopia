#include "cdpch.hpp"
#include "Cardia/Renderer/Renderer.hpp"

#include "Cardia/Core/Window.hpp"


namespace Cardia
{
	std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

	void Renderer::beginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const VertexArray* vertexArray, Shader* shader, const glm::mat4& transform)
	{
		shader->bind();
		shader->setMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->setMat4("u_Model", transform);
		shader->setMat3("u_TransposedInvertedModel", glm::transpose(glm::inverse(glm::mat3(transform))));

		vertexArray->bind();
		RenderAPI::get().drawIndexed(vertexArray);
	}
}

