#include "TextureCache.h"
#include "Component.h"

UIF::TextureCache::~TextureCache(){
	for(auto* texture : this->textures){
		SDL_DestroyTexture(texture);
	}
	UIF::ContainerEraseAll(this->textures);
}

SDL_Texture* UIF::TextureCache::Get_Texture(UIF::Component* component){
	return this->textures[component->Get_TVec_ID()];
}

bool UIF::TextureCache::Add_Texture(const std::string& filepath, UIF::Window* window, UIF::Component* component){
	//Texture hasn't already been loaded.
	if(!this->redirect_lookup[filepath]){
		SDL_Surface* temp_surface = IMG_Load(filepath.c_str());
		if(!temp_surface){
			component->Set_TVec_ID(NO_TEXTURE);
			SDL_Log("Failed to load image: SDL error%s\n", SDL_GetError());
			return false;
		}
		SDL_Texture* texture = SDL_CreateTextureFromSurface(window->Get_Renderer(), temp_surface);
		delete temp_surface;

		if(!texture){
			component->Set_TVec_ID(NO_TEXTURE);
			SDL_Log("Failed to create texture. SDL error%s\n", SDL_GetError());
 			return false;
		}
				
		this->textures.emplace_back(texture);
		this->redirect_lookup[filepath] = static_cast<uint32_t>(textures.size() - 1);
		component->Set_TVec_ID(static_cast<uint32_t>(textures.size() - 1));

	}
	else{
		component->Set_TVec_ID(redirect_lookup[filepath]);
	}

	return true;
}
