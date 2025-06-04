// Sprite.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class Sprite {
public:
    unsigned int VAO;
    unsigned int textureID;
    glm::vec2 position;
    glm::vec2 scale;
    float rotation; // em graus

    Shader* shader; // ponteiro para o shader usado

    Sprite(unsigned int texture, Shader* shader);
    ~Sprite();

    void Draw(const glm::mat4& projection);
    void SetPosition(const glm::vec2& pos);
    void SetScale(const glm::vec2& scale);
    void SetRotation(float angle);
};
