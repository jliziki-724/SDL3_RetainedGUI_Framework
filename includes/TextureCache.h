#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "UIFCore.h"

namespace UIF{

	class TextureCache{
		private:
			std::unordered_map<std::string, uint32_t> redirect_lookup; /*Check if the image data has already been loaded based on filepath.*/
									 		
		std::vector<SDL_Texture*> textures { nullptr }; //Insert one elemnt to reserve index 0 as sentinel.

		public:	
		
		void Add_Texture(const std::string& filepath, UIF::Window* window, UIF::Component* component);	
		SDL_Texture* Get_Texture(UIF::Component* component);

		TextureCache() = default;
	        ~TextureCache();
		TextureCache(const TextureCache&) = delete;
		TextureCache& operator=(const TextureCache&) = delete;
	};
};


#endif //TEXTURECACHE_H
