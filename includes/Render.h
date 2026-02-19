#ifndef RENDER_H
#define RENDER_H

#include "UIFCore.h"
#include "NotificationBus.h"

namespace UIF{

	class Render{
		private:
			inline static SDL_Color last_color{}; // <- Lazy cache. Not Thread Safe. However, rendering is single threaded.

			std::vector<std::vector<UIF::Component*>>& component_vec; //Implement spatial partitioning, etc. 
		        std::vector<float>checksums;

			float Make_Checksum(UIF::Component* component);
			bool Comp_Checksum(UIF::Component* component);

			void Update(const std::vector<UIF::Window*>& component_vec);
			//Re-Use this later to handle occluded Components or Disappearing Elements.
			void Update_Draw(const std::vector<UIF::Window*>& windows);
			void Render_Update(UIF::Window* window, UIF::Component* component);
			void Render_Feed(UIF::Window* window);
			void Render_Present(const std::vector<UIF::Window*>& windows);

			Render(std::vector<std::vector<UIF::Component*>>& component_vec);
			~Render() = default;
			Render(const Render&) = delete;
			Render& operator=(const Render&) = delete;

			friend WindowManager;
	};
}

#endif //RENDER_H
