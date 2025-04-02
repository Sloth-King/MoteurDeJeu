// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <ctime>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/controls.hpp>

//Image
#include <common/stb_image.h>


//Scene tree
#include <common/scene_tree/mesh.hpp>
#include <common/scene_tree/resourceLoader.hpp>
#include <common/scene_tree/textureClass.hpp>
#include <common/scene_tree/transform.hpp>
#include <common/scene_tree/gameObject.hpp>




void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

enum CameraMode {
    free_roam,
    orbit,
};

CameraMode mode = free_roam; 

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;

int width = 16;
int height = 16;

bool rebuild = false;

float rotation_speed = 0.2;

float heightScale = 0.2;


/*******************************************************************************/
//Other functions

struct Vertex{
    glm::vec3 position;
    glm::vec2 uv;
};

unsigned char* convert_REDtoRGB(unsigned char * data , int width , int height){
    unsigned char* rgbData = new unsigned char[width * height * 3];
    for (int i = 0; i < width * height; ++i) {
    rgbData[i * 3] = data[i];
    rgbData[i * 3 + 1] = data[i];
    rgbData[i * 3 + 2] = data[i];
    }

    return rgbData;
}

//FIXME : Textures not working
//Working on textures 
GLuint load_texture(std::string filename){


    int imgWidth , imgHeight , channels;    
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(filename.c_str() , &imgWidth , &imgHeight , &channels , 0);



    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1 , &textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D , textureID);

    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);


    GLenum format;
    if (channels == 1) {
        data = convert_REDtoRGB(data , imgWidth , imgHeight);
        format = GL_RGB;
    } else if (channels == 3) {
        format = GL_RGB;
    } else {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, data);


    glGenerateMipmap(GL_TEXTURE_2D);



    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D , 0);

    return textureID;
}

void bind(GLuint texture , unsigned int slot = 0){
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D , texture);
}

void create_plane(
    std::vector<unsigned short> &out_indices, 
    std::vector<std::vector<unsigned short>> &out_triangles, 
    std::vector<glm::vec3> &out_indexed_vertices,
    int height = 16,
    int width= 16){

        out_indices.clear();
        out_triangles.clear();
        out_indexed_vertices.clear();

        srand(std::time(0));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                //TP1 question 2. Set it to 0 for question 1
                float random_altitude = std::fmod(std::rand(),1.000001f);
                out_indexed_vertices.push_back(glm::vec3((float)j / (width - 1)-0.5 , random_altitude-0.5f , (float)i / (height - 1)));
            }
        }

        for (int i = 0; i < height - 1; i++) {
            for (int j = 0; j < width - 1; j++) {
                unsigned short topLeft = i * width + j;
                unsigned short topRight = topLeft + 1;
                unsigned short bottomLeft = (i + 1) * width + j;
                unsigned short bottomRight = bottomLeft + 1;

                out_indices.push_back(topLeft);
                out_indices.push_back(bottomLeft);
                out_indices.push_back(topRight);

                out_indices.push_back(topRight);
                out_indices.push_back(bottomLeft);
                out_indices.push_back(bottomRight);

                out_triangles.push_back({topLeft, bottomLeft, topRight});
                out_triangles.push_back({topRight, bottomLeft, bottomRight});
            }
        }
}

