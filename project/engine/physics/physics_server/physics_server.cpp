#include "engine/physics/physics_server/physics_server.h"
#include "engine/core/core_components/components.h"

#include <utility> //for swap

// All maths and logic behind the intersections are from here :
// https://github.com/IainWinter/IwEngine/blob/3e2052855fea85718b7a499a7b1a3befd49d812b/IwEngine/include/iw/physics/impl/TestCollision.h#L25

// Collision detection functions (Maybe move them to be somewhere else idk yet)

// Sphere Vs Sphere intersection
intersectionData intersectionSphereSphere(const Collider *a, const C_Transform *at,
                                          const Collider *b, const C_Transform *bt)
{

    // std::cout << "Sphere Sphere" << std::endl;

    assert(a->type == ColliderType::SPHERE && b->type == ColliderType::SPHERE);

    const SphereCollider *sphere_a = static_cast<const SphereCollider *>(a);
    const SphereCollider *sphere_b = static_cast<const SphereCollider *>(b);

    intersectionData res;

    glm::vec3 aCenter = sphere_a->center + at->getGlobalPosition();
    glm::vec3 bCenter = sphere_b->center + bt->getGlobalPosition();

    float aRadius = sphere_a->radius * at->getGlobalScale().x;
    float bRadius = sphere_b->radius * bt->getGlobalScale().x;

    glm::vec3 ab = bCenter - aCenter;

    float distance = glm::length(ab);

    if (distance < 0.0001f || distance > aRadius + bRadius)
    {
        res.isIntersection = false;
        return res;
    }

    glm::vec3 normal = glm::normalize(ab);
    glm::vec3 intersectionA = aCenter + normal * aRadius;
    glm::vec3 intersectionB = bCenter - normal * bRadius;

    // std::cout << "Intersection : Sphere Sphere" << std::endl;

    res.isIntersection = true;
    res.intersectionNormal = normal;
    res.intersectionPointA = intersectionA;
    res.intersectionPointB = intersectionB;

    return res;
}

intersectionData intersectionSpherePlane(const Collider *a, const C_Transform *at,
                                         const Collider *b, const C_Transform *bt)
{

    // std::cout << "Sphere Plane" << std::endl;

    assert(a->type == ColliderType::SPHERE && b->type == ColliderType::PLANE);

    intersectionData res;

    const SphereCollider *sphere = static_cast<const SphereCollider *>(a);
    const PlaneCollider *plane = static_cast<const PlaneCollider *>(b);

    glm::vec3 sphereCenter = sphere->center + at->getGlobalPosition();
    float sphereRadius = sphere->radius * at->getGlobalScale().x; // Assuming our scale is uniform, if not juste use the max

    // glm::vec3 planeNormal = glm::normalize(bt->getGlobalRotation() * plane->normal); //TODO when i code getRotation
    glm::vec3 planeNormal = glm::normalize(plane->normal);
    glm::vec3 planeNormal2 = plane->normal;
    // glm::vec3 planeNormal = glm::normalize(glm::vec3(0.0,1.0,0.0));

    float planeDistance = plane->distance;
    glm::vec3 onPlane = planeNormal * planeDistance + bt->getGlobalPosition();

    float distance = glm::dot(sphereCenter - onPlane, planeNormal);
    // std::cout << "----------------------------------------------------------------" << std::endl;
    // std::cout << "planeNormal (not normalized): (" << planeNormal2.x << ", " << planeNormal2.y << ", " << planeNormal2.z << ")" << std::endl;
    // std::cout << "planeNormal : (" << planeNormal.x << ", " << planeNormal.y << ", " << planeNormal.z << ")" << std::endl;
    // std::cout << "plane global position: (" << bt->getGlobalPosition().x << ", " << bt->getGlobalPosition().y << ", " << bt->getGlobalPosition().z << ")" << std::endl;
    // std::cout << "sphere global position : (" << at->getGlobalPosition().x << ", " << at->getGlobalPosition().y << ", " << at->getGlobalPosition().z << ")" << std::endl;
    // std::cout << "onPlane : (" << onPlane.x << ", " << onPlane.y << ", " << onPlane.z << ")" << std::endl;
    // std::cout << "distance : " << distance << std::endl;
    // std::cout << "radius : " << sphereRadius << std::endl;
    if (distance > sphereRadius)
    {
        res.isIntersection = false;
        return res;
    }

    glm::vec3 intersectionPointB = sphereCenter - planeNormal * distance; //* (2.0f * (glm::dot(planeNormal, sphereCenter - onPlane) < 0) - 1.0f);
    glm::vec3 intersectionPointA = sphereCenter - planeNormal * sphereRadius;

    // std::cout << "Intersection : Sphere Plane" << std::endl;
    res.isIntersection = true;
    res.intersectionPointA = intersectionPointA;
    res.intersectionPointB = intersectionPointB;
    res.intersectionNormal = planeNormal;

    return res;
}

