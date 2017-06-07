#include "Scene.h"

Scene::Scene()
    : _counter { 0 }
{}


Scene::~Scene() {
	for (auto& E : _entities) {
        if (E.second) {
            delete E.second;
            E.second = nullptr;
		}
	}
}


void Scene::update(int timestep) {
    for (auto& E : _entities) {
       E.second->update(timestep);
    }
}


void Scene::drawShapes(QOpenGLShaderProgram* shader) {
    for (auto& S : _shapes) {
        S->draw(shader);
	}
}

void Scene::drawLights(QOpenGLShaderProgram* shader) {
    for (auto& L : _lights) {
        L->draw(shader);
    }
}


void Scene::add(GameEntity* ent, const std::string& name) {
	
	Q_ASSERT(ent); 	//we really do not want a nullptr here

	//make sure we have a unique Name:
	if (_entities.find(name) != _entities.end()) {
            qDebug() << "Name already exists:";
            qDebug() << name.c_str();
            qDebug() << "List of all entities: ";
            for (auto& E : _entities) qDebug() << E.first.c_str();
			throw SceneError{ ("<Scene::add> An entitiy with the name " + name + " already exists.").c_str()  };
	}

	//if no name was assigned by the caller, give our own:
	std::string entName = name;
	if (entName == "") {
		entName = "Entity" + std::to_string(_counter++);
	}

	//insert the entity in it respective list:
	addToList(ent);

	//and in the map:
    _entities.insert(std::make_pair(entName, ent));
}


void Scene::remove(const std::string& name) {
	
	GameEntity* ent = get(name);
	removeFromList(ent);
	
	delete ent;
	ent = nullptr;

	_entities.erase(name);	
}


GameEntity* Scene::get(const std::string& name) {
	
	auto ent = _entities.find(name);
	if (ent == _entities.end()) throw SceneError{ ("<Scene::get> Did not find Game Entity named " + name).c_str() };
    return ent->second;
}

std::vector<GameEntity*> Scene::tagObstacles(GameEntity *center, float radius) {

    std::vector<GameEntity*> tagged;
    float rSquared = radius * radius;

    for (auto& E : _entities) {
        if (E.second == center) continue; //do not self-tag
        QVector3D dir = E.second->transform()->pos() - center->transform()->pos();
        if (dir.lengthSquared() <= rSquared) {
            tagged.push_back(E.second);
        }
    }
    return tagged;
}


void Scene::addToList(GameEntity* ent) {
	switch (ent->type()) {
		case EntType::base:
		case EntType::moving:
			_shapes.push_back(ent);
			break;
		case EntType::light:
			_lights.push_back(ent);
			break;
		default:
			throw SceneError{ "<Scene::addToList> Unknown Entity type!"  };
	}
}


void Scene::removeFromList(GameEntity* ent) {
	switch (ent->type()) {
		case EntType::base:
		case EntType::moving:
			_shapes.erase(std::remove(_shapes.begin(), _shapes.end(), ent), _shapes.end());
			break;
		case EntType::light:
			_lights.erase(std::remove(_lights.begin(), _lights.end(), ent), _lights.end());
			break;
		default:
			throw SceneError{ "<Scene::removeFromList> Unknown entity type! " };
	}
}
