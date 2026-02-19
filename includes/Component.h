#ifndef COMPONENT_H
#define COMPONENT_H

#include "UIFCore.h"
#include "Window.h"
#include "GlobalBus.h"
#include "TextureCache.h"

namespace UIF{

	struct ColoredFRect{
		SDL_FRect* dst_frect { new SDL_FRect{} };
		SDL_FRect src_frect {};
		SDL_Color RGBA{ 255, 255, 255, SDL_ALPHA_OPAQUE };
	};

	class Component{
		protected:
			inline static uint32_t Component_ID{}; // To use in error logs... TypeName + ID <- Remember to actually add this.
			uint32_t ID{ Component_ID++ };

			ColoredFRect cfrect{};
			
			UIF::Component* parent{ nullptr };
			std::vector<Component*> children;

			std::string name{};

			//Texture Vector ID
			uint32_t TVec_ID{};
			//Helpers Vector ID
			uint32_t HVec_ID{};

	        	float aspect{};
			float win_ratio{};
	
			//Activity state... Should component respond to input? Animate?
			bool is_active{ true };
			//Draw state... Should component be drawn?
			bool need_draw{ true };

			Component(Component* component);
			Component(const std::string& filepath, UIF::Window* window, float x, float y, float w, float h); //Image based Components
			Component(const Component&) = delete; 
			Component& operator=(const Component&) = delete;
		public:	
			virtual ~Component() = default;

			//Failures not checked as failure defaults the Component to FRect based rather than Asset, opportunities to reload can Textures occur later.
			template <typename T>
				static T* Create(const std::string& filepath, UIF::Window* window, float x = 0.0f, float y = 0.0f, 
						 float w = 1.0f, float h = 1.0f){
					T* component = new T(filepath, window, x, y, w, h);	
					return component;
				}

			//Clone usable in next frame... 
			template<typename T>
				static T* Clone(UIF::Component* component){
					T* clone = new T(component); 

					clone->cfrect.dst_frect = new SDL_FRect{ component->cfrect.dst_frect->x,
						component->cfrect.dst_frect->y,
						component->cfrect.dst_frect->w,
						component->cfrect.dst_frect->h };

					//Constructs a deep copy of every copied child resource pointer.
					for(int idx{}; idx < clone->children.size(); idx++){
						clone->children[idx] = Clone<T>(clone->children[idx]);
					}	
					return clone;
				}
	
			static void Delete(UIF::Component* component);
			static UIF::Component* Query_Hit(UIF::Component* component);

			UIF::Component* Add_Helper(UIF::HelperType helper_type, UIF::Invoker invoker);
			UIF::Component* Remove_Helper(UIF::HelperType helper_type, UIF::Invoker invoker);
		        UIF::Component* Add_Child(UIF::Component* component);
			UIF::Component* Remove_Child(UIF::Component* component);

			UIF::Component* Mod_Dst(UIF::Window* window, float x, float y, float w, float h);
			UIF::Component* Mod_Src(float x, float y, float w, float h);
			UIF::Component* Mod_Color(int16_t r, int16_t g, int16_t b);
			UIF::Component* Mod_Opacity(int16_t a);
			
			UIF::Component* Link_To_Texture(UIF::Component* component);

			float Get_Aspect();
			float Get_WinRatio();
			const ColoredFRect& Get_CFRect();
			
			uint32_t Get_ID();

			void Set_TVec_ID(uint32_t new_tvec_id);
			uint32_t Get_TVec_ID();

			void Set_HVec_ID(uint32_t new_hvec_id);
			uint32_t Get_HVec_ID();

			void Set_Active(bool new_active);
			bool Is_Active();
			void Set_Draw(bool new_need_draw);
			bool Need_Draw();

			const std::vector<UIF::Component*>& Get_Children();
		};

		class Workspace : public Component{
			private:
				using Component::Component;
				std::vector<UIF::Anchor*> anchors;
			public:
				void Set_Children_Draggable(UIF::Component* component);
				void Snap_Child_Anchor(UIF::Component* component, UIF::Anchor* Anchor); //Run on separate thread.
				void Resize_Component_Corner_Drag(UIF::Component* component); //Run on separate thread
				void Resize_Component_Side_Drag(UIF::Component* component); //Run on separate thread.
		};

		class Anchor : public Component{
			private:
				using Component::Component;	

			public:
				void Init(SDL_Color color, int w, int h);




		};
	        class Image : public Component{
			private:
				using Component::Component;
			public:

		};

		



		class CheckBox : public Component{
			private:
				using Component::Component;

			public:
				
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
				

		};


		class Button : public Component{
			private:
				using Component::Component;
			
		  	public:
		
			
		};

		class RadioButton : public Button{
		};


		class MessageBox : public Component{
			private:
				using Component::Component;

			public:
				void Init(std::string_view name);

		};

		class SideBar : public Component{
			private:
				using Component::Component;

			public:

		
		};

		class Tab : public Component{
			private:
				using Component::Component;
			public:	
				void Init(std::string_view name);


	
		};

		class TabBarContainer : public Component{
			private:
				using Component::Component;
				
				const float tab_width{};
				const float tab_height{};

				float tabs_x{};
				float tabs_y{}; 

			public:
				void Init(UIF::Window* window, float x, float y, float w, float h, int tab_no);
				void Add_Tab();


		};

		class TextSurface : public Component{

		};
};

#endif //COMPONENT_H
