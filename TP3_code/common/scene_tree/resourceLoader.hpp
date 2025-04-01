#pragma once

#include "mesh.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

namespace ResourceLoader{

    Mesh load_mesh_off(std::string filename);


    Mesh load_mesh_obj(std::string path);
}
