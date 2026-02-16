#ifndef UIFCORE_H
#define UIFCORE_H

#include <typeinfo>
#include <memory>
#include <limits>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace UIF{
	//Editing Interface
	class Workspace;
	class Mouse; // <- Customisable mouse which follows the system cursor.

	//Textures
	class TextureCache;
	inline TextureCache* tex_cache{ nullptr };

	//Windows
	class WindowManager;
	class Window;

	//Render
	class Render;

	//Components
	class Anchor;
	class MessageBox;
	class Component;
	class ChildComponent;
	class CheckBox;
	class Button;
	class DropMenu;
	class RadioButton;
	class SideBar;
	class ScrollBar;
	class TabBar;
	class TextSurface;
	class Image;
	class ToolTip;


	//General Helpers
	class HelperManager;
	class Helper;
	class LoadHelper;
	class DefaultHelper;
	class PlayBackHelper;
	class ToolTipHelper; // <- Display a component's associated tooltip
	class DragHelper;

	//Cursor Helpers <- Graphical Views of a Behaviour, not modifications of the cursor behaviour itself.
	class CaretCursorHelper;
	class SelectCursorHelper; 
	class MoveCursorHelper; 

	//TextBox
	class WriteTextHelper;

	//Layout Helpers
	class ScaleByWidthHelper;
	class ScaleByHeightHelper;
	class ScaleUniformHelper;
	class ScaleFitHelper;

	//Utility Functions - Erasure/Deletion of Vectors/Deques/Map elements/Draining to Targets
	//Could make use of reflections <- i.e. shrink_to_fit/size, etc

	template<typename T, typename U>
		void ContainerShrinkOnResize (T& iterable_container, U size_snapshot){
			if(size_snapshot > iterable_container.size()){
				iterable_container.shrink_to_fit();
			}
		}
	template<typename T, typename U>
		inline void ContainerTargetErase(T& iterable_container, U target){
			const size_t size_snapshot { iterable_container.size() }; //Use to determine whether to shrink.
			for(size_t idx{}; idx < iterable_container.size(); idx++){
				if(iterable_container[idx] == target){
					iterable_container.erase(iterable_container.begin() + idx);
					break;
				}
			}
			ContainerShrinkOnResize(iterable_container, size_snapshot);
		}
	template<typename T, typename U>
		inline void ContainerTargetEraseAndDelete(T& iterable_container, U target){
			const size_t size_snapshot { iterable_container.size() };
			for(size_t idx{}; idx < iterable_container.size(); idx++){
				if(iterable_container[idx] == target){
					delete target;
					iterable_container.erase(iterable_container.begin() + idx);
					break;
				}
			}
			ContainerShrinkOnResize(iterable_container, size_snapshot);
		}
	template<typename T, typename U> // <- Generalize drains on the global bus.
		inline void ContainerDrainToTarget(T& iterable_container, U& target){
			const size_t size_snapshot { iterable_container.size() };
			for(size_t idx{}; idx < iterable_container.size(); idx++){
				target.emplace_back(iterable_container[idx]);
				iterable_container.erase(iterable_container.begin() + idx);
				
				idx--; 
			}
			ContainerShrinkOnResize(iterable_container, size_snapshot);
		}
	template<typename T>
		inline void ContainerEraseAndDeleteAll(T& iterable_container){
			while(!iterable_container.empty()){
				delete iterable_container.back();
				iterable_container.pop_back();
			}
			iterable_container.pop_back();
			iterable_container.shrink_to_fit();
		}
	template<typename T> 
		inline void ContainerEraseAll(T& iterable_container){
			iterable_container.clear();
			iterable_container.shrink_to_fit();
		}

	//Auxiliary types	
	enum class Invoker;
	struct Dimensions;
	struct ColoredFRect;

	namespace Data{
		enum class Recipient;
		enum class DataLine;
		enum class HelperOp;

		struct HelperPkg;

		class NotificationBus;
		class GlobalBus;
	
		constexpr int default_reserve { 256 };
		inline GlobalBus* global_bus{ nullptr };
	};



};


#endif //UIFCORE_H
