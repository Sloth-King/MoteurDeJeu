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
#include <common/stb.cpp>


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

    //Chargement du fichier de maillage
    //std::string filename("chair.off");
    //loadOFF(filename, indexed_vertices, indices, triangles );

    //plane

    //create_plane(indices , triangles , indexed_vertices);
    
    std::string h_map_1024 = "textures/heightmap-1024x1024.png";
    std::string h_map_mountain = "textures/Heightmap_Mountain.png";
    std::string h_map_rocky = "textures/Heightmap_Rocky.png"; 

    std::string grass_texture = "textures/grass.png";
    std::string rock_texture = "textures/rock.png";
    std::string snowrocks_texture = "textures/snowrocks.png";

    GLuint grassID = load_texture(grass_texture);
    GLuint rockID = load_texture(rock_texture);
    GLuint snowID = load_texture(snowrocks_texture);

    //GLuint heightmapID = load_texture(h_map_1024);
    //GLuint heightmapID = load_texture(h_map_mountain);
    GLuint heightmapID = load_texture(h_map_mountain);




    // glUseProgram(programID);
    // GLuint tex0uniform = glGetUniformLocation(programID, "tex0");
    // glUniform1i(tex0uniform, 0);  // 0 corresponds to GL_TEXTURE0


    // GLuint rockID = load_texture(rock_texture);
    // GLuint snowrocksID = load_texture(snowrocks_texture);

    create_plane_with_texture(indices, triangles , vertex , height , width);

    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &vertex[0], GL_STATIC_DRAW);

    //texturebuffer
    GLuint texcoordbuffer;
    glGenBuffers(1, &texcoordbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &vertex[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    GLuint heightScaleID = glGetUniformLocation(programID , "heightScale");



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

        if(rebuild){
            create_plane_with_texture(indices, triangles , vertex , height , width);

            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &vertex[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
            glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &vertex[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

            glUniform1f(heightScaleID , heightScale);

            rebuild = false;
        }

        //computeMatricesFromInputs();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Model matrix : an identity matrix (model will be at the origin) then change

        if(mode == orbit){
            angle+= rotation_speed;
        }
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::scale(model_matrix, glm::vec3(zoom, zoom, zoom));

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up

        glm::mat4 view_matrix = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);


        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms

        glm::mat4 MVP = projection_matrix * view_matrix * model_matrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),                  // stride
                    (void*)0            // array buffer offset
                    );

    
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),                  // stride
                    (void*)offsetof(Vertex, uv)            // array buffer offset
                    );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        //To test if the pb was my triangles
        //glDisable(GL_CULL_FACE);

        glActiveTexture(GL_TEXTURE0 + 3);
        glBindTexture(GL_TEXTURE_2D, grassID);
        GLuint texUniform = glGetUniformLocation(programID, "grass");
        glUniform1i(texUniform, 3);

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, rockID);
        GLuint texUniform_r = glGetUniformLocation(programID, "rock");
        glUniform1i(texUniform_r, 1);

        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, snowID);
        GLuint texUniform_s = glGetUniformLocation(programID, "snow");
        glUniform1i(texUniform_s, 2);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, heightmapID);
        GLuint texUniform_h = glGetUniformLocation(programID, "h");
        glUniform1i(texUniform_h, 0);

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteTextures(1 , &grassID);

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