// https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
//  We use the square distance from the closest point to compare the cube

intersectionData intersectionSphereCube(const Collider *a, const C_Transform *at,
                                        const Collider *b, const C_Transform *bt)
{

    //std::cout << "Sphere Cube" << std::endl;

    assert(a->type == ColliderType::SPHERE && b->type == ColliderType::CUBE);

    intersectionData res;

    const SphereCollider *sphere = static_cast<const SphereCollider *>(a);
    const CubeCollider *cube = static_cast<const CubeCollider *>(b);

    // sphere to world
    glm::vec3 sphereCenter = sphere->center + at->getGlobalPosition();
    float sphereRadius = sphere->radius * at->getGlobalScale().x; // Assuming our scale is uniform, if not just use the max

    // cube to world
    glm::vec3 cubeCenter = cube->center + bt->getGlobalPosition();
    glm::vec3 cubeMin = cube->min() + bt->getGlobalPosition();
    glm::vec3 cubeMax = cube->max() + bt->getGlobalPosition();
    glm::vec3 halfSize = cube->halfSize * bt->getGlobalScale(); // I think this scales this properly

    // closest points : https://gdbooks.gitbooks.io/3dcollisions/content/Chapter1/closest_point_aabb.html
    glm::vec3 closestPoint = glm::clamp(sphereCenter, cubeMin, cubeMax);

    // TODO : Remove if the clam works
    // for (int i = 0; i < 3; i++) {
    //     float val = sphereCenter[i];
    //     if (val < cubeMin[i]) val = cubeMin[i];
    //     if (val > cubeMax[i]) val = cubeMax[i];
    //     closestPoint[i] = val;
    // }

    // square distance, not 100% sure but pretty sure this should work
    float sqDist = glm::dot(sphereCenter - closestPoint, sphereCenter - closestPoint);

    if (sqDist > sphere->radius * sphere->radius)
    {
        res.isIntersection = false;
        return res;
    }

    glm::vec3 intersectionNormal = glm::normalize(closestPoint - sphereCenter);
    glm::vec3 sphereIntersectionPoint = sphereCenter + intersectionNormal * sphereRadius; // FIXME NO CLUE IF THIS IS RIGHT

    res.isIntersection = true;
    res.intersectionPointA = sphereIntersectionPoint;
    res.intersectionPointB = closestPoint;
    res.intersectionNormal = intersectionNormal;

    return res;
}

// FIXME : Adapt to world space maybe
intersectionData intersectionPlanePlane(const Collider *a, const C_Transform *at,
                                        const Collider *b, const C_Transform *bt)
{

    // std::cout << "Plane Plane" << std::endl;

    assert(a->type == ColliderType::PLANE && b->type == ColliderType::PLANE);

    const PlaneCollider *aPlane = static_cast<const PlaneCollider *>(a);
    const PlaneCollider *bPlane = static_cast<const PlaneCollider *>(b);

    intersectionData res;

    glm::vec3 aNormal = aPlane->normal;
    glm::vec3 bNormal = bPlane->normal;

    float aDistance = aPlane->distance;
    float bDistance = bPlane->distance;

    glm::vec3 direction = glm::cross(aNormal, bNormal);

    if (glm::length(direction) < 0.0001f)
    {
        res.isIntersection = false;
        return res;
    }

    // Find a point on the line of intersection
    float det = glm::dot(aNormal, glm::cross(bNormal, direction));
    if (fabs(det) < 0.0001f)
    {
        res.isIntersection = false;
        return res;
    }

    glm::vec3 pointOnLine = ((bDistance * glm::cross(direction, aNormal)) +
                             (aDistance * glm::cross(bNormal, direction))) /
                            det;

    // std::cout << "Intersection : Plane Plane" << std::endl;

    res.isIntersection = true;
    res.intersectionNormal = glm::normalize(direction);
    res.intersectionPointA = pointOnLine;
    res.intersectionPointB = pointOnLine;

    return res;
}

