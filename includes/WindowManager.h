#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "HelperManager.h"
#include "Render.h"

namespace UIF{

	/*WindowManager owns all Windows and associated Components.*/

	struct ArrOfCVec{
		std::array<std::vector<UIF::Component*>, static_cast<long unsigned int>(UIF::Invoker::INVOKER_COUNT)> arr_vec;
	};

	class WindowManager{
	 private:
		inline static uint32_t CVec_ID{};

		std::vector<UIF::Window*>windows; 
		std::vector<std::vector<UIF::Component*>>component_vec{};
 
		static constexpr int total_partitions{ 8 };
		int partition_size{};
		std::vector<UIF::ArrOfCVec> window_partitions;
		//Window_ID->Window's Regions->Region's Components


		UIF::Window* focus_window{ nullptr };
		UIF::HelperManager helper_mgr{};
		UIF::Render renderer { component_vec };

		bool quit{};

		SDL_Event event;

		void Dispatch();
		void Update();
		void Render();

		void Partition_Windows();

		void Throttle_Frame_Rate(UIF::Window* window);

		void Delete_Window();
		void Interact_Window(UIF::Invoker invoker);
		void Component_Event(UIF::Component* component, UIF::Invoker invoker);
		void Query_ID(SDL_WindowID id);
		void Query_Highest_Priority();

 	public:
		static Window* Create(std::string_view t, int w, int h, int flag);

		WindowManager() = default;
		~WindowManager() = default; // <- Considered cleaning up abnormal terminations... However, OS reclaims resources anyways.

		void Create_Window(std::string_view title, int w, int h, int flag = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
		void Add_Component(UIF::Component* component, UIF::Window* window);  
		void Remove_Component(UIF::Component* component, UIF::Window* window);
		void Run();                
     
		UIF::Window* operator[](const std::string& window);

		WindowManager(const WindowManager&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;
	};

}

#endif //WINDOWMANAGER_H
