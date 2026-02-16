#include "NotificationBus.h"

void UIF::Data::NotificationBus::Check(UIF::Data::DataLine d_line){
	switch(d_line){
		case UIF::Data::DataLine::COMPONENT_LINE:
			Data::global_bus->Pull_ComponentLine(comp_notifications);
			break;

		case UIF::Data::DataLine::HELPER_LINE:
			Data::global_bus->Pull_HelperLine(helper_notifications);
			break;

		default:
			break;
	}
}	

void UIF::Data::NotificationBus::Clear(){
	this->comp_notifications.clear();
	this->helper_notifications.clear();
}

const std::vector<UIF::Component*>& UIF::Data::NotificationBus::Read_Comp(){
	return this->comp_notifications;
}

const std::vector<UIF::Data::HelperPkg>& UIF::Data::NotificationBus::Read_Helper(){
	return this->helper_notifications;
}
