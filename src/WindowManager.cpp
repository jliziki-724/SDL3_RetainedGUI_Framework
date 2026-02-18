#include "WindowManager.h"

UIF::Window* UIF::WindowManager::Create(std::string_view t, int w, int h, int flag){
	Window* window = new Window(CVec_ID++, t, w, h, flag);
	if(!window->Is_Init()){
		delete window;
		return nullptr;
	}	
	return window;
}

void UIF::WindowManager::Create_Window(std::string_view title, int w, int h, int flag){	
	UIF::Window* Window = Create(title, w, h, flag);
	if(!Window){ //Simply terminate, no exception.  
		quit = true;
		return;
	}
	//Initial Priority for Focus Order for use in event of minimising/maximizing. etc.
	Window->Set_Priority(SDL_GetTicks());
	this->windows.emplace_back(Window);

	//Window->Components
	this->component_vec.emplace_back(std::vector<UIF::Component*>{});

	//ID(title)->Window
	this->focus_window = Window;
}

//Add a component to the target window specified.
void UIF::WindowManager::Add_Component(UIF::Component* component, UIF::Window* window){
	this->component_vec[window->Get_CVec_ID()].emplace_back(component);
}

void UIF::WindowManager::Remove_Component(UIF::Component* component, UIF::Window* window){
	UIF::ContainerTargetEraseAndDelete(component_vec[window->Get_CVec_ID()], component);
}

void UIF::WindowManager::Delete_Window(){
	//Avert double free on 2+ clicks... 
	if(!this->focus_window){ 
		return;
	}
	SDL_HideWindow(this->focus_window->Get_Window());
	//De-alloc Windows components
	for(auto* component : this->component_vec[this->focus_window->Get_CVec_ID()]){
		UIF::Component::Delete(component);
	}
        //Remove from windows vector.	
	UIF::ContainerTargetEraseAndDelete(this->windows, this->focus_window);
	if(this->windows.empty()){
		quit = true;
	}
	Query_Highest_Priority(); //Set a new focus window
}

void UIF::WindowManager::Update(){
	this->helper_mgr.Update();
}


void UIF::WindowManager::Render(){
	for(auto* window : this->windows){
		if(window->Is_Active()){
			this->renderer.Render_Active(window);
		}
	}
}

void UIF::WindowManager::Dispatch(){
	while(SDL_PollEvent(&this->event)){
		switch(this->event.type){
			case SDL_EVENT_MOUSE_BUTTON_UP:
				Query_ID(this->event.button.windowID); 
				Interact_Window(UIF::Invoker::CLICK);
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				Query_ID(this->event.button.windowID);
				Interact_Window(UIF::Invoker::LONG_CLICK);
				break;

			case SDL_EVENT_MOUSE_MOTION:
				Query_ID(this->event.window.windowID); 
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				Query_ID(this->event.window.windowID);
				this->focus_window->Update_Dimensions();
				Interact_Window(UIF::Invoker::RESIZE);
				break;
			
			case SDL_EVENT_WINDOW_RESTORED:
				Query_ID(this->event.window.windowID);
				break;

			case SDL_EVENT_WINDOW_MAXIMIZED:
				Query_ID(this->event.window.windowID);
				Interact_Window(UIF::Invoker::RESIZE);
				break;

			case SDL_EVENT_WINDOW_MINIMIZED:
				Query_Highest_Priority();
				break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				Query_ID(this->event.window.windowID);
				Delete_Window();
				break;


			case SDL_EVENT_QUIT:
				this->quit = true;
				break;

			default:
				break;
		}
	}

}

UIF::Window* UIF::WindowManager::operator[](const std::string& window){
	for(auto* win : this->windows){
		if(win->Get_Title() == window){
			return win;
		}
	}
	std::cout << "Typo" << "\n";
	return nullptr;
}

void UIF::WindowManager::Query_Highest_Priority(){
	uint64_t max{};
	for(auto* window : this->windows){
		if(window->Get_Priority() > max){
			max = window->Get_Priority();
			this->focus_window = window;
		}
	}
}

/*Will cause a seg fault if a window - one created by the process - that is not registered by the WindowManager is clicked, as focus_window will become nullptr.*/
void UIF::WindowManager::Query_ID(SDL_WindowID id){
	for(auto* window : this->windows){
		if(window->Get_ID() == id){
			window->Set_Priority(SDL_GetTicks());
			this->focus_window = window;
		}
	}
}

//No Z-Order, so all overlapping components - i.e non-children - are liable to be hit on a click.
void UIF::WindowManager::Interact_Window(UIF::Invoker invoker){ 
	focus_window->Set_Priority(SDL_GetTicks());
	for(auto* component : this->component_vec[focus_window->Get_CVec_ID()]){
		Component_Event(component, invoker);
	}
	
};

void UIF::WindowManager::Component_Event(UIF::Component* component, UIF::Invoker invoker){  
 	 //Force a Hit on all Components on RESIZE. Handled by HelperMgr
	if(invoker == UIF::Invoker::RESIZE){
		for(auto* component : component_vec[focus_window->Get_CVec_ID()]){
			this->helper_mgr.Invoke(component, focus_window, invoker); 	
		}
		return;
	 }

	auto hit_test = [](UIF::Component* component){
   		const ColoredFRect temp = component->Get_CFRect();
  		float m_x{};
  		float m_y {};
		SDL_GetMouseState(&m_x, &m_y);

  		if(m_x >= temp.dst_frect->x &&
    		 m_x <= (temp.dst_frect->x + temp.dst_frect->w) &&
   		  m_y >= temp.dst_frect->y &&      
    		 m_y <= (temp.dst_frect->y + temp.dst_frect->h)){
			return true;
		}
		return false;	
  	};

	if(hit_test(component)){
		this->helper_mgr.Invoke(UIF::Component::Query_Hit(component),focus_window, invoker);
	}
}

void UIF::WindowManager::Run(){
	uint64_t start_ticks{};
	uint64_t end_ticks{};	

	 auto cache_window = [this](){
	 	auto cache_FPS { focus_window->Get_FPS() };
	 	auto* cache_focus { focus_window };

		if(cache_focus != this->focus_window){
			cache_focus = this->focus_window;
			cache_FPS = this->focus_window->Get_FPS();
			return cache_FPS;
		}
		return cache_FPS;
	};

	Update();	
	while(true){
		start_ticks = SDL_GetTicks();
		Dispatch();
		if(this->quit){
			break;
		}
		Update();
		Render();

		end_ticks = SDL_GetTicks();
		if( (static_cast<float>(end_ticks) - static_cast<float>(start_ticks) ) < (1000.0 / cache_window())){
			SDL_Delay(1000.0 / cache_window());
		}
	}
 }
