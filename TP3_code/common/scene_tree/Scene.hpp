#include "Camera.hpp"
#include <memory>
#include "gameObject.hpp"


class Scene{
    //Maybe for now no scene camera, and just limit it to do the camera in the main we'll see.
    Camera *sceneCamera;
    std::unique_ptr<GameObject> root;
};