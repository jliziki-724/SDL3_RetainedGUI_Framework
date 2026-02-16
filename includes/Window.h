#ifndef WINDOW_H
#define WINDOW_H



#include "UIFCore.h"

namespace UIF{
         
	struct Dimensions{
		int w{};
		int h{};
	};

	class Window{
	 private:
		SDL_Window* window{ nullptr };
    		SDL_Renderer* renderer { nullptr };
		SDL_WindowID id{};

		Dimensions win_dim{};
	        uint64_t priority{}; //Precedence based on timestamp 
		int frame_rate{ 60 };

		const uint32_t CVec_ID{};
		const std::string& title{};

		bool is_active{};
		bool Is_Init() const;

		void Set_Priority(uint64_t t_stamp);
		void Set_Active(bool new_active);
		void Set_Framerate(int new_Framerate);
		void Update_Dimensions();

		Window(uint32_t cvec_id, const std::string& t, int w, int h, int flag);
	 public:

		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete; 

  		SDL_Window* Get_Window();
		SDL_Renderer* Get_Renderer();
		SDL_WindowID Get_ID();

		int Get_FPS();
		bool Is_Active();

		uint32_t Get_CVec_ID();
		uint64_t Get_Priority();

		const std::string& Get_Title();
		const UIF::Dimensions& Get_Dimensions();

		friend WindowManager;
	};

};

#endif //WINDOW_H
