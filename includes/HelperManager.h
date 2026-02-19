#ifndef HELPERMANAGER_H
#define HELPERMANAGER_H


#include "Helper.h"
#include "NotificationBus.h" 

/* HELPERS:
 *  
 *
 *
 * */

namespace UIF{

	//Descriptor of what invokes an event.
	enum class Invoker{
		HOVER,
		CLICK,
		LONG_CLICK,
		DOUBLE_CLICK,
		RESIZE,
		INVOKER_COUNT
	};  

	//Abstraction specifically for this class, for readability.
	struct ArrOfHVec{
		std::array<std::vector<UIF::Helper*>, static_cast<long unsigned int>(UIF::Invoker::INVOKER_COUNT)> arr_vec;
	};

	class HelperManager{
		private: 
			std::vector<ArrOfHVec> component_helper_vec;	
			//Helper Lookup/List of Helpers
			std::array<UIF::Helper*, static_cast<long unsigned int>(UIF::HelperType::HELPER_COUNT)> helper_arr;
			
			//File I/O Helper
			UIF::LoadHelper load_helper { UIF::HelperType::LOAD, helper_arr };

			//Playback
			UIF::PlayBackHelper playback_helper { UIF::HelperType::PLAYBACK, helper_arr };

			//No-Op Helper
			UIF::DefaultHelper default_helper { UIF::HelperType::DEFAULT, helper_arr };

			//Text Input Helpers
			UIF::DefaultHelper TextInputHelper { UIF::HelperType::TEXT_INPUT, helper_arr };

			//Tooltip Helpers
			UIF::ToolTipHelper tooltip_helper { UIF::HelperType::TOOLTIP, helper_arr };

			//Move
			UIF::DragHelper drag_helper { UIF::HelperType::DRAG, helper_arr };

			//Cursor Helpers
			UIF::CaretCursorHelper caretcursor_helper { UIF::HelperType::CARET_CURSOR, helper_arr };
			UIF::MoveCursorHelper movecursor_helper { UIF::HelperType::MOVE_CURSOR, helper_arr };
			UIF::SelectCursorHelper selectcursor_helper { UIF::HelperType::SELECT_CURSOR, helper_arr };

			//Scaling Helpers
			UIF::ScaleByWidthHelper scalebywidth_helper { UIF::HelperType::SCALE_WIDTH, helper_arr };
			UIF::ScaleByHeightHelper scalebyheight_helper { UIF::HelperType::SCALE_HEIGHT, helper_arr };
			UIF::ScaleUniformHelper scaleuniform_helper { UIF::HelperType::SCALE_UNIFORM, helper_arr };

			//Fit Scales
			UIF::ScaleFitHelper scalefit_helper { UIF::HelperType::SCALE_FIT, helper_arr };
			UIF::ScaleFitHeightHelper scalefitheight_helper { UIF::HelperType::SCALE_FIT_HEIGHT, helper_arr };
			UIF::ScaleFitWidthHelper scalefitwidth_helper { UIF::HelperType::SCALE_FIT_WIDTH, helper_arr };
			
			std::vector<UIF::Helper*> invoker_helpers { &default_helper };

			UIF::Data::NotificationBus notifications;
 
			HelperManager();
			~HelperManager() = default;

			void Update();
			void Invoke(UIF::Component* component, UIF::Window* window, UIF::Invoker invoker);	

			HelperManager(const HelperManager&) = delete;
			HelperManager& operator=(const HelperManager&) = delete;

			friend WindowManager;
	};
};

#endif //HELPERMANAGER_H
