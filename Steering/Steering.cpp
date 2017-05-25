#include "Steering.h"

Steering::Steering(Scene* scene) 
    : _wanderOn { false },
      _scene	{ scene }
{
	Q_ASSERT(_scene);
}

Steering::~Steering() {

}
