#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "HelperManager.h"
#include "Render.h"

namespace UIF{

	/*WindowManager owns all Windows and associated Components.*/
	class WindowManager{
	 private:
		inline static uint32_t CVec_ID{};

		std::vector<UIF::Window*>windows; 
		std::vector<std::vector<UIF::Component*>> component_vec{};

		UIF::Window* focus_window{ nullptr };
		UIF::HelperManager helper_mgr{};
		UIF::Render renderer { component_vec };

		bool quit{};

		SDL_Event event;

		void Dispatch();
		void Update();
		void Render();

		void Delete_Window();
		void Interact_Window(UIF::Invoker invoker);
		void Component_Event(UIF::Component* component, UIF::Invoker invoker);
		void Query_ID(SDL_WindowID id);
		void Query_Highest_Priority();

		UIF::Window* Query_Title(const std::string& title);
 	public:
		static Window* Create(const std::string& t, int w, int h, int flag);

		WindowManager() = default;
		~WindowManager() = default; // <- Considered cleaning up abnormal terminations... However, OS reclaims resources anyways.

		void Create_Window(const std::string& title, int w, int h, int flag = SDL_WINDOW_RESIZABLE);
		void Add_Component(UIF::Component* component, const std::string& window);  
		void Remove_Component(UIF::Component* component, const std::string& window);
		void Run();                
     
		UIF::Window* operator[](const std::string& window);

		WindowManager(const WindowManager&) = delete;
		WindowManager& operator=(const WindowManager&) = delete;
	};

}

#endif //WINDOWMANAGER_H
