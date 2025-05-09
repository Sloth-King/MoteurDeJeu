#include "engine/physics/physics_server/physics_server.h"
#include "engine/core/core_components/components.h"

#include <utility> //for swap

// All maths and logic behind the intersections are from here : 
// https://github.com/IainWinter/IwEngine/blob/3e2052855fea85718b7a499a7b1a3befd49d812b/IwEngine/include/iw/physics/impl/TestCollision.h#L25


// Collision detection functions (Maybe move them to be somewhere else idk yet)

// Sphere Vs Sphere intersection
intersectionData intersectionSphereSphere(const Collider *a, const Collider *b){

    assert(a->type == ColliderType::SPHERE && b->type == ColliderType::SPHERE);

    const SphereCollider* sphere_a = static_cast<const SphereCollider*>(a);
    const SphereCollider* sphere_b = static_cast<const SphereCollider*>(b);

    intersectionData res;

    glm::vec3 aCenter = sphere_a->center;
    glm::vec3 bCenter= sphere_b->center;

    float aRadius = sphere_a->radius;
    float bRadius = sphere_b->radius; 

    glm::vec3 ab = bCenter - aCenter;

    float distance = glm::length(ab);

    if (distance < 0.0001f || distance > aRadius + bRadius){
        res.isIntersection = false;
        return res;
    }

    glm::vec3 normal = glm::normalize(ab);
    glm::vec3 intersectionA = aCenter + normal * aRadius;
    glm::vec3 intersectionB = bCenter - normal * bRadius;

    std::cout << "Intersection : Sphere Sphere" << std::endl;

    res.isIntersection = true;
    res.intersectionNormal = normal;
    res.intersectionPointA = intersectionA;
    res.intersectionPointB = intersectionB;

    return res;
}   


intersectionData intersectionSpherePlane(const Collider* a, const Collider* b){

    assert(a->type == ColliderType::SPHERE && b->type == ColliderType::PLANE);

    intersectionData res;

    const SphereCollider* sphere = static_cast<const SphereCollider*>(a);
    const PlaneCollider* plane = static_cast<const PlaneCollider*>(b);

    glm::vec3 sphereCenter = sphere->center;
    float sphereRadius = sphere->radius;

    glm::vec3 planeNormal = plane->normal;
    float planeDistance = plane->distance;
    glm::vec3 onPlane = planeNormal * planeDistance; // + converted to world pos //TODO


    float distance = glm::dot(sphereCenter - onPlane, planeNormal);

    if(distance > sphereRadius){
        res.isIntersection = false;
        return res;
    }

    glm::vec3 intersectionPointA = sphereCenter - planeNormal * distance;
    glm::vec3 intersectionPointB = sphereCenter - planeNormal * sphereRadius;

    std::cout << "Intersection : Sphere Plane" << std::endl;
    res.isIntersection = true;
    res.intersectionPointA = intersectionPointA;
    res.intersectionPointB = intersectionPointB;
    res.intersectionNormal = planeNormal;

    return res;
}

// FIXME : This might be useless maybe remove
intersectionData intersectionPlanePlane(const Collider* a, const Collider* b){

    assert(a->type == ColliderType::PLANE && b->type == ColliderType::PLANE);


    const PlaneCollider* aPlane = static_cast<const PlaneCollider*>(a);
    const PlaneCollider* bPlane = static_cast<const PlaneCollider*>(b);

    intersectionData res;

    glm::vec3 aNormal = aPlane->normal;
    glm::vec3 bNormal = bPlane->normal;

    float aDistance = aPlane->distance;
    float bDistance = bPlane->distance;

    glm::vec3 direction = glm::cross(aNormal, bNormal);

    if (glm::length(direction) < 0.0001f) {
        res.isIntersection = false;
        return res;
    }

    // Find a point on the line of intersection
    float det = glm::dot(aNormal, glm::cross(bNormal, direction));
    if (fabs(det) < 0.0001f) {
        res.isIntersection = false;
        return res;
    }

    glm::vec3 pointOnLine = ((bDistance * glm::cross(direction, aNormal)) +
                             (aDistance * glm::cross(bNormal, direction))) / det;

    std::cout << "Intersection : Plane Plane" << std::endl;

    res.isIntersection = true;
    res.intersectionNormal = glm::normalize(direction);
    res.intersectionPointA = pointOnLine;
    res.intersectionPointB = pointOnLine; 

    return res;
}

