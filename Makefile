prog: main.o Matrix.o TriMesh.o Point.o Triangle.o Loader.o Camera.o Clipper.o Shader.o Renderer.o
	g++ -std=c++17 -Wall -Werror -g -pg -o tolima main.o Camera.o Matrix.o TriMesh.o Point.o Triangle.o Loader.o Clipper.o Shader.o Renderer.o Scene.o Object.o -lSDL2main -lSDL2

main.o : main.cpp TriMesh.o Matrix.o Loader.o Camera.o Clipper.o Shader.o Renderer.o Scene.o
	g++ -std=c++17 -Wall -g -pg -c main.cpp -lSDL2main -lSDL2

Renderer.o : Renderer.cpp TriMesh.o Camera.o Clipper.o Shader.o
	g++ -std=c++17 -Wall -g  -pg -c Renderer.cpp

Camera.o: Camera.cpp Point.o Matrix.o TriMesh.o
	g++ -std=c++17 -Wall -g  -pg -c Camera.cpp 

Clipper.o: Point.o Triangle.o TriMesh.o
	g++ -std=c++17 -Wall -g -pg -c Clipper.cpp

Matrix.o : Matrix.cpp Point.o
	g++ -std=c++17 -Wall -g -pg -c Matrix.cpp 

Loader.o : Loader.cpp TriMesh.o Object.o
	g++ -std=c++17 -Wall -g -pg -c Loader.cpp

Scene.o : Scene.cpp Object.o
	g++ -std=c++17 -Wall -g -pg -c Scene.cpp

Object.o : Object.cpp TriMesh.o Shader.o
	g++ -std=c++17 -Wall -g -pg -c Object.cpp

Shader.o: Shader.cpp Point.o Triangle.o TriMesh.o
	g++ -std=c++17 -Wall -g -pg -c Shader.cpp 

TriMesh.o: TriMesh.cpp Triangle.o
	g++ -std=c++17 -Wall -g -c TriMesh.cpp

Triangle.o: Triangle.cpp Point.o
	g++ -std=c++17 -Wall -g -c Triangle.cpp

Point.o:
	g++ -std=c++17 -Wall -g -c Point.cpp

