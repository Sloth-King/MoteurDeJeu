#pragma once
#include <set>
#include "engine/render/components/components.h"




class Game;
class RenderingServer{
friend class Game;
    std::set<C_Mesh * > objects;

    GLuint gBuffer;
    GLuint gShader = 0;
    GLuint lightShader = 0;
    GLuint gPosition, gNormal, gAlbedoTex, gNormalTex, gRoughnessTex, gMetallicTex, gTangent, gBitangent, gDepth;


    void setupBuffers();



    void renderMeshesDeferred() const noexcept{
        for (auto* object: objects){
            object->renderDeferred(gShader);
        }
    }

    void renderScene(){
        //renderSceneForward();
        renderSceneDeferred();
    }

    void renderSceneForward() const noexcept;

    void renderSceneDeferred() const noexcept;

    GLFWwindow * context;
    Game * game;

    void setContext(GLFWwindow * newcont) noexcept {
        context = newcont;
    }

    void setGame(Game * newgame) noexcept {
        game = newgame;
    }

    void renderScene() const noexcept;

    void geometryPass() const noexcept;

    void lightPass() const noexcept;


    void BindForWriting()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    }

    void BindForReading()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    }

    void SetReadBuffer(int TextureType)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
    } 


public:

    void addObject(C_Mesh * g ) noexcept {
        objects.insert(g);
    }

    void removeObject(C_Mesh * g )noexcept {
        objects.erase(g);
    }

};