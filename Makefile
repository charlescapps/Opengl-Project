default: src/Project2/*
	g++ -g -o main src/Project2/*.cpp src/Project2/*.c -lGLU -lfltk -lXext -lX11 -lm  -lobj `fltk-config --use-glut --use-gl --ldflags --cxxflags`

