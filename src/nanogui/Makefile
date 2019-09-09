# CURRENTLY MAC OS ONLY

CXX = g++

CXX_FLAGS = -fvisibility=hidden  -Wall -Wextra -std=c++14 -flto=thin -stdlib=libc++ -O3 -DNDEBUG   -fobjc-arc

CXX_DEFINES = -DCORO_SJLJ -DGLAD_GLAPI_EXPORT -DNANOGUI_PYTHON -DNANOGUI_SHARED -DNVG_SHARED

CXX_INCLUDES = -I../../nanogui/ext/eigen -I../../nanogui/ext/glfw/include -I../../nanogui/ext/nanovg/src -I../../nanogui/include -I../../nanogui/build -I../../nanogui/ext/coro -I../../nanogui/ext/pybind11/include -I/Library/Frameworks/Python.framework/Versions/3.6/include/python3.6m 

FRAMEWORKS = -rpath ../../nanogui/build libnanogui.dylib -framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit 

main:
	echo "Create object file"
	$(CXX) $(CXX_FLAGS) $(CXX_DEFINES) $(CXX_INCLUDES) -o mfm_gui.cpp.o -c mfm_gui.cpp
	echo "Create executable"
	$(CXX) $(CXX_FLAGS) $(CXX_DEFINES) $(CXX_INCLUDES) $(FRAMEWORKS) mfm_gui.cpp.o -o mfm_gui

.PHONY: clean
clean:
	rm *.o
	rm mfm_gui