// https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
intersectionData intersectionPlaneCube(const Collider *a, const C_Transform *at,
                                       const Collider *b, const C_Transform *bt)
{

    //std::cout << "Plane Cube" << std::endl;

    assert(a->type == ColliderType::PLANE && b->type == ColliderType::CUBE);

    const PlaneCollider *plane = static_cast<const PlaneCollider *>(a);
    const CubeCollider *cube = static_cast<const CubeCollider *>(b);

    intersectionData res;

    glm::vec3 planeNormal = glm::normalize(plane->normal); // TODO : adapt to rotation
    float planeDistance = plane->distance;

    glm::vec3 cubeMin = cube->min() + bt->getGlobalPosition();
    glm::vec3 cubeMax = cube->max() + bt->getGlobalPosition();

    // check if any of the cube's vertices are on the plane
    bool isIntersecting = false;
    glm::vec3 intersectionPoint;

    bool hasFront = false, hasBack = false;
    float minDistance = FLT_MAX;
    glm::vec3 closestPoint;

    for (auto vertex : cube->getVertices()) {
        vertex += bt->getGlobalPosition();
        float distance = glm::dot(planeNormal, vertex) - planeDistance;

        if (distance > 0) hasFront = true;
        else if (distance < 0) hasBack = true;

        if (std::abs(distance) < minDistance) {
            minDistance = std::abs(distance);
            closestPoint = vertex;
        }

        if (hasFront && hasBack)
            break;
    }

    if (!(hasFront && hasBack)) {
        res.isIntersection = false;
        return res;
    }

    res.isIntersection = true;
    res.intersectionNormal = planeNormal;
    res.intersectionPointA = intersectionPoint;
    res.intersectionPointB = intersectionPoint;

    return res;
}

intersectionData intersectionCubeCube(const Collider *a, const C_Transform *at,
                                      const Collider *b, const C_Transform *bt)
{

    //std::cout << "Cube Cube" << std::endl;

    assert(a->type == ColliderType::CUBE && b->type == ColliderType::CUBE);

    const CubeCollider *aCube = static_cast<const CubeCollider *>(a);
    const CubeCollider *bCube = static_cast<const CubeCollider *>(b);

    intersectionData res;

    // first cube to world
    glm::vec3 aCubeCenter = aCube->center + at->getGlobalPosition();
    glm::vec3 aCubeMin = aCube->min() + at->getGlobalPosition();
    glm::vec3 aCubeMax = aCube->max() + at->getGlobalPosition();
    glm::vec3 aHalfSize = aCube->halfSize * at->getGlobalScale();

    // second cube to world
    glm::vec3 bCubeCenter = bCube->center + bt->getGlobalPosition();
    glm::vec3 bCubeMin = bCube->min() + bt->getGlobalPosition();
    glm::vec3 bCubeMax = bCube->max() + bt->getGlobalPosition();
    glm::vec3 bHalfSize = bCube->halfSize * bt->getGlobalScale();

    // check if intere
    if (!(aCubeMin.x <= bCubeMax.x && aCubeMax.x >= bCubeMin.x) &&
        (aCubeMin.y <= bCubeMax.y && aCubeMax.y >= bCubeMin.y) &&
        (aCubeMin.z <= bCubeMax.z && aCubeMax.z >= bCubeMin.z))
    {
        res.isIntersection = false;
        return res;
    }

    glm::vec3 contactPointA = glm::clamp(bCubeCenter, aCubeMin, aCubeMax);
    glm::vec3 contactPointB = glm::clamp(aCubeCenter, aCubeMin, aCubeMax);

    res.isIntersection = true;
    res.intersectionNormal = glm::normalize(bCubeCenter - aCubeCenter); // i think that's right idk
    res.intersectionPointA = contactPointA;
    res.intersectionPointB = contactPointB;

    return res;
}

