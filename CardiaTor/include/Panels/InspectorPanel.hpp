﻿#pragma once
#include <functional>

#include "IPanel.hpp"
#include "Cardia/ECS/Entity.hpp"
#include "Cardia/Scripting/ScriptInstance.hpp"
#include "Cardia/Scripting/ScriptClass.hpp"


namespace Cardia::Panel
{
        class InspectorPanel : public IPanel
        {
        public:
                InspectorPanel() = default;
                ~InspectorPanel() override = default;
                void OnImGuiRender() override;
		void SetSelectedEntity(Entity entity);


        private:
                template<typename T>
                void DrawInspectorComponent(const char* name, std::function<void(T&)> func);
	        bool DrawField(ScriptInstance* behaviorInstance, ScriptFieldType type, const char* fieldName, py::object& field);
	        Entity m_SelectedEntity;
        };
}
