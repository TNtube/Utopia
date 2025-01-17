#include "EditorCamera.hpp"
#include "Cardia/Core/Input.hpp"
#include "Cardia/Core/KeyCodes.hpp"


namespace Cardia
{
	EditorCamera::EditorCamera(Radianf fov, float nearClip, float farClip)
	{
		m_Camera.SetPerspective({fov, nearClip, farClip});
		m_Transform.SetPosition({0, 5, -20});
	}

	void EditorCamera::OnUpdate()
	{
		const Vector2f& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		Vector2f delta = (mouse - m_InitialMousePosition) * 0.01f;
		m_InitialMousePosition = mouse;

		if (Input::IsMouseButtonPressed(Mouse::Middle))
			MousePan(delta);
		else if (Input::IsMouseButtonPressed(Mouse::Right))
		{
			MouseRotate(delta);
			if (Input::IsKeyPressed(Key::A)) {
				m_Transform.Translate(
					-m_Transform.Right()
					* m_MovementSpeed
					* Time::GetDeltaTime().AsSeconds());
			}
			if (Input::IsKeyPressed(Key::D)) {
				m_Transform.Translate(
					m_Transform.Right()
					* m_MovementSpeed
					* Time::GetDeltaTime().AsSeconds());
			}
			if (Input::IsKeyPressed(Key::W)) {
				m_Transform.Translate(
					m_Transform.Forward()
					* m_MovementSpeed
					* Time::GetDeltaTime().AsSeconds());
			}
			if (Input::IsKeyPressed(Key::S)) {
				m_Transform.Translate(
					-m_Transform.Forward()
					* m_MovementSpeed *
					Time::GetDeltaTime().AsSeconds());
			}
			if (Input::IsKeyPressed(Key::Q)) {
				m_Transform.Translate(
					-m_Transform.Up()
					* m_MovementSpeed *
					Time::GetDeltaTime().AsSeconds());
			}
			if (Input::IsKeyPressed(Key::E)) {
				m_Transform.Translate(
					m_Transform.Up()
					* m_MovementSpeed *
					Time::GetDeltaTime().AsSeconds());
			}

			if (Input::IsKeyPressed(Key::LeftShift)) {
				m_MovementSpeed = std::lerp(m_MovementSpeed, m_MaxMovementSpeed, Time::GetDeltaTime().AsSeconds());
			}
			else if (m_MovementSpeed - m_BaseMovementSpeed >= 0.01f) {
				m_MovementSpeed = std::lerp(m_MovementSpeed, m_BaseMovementSpeed, Time::GetDeltaTime().AsSeconds() * 2);
			}
		}

		if (m_Transform.IsDirty()) {
			m_Transform.RecomputeWorld({});
		}
	}

	void EditorCamera::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& event) -> bool {
			float delta = event.getOffSetY();
			m_Transform.Translate(
				m_Transform.Forward()
				* delta
				* m_MovementSpeed
				* Time::GetDeltaTime().AsSeconds());

			m_Transform.RecomputeWorld({});
			return false;
		});
	}

	void EditorCamera::MousePan(const Vector2f& delta)
	{
		if (delta == Vector2f::Zero())
			return;

		m_Transform.Translate(
			-m_Transform.Right()
			* delta.x
			* m_MovementSpeed
			* Time::GetDeltaTime().AsSeconds());
		m_Transform.Translate(
			m_Transform.Up()
			* delta.y
			* m_MovementSpeed
			* Time::GetDeltaTime().AsSeconds());
	}

	void EditorCamera::MouseRotate(const Vector2f &delta)
	{
		if (delta == Vector2f::Zero())
			return;
		
		m_Transform.SetRotation(
			m_Transform.GetRotation()
			+ Vector3f(delta.y, delta.x, 0)
			* RotationSpeed()
			* Time::GetDeltaTime().AsSeconds());
	}


	float EditorCamera::RotationSpeed() const
	{
		return 60.0f;
	}
}