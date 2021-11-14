prog: main.o Matrix.o TriMesh.o Point.o Triangle.o Loader.o Camera.o Clipper.o Shader.o Renderer.o
	g++ -Wall -Werror -g -o tolima main.o Camera.o Matrix.o TriMesh.o Point.o Triangle.o Loader.o Clipper.o Shader.o Renderer.o Scene.o Object.o -lSDL2main -lSDL2

main.o : main.cpp TriMesh.o Matrix.o Loader.o Camera.o Clipper.o Shader.o Renderer.o Scene.o
	g++ -Wall -g -c main.cpp -lSDL2main -lSDL2

Renderer.o : Renderer.cpp TriMesh.o Camera.o Clipper.o Shader.o
	g++ -Wall -g -c Renderer.cpp

Camera.o: Camera.cpp Point.o Matrix.o TriMesh.o
	g++ -Wall -g -c Camera.cpp 

Clipper.o: Point.o Triangle.o TriMesh.o
	g++ -Wall -g -c Clipper.cpp

Matrix.o : Matrix.cpp Point.o
	g++ -Wall -g -c Matrix.cpp 

Loader.o : Loader.cpp TriMesh.o Object.o
	g++ -Wall -g -c Loader.cpp

Scene.o : Scene.cpp Object.o
	g++ -Wall -g -c Scene.cpp

Object.o : Object.cpp TriMesh.o Shader.o
	g++ -Wall -g -c Object.cpp

Shader.o: Shader.cpp Point.o Triangle.o TriMesh.o
	g++ -Wall -g -c Shader.cpp 

TriMesh.o: TriMesh.cpp Triangle.o
	g++ -Wall -g -c TriMesh.cpp

Triangle.o: Triangle.cpp Point.o
	g++ -Wall -g -c Triangle.cpp

Point.o:
	g++ -Wall -g -c Point.cpp

