#include "HelperManager.h"
#include "Component.h"

void UIF::HelperManager::Invoke(UIF::Component* component, UIF::Window* window, UIF::Invoker invoker){
	for(auto* helper : this->helper_vec[component->Get_HVec_ID()][invoker]){
		helper->Execute(component, window);
	}
}

void UIF::HelperManager::Update(){
	this->notifications.Check(UIF::Data::DataLine::COMPONENT_LINE);
	this->notifications.Check(UIF::Data::DataLine::HELPER_LINE);

	for(auto& component : this->notifications.Read_Comp()){
		this->helper_vec.emplace_back(this->invoker_defaults);
		component->Set_HVec_ID(this->helper_vec.size() - 1);
	}
	for(auto& pkg: notifications.Read_Helper()){
		if(pkg.helper_op == UIF::Data::HelperOp::ADD_HELPER){
 			this->helper_vec[pkg.component->Get_HVec_ID()][pkg.invoker].emplace_back(this->lookup_helpers[pkg.helper]);
		}
		else{
			UIF::ContainerTargetErase(this->helper_vec[pkg.component->Get_HVec_ID()][pkg.invoker], this->lookup_helpers[pkg.helper]);
		}
	}
	this->notifications.Clear();
}
