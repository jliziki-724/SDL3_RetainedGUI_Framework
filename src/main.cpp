
#include "UIFEditor.h"


bool Init();

int main(){

	if(Init()){
		 UIFEditor::Configure();
 	}
 
 	SDL_Quit();
	return 0;
}

bool Init(){
	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
		SDL_Log("SDL failed to initialize. SDL error: %s\n", SDL_GetError());
		return false;
	}
	
	return true;
}


