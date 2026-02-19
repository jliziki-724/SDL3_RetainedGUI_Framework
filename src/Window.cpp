#include "Window.h"

UIF::Window::Window(uint32_t cvec_id, std::string_view t, int w, int h, int flag) : CVec_ID(cvec_id), title(static_cast<std::string>(t)){
	window = SDL_CreateWindow(static_cast<std::string>(t).c_str() , w, h, flag);
	if(!window){
		SDL_Log("Failed to create window. SDL error%s\n", SDL_GetError());
	}
	else{
		this->id = SDL_GetWindowID(window);
		this->Update_Dimensions();
	}
	renderer = SDL_CreateRenderer(window, NULL);
	if(!renderer){
		SDL_Log("Failed to create renderer. SDL error%s\n", SDL_GetError());
	}
	else{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}
}

bool UIF::Window::Is_Init() const {
	if(!this->window || !this->renderer){
		return false;
	}
	return true;
}

UIF::Window::~Window(){
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

//Use SDL_GetRender ?
SDL_Renderer* UIF::Window::Get_Renderer(){
	return this->renderer;
}

SDL_Window* UIF::Window::Get_Window(){
    return this->window;
}

int UIF::Window::Get_Frame_Rate(){
	return this->frame_rate;
}

bool UIF::Window::Is_Active(){
	return this->is_active;
}

uint32_t UIF::Window::Get_CVec_ID(){
	return this->CVec_ID;
}

uint64_t UIF::Window::Get_Priority(){
	return this->priority;
}

const UIF::Dimensions& UIF::Window::Get_Dimensions(){
	return this->win_dim;
}

SDL_WindowID UIF::Window::Get_ID(){
	return this->id;
}

const std::string& UIF::Window::Get_Title(){
	return this->title;
}

void UIF::Window::Set_Active(bool new_active){
	this->is_active = new_active;
}

void UIF::Window::Set_Frame_Rate(int new_frame_rate){
	this->frame_rate = new_frame_rate;
}
	
void UIF::Window::Set_Priority(uint64_t timestamp){
	this->priority = timestamp;
}

void UIF::Window::Update_Dimensions(){
	SDL_GetWindowSize(this->window, &this->win_dim.w, &this->win_dim.h);
}
