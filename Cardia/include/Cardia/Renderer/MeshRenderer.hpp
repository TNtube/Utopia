#pragma once

#include <vector>
#include <memory>
#include "Cardia/DataStructure/Mesh.hpp"
#include "SubMeshRenderer.hpp"


namespace Cardia
{
	class MeshRenderer
	{
	public:
		MeshRenderer() = default;

		void SubmitMesh(const Device& device, const std::shared_ptr<Mesh>& mesh);
		std::shared_ptr<Mesh> GetMesh() { return m_Mesh; }
		void Draw(VkCommandBuffer commandBuffer) const;
	private:
		std::vector<SubMeshRenderer> m_SubMeshRenderers {};
		std::shared_ptr<Mesh> m_Mesh;
	};
}