﻿#include "cdpch.hpp"

#include "Cardia/ECS/Components.hpp"

namespace Cardia::Component
{
        void Script::reloadFile()
        {
                Log::info("Loading path {0}", m_Path);
                const std::ifstream t(m_Path);
                std::stringstream buffer;
                buffer << t.rdbuf();
                m_Content = buffer.str();
		auto filename = std::filesystem::path(m_Path).filename().replace_extension().string();
		scriptClass = ScriptEngine::Instance().GetClassFromPyFile(filename, m_Content);
        }
}
