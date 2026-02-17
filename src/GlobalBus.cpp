#include "GlobalBus.h"

UIF::Data::GlobalBus::GlobalBus(){
	helper_line.reserve(default_reserve);
	component_line.reserve(default_reserve);
}

void UIF::Data::GlobalBus::Add_ComponentLine(UIF::Component* component){
	component_line.emplace_back(component);
}

void UIF::Data::GlobalBus::Add_HelperLine(UIF::Component* component, UIF::Invoker invoker, UIF::HelperType helper_type, UIF::Data::HelperOp helper_op){
	HelperPkg helper_pkg{ component, invoker, helper_op, helper_type };
	helper_line.emplace_back(helper_pkg);
}

void UIF::Data::GlobalBus::Pull_ComponentLine(std::vector<UIF::Component*>& target){
	UIF::ContainerDrainToTarget(component_line, target);
}

void UIF::Data::GlobalBus::Pull_HelperLine(std::vector<HelperPkg>& target){
	UIF::ContainerDrainToTarget(helper_line, target);
}
