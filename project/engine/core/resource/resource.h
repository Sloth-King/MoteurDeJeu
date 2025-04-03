
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <memory>

// https://giordi91.github.io/post/resourcesystem/

class ResourceManager;

class ResourceHandle{
private:
    ResourceManager & owner; // the manager should live long enough for this to be safe (aka for the whole game)

    uint32_t index = 0;

}


// not virtual since we do not need RTTI, but it makes it a bit awkward so please follow the patterns
class ResourceManager{
private:
    uint32_t next_index = 1;
    ResourceManager() = default; // don't ever instanciate this one !
public:
    T load(std::string path) final{

    }

    ResourceHandle onload(std::string path); // redefine this
    
    
    void get(ResourceHandle &);
    
}