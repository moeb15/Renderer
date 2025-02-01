#pragma once
#include "Scenes/Scene.h"

namespace Yassin
{
	class TestScene : public Scene
	{
	public:
		void Init(Camera* camera, DirectionalLight* sun, PointLight* light) override;
	};
}

