#include "HelperManager.h"
#include "Component.h"

UIF::HelperManager::HelperManager(){
	component_helper_vec.reserve(UIF::Data::default_reserve);
}

void UIF::HelperManager::Invoke(UIF::Component* component, UIF::Window* window, UIF::Invoker invoker){
	for(auto* helper : this->component_helper_vec[component->Get_HVec_ID()].arr_vec[static_cast<int>(invoker)]){
		helper->Execute(component, window);
	}
	if(invoker == UIF::Invoker::RESIZE){
		for(auto* child : component->Get_Children()){
			Invoke(child, window, invoker);
		}
	}
}

void UIF::HelperManager::Update(){
	this->notifications.Check(UIF::Data::DataLine::COMPONENT_LINE);
	this->notifications.Check(UIF::Data::DataLine::HELPER_LINE);

	//Allocate the Component an ArrOfVec - to which they are bound by HVec_ID which indexes component_helper_vec - containing a Vec of Helpers.
	for(auto& component : this->notifications.Read_Comp()){
		ArrOfVec arr_of_vec{};
		for(int idx{}; idx < arr_of_vec.arr_vec.size() - 1; idx++){
			arr_of_vec.arr_vec[idx] = invoker_helpers;
		}
		this->component_helper_vec.emplace_back(arr_of_vec);
		component->Set_HVec_ID(static_cast<uint32_t>(this->component_helper_vec.size() - 1));
	}
	for(auto& pkg: notifications.Read_Helper()){
		if(pkg.helper_op == UIF::Data::HelperOp::ADD_HELPER){
 			this->component_helper_vec[pkg.component->Get_HVec_ID()].arr_vec[static_cast<int>(pkg.invoker)] //Get the Component's vector of Helpers.
				.emplace_back(this->helper_arr[static_cast<int>(pkg.helper_type)]); //'pkg.invoker = Index for the vector containing the relevant behaviour
		}
		else{
			UIF::ContainerTargetErase(this->component_helper_vec[pkg.component->Get_HVec_ID()] //Get the Component's vector of Helpers.
					.arr_vec[static_cast<int>(pkg.invoker)], this->helper_arr[static_cast<int>(pkg.helper_type)]); //pkg.invoker = Index for the Vector containing the relevant behaviour.
		}
	}
	this->notifications.Clear();
}
