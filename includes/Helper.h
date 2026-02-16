#ifndef HELPER_H
#define HELPER_H

#include "Component.h"
#include <cmath>

namespace UIF
{
	class Helper{
		protected:
		 	Helper(const std::string& helper, std::unordered_map<std::string, Helper*>& helpers){ 
				helpers[helper] = this;
			}
		public:
       			virtual void Execute(UIF::Component* component, UIF::Window* window) = 0;	
			virtual ~Helper() = default;

			friend HelperManager;
	};


	//A Dummy Helper for components without behaviour. 
	class DefaultHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component,  UIF::Window* window) override{
		}
	};

	class LoadHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component, UIF::Window* window) override{
			
			}
	};

	class PlayBackHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component*  component, UIF::Window* window) override{
			}
	};

	//Layout Helpers
	class ScaleFitHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component, UIF::Window* window) override{
				component->Mod_Dst(window, component->Get_CFRect().dst_frect->x, component->Get_CFRect().dst_frect->y, 
					static_cast<float>(window->Get_Dimensions().w),
					static_cast<float>(window->Get_Dimensions().h));
			}
	};

	class ScaleUniformHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component*  component, UIF::Window* window) override{
				//Reduce the window by the factor given by the Component's 'WinRatio' <- The area the component must occupy relative to the window.
				//Then distribute the reduced area according to the aspect ratio.
				const float scale_target = (static_cast<float>(window->Get_Dimensions().w) * static_cast<float>(window->Get_Dimensions().h)) / component->Get_WinRatio();
				component->Mod_Dst(window, component->Get_CFRect().dst_frect->x, component->Get_CFRect().dst_frect->y,
						std::sqrt(scale_target * component->Get_Aspect()), std::sqrt(scale_target / component->Get_Aspect()));
			}
	};

	//CONSIDER: Is ScaleByWidth a fixed arbitrary percentage or user defined?
	class ScaleByWidthHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component,  UIF::Window* window) override{
			}
	};

	//CONSIDER: Is ScaleByHeight a fixed arbitrary percentage or user defined?
	class ScaleByHeightHelper : public Helper{
		using Helper::Helper;
		virtual void Execute(UIF::Component* component, UIF::Window* window) override{

		}
	};

  	//Cursor Helpers
	class CaretCursorHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component, UIF::Window* window) override{
			}
	};
	
	class SelectCursorHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component, UIF::Window* window) override{
			}
	};

	class MoveCursorHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component,  UIF::Window* window) override{
			}
	};


	//Text Input Helpers
	class WriteTextHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* omponent, UIF::Window* window) override{
			}
	};

	//ToolTip Helpers
	class ToolTipHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component, UIF::Window* window) override{
			}
	};

	class DragHelper : public Helper{
		public:
			using Helper::Helper;
			virtual void Execute(UIF::Component* component, UIF::Window* window){
				//component->Mod_Dst(x, y, component->Get_FRect().w, component->Get_FRect().h);
			}
	};
};


#endif //HELPER_H
