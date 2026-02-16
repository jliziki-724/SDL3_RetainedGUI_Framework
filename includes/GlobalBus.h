#ifndef GLOBALBUS_H
#define GLOBALBUS_H

#include "UIFCore.h"

namespace UIF{
	
	namespace Data{

		enum class DataLine{
			COMPONENT_LINE,
			HELPER_LINE,
			AUDIO_LINE // <- Add in future... Making use of FFMPEG or... SDL_Mixer
		};

		enum class HelperOp{
			ADD_HELPER,
			REMOVE_HELPER
		};

		struct HelperPkg{
		        UIF::Component* component{ nullptr };
			UIF::Invoker invoker{};
			UIF::Data::HelperOp helper_op{};
			std::string helper{};
		};

	class GlobalBus{
		private:
			std::vector<UIF::Component*> component_line;
			std::vector<HelperPkg> helper_line;

		public:

			GlobalBus();
			~GlobalBus() = default;
			GlobalBus(const GlobalBus&) = delete;
			GlobalBus& operator=(const GlobalBus&) = delete;

		//	template<typename T>
			//	bool Report(T* ptr, ...){
			//	}
				
			void Add_ComponentLine(UIF::Component* component);
			void Add_HelperLine(UIF::Component* component, UIF::Invoker invoker, const std::string& helper, UIF::Data::HelperOp helper_op);

			void Pull_ComponentLine(std::vector<UIF::Component*>& target);
			void Pull_HelperLine(std::vector<HelperPkg>& target);

		};	
	};

	
};

#endif //GLOBALBUS_H