/////////////////////////////////////////////////////////////////////////////

void PhysicsServer::addObject(GameObjectData *go)
{
    Objects.insert(go);
}

void PhysicsServer::removeObject(GameObjectData *go)
{
    Objects.erase(go);
}

std::vector<intersectionData> PhysicsServer::computeCollisions()
{
    std::vector<intersectionData> intersectionList;

    // Check this out to understand this
    // https://winter.dev/articles/physics-engine

    using FindContactFunc = intersectionData (*)(const Collider *, const C_Transform *, const Collider *, const C_Transform *);

    static const FindContactFunc functionTables[4][4] = {
        {nullptr, nullptr, nullptr, nullptr},
        {nullptr, intersectionSphereSphere, intersectionSpherePlane, intersectionSphereCube},
        {nullptr, nullptr, intersectionPlanePlane, intersectionPlaneCube},
        {nullptr, nullptr, nullptr, intersectionCubeCube}
    };

    for (const auto &objectA : Objects)
    {
        if (!objectA->hasComponent<C_Collider>() || !objectA->hasComponent<C_RigidBody>())
            continue;
        for (const auto &objectB : Objects)
        {

            // Check if the object has the right components AND that they're not the same object
            if (
                !objectB->hasComponent<C_Collider>() ||
                !objectB->hasComponent<C_RigidBody>() ||
                objectA->getId() <= objectB->getId())
                continue;

            intersectionData intersection;

            // check if it can work in our table, if not, flip it

            Collider *colliderA = &objectA->getComponent<C_Collider>()->collider.base;
            C_Transform *transformA = objectA->getComponent<C_Transform>();

            Collider *colliderB = &objectB->getComponent<C_Collider>()->collider.base;
            C_Transform *transformB = objectB->getComponent<C_Transform>();

            bool swap = (objectB->getComponent<C_Collider>()->collider.base.type) < (objectA->getComponent<C_Collider>()->collider.base.type);

            // flip it (or not)
            if (swap)
            {

                // safety check
                //  std::cout << "id 1st : " << objectB->getId() << std::endl;
                //  std::cout << "id 2nd : " << objectA->getId() << std::endl;
                auto func = functionTables[colliderB->type][colliderA->type];
                if (!func)
                {
                    std::cout << "func[" << colliderB->type << "][" << colliderA->type << "] does not exist" << std::endl;
                    continue;
                }

                intersection = func(colliderB, transformB, colliderA, transformA);
                intersection.objectA = objectB;
                intersection.objectB = objectA;
            }
            else
            {

                // same safety check
                //  std::cout << "id 1st : " << objectA->getId() << std::endl;
                //  std::cout << "id 2nd : " << objectB->getId() << std::endl;
                auto func = functionTables[colliderA->type][colliderB->type];
                if (!func)
                {
                    std::cout << "func[" << colliderA->type << "][" << colliderB->type << "] does not exist" << std::endl;
                    continue;
                }

                intersection = func(colliderA, transformA, colliderB, transformB);
                intersection.colliderA = colliderA;
                intersection.colliderB = colliderB;
                intersection.objectA = objectA;
                intersection.objectB = objectB;
            }

            intersectionList.push_back(intersection);
        }
    }
    return intersectionList;
}

