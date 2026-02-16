#ifndef HELPERMANAGER_H
#define HELPERMANAGER_H


#include "Helper.h"
#include "NotificationBus.h" 

/* HELPERS:
 * LoadHelper-> "load" <- Opens a file explorer, retrieves file contents.
 * DefaultHelper -> "default" <- No behaviour.
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

	class HelperManager{
		private: 
		
			std::vector<std::unordered_map<UIF::Invoker, std::vector<UIF::Helper*>>> helper_vec;
			//Helper Lookup/List of Helpers 
			std::unordered_map<std::string, Helper*>lookup_helpers;

			//File I/O Helper
			UIF::LoadHelper load_helper { "load", lookup_helpers };

			//Playback
			UIF::PlayBackHelper playback_helper { "playback_helper", lookup_helpers };

			//No-Op Helper
			UIF::DefaultHelper default_helper { "default", lookup_helpers };

			//Text Input Helpers
			UIF::DefaultHelper TextInputHelper { "text_input", lookup_helpers };

			//Tooltip Helpers
			UIF::ToolTipHelper tooltip_helper { "tooltip", lookup_helpers };

			//Move
			UIF::DragHelper drag_helper { "drag", lookup_helpers };

			//Cursor Helpers
			UIF::CaretCursorHelper caretcursor_helper { "caret_cursor", lookup_helpers };
			UIF::MoveCursorHelper movecursor_helper { "move_cursor", lookup_helpers };
			UIF::SelectCursorHelper selectcursor_helper { "select_cursor", lookup_helpers };

			//Scaling Helpers
			UIF::ScaleByWidthHelper scalebywidth_helper { "scale_width", lookup_helpers };
			UIF::ScaleByHeightHelper scalebyheight_helper { "scale_height", lookup_helpers };
			UIF::ScaleUniformHelper scaleuniform_helper { "scale_uniform", lookup_helpers };
			UIF::ScaleFitHelper scalefit_helper { "scale_fit", lookup_helpers };
			
			std::vector<UIF::Helper*> invoker_helpers { &default_helper };
			const std::unordered_map<UIF::Invoker, std::vector<UIF::Helper*>>invoker_defaults {
				{ UIF::Invoker::HOVER, invoker_helpers }, 
			 	{ UIF::Invoker::CLICK, invoker_helpers },
				{ UIF::Invoker::LONG_CLICK, invoker_helpers },
				{ UIF::Invoker::DOUBLE_CLICK, invoker_helpers },
				{ UIF::Invoker::RESIZE, invoker_helpers} };

			UIF::Data::NotificationBus notifications;
 
			HelperManager() = default;
			~HelperManager() = default;

			void Update();
			void Invoke(UIF::Component* component, UIF::Window* window, UIF::Invoker invoker);	

			HelperManager(const HelperManager&) = delete;
			HelperManager& operator=(const HelperManager&) = delete;

			friend WindowManager;
	};
};

#endif //HELPERMANAGER_H
