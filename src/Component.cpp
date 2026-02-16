#include "Component.h"
#include "HelperManager.h"

//BASE CLASS - DEFINITIONS
UIF::Component::Component(const std::string& filepath, UIF::Window* window, float x, float y, float w, float h){
	if(!filepath.empty()){
		tex_cache->Add_Texture(filepath, window, this);
 		Mod_Src(0, 0, static_cast<float>(tex_cache->Get_Texture(this)->w), static_cast<float>(tex_cache->Get_Texture(this)->h));
		Mod_Dst(window, x, y, static_cast<float>(tex_cache->Get_Texture(this)->w), static_cast<float>(tex_cache->Get_Texture(this)->h));
	}
	else{	
		Mod_Dst(window, x, y, w, h); 
	}

	this->aspect = this->cfrect.dst_frect->w / this->cfrect.dst_frect->h;
	this->win_ratio = (this->cfrect.dst_frect->w * this->cfrect.dst_frect->h) / 
		(static_cast<float>(window->Get_Dimensions().w) * static_cast<float>(window->Get_Dimensions().h)); // <- Capture the initial relative area for use as a Max/Min scaling constraint..
	

	//If component is larger than the window. Fit to Window (or workspace in future), recalculate the ratio which should now be 1:1.
	if(this->win_ratio > 1){
		Mod_Dst(window, this->cfrect.dst_frect->x, this->cfrect.dst_frect->y, 
			static_cast<float>(window->Get_Dimensions().w),
			static_cast<float>(window->Get_Dimensions().h));

		this->win_ratio = (this->cfrect.dst_frect->w * this->cfrect.dst_frect->h) / 
			(static_cast<float>(window->Get_Dimensions().w) * static_cast<float>(window->Get_Dimensions().h));
	}
	this->children.reserve(UIF::Data::default_reserve);
}

UIF::Component::Component(UIF::Component* component){
	this->cfrect = component->cfrect; 
	this->cfrect.dst_frect = new SDL_FRect{ component->cfrect.dst_frect->x,
						component->cfrect.dst_frect->y,
						component->cfrect.dst_frect->w,
						component->cfrect.dst_frect->h };

	this->parent = component->parent; 
	this->children = component->children;

	this->TVec_ID = component->TVec_ID;

	this->aspect = component->aspect;
	this->win_ratio = component->win_ratio;

	UIF::Data::global_bus->Add_ComponentLine(this);
}

//TAKE ANOTHER LOOK AT THIS...
void UIF::Component::Delete(UIF::Component* component){
	if(component->children.empty()){
		delete component->cfrect.dst_frect;
		delete component;
		return;
	}

	for(auto* child : component->children){
			Delete(child);
	}
}

void UIF::Component::Render(UIF::Window* window){
	SDL_Renderer* const cache_render { window->Get_Renderer() }; //Repeated calls to get the renderer per cycle are unideal.	
	//Don't set the draw color if it hasn't changed.
	auto color_cache = [](SDL_Color& last_color, const SDL_Color& RGBA){
		if(last_color.r != RGBA.r &&
		   last_color.b != RGBA.b &&
		   last_color.g != RGBA.g &&
		   last_color.a != RGBA.a){
			last_color = RGBA;
			return false;
		   }

		return true;
	};

	if(!this->TVec_ID){
		if(!color_cache(last_color, this->cfrect.RGBA)){
			SDL_SetRenderDrawColor(cache_render, this->cfrect.RGBA.r, this->cfrect.RGBA.g, this->cfrect.RGBA.b, this->cfrect.RGBA.a);	
		}
		SDL_RenderFillRect(cache_render, 
				this->cfrect.dst_frect);

	}
	else{
		SDL_RenderTexture(cache_render, tex_cache->Get_Texture(this), &this->cfrect.src_frect, this->cfrect.dst_frect);
	}
	for(auto* child : this->children){
		if(child->Is_Active()){
			if(!child->TVec_ID){
				if(!color_cache(last_color, child->cfrect.RGBA)){
					SDL_SetRenderDrawColor(cache_render, child->cfrect.RGBA.a, child->cfrect.RGBA.g,
						child->cfrect.RGBA.b, child->cfrect.RGBA.a);
				}
				SDL_RenderFillRect(cache_render, 
					child->cfrect.dst_frect);
			}
			else{
				SDL_RenderTexture(cache_render, tex_cache->Get_Texture(child), &child->cfrect.src_frect, child->cfrect.dst_frect); 
			}
		}
	}
}