/////////////////////////////////////////////////////////////////////////////

std::vector<intersectionData> PhysicsServer::computeCollisions(){
    std::vector<intersectionData> intersectionList;

    // Check this out to understand this
    // https://winter.dev/articles/physics-engine

    using FindContactFunc = intersectionData(*)(const Collider*, const Collider*);   
    
    static const FindContactFunc functionTables[2][2] = {
            {intersectionSphereSphere, intersectionSpherePlane},
            {nullptr, intersectionPlanePlane}
    };

    for(const auto& objectA : Objects){
        if(!objectA->hasComponent<C_Collider>() || !objectA->hasComponent<C_RigidBody>()) continue;
        for(const auto& objectB : Objects){

            //Check if the object has the right components AND that they're not the same object
            if((!objectB->hasComponent<C_Collider>() || !objectB->hasComponent<C_RigidBody>()) && objectA == objectB) continue;

            intersectionData intersection;

            //check if it can work in our table, if not, flip it

            Collider* colliderA = &objectA->getComponent<C_Collider>()->collider;
            Collider* colliderB = &objectB->getComponent<C_Collider>()->collider;

            bool swap = (objectB->getComponent<C_Collider>()->collider.type) < (objectA->getComponent<C_Collider>()->collider.type);

            //flip it (or not)
            if(swap){

                //safety check
                auto func = functionTables[colliderB->type][colliderA->type];
                if (!func){
                    std::cout << "func[" << colliderB->type << "][" << colliderA->type << "] does not exist" << std::endl;
                    continue;  
                }               

                intersection = func(colliderB, colliderA);
                intersection.objectA = objectB;
                intersection.objectB = objectA;
            } else {

                //same safety check
                auto func = functionTables[colliderA->type][colliderB->type];
                if (!func){
                    std::cout << "func[" << colliderA->type << "][" << colliderB->type << "] does not exist" << std::endl;
                    continue;  
                }

                intersection = func(colliderA, colliderB);
                intersection.objectA = objectA;
                intersection.objectB = objectB;
            }

            intersectionList.push_back(intersection);
        }
    }    
    return intersectionList;
}

void PhysicsServer::solveCollisions(std::vector<intersectionData> collisions){
    for(const auto& collision : collisions){

        // TODO : Manage static bodies
        // idea : set them to default values like velocity = 0 at every collision maybe ? 
        

        // TODO : Two rigid body collisions : 

        //TEMP naive version just to test if it actually works i guess
        glm::vec3 moveVector = collision.intersectionPointB - collision.intersectionPointA;
        collision.objectA->getComponent<C_Transform>()->move(moveVector);
    }
}

void PhysicsServer::addObject(GameObject* go){
    Objects.insert(go);
}

void PhysicsServer::removeObject(GameObject* go){
    Objects.erase(go);
}

void PhysicsServer::applyPhysics(float deltatime){
    std::cout << "hello" << std::endl;
    int cpt = 0;
    for(const auto& object : Objects){
        std::cout << "Object type : " << object->getComponent<C_Collider>()->collider.type << std::endl;
        if(object->getComponent<C_Collider>()->collider.type == SPHERE){
            cpt++;
            object->getComponent<C_Transform>()->move(-gravity*deltatime);
        }
    }
    std::cout << "Num spheres : " << cpt << std::endl;
}

void PhysicsServer::step(float deltatime){
    
    std::vector<intersectionData> collisions = computeCollisions();
    solveCollisions(collisions);
    applyPhysics(deltatime);

}
