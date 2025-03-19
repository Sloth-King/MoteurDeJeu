#include "Camera.hpp"
#include <memory>
#include "gameObject.hpp"


class Scene{
    Camera *sceneCamera;
    std::unique_ptr<GameObject> root;
};