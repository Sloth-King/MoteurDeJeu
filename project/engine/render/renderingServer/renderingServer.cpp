#include "renderingServer.h"
#include "engine/core/game/game.h"
#include "deferred.glsl"

inline void limit_fps(int FPS){
    static double last_time = glfwGetTime();

    double current_time = glfwGetTime();
    double ms = 1.0 / FPS;
    while (current_time - last_time <= ms){
        current_time = glfwGetTime();
    }

    last_time = current_time;
}

inline void showFPS(GLFWwindow* window){

    static const float timeBetweenUpdates = 0.5;
    static double last_time = glfwGetTime();
    static unsigned int nbframes = 0;
    static const std::string windowTitle ("FPS - ");
    double current_time = glfwGetTime();

    nbframes++;
    
    if (current_time - last_time > timeBetweenUpdates){ // seconds
        glfwSetWindowTitle(window, (windowTitle + std::to_string((uint32_t)(nbframes/timeBetweenUpdates))).c_str());
        last_time = current_time;
        nbframes = 0;
    }
}

void RenderingServer::renderSceneForward() const noexcept {
    const Scene* curr_scene = &game->current_scene;

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto* object: objects){
        object->renderForward();
    }
    curr_scene->environment.render(curr_scene->current_camera->getViewMatrix(), curr_scene->current_camera->getProjectionMatrix());

    glfwSwapBuffers(context);
}


void RenderingServer::renderSceneDeferred() const noexcept{

    // expects the context and game to be set, otherwise segfaults.
    geometryPass();
    lightPass();

    glfwSwapBuffers(context);

    showFPS(context);
}


void RenderingServer::geometryPass() const noexcept{

    const Scene* curr_scene = &game->current_scene;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    glUseProgram(gShader);
    glDisable(GL_BLEND);


    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderMeshesDeferred();

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

}


GLuint _squareVBO;
GLuint _squareVAO;

void RenderingServer::lightPass() const noexcept{

    const Scene* curr_scene = &game->current_scene;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(lightShader);
    
    glActiveTexture(GL_TEXTURE0+0); glBindTexture(GL_TEXTURE_2D, gPosition); glUniform1i(glGetUniformLocation(lightShader, "gPosition"), 0);
    glActiveTexture(GL_TEXTURE0+1); glBindTexture(GL_TEXTURE_2D, gNormal); glUniform1i(glGetUniformLocation(lightShader, "gNormal"), 1);
    glActiveTexture(GL_TEXTURE0+2); glBindTexture(GL_TEXTURE_2D, gTangent); glUniform1i(glGetUniformLocation(lightShader, "gTangent"), 2);
    glActiveTexture(GL_TEXTURE0+3); glBindTexture(GL_TEXTURE_2D, gBitangent); glUniform1i(glGetUniformLocation(lightShader, "gBitangent"), 3);
    glActiveTexture(GL_TEXTURE0+4); glBindTexture(GL_TEXTURE_2D, gDepth); glUniform1i(glGetUniformLocation(lightShader, "gDepth"), 4);
    glActiveTexture(GL_TEXTURE0+5); glBindTexture(GL_TEXTURE_2D, gAlbedoTex); glUniform1i(glGetUniformLocation(lightShader, "gAlbedoTex"), 5);
    glActiveTexture(GL_TEXTURE0+6); glBindTexture(GL_TEXTURE_2D, gNormalTex); glUniform1i(glGetUniformLocation(lightShader, "gNormalTex"), 6);
    glActiveTexture(GL_TEXTURE0+7); glBindTexture(GL_TEXTURE_2D, gRoughnessTex); glUniform1i(glGetUniformLocation(lightShader, "gRoughnessTex"), 7);
    glActiveTexture(GL_TEXTURE0+8); glBindTexture(GL_TEXTURE_2D, gMetallicTex); glUniform1i(glGetUniformLocation(lightShader, "gMetallicTex"), 8);

    glm::vec3 viewvector = curr_scene->current_camera->getForwardVector();
    glUniform3fv(glGetUniformLocation(lightShader, "view"), 1, &viewvector[0]);

    glBindVertexArray(_squareVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    //curr_scene->environment.render(curr_scene->current_camera->getViewMatrix(), curr_scene->current_camera->getProjectionMatrix());
}


float squareVertices[] = {
    -1, -1,
    1, -1,
    1, 1,

    -1, -1,
    1, 1,
    -1, 1};

// https://learnopengl.com/Advanced-Lighting/Deferred-Shading
void RenderingServer::setupBuffers(){
    /*
    layout (location = 0) out vec3 __Position__;
    layout (location = 1) out vec3 __Normal__;
    layout (location = 3) out vec3 __Tangent__;
    layout (location = 4) out vec3 __Bitangent__;
    layout (location = 5) out vec4 __Albedo__;
    layout (location = 6) out float __Depth__;
    
    */

    glDeleteFramebuffers(1, &gBuffer);
    glDeleteBuffers(9, &gPosition);
    
    if (!gShader){
        gShader = loadShaders(vertex_deferred, fragment_deferred);
    }

    if (!lightShader){
        lightShader = loadShaders(PBR_vertex_deferred, fragment_PBR_deferred);

        glGenVertexArrays(1, &_squareVAO);
        glBindVertexArray(_squareVAO);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &_squareVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _squareVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), &squareVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glBindVertexArray(0);
    }

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    auto SCR_WIDTH = game->settings.windowWidth;
    auto SCR_HEIGHT = game->settings.windowHeight;
    
    // - position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    
    // - normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - tangents color buffer
    glGenTextures(1, &gTangent);
    glBindTexture(GL_TEXTURE_2D, gTangent);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gTangent, 0);
    
    // - bitangents color buffer
    glGenTextures(1, &gBitangent);
    glBindTexture(GL_TEXTURE_2D, gBitangent);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBitangent, 0);

    // - depth color buffer
    glGenTextures(1, &gDepth);
    glBindTexture(GL_TEXTURE_2D, gDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);
    
    // - albedo color buffer
    glGenTextures(1, &gAlbedoTex);
    glBindTexture(GL_TEXTURE_2D, gAlbedoTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gAlbedoTex, 0);

    // - normals color buffer
    glGenTextures(1, &gNormalTex);
    glBindTexture(GL_TEXTURE_2D, gNormalTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gNormalTex, 0);

    // - roughness color buffer
    glGenTextures(1, &gRoughnessTex);
    glBindTexture(GL_TEXTURE_2D, gRoughnessTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, gRoughnessTex, 0);

    // - metallic color buffer
    glGenTextures(1, &gMetallicTex);
    glBindTexture(GL_TEXTURE_2D, gMetallicTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, gMetallicTex, 0);


    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7};
    glDrawBuffers(8, attachments);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}