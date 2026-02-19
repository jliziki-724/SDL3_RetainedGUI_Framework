#ifndef UIFEDITOR_H
#define UIFEDITOR_H

#include "UIFramework.h"

//Bulk of the code of the Editor UI
//Example of Lib use
namespace UIFEditor{
 

	

	void Configure(){

		//Abstract all of this away;
		UIF::Data::global_bus = new UIF::Data::GlobalBus();
		UIF::tex_cache = new UIF::TextureCache();
		UIF::WindowManager mgr;
		//Windows Here.
		mgr.Create_Window("UIFEditor", 1280, 720);	

		//Assets Here.	
		auto* workspace{ UIF::Component::Create<UIF::Workspace>("", mgr["UIFEditor"]) };
		workspace->Add_Child( UIF::Component::Create<UIF::TabBarContainer>("", mgr["UIFEditor"], 0, 0, 1, 30)
				->Add_Helper(UIF::HelperType::SCALE_FIT_WIDTH, UIF::Invoker::RESIZE) );



		mgr.Add_Component(workspace, mgr["UIFEditor"]);
		mgr.Run();

		delete UIF::Data::global_bus;
		delete UIF::tex_cache;
	}



};


#endif //UIFEDITOR_H
