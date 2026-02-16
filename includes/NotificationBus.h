#ifndef NOTIFICATIONBUS_H
#define NOTIFICATIONBUS_H

#include "UIFCore.h"
#include "GlobalBus.h"

namespace UIF{

	namespace Data{ 

		class NotificationBus{
			private:
				std::vector<UIF::Component*>comp_notifications; 
				std::vector<UIF::Data::HelperPkg>helper_notifications;
			public:
				void Check(UIF::Data::DataLine d_line);
				void Clear();
				const std::vector<UIF::Component*>& Read_Comp();
				const std::vector<UIF::Data::HelperPkg>& Read_Helper();

				NotificationBus() = default;
				~NotificationBus() = default;

				NotificationBus(const NotificationBus&) = delete;
				NotificationBus& operator=(const NotificationBus&) = delete;
		};
	}
}

#endif //NOTIFICATIONBUS_H
