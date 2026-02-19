#include "Component.h"
#include "HelperManager.h"

//BASE CLASS - DEFINITIONS
UIF::Component::Component(const std::string& filepath, UIF::Window* window, float x, float y, float w, float h){
	//On failure to load asset, the Component is treated as FRect. i.e. Renders as FRect instead.
	if(tex_cache->Add_Texture(filepath, window, this)){
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
	if(this->win_ratio > 1.0f){
		Mod_Dst(window, this->cfrect.dst_frect->x, this->cfrect.dst_frect->y, 
			static_cast<float>(window->Get_Dimensions().w),
			static_cast<float>(window->Get_Dimensions().h));

		this->win_ratio = (this->cfrect.dst_frect->w * this->cfrect.dst_frect->h) / 
			(static_cast<float>(window->Get_Dimensions().w) * static_cast<float>(window->Get_Dimensions().h));
	}
	this->children.reserve(UIF::Data::default_reserve);
	UIF::Data::global_bus->Add_ComponentLine(this); // <- Put in Bus to be allocated invokers and helpers.	
}

UIF::Component::Component(UIF::Component* component){
	this->cfrect = component->cfrect; 

	this->parent = component->parent; 
	this->children = component->children;

	this->TVec_ID = component->TVec_ID;

	this->aspect = component->aspect;
	this->win_ratio = component->win_ratio;

	UIF::Data::global_bus->Add_ComponentLine(this);
}

//Even more DFS...
void UIF::Component::Delete(UIF::Component* component){
	for(auto* child : component->children){
			Delete(child);
	}
	delete component->cfrect.dst_frect;
	delete component;
}


//Depth First Search. If Component hit, check children... And so forth for all hit components... then return the hit child with no children.
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

        //Following render order, will test top-most elements first.
	for(int idx{ component->children.size() - 1 }; idx >= 0; idx--){
		if(component->children[idx]->Is_Active()){
			if(hit_test(component->children[idx])){
				return Query_Hit(component->children[idx]);
			}
		}
	}

	return component;
}

UIF::Component* UIF::Component::Mod_Src(float x, float y, float w, float h){
	if(x < 0 || x > tex_cache->Get_Texture(this)->w ||
	   y < 0 || y > tex_cache->Get_Texture(this)->h ||
	   w < 1.0f || 
	   h < 1.0f ){
		return this;
	}
	this->cfrect.src_frect.x = x;
	this->cfrect.src_frect.y = y;

	this->cfrect.src_frect.w = w;
	this->cfrect.src_frect.h = h;

	return this;
}

UIF::Component* UIF::Component::Mod_Dst(UIF::Window* window, float x, float y, float w, float h){
	if(x < 0 || x > window->Get_Dimensions().w ||
	   y < 0 || y > window->Get_Dimensions().h ||
	   w < 1.0f || 
	   h < 1.0f ){
		return this;
	}
	this->cfrect.dst_frect->x = x;
	this->cfrect.dst_frect->y = y;

	this->cfrect.dst_frect->w = w;
	this->cfrect.dst_frect->h = h;

	return this;
}

UIF::Component* UIF::Component::Mod_Color(int16_t r, int16_t g, int16_t b){
	//Detect Overflow in either direction.
	if( static_cast<float>(r + g + b) / 
			static_cast<float>(std::numeric_limits<uint8_t>::max()) > 1 
			|| (r * g * b) < 0){
		return this;
	}
	this->cfrect.RGBA.r = static_cast<uint8_t>(r);
	this->cfrect.RGBA.g = static_cast<uint8_t>(g);
	this->cfrect.RGBA.b = static_cast<uint8_t>(b);

	return this;
}

UIF::Component* UIF::Component::Mod_Opacity(int16_t a){
	if(a > std::numeric_limits<uint8_t>::max() 
			|| a < 0){
		return nullptr;
	}
	this->cfrect.RGBA.a = static_cast<uint8_t>(a);

	return this;
}

UIF::Component* UIF::Component::Link_To_Texture(UIF::Component* component){
	this->TVec_ID = component->Get_TVec_ID();
	return this;
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

void UIF::Component::Set_Draw(bool new_need_draw){
	this->need_draw = new_need_draw;
}

bool UIF::Component::Need_Draw(){
	return this->need_draw;
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

float UIF::Component::Get_Aspect(){
	return this->aspect;
}

float UIF::Component::Get_WinRatio(){
	return this->win_ratio;
}

const std::vector<UIF::Component*>& UIF::Component::Get_Children(){
	return this->children;
}

UIF::Component* UIF::Component::Add_Helper(UIF::HelperType helper_type, UIF::Invoker invoker){ 
	UIF::Data::global_bus->Add_HelperLine(this, invoker, helper_type, UIF::Data::HelperOp::ADD_HELPER);
	return this;
}

UIF::Component* UIF::Component::Remove_Helper(UIF::HelperType helper_type, UIF::Invoker invoker){
	UIF::Data::global_bus->Add_HelperLine(this, invoker,  helper_type, UIF::Data::HelperOp::REMOVE_HELPER);
	return this;
}

//Returns pointer to child for more chaining, i.e. Child with Properties. 
//Parent->Add_Child(x)->Add_Helper(); <- Child or Parent Modified? Nuanced either way given the presentation.
UIF::Component* UIF::Component::Add_Child(UIF::Component* component){
	component->parent = this;
	this->children.emplace_back(component);
	return component;
}

//Returns pointer to child.
//Moving child elsewhere or discarding...
UIF::Component* UIF::Component::Remove_Child(UIF::Component* component){
	component->parent = nullptr;
	UIF::ContainerTargetErase(this->children, component);
	return component;
}


//DERVIED CLASS - DEFINITIONS

void UIF::Workspace::Set_Children_Draggable(UIF::Component* component){
	for(auto* child : component->Get_Children()){
		Set_Children_Draggable(child);
		child->Add_Helper(UIF::HelperType::DRAG, UIF::Invoker::CLICK);
	}
}

void UIF::TabBarContainer::Init(UIF::Window* window, float x, float y, float w, float h, int tab_no){
	this->need_draw = false;

	int counter{};
	auto calc_space = [&counter, x, w](){
		if(!counter){
			++counter;
			return x;
		}
			return (x + (++counter * w) + 5);
	};

		for(int idx{}; idx < tab_no; idx++){
			 children.emplace_back(UIF::Component::Create<UIF::Tab>("", window, x, y, w, h));
		}

		for(auto* child : children){
			child->Mod_Dst(window, calc_space(), y, w, h);
	}
}

