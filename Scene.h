/* 	Class Scene

	Keeps track of all GameEntities and their names.
	Will be used by GameEntities to determine their surroundings.
	Also, this class is responsible for deleting the GameEntities from Memory.
*/


#pragma once

#include <string>
#include <map>
#include <exception>
#include <QOpenGLShaderProgram>

#include "Entities/gameentity.h"
#include "Entities/light.h"

//small class which can be thrown as Exception:
class SceneError : public std::exception {
public:
	SceneError(const char* message) : _msg{ message } {}

private:
	const char* _msg;	
};

class Scene {
public:
	Scene();
	~Scene();

	void 	update	(int timestep);
    void 	drawShapes(QOpenGLShaderProgram* shader);
    void    drawLights(QOpenGLShaderProgram* shader);

	void 	add(GameEntity* entity, const std::string& name = "");
	void 	remove(const std::string& name);

	//get a pointer to a game Entity with a given name:
	GameEntity* get(const std::string& name);

	//overload operators [] for convenience:
	GameEntity* operator [] (const std::string& name) {
		return get(name);
	}

private:
	void addToList(GameEntity* ent);
	void removeFromList(GameEntity* ent);

	std::map<std::string, GameEntity*> _entities;
	
	//seperate lists for seperate drawing:
	std::vector<GameEntity*> 	_shapes;
	std::vector<GameEntity*>	_lights;
	
	int _counter; //helper variable for auto-naming
};
