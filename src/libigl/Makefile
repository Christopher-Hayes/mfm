CXX = g++

CXX_FLAGS = -fPIC -O3 -DNDEBUG -std=gnu++11

CXX_DEFINES = -DIGL_STATIC_LIBRARY
#-DTUTORIAL_SHARED_PATH=\"/Users/chris/mfm/code/libigl/cmake/../external/../tutorial/data\"

CXX_INCLUDES = -isystem ../../libigl/include -isystem ../../libigl/external/eigen -I../../libigl/external/glad/include -I../../libigl/external/glfw/include
#-isystem /Users/chris/mfm/code/libigl/cmake/../include -isystem /Users/chris/mfm/code/libigl/cmake/../external/eigen -I/Users/chris/mfm/code/libigl/external/glad/include -I/Users/chris/mfm/code/libigl/external/glfw/include -I/Users/chris/mfm/code/libigl/tutorial 

FRAMEWORKS = -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo 

LIBS = libigl.a libigl_opengl.a libigl_opengl_glfw.a /System/Library/Frameworks/OpenGL.framework/OpenGL.tbd libglad.a libglfw3.a


main:
	echo "Build object file"
	$(CXX) $(CXX_FLAGS) $(CXX_DEFINES) $(CXX_INCLUDES) -o main.cpp.o -c main.cpp
	echo "Create executable"
	$(CXX) $(CXX_FLAGS) $(CXX_DEFINES) $(CXX_INCLUDES) $(FRAMEWORKS) $(LIBS) main.cpp.o -o main

.PHONY: clean
clean:
	rm *.o
	rm main
