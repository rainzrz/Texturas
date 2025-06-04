#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <string>

class Sprite {
public:
    Sprite();
    ~Sprite();

    bool loadTexture(const std::string& path);
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void draw();

private:
    GLuint textureID;
    GLuint VAO, VBO, EBO;
    float posX, posY;
    float sizeX, sizeY;

    void setupMesh();
};

#endif
