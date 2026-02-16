#ifndef RENDER_H
#define RENDER_H

#include "UIFCore.h"
#include "NotificationBus.h"

namespace UIF{

	class Render{
		private:
			std::vector<std::vector<UIF::Component*>>& component_vec; //Implement spatial partitioning, etc. 
		        std::unordered_map<UIF::Component*, float>checksums; //<- Associate Checksum to component rather than embed into type.									   
			float Make_Checksum(UIF::Component* component);
			bool Comp_Checksum(UIF::Component* component);
		

			//Re-Use this later to handle occluded Components or Disappearing Elements.
			void Update_Active(UIF::Window* window);
			void Render_Active(UIF::Window* window);

			Render(std::vector<std::vector<UIF::Component*>>& component_vec);
			~Render() = default;
			Render(const Render&) = delete;
			Render& operator=(const Render&) = delete;

			friend WindowManager;
	};
}

#endif //RENDER_H
