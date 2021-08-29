#pragma once

#include "Utopia/Layer/Layer.hpp"

namespace Utopia
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void onPush() override;
		void onPop() override;
		void onImGuiDraw() override;

		void Begin();
		void End();


		float m_Time {};
	};
}
