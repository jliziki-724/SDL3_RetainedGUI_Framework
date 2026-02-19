#include "Render.h"
#include "Component.h"

UIF::Render::Render(std::vector<std::vector<UIF::Component*>>& c_vec) 
	: component_vec(c_vec) {
		checksums.reserve(UIF::Data::default_reserve);
}

void UIF::Render::Update_Draw(const std::vector<UIF::Window*>& windows){
	for(auto* window : windows){
		for(int idx{ this->component_vec[window->Get_CVec_ID()].size() }; idx >= 0; idx--){

		}
	}
}

void UIF::Render::Update(const std::vector<UIF::Window*>& windows){
	Update_Draw(windows);
	for(auto* window : windows){
		if(window->Is_Active()){
			Render_Feed(window);
		}
	}
}

void UIF::Render::Render_Feed(UIF::Window* window){
	for(auto* component : this->component_vec[window->Get_CVec_ID()]){
		Render_Update(window, component);
	}
}

void UIF::Render::Render_Present(const std::vector<UIF::Window*>& windows){
	for(auto* window : windows){
		if(window->Is_Active()){
			//SDL_SetRenderDrawColor(window->Get_Renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
			//SDL_RenderClear(window->Get_Renderer());
			SDL_RenderPresent(window->Get_Renderer());
		}
	}
}

//Another DFS... Render Component(Parent) first, then render the child, then Render the child's children... And so forth.
void UIF::Render::Render_Update(UIF::Window* window, UIF::Component* component = nullptr){
	SDL_Renderer* const cache_render { window->Get_Renderer() }; //Repeated calls to get the renderer per cycle are unideal.	
	auto cached = [](SDL_Color& last_color, const SDL_Color& RGBA){	//Don't set the draw color if it hasn't changed.
		if(last_color.r != RGBA.r &&
		   last_color.b != RGBA.b &&
		   last_color.g != RGBA.g &&
		   last_color.a != RGBA.a){
			last_color = RGBA;
			return false;
		   }

		return true;
	};

	auto render_cfrect = [cached, cache_render](SDL_Color& last_color, UIF::Component* component){
		if(!cached(last_color, component->Get_CFRect().RGBA)){
			SDL_SetRenderDrawColor(cache_render, component->Get_CFRect().RGBA.r, component->Get_CFRect().RGBA.g, component->Get_CFRect().RGBA.b, component->Get_CFRect().RGBA.a);
		}
		SDL_RenderFillRect(cache_render, 
				component->Get_CFRect().dst_frect);
	};

	if(component->Need_Draw()){
		if(component->Get_TVec_ID() == UIF::TextureCache::NO_TEXTURE){
			render_cfrect(last_color, component);
		}
		else{
			SDL_RenderTexture(cache_render, tex_cache->Get_Texture(component), &component->Get_CFRect().src_frect, component->Get_CFRect().dst_frect);
		}
	}
	for(auto* child : component->Get_Children()){
		if(child->Need_Draw()){
			if(child->Get_TVec_ID() == UIF::TextureCache::NO_TEXTURE){
				render_cfrect(last_color, child);
			
			}
			else{
				SDL_RenderTexture(cache_render, tex_cache->Get_Texture(child), &child->Get_CFRect().src_frect, child->Get_CFRect().dst_frect); 
			}
			Render_Update(window, child);
		}
	}
}


float UIF::Render::Make_Checksum(UIF::Component* component){
	return 	component->Get_CFRect().dst_frect->x +
		component->Get_CFRect().dst_frect->y +
		component->Get_CFRect().dst_frect->w +
		component->Get_CFRect().dst_frect->h;
}

bool UIF::Render::Comp_Checksum(UIF::Component* component){
	if(Make_Checksum(component) != this->checksums[component->Get_ID()]){
		return false;
	}
	return true;
}