void create_plane_with_texture(
    std::vector<unsigned short> &out_indices, 
    std::vector<std::vector<unsigned short>> &out_triangles, 
    std::vector<Vertex> &vertex_coords,
    int height = 16,
    int width= 16){

        std::cout << "come on mane" << std::endl;

        out_indices.clear();
        out_triangles.clear();
        //out_indexed_vertices.clear();
        //indexed_tex_coords.clear();
        vertex_coords.clear();

        srand(std::time(0));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                
                float u = (float)j / (width - 1);
                float v = (float)i / (height - 1);

                float random_altitude = std::fmod(std::rand(),1.000001f);

                float x = (float)j / (width - 1)*2 -1;
                float y = random_altitude;
                float z = (float)i / (height - 1)*2 -1;

                Vertex vx = {glm::vec3(x,y,z), glm::vec2(u,v)};
                vertex_coords.push_back(vx);
                //out_indexed_vertices.push_back(glm::vec3((float)j / (width - 1)-0.5 , (altitude*h_scale)-0.5f , (float)i / (height - 1)));
            }
        }

        for (int i = 0; i < height - 1; i++) {
            for (int j = 0; j < width - 1; j++) {
                unsigned short topLeft = i * width + j;
                unsigned short topRight = topLeft + 1;
                unsigned short bottomLeft = (i + 1) * width + j;
                unsigned short bottomRight = bottomLeft + 1;

                out_indices.push_back(topLeft);
                out_indices.push_back(bottomLeft);
                out_indices.push_back(topRight);

                out_indices.push_back(topRight);
                out_indices.push_back(bottomLeft);
                out_indices.push_back(bottomRight);

                out_triangles.push_back({topLeft, bottomLeft, topRight});
                out_triangles.push_back({topRight, bottomLeft, bottomRight});
            }
        }

}

// void create_plane_heightmaptex_texturetex(); // Commented out as it's not used


