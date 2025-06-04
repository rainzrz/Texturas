// Sprite.cpp
#include "Sprite.h"
#include <glad/glad.h>

Sprite::Sprite(unsigned int texture, Shader* shader)
    : textureID(texture), shader(shader), position(0.0f), scale(1.0f), rotation(0.0f) 
{
    // setup VAO com um quad 1x1 centrado na origem
    float vertices[] = {
        // pos      // tex coords
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // tex coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &VAO);
}

void Sprite::Draw(const glm::mat4& projection) {
    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    shader->setMat4("projection", projection);
    shader->setMat4("model", model);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sprite::SetPosition(const glm::vec2& pos) { position = pos; }
void Sprite::SetScale(const glm::vec2& sc) { scale = sc; }
void Sprite::SetRotation(float angle) { rotation = angle; }