//Depth First Search. If Component hit, check children... And so forth for all hit components until a hit child is found with no children, then return the child with no children.
UIF::Component* UIF::Component::Query_Hit(UIF::Component* component){
	auto hit_test = [](UIF::Component* component){
 		float m_x{};
  		float m_y{};
		SDL_GetMouseState(&m_x, &m_y);
  		if(m_x >= component->cfrect.dst_frect->x &&
    		 m_x <= (component->cfrect.dst_frect->x + component->cfrect.dst_frect->w) &&
   	 	 m_y >= component->cfrect.dst_frect->y &&      
    	 	m_y <= (component->cfrect.dst_frect->y + component->cfrect.dst_frect->h)){
			return true;
		}
		return false;
	};

	//Already confirmed to be hit, so just return.
	if(component->children.empty()){
		return component;
	}

	for(auto* child : component->children){
		if(child->Is_Active()){
			if(hit_test(child)){
				return Query_Hit(child);
			}
		}
	}

	//No children hit, hit parent.
	return this;
}

//Consider FRect sources?
void UIF::Component::Mod_Src(float x, float y, float w, float h){
	if(x < 0 || x > tex_cache->Get_Texture(this)->w ||
	   y < 0 || y > tex_cache->Get_Texture(this)->h ||
	   w < 1 || 
	   h < 1 ){
		return;
	}
	this->cfrect.src_frect.x = x;
	this->cfrect.src_frect.y = y;

	this->cfrect.src_frect.w = w;
	this->cfrect.src_frect.h = h;
}

void UIF::Component::Mod_Dst(UIF::Window* window, float x, float y, float w, float h){
	if(x < 0 || x > window->Get_Dimensions().w ||
	   y < 0 || y > window->Get_Dimensions().h ||
	   w < 1 || 
	   h < 1 ){
		return;
	}
	this->cfrect.dst_frect->x = x;
	this->cfrect.dst_frect->y = y;

	this->cfrect.dst_frect->w = w;
	this->cfrect.dst_frect->h = h;
}

void UIF::Component::Mod_Color(SDL_Color& RGBA, int16_t r, int16_t g, int16_t b){
	//Detect Overflow in either direction.
	if( static_cast<float>(r + g + b) / 
			static_cast<float>(std::numeric_limits<uint8_t>::max()) > 1 
			|| (r * g * b) < 0){
		return;
	}
	RGBA.r = static_cast<uint8_t>(r);
	RGBA.g = static_cast<uint8_t>(g);
	RGBA.b = static_cast<uint8_t>(b);
}

void UIF::Component::Mod_Opacity(SDL_Color& RGBA, int16_t a){
	if(a > std::numeric_limits<uint8_t>::max() 
			|| a < 0){
		return;
	}
	RGBA.a = static_cast<uint8_t>(a);	
}

void UIF::Component::Link_To_Texture(UIF::Component* lhs, UIF::Component* rhs){
	lhs->Set_TVec_ID(rhs->Get_TVec_ID());
}

void UIF::Component::Set_TVec_ID(uint32_t new_tvec_id){
	this->TVec_ID = new_tvec_id;
}

void UIF::Component::Set_HVec_ID(uint32_t new_hvec_id){
	this->HVec_ID = new_hvec_id;
}

void UIF::Component::Set_Active(bool new_active){
	this->is_active = new_active;
}

bool UIF::Component::Is_Active(){
	return this->is_active;
}

uint32_t UIF::Component::Get_ID(){
	return this->ID;
}

uint32_t UIF::Component::Get_TVec_ID(){
	return this->TVec_ID;
}
			
uint32_t UIF::Component::Get_HVec_ID(){
	return this->HVec_ID;
}

const UIF::ColoredFRect& UIF::Component::Get_CFRect(){
	return this->cfrect;
};

const float& UIF::Component::Get_Aspect(){
	return this->aspect;
}

const float& UIF::Component::Get_WinRatio(){
	return this->win_ratio;
}

//Component can be invalid, if and only if it is expressly desired to be asset based.
bool UIF::Component::State_Valid(){
	if(!tex_cache->Get_Texture(this)){
		return false;
	}
	else if(!tex_cache->Get_Texture(this)->w || !tex_cache->Get_Texture(this)->h){ //Guard for 0x1/0x0 Textures... Will cause crashes on division from aspect ratio/win_ratio.
		return false;
	}

	return true;
}

UIF::Component* UIF::Component::Add_Helper(const std::string& helper, UIF::Invoker invoker){ 
	UIF::Data::global_bus->Add_HelperLine(this, invoker, helper, UIF::Data::HelperOp::ADD_HELPER);
	return this;
}

UIF::Component* UIF::Component::Remove_Helper(const std::string& helper, UIF::Invoker invoker){
	UIF::Data::global_bus->Add_HelperLine(this, invoker,  helper, UIF::Data::HelperOp::REMOVE_HELPER);
	return this;
}

UIF::Component* UIF::Component::Add_Child(UIF::Component* component){
	component->parent = this;
	this->children.emplace_back(component);
	return component;
}

//Moving child elsewhere or discarding...
UIF::Component* UIF::Component::Remove_Child(UIF::Component* component){
	component->parent = nullptr;
	UIF::ContainerTargetErase(this->children, component);
	return component;
}


//DERVIED CLASS - DEFINITIONS


