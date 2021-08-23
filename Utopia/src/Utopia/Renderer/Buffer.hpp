#pragma once
#include "Utopia/Core.hpp"

namespace Utopia
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:	return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:	return 4 * 3 * 3;
			case ShaderDataType::Mat4:	return 4 * 4 * 4;
			case ShaderDataType::Int:	return 4;
			case ShaderDataType::Int2:	return 4 * 2;
			case ShaderDataType::Int3:	return 4 * 3;
			case ShaderDataType::Int4:	return 4 * 4;
			case ShaderDataType::Bool:	return 1;
			default:
				utCoreAssert(false, "Unknown ShaderDataType.");
				return 0;
		}
	}

	struct BufferElement
	{
		ShaderDataType type;
		std::string name;
		unsigned offset{};
		int size{};
		bool normalized{};

		BufferElement() = default;
		BufferElement(ShaderDataType type, std::string name, bool normalized = false)
			: type(type), name(std::move(name)), offset(0), size(ShaderDataTypeSize(type)), normalized(false) {}

		int getElementCount() const
		{
			switch (type)
			{
				case ShaderDataType::Float:	return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:	return 3 * 3;
				case ShaderDataType::Mat4:	return 4 * 4;
				case ShaderDataType::Int:	return 1;
				case ShaderDataType::Int2:	return 2;
				case ShaderDataType::Int3:	return 3;
				case ShaderDataType::Int4:	return 4;
				case ShaderDataType::Bool:	return 1;
				default:
					utCoreAssert(false, "Unknown ShaderDataType.");
					return 0;
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) {
			calculateOffsetAndStride();
		}
		inline const int getStride() const { return m_Stride; }
		inline const auto& getElement() const { return m_Elements; }
		auto begin() { return m_Elements.begin(); }
		auto end() { return m_Elements.end(); }
		auto begin() const { return m_Elements.begin(); }
		auto end() const { return m_Elements.end(); }

	private:
		void calculateOffsetAndStride()
		{
			m_Stride = 0;
			int offset = 0;
			for (auto& element : m_Elements)
			{
				element.offset = offset;
				offset += element.size;
				m_Stride += element.size;
			}
		}

		std::vector<BufferElement> m_Elements;
		int m_Stride{};
	};

	class VertexBuffer
	{
	public:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& getLayout() const = 0;

		static VertexBuffer* create(float* vertices, unsigned size);
	};

	class IndexBuffer
	{
	public:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual int getCount() const = 0;
		static IndexBuffer* create(unsigned* indices, unsigned count);
	};

}