void PhysicsServer::resolveCollision(const intersectionData &a)
{

    static const float MARGIN = 0.001f;

    auto *objectA = a.objectA;
    auto *objectB = a.objectB;

    glm::vec3 overlapVector(a.intersectionPointB - a.intersectionPointA);
    float mass_ratio = objectB->getComponent<C_RigidBody>()->mass / (objectB->getComponent<C_RigidBody>()->mass + objectA->getComponent<C_RigidBody>()->mass);
    // std::cout << "MASS RATIO " << mass_ratio << std::endl;
    glm::vec3 moveA = overlapVector * (mass_ratio + MARGIN);

    auto v = objectA->getComponent<C_Transform>()->getGlobalPosition();
    objectA->getComponent<C_Transform>()->moveGlobal(overlapVector * (mass_ratio + MARGIN));

    // objectA->getComponent<C_Transform>()->setScale(glm::vec3(glm::length(overlapVector*20.0f)));
    // objectB->getComponent<C_Transform>()->moveGlobal(-1.0f * overlapVector  * (1.0f - mass_ratio + MARGIN));
}

bool t = false;

void PhysicsServer::solveCollisions(std::vector<intersectionData> collisions, float deltatime)
{
    for (const auto &collision : collisions)
    {
        if (collision.isIntersection)
        {
            resolveCollision(collision);
            collision.objectA->getComponent<C_RigidBody>()->linear_velocity =
                glm::reflect(collision.objectA->getComponent<C_RigidBody>()->linear_velocity, collision.intersectionNormal);

            collision.objectB->getComponent<C_RigidBody>()->linear_velocity =
                glm::reflect(collision.objectB->getComponent<C_RigidBody>()->linear_velocity, collision.intersectionNormal);

            collision.objectA->getComponent<C_RigidBody>()->linear_velocity -= collision.objectA->getComponent<C_RigidBody>()->acceleration * deltatime;
            collision.objectB->getComponent<C_RigidBody>()->linear_velocity -= collision.objectA->getComponent<C_RigidBody>()->acceleration * deltatime;

            float THRESH = 0.0000005f;
            if (glm::dot(collision.objectA->getComponent<C_RigidBody>()->linear_velocity, collision.objectA->getComponent<C_RigidBody>()->linear_velocity) < THRESH)
                collision.objectA->getComponent<C_RigidBody>()->linear_velocity = glm::vec3(0.0);
            if (glm::dot(collision.objectB->getComponent<C_RigidBody>()->linear_velocity, collision.objectB->getComponent<C_RigidBody>()->linear_velocity) < THRESH)
                collision.objectB->getComponent<C_RigidBody>()->linear_velocity = glm::vec3(0.0);

            //t = true;
        }
    }
}

// Source is this book :
// https://books.google.fr/books?hl=en&lr=&id=dSbMBQAAQBAJ&oi=fnd&pg=PP1&dq=3d+physics+engine&ots=qe2HUKonLp&sig=JMjj1CI9Jnisktw-MRhHuGTIczg&redir_esc=y#v=onepage&q&f=true
void PhysicsServer::integrate(float deltatime)
{
    for (const auto &object : Objects)
    {
        if (object->getComponent<C_Collider>()->collider.base.type == SPHERE ||object->getComponent<C_Collider>()->collider.base.type == CUBE)
        {
            auto *objectBody = object->getComponent<C_RigidBody>();
            glm::vec3 objectTransform = object->getComponent<C_Transform>()->getPosition();

            objectBody->acceleration = gravity; // for now thats all it is
            // update the position
            object->getComponent<C_Transform>()->move(objectBody->linear_velocity);

            // update de velocity
            objectBody->linear_velocity += objectBody->acceleration * deltatime;

            objectBody->linear_velocity *= pow(objectBody->damping, deltatime);

            objectTransform = object->getComponent<C_Transform>()->getPosition();

            // std::cout << "acceleration : (" << objectBody->acceleration.x << "," << objectBody->acceleration.y  << "," << objectBody->acceleration.z << ")" << std::endl;
            // std::cout << "velocity : (" << objectBody->linear_velocity .x << "," << objectBody->linear_velocity .y  << "," << objectBody->linear_velocity .z << ")" << std::endl;
            // std::cout << "transform : (" << objectTransform.x << "," << objectTransform.y  << "," << objectTransform.z << ")" << std::endl;
        }
    }
}

void PhysicsServer::step(float deltatime)
{
    if (t)
        return;
    std::vector<intersectionData> collisions = computeCollisions();
    solveCollisions(collisions, deltatime);
    integrate(deltatime);
}
