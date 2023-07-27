﻿#include "cdpch.hpp"

#include "Cardia/ECS/Components.hpp"

namespace Cardia::Component
{
	void Script::ReloadFile()
	{
		if (m_Path.empty()) return;
		Log::Info("Loading path {0}", m_Path);
		const std::ifstream t(m_Path);
		std::stringstream buffer;
		buffer << t.rdbuf();
		auto filePath = std::filesystem::path(m_Path);
		scriptClass = ScriptEngine::Instance().GetClassFromPyFile(filePath);
		scriptClass.RegisterAttributes();
	}

	Vector3f Transform::Forward() const
	{
		Vector3f forward;

		forward.x = cos(rotation.x) * sin(rotation.y);
		forward.y = -sin(rotation.x);
		forward.z = cos(rotation.x) * cos(rotation.y);

		return forward;
	}

	Vector3f Transform::Up() const
	{
		return Forward().Cross(Right());
	}

	Vector3f Transform::Right() const
	{
		Vector3f right;
		right.x =  cos(rotation.y);
		right.y =  0;
		right.z = -sin(rotation.y);
		return right;
	}
}
