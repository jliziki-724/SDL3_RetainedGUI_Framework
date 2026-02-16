#ifndef COMPONENT_H
#define COMPONENT_H

#include "UIFCore.h"
#include "Window.h"
#include "GlobalBus.h"
#include "TextureCache.h"

namespace UIF{

	struct ColoredFRect{
		SDL_FRect* dst_frect { new SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f} };
		SDL_FRect src_frect {};
		SDL_Color RGBA{ 0, 0, 0, SDL_ALPHA_OPAQUE };
	};

	//Consider Naming Components?
	class Component{
		protected:
			inline static SDL_Color last_color{}; // <- Lazy cache. Not Thread Safe. However, rendering is single threaded.
			inline static uint32_t Component_ID{}; // To use in error logs... TypeName + ID <- Remember to actually add this.
			uint32_t ID{ Component_ID++ };

			ColoredFRect cfrect{};

			UIF::Component* parent{ nullptr };
			std::vector<Component*> children;

			//Texture Vector ID
			uint32_t TVec_ID{};

			//Helpers Vector ID
			uint32_t HVec_ID{};

	        	float aspect{};
			float win_ratio{};
		
			bool is_active{ true };
			bool State_Valid();	

			Component(Component* component);
			Component(const std::string& filepath, UIF::Window* window, float x, float y, float w, float h); //Image based Components
			Component(const Component&) = delete; 
			Component& operator=(const Component&) = delete;
		public:	
			virtual ~Component() = default;

			template <typename T>
				static T* Create(const std::string& filepath, UIF::Window* window, float x = 0.0f, float y = 0.0f, 
						 float w = 1.0f, float h = 1.0f){
					T* component = new T(filepath, window, x, y, w, h);
					//Component can be invalid, if and only if it is expressly desired to be asset based.
				        if(!filepath.empty()){
						if(!component->State_Valid()){
							delete component;
							return nullptr;
						}
					}

					UIF::Data::global_bus->Add_ComponentLine(component); // <- Put in Bus to be allocated invokers and helpers.	
					return component;
				}

			//TO FIX/FINISH!!!
			template<typename T>
				static T* Clone(UIF::Component* component){
					T* clone = new T(component); 	
					UIF::Data::global_bus->Add_ComponentLine(clone);	
					if(clone->children.empty()){
						return clone;
					}

					//Construct new Component for every child.
					for(int idx{}; idx < clone->children.size(); idx++){
						clone->children[idx] = Clone<T>(clone->children[idx]);		
					}
				}

			//Would be static if didn't require 'this', slightly awkward call site.
			UIF::Component* Query_Hit(UIF::Component* component);
			static void Delete(UIF::Component* component);
			virtual void Update(UIF::Window* window) = 0; //Update Layout/Geometry
			virtual void Render(UIF::Window* window);

			UIF::Component* Add_Helper(const std::string& helper, UIF::Invoker invoker);
			UIF::Component* Remove_Helper(const std::string& helper, UIF::Invoker invoker);
		        UIF::Component* Add_Child(UIF::Component* component);
			UIF::Component* Remove_Child(UIF::Component* component);


			void Mod_Dst(UIF::Window* window, float x, float y, float w, float h);
			void Mod_Src(float x, float y, float w, float h);
			void Mod_Color(SDL_Color& RGBA, int16_t r, int16_t g, int16_t b);
			void Mod_Opacity(SDL_Color& RGBA, int16_t a);
			
			void Link_To_Texture(UIF::Component* lhs, UIF::Component* rhs);


			//Frequently accessed attributes. Return by Copy instead...
			const float& Get_Aspect();
			const float& Get_WinRatio();
			const ColoredFRect& Get_CFRect();
			
			uint32_t Get_ID();

			void Set_TVec_ID(uint32_t new_tvec_id);
			uint32_t Get_TVec_ID();

			void Set_HVec_ID(uint32_t new_hvec_id);
			uint32_t Get_HVec_ID();

			void Set_Active(bool new_active);
			bool Is_Active();
		};

	        class Image : public Component{
			private:
				using Component::Component;
			public:
				virtual void Update(UIF::Window* window) override{
				}
		};

		
		class Anchor : public Component{
			private:
				using Component::Component;
				bool snapped_to{}; // <- Occupied by a Component

			public:
				bool Snapped_To(){
					return snapped_to;
				}

				//Check for Occlusion... etc.
				virtual void Update(UIF::Window* window) override{
				}

		};


		class CheckBox : public Component{
			private:
				using Component::Component;

			public:
				virtual void Update(UIF::Window* window) override{
				}
		};

		class DropMenu : public Component{
			private:
				using Component::Component;

			public:
				

		};

		//Occupy percentage of width according to percentage of data processed...
		class ProgressBar : public Component{
			private:
				using Component::Component;
				float progress{};

			public:
				virtual void Update(UIF::Window* window) override{}

		};


		class Button : public Component{
			private:
				using Component::Component;
			
		  	public:
				virtual void Update(UIF::Window* window) override{
				}
			
		};

		class RadioButton : public Button{
		};


		class MessageBox : public Component{
			private:
				using Component::Component;

			public:
				virtual void Update(UIF::Window* window) override{
				}
		};

		class SideBar : public Component{
			private:
				using Component::Component;

			public:
				virtual void Update(UIF::Window* window) override{}
		
		};

		class TabBar : public Component{
			private:
				using Component::Component;
				
				const float tab_width{};
				const float tab_height{};

				float tabs_x{};
				float tabs_y{}; // <-  

			public:
				void Init(UIF::Window* window, float x, float y, float w, float h){
					auto calc_space = [this, x, w](){
						int counter{};

						if(!counter){
							return x;
						}
						return (x + (++counter * w)) + 1;
					};

					for(auto* child : children){
						child->Mod_Dst(window, calc_space(), y, w, h);
					}
				}

				void Add_Tab(){
				}
	
				virtual void Update(UIF::Window* window) override{
				}	

		};

		class TextSurface : public Component{

		};
};

#endif //COMPONENT_H
