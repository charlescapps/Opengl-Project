default: src/Project2/*
	g++ -o main src/Project2/*.cpp src/Project2/*.c -lGLU -lfltk -lXext -lX11 -lm `fltk-config --use-glut --use-gl --ldflags --cxxflags`

