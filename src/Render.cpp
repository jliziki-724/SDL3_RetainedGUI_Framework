#include "Render.h"
#include "Component.h"

UIF::Render::Render(std::vector<std::vector<UIF::Component*>>& c_vec) 
	: component_vec(c_vec) {
}


void UIF::Render::Update_Active(UIF::Window* window){
}

void UIF::Render::Render_Active(UIF::Window* window){
	SDL_RenderClear(window->Get_Renderer());
	for(auto* component : this->component_vec[window->Get_CVec_ID()]){
		if(component->Is_Active()){
			component->Render(window);
		}
	}
	SDL_RenderPresent(window->Get_Renderer());
}

float UIF::Render::Make_Checksum(UIF::Component* component){
	return 	component->Get_CFRect().dst_frect->x +
		component->Get_CFRect().dst_frect->y +
		component->Get_CFRect().dst_frect->w +
		component->Get_CFRect().dst_frect->h;
}

bool UIF::Render::Comp_Checksum(UIF::Component* component){
	if(Make_Checksum(component) != this->checksums[component]){
		return false;
	}
	return true;
}