/*******************************************************************************/

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP1 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    //TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_tex_coords;
    std::vector<Vertex> vertex;
    
    /* The simple solar system 

    //SUN 
    std::string filename = "sphere2.off";

    //Initialize meshes
    Mesh sun_mesh = ResourceLoader::load_mesh_off(filename);
    Mesh earth_mesh = ResourceLoader::load_mesh_off(filename);
    Mesh moon_mesh = ResourceLoader::load_mesh_off(filename);

    //Set shader type shit
    sun_mesh.setShader("vertex_shader.glsl" , "fragment_shader.glsl");
    earth_mesh.setShader("vertex_shader.glsl" , "fragment_shader.glsl");
    moon_mesh.setShader("vertex_shader.glsl" , "fragment_shader.glsl");

    //calculate UV (for .off only)
    sun_mesh.calculateUV_Sphere();
    earth_mesh.calculateUV_Sphere();
    moon_mesh.calculateUV_Sphere();


    //I forgor what this does    
    GLuint texSamplerID = glGetUniformLocation(programID , "textureSampler");
    glUniform1i(texSamplerID , 0);

    //load a texture
    Texture sun_texture = Texture("textures/2k_sun.jpg");
    Texture earth_texture = Texture("textures/2k_earth_daymap.jpg");
    Texture moon_texture = Texture("textures/2k_moon.jpg");


    //bind it
    sun_mesh.addTexture(sun_texture , "mesh_texture");
    earth_mesh.addTexture(earth_texture , "mesh_texture");
    moon_mesh.addTexture(moon_texture , "mesh_texture");


    //Create the gameobjects
    GameObject sunGO = GameObject(sun_mesh, "sunGO");
    GameObject earthGO = GameObject(earth_mesh,"earthGO");
    GameObject moonGO = GameObject(moon_mesh , "moonGO");

    //Manage parent child hierarchy
    sunGO.addChild(&earthGO);
    earthGO.addChild(&moonGO);

    //Initial transforms

    //shrink shmove and roate the earth
    earthGO.transform.setPosition(glm::vec3(-3.0f , 0.0f , 0.0f));
    earthGO.transform.setScale(glm::vec3(0.25f , 0.25f , 0.25f));
    earthGO.transform.setRotation(glm::vec3(23.0f, 0.0f, 0.0f));

    //shrink shmoove and return the moon to its original shits
    moonGO.transform.setScale(glm::vec3(0.5f,0.5f,0.5f));
    moonGO.transform.setPosition(glm::vec3(-2.0f , 1.0f , 0.0f)); 
    */

    std::string filename = "sphere.off";
    
    Mesh terrain = Mesh::gen_tesselatedSquare(100, 100 , 10 , 10);
    Mesh mesh = ResourceLoader::load_mesh_off(filename);

    terrain.setShader("terrain_vertex_shader.glsl" , "terrain_fragment_shader.glsl");
    mesh.setShader("vertex_shader.glsl" , "fragment_shader.glsl");

    mesh.calculateUV_Sphere();

    //FIXME : other heightmaps create seg faults
    Texture heightmap("textures/coolheightmap.jpg");
    Texture grass("textures/grass.png");
    Texture rock("textures/rock.png");
    Texture snow("textures/snowrocks.png");
    Texture sun("textures/2k_sun.jpg");

    terrain.addTexture(grass , "grass");
    terrain.addTexture(rock , "rock");
    terrain.addTexture(snow , "snow");
    terrain.addTexture(heightmap , "h");
    mesh.addTexture(sun , "mesh_texture"); 


    
    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    //GLuint heightScaleID = glGetUniformLocation(programID , "heightScale");



    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Model matrix : an identity matrix (model will be at the origin) then change

        if(mode == orbit){
            angle+= rotation_speed;
        }

        // glm::mat4 model_matrix = glm::mat4(1.0f);
        // model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model_matrix = glm::scale(model_matrix, glm::vec3(zoom, zoom, zoom));

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up

        glm::mat4 view_matrix = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);


        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms

        //MVP will be calculated in each mesh, so for now just send VP
        glm::mat4 VP = projection_matrix * view_matrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &VP[0][0]);

        // // Index buffer
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        //To test if the pb was my triangles
        //glDisable(GL_CULL_FACE);

        //mesh.render(VP);

        /*Planets movement in real time

        sunGO.renderScene(VP);
        sunGO.transform.rotate(glm::vec3(0.0f , 0.1f , 0.0f));
        earthGO.transform.rotate(glm::vec3(0.0f , 0.5f , 0.0f));
        
        moonGO.transform.rotate(glm::vec3(0.0f, 1.2f, 0.0f)); 
        sunGO.updateSelfAndChildren();

        */

        terrain.render(VP);
        mesh.render(VP);


        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    // glDeleteBuffers(1, &vertexbuffer);
    // glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera zoom in and out
    float cameraSpeed = 2.5 * deltaTime;

    //handle free cam based controls
    if(mode == free_roam){
        //forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera_position += cameraSpeed * camera_target;
        //backwards
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera_position -= cameraSpeed * camera_target;
        //right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera_position -= cameraSpeed * glm::cross(camera_up,camera_target);
        //left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera_position += cameraSpeed * glm::cross(camera_up,camera_target);
        //up
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera_position += cameraSpeed * camera_up;
        //down
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera_position -= cameraSpeed * camera_up;
    }
    //handle orbit based controls
    if(mode == orbit){
        //decrease rotation speed
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            rotation_speed-=0.1;
        }
        //increase rotation speed
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            rotation_speed+=0.1;
        }
    }


    //increase res
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){
        height++;
        width++;
        rebuild = true;
    }
    //decrease res
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){
        height--;
        width--;
        rebuild = true;
        }
        //M for mountains heheheh
    //increase heightscale
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
        heightScale+=0.05;
        rebuild = true;
    }
    //N for no mountains :( 
    //decrease heightscale
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
        if(heightScale - 0.05 > 0){
            heightScale-=0.05;
            rebuild = true;
        }
    }
        static bool cKeyPressed = false;

        //press c to toggle camera mode
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed){
        cKeyPressed = true;
        if(mode == orbit){
            mode = free_roam;
        }else{
            mode = orbit;
            camera_position = vec3(0.0,3.0,3.0);
            camera_target = glm::vec3(0.0, -1.0, -1.0);
        }
    }
    //make sure its just a press ones ting
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE){
        cKeyPressed = false;
    }




    //TODO add translations

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
