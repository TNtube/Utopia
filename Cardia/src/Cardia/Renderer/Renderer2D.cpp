#include "cdpch.hpp"
#include "Cardia/Renderer/Renderer2D.hpp"
#include "Cardia/Renderer/RenderAPI.hpp"
#include "Cardia/Renderer/Shader.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <numeric>


namespace Cardia
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoord;
		float textureIndex;
		float tilingFactor;
	};

	struct MeshIndices
	{
		std::vector<uint32_t> indices;
	};

	struct Renderer2DData {
		const uint32_t maxTriangle = 10000;
		const uint32_t maxVertices = maxTriangle * 3;
		const uint32_t maxIndices = maxTriangle * 3;
		static constexpr int maxTextureSlots = 32; // TODO: get it from RenderAPI

		glm::vec3 camPos {};

		std::unique_ptr<VertexArray> vertexArray;
		VertexBuffer* vertexBuffer = nullptr;
		IndexBuffer* indexBuffer = nullptr;
		std::unique_ptr<Shader> basicShader;
		std::unique_ptr<Texture2D> whiteTexture;

		std::vector<Vertex> vertexBufferData;
		uint32_t vertexIndexCount = 0;

		std::vector<MeshIndices> indexBufferData;
		uint32_t indexOffset {};

		std::array<const Texture2D*, maxTextureSlots> textureSlots {};
		int textureSlotIndex = 1;

		Renderer2D::Stats stats {};
	};

	static std::unique_ptr<Renderer2DData> s_Data;

	void Renderer2D::init()
	{
		s_Data = std::make_unique<Renderer2DData>();

		s_Data->vertexArray = VertexArray::create();

		std::unique_ptr<VertexBuffer> vbo;
		vbo = VertexBuffer::create(s_Data->maxVertices * sizeof(Vertex));

		vbo->setLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexPos"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"}
		});

		s_Data->vertexBuffer = vbo.get();
		s_Data->vertexArray->setVertexBuffer(std::move(vbo));

		s_Data->vertexBufferData = std::vector<Vertex>();
		s_Data->indexBufferData = std::vector<MeshIndices>();

		std::unique_ptr<IndexBuffer> ibo = IndexBuffer::create(s_Data->maxIndices);
		s_Data->indexBuffer = ibo.get();
		s_Data->vertexArray->setIndexBuffer(std::move(ibo));
		s_Data->indexOffset = 0;


		s_Data->basicShader = Shader::create({"assets/shaders/basic.vert", "assets/shaders/basic.frag"});

		uint32_t whiteColor = 0xffffffff;
		s_Data->whiteTexture = Texture2D::create(1, 1, &whiteColor);

		std::array<int, Renderer2DData::maxTextureSlots> samplers {};
		for (int32_t i = 0; i < Renderer2DData::maxTextureSlots; ++i)
		{
			samplers[i] = i;
		}

		s_Data->basicShader->bind();
		s_Data->basicShader->setIntArray("u_Textures", samplers.data(), Renderer2DData::maxTextureSlots);

		// Always white tex at pos 0
		s_Data->textureSlots[0] = s_Data->whiteTexture.get();
	}

	void Renderer2D::quit()
	{
		s_Data.reset();
	}

	void Renderer2D::beginScene(Camera& camera, glm::mat4& transform)
	{
		s_Data->basicShader->bind();
		s_Data->camPos = glm::vec3(transform[3]);
		s_Data->basicShader->setMat4("u_ViewProjection", camera.getProjectionMatrix() * glm::inverse(transform));
		s_Data->stats.drawCalls = 0;
		s_Data->stats.triangleCount = 0;
		startBash();
	}

	void Renderer2D::beginScene(Camera& camera, const glm::vec3& position)
	{
		s_Data->basicShader->bind();
		s_Data->camPos = position;
		s_Data->basicShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		s_Data->stats.drawCalls = 0;
		s_Data->stats.triangleCount = 0;
		startBash();
	}


	void Renderer2D::startBash()
	{
		s_Data->vertexIndexCount = 0;
		s_Data->indexOffset = 0;
		s_Data->vertexBufferData.clear();
		s_Data->indexBufferData.clear();
		s_Data->textureSlotIndex = 1;
	}

	void Renderer2D::nextBatch()
	{
		render();
		startBash();
	}

	void Renderer2D::endScene()
	{
		render();
	}

	void Renderer2D::render()
	{
		std::ranges::sort(s_Data->indexBufferData, [](const MeshIndices a, const MeshIndices b)
		{
			const auto lambda = [](const glm::vec3 va, const uint32_t ib)
			{
				return va + s_Data->vertexBufferData[ib].position;
			};
			const auto vertexA = std::accumulate(a.indices.begin(), a.indices.end(), glm::vec3(0), lambda) / 3.0f;
			const auto vertexB = std::accumulate(b.indices.begin(), b.indices.end(), glm::vec3(0), lambda) / 3.0f;
			return glm::distance(vertexA, s_Data->camPos) >= glm::distance(vertexB, s_Data->camPos);
		});

		std::vector<uint32_t> indexBuffer;

		for (const auto& object: s_Data->indexBufferData)
		{
			for (const auto index: object.indices)
			{
				indexBuffer.push_back(index);
			}
		}

		s_Data->vertexBuffer->setData(s_Data->vertexBufferData.data(), static_cast<int>(indexBuffer.size()) * sizeof(Vertex));
		s_Data->indexBuffer->setData(indexBuffer.data(), static_cast<int>(indexBuffer.size()) * sizeof(uint32_t));

		s_Data->basicShader->bind();
		for (int i = 0; i < s_Data->textureSlotIndex; ++i)
		{
			s_Data->textureSlots[i]->bind(i);
		}

		RenderAPI::get().drawIndexed(s_Data->vertexArray.get(), s_Data->vertexIndexCount);
		s_Data->stats.drawCalls++;
	}

	Renderer2D::Stats& Renderer2D::getStats()
	{
		return s_Data->stats;
	}

	void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4 &color)
	{
		drawRect(position, size, nullptr, color);
	}

	void Renderer2D::drawRect(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
	{
		drawRect(position, size, rotation, nullptr, color);
	}

	void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const Texture2D* texture, float tilingFactor)
	{
		drawRect(position, size, texture, glm::vec4(1.0f), tilingFactor);
	}

	void Renderer2D::drawRect(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Texture2D *texture, float tilingFactor)
	{
		drawRect(position, size, rotation, texture, glm::vec4(1.0f), tilingFactor);
	}

	void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const Texture2D* texture, const glm::vec4 &color, float tilingFactor)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				      * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		drawRect(transform, texture, color, tilingFactor);
	}

	void Renderer2D::drawRect(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Texture2D *texture, const glm::vec4 &color, float tilingFactor)
	{
		const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				      * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f})
				      * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		drawRect(transform, texture, color, tilingFactor);
	}

	void Renderer2D::drawRect(const glm::mat4 &transform, const glm::vec4 &color)
	{
		drawRect(transform, nullptr, color);
	}

	void Renderer2D::drawRect(const glm::mat4 &transform, const Texture2D *texture, const glm::vec4 &color, float tilingFactor)
	{
		if (s_Data->vertexIndexCount >= s_Data->maxIndices)
			nextBatch();

		float textureIndex = 0;
		for(int i = 1; i < s_Data->textureSlotIndex; ++i) {
			if (texture && *s_Data->textureSlots[i] == *texture) {
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0 && texture) {
			if (s_Data->textureSlotIndex >= Renderer2DData::maxTextureSlots)
				nextBatch();

			s_Data->textureSlots[s_Data->textureSlotIndex] = texture;
			textureIndex = static_cast<float>(s_Data->textureSlotIndex);
			s_Data->textureSlotIndex++;
		}

		constexpr glm::vec2 texCoords[] {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		constexpr glm::vec4 rectPositions[]
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f },
		};

		for (int i = 0; i < 4; ++i)
		{
			s_Data->vertexBufferData.emplace_back();
			auto& vertex = s_Data->vertexBufferData.back();
			vertex.position = transform * rectPositions[i];
			vertex.color = color;
			vertex.textureCoord = texCoords[i];
			vertex.textureIndex = textureIndex;
			vertex.tilingFactor = tilingFactor;
		}

		constexpr uint32_t triangle_indices[] { 0, 1, 2, 2, 3, 0 };

		s_Data->indexBufferData.emplace_back();
		for (const auto& index : triangle_indices)
		{
			s_Data->indexBufferData.back().indices.push_back(s_Data->indexOffset + index);
		}
		
		s_Data->indexOffset += 4;
		s_Data->vertexIndexCount += 6;
		s_Data->stats.triangleCount += 2;
	}
}