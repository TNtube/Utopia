#include "Command/Commands.hpp"
#include "Cardia/Application.hpp"

namespace Cardia
{

	UpdateTransformPositionCommand::UpdateTransformPositionCommand(const UUID& uuid, Vector3f oldPosition)
		: m_UUID(uuid), m_OldPosition(oldPosition) {}

	void UpdateTransformPositionCommand::Redo(Application* ctx)
	{
		auto& transform = ctx->GetCurrentScene()->GetEntityByUUID(m_UUID).GetComponent<Component::Transform>();
		const auto temp = transform.GetPosition();
		transform.SetPosition(m_OldPosition);

		m_OldPosition = temp;
	}

	void UpdateTransformPositionCommand::Undo(Application* ctx)
	{
		auto& transform = ctx->GetCurrentScene()->GetEntityByUUID(m_UUID).GetComponent<Component::Transform>();
		const auto temp = transform.GetPosition();
		transform.SetPosition(m_OldPosition);

		m_OldPosition = temp;
	}
}
