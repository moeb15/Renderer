#pragma once
#include "Scenes/Scene.h"

namespace Yassin
{
	class SponzaScene : public Scene
	{
	public:
		void Init(Camera* camera, DirectionalLight* sun, PointLight* light) override;
	};
}

