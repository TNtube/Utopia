#include "cdpch.hpp"
#include "Cardia/Renderer/Buffer.hpp"
#include "Cardia/Core/Log.hpp"


namespace Cardia
{
	Buffer::Buffer(Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		: m_Device(device)
	{
		VkBufferCreateInfo bufferInfo {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		const auto& vkDevice = m_Device.GetDevice();

		if (vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS)
			throw std::runtime_error("Vulkan : failed to create vertex buffer !");

		VkMemoryRequirements memoryRequirements {};
		vkGetBufferMemoryRequirements(vkDevice, m_Buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;
		allocateInfo.memoryTypeIndex = m_Device.FindMemoryType(memoryRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(vkDevice, &allocateInfo, nullptr, &m_BufferMemory) != VK_SUCCESS)
			throw std::runtime_error("Vulkan : Failed to allocate vertex buffer memory !");

		vkBindBufferMemory(vkDevice, m_Buffer, m_BufferMemory, 0);
	}

	Buffer::Buffer(Buffer&& other) noexcept : m_Device(other.m_Device)
	{
		m_Buffer = other.m_Buffer;
		m_BufferMemory = other.m_BufferMemory;
		other.m_Buffer = VK_NULL_HANDLE;
		other.m_BufferMemory = VK_NULL_HANDLE;
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		m_Buffer = other.m_Buffer;
		m_BufferMemory = other.m_BufferMemory;
		other.m_Buffer = VK_NULL_HANDLE;
		other.m_BufferMemory = VK_NULL_HANDLE;
		return *this;
	}

	void Buffer::UploadData(size_t size, const void* data) const
	{
		const auto& device = m_Device.GetDevice();
		void* memory;
		vkMapMemory(device, m_BufferMemory, 0, size, 0, &memory);
		memcpy(memory, data, size);
		vkUnmapMemory(device, m_BufferMemory);
	}

	Buffer::~Buffer()
	{
		const auto& device = m_Device.GetDevice();
		vkDestroyBuffer(device, m_Buffer, nullptr);
		vkFreeMemory(device, m_BufferMemory, nullptr);
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::create(uint32_t size)
	{
		return nullptr;
	}

	std::unique_ptr<IndexBuffer> IndexBuffer::create(uint32_t *indices, uint32_t count)
	{
		return nullptr;
	}

	std::unique_ptr<IndexBuffer> IndexBuffer::create(uint32_t count)
	{
		return nullptr;
	}

	std::unique_ptr<StorageBuffer> StorageBuffer::create(uint32_t size)
	{
		return nullptr;
	}

	std::unique_ptr<StorageBuffer> StorageBuffer::create(void *data, uint32_t size)
	{
		return nullptr;
	}

	std::unique_ptr<UniformBuffer> UniformBuffer::create(uint32_t size)
	{
		return nullptr;
	}

	std::unique_ptr<UniformBuffer> UniformBuffer::create(void *data, uint32_t size)
	{
		return nullptr;
	}
}
