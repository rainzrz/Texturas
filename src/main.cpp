#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Funções auxiliares
void checkCompileErrors(unsigned int shader, std::string type);
std::string readFile(const char* filepath);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Quad vertices (posições + texcoords)
float vertices[] = {
    // pos        // tex
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

int main()
{
    // Inicializa GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria janela
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Multi Textured Sprites", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializa GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Habilita blending
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compila shaders
    std::string vertexCode = readFile("src/shaders.vs");
    std::string fragmentCode = readFile("src/shaders.fs");
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Setup buffers e arrays
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Carrega texturas
    unsigned int backgroundTexture = loadTexture("textures/background.png");

    std::vector<unsigned int> textures;
    textures.push_back(loadTexture("textures/texture1.png"));
    textures.push_back(loadTexture("textures/texture2.png"));
    textures.push_back(loadTexture("textures/texture3.png"));

    // Configura projeção ortográfica
    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH,
                                      0.0f, (float)SCR_HEIGHT,
                                      -1.0f, 1.0f);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        // 1. Desenha background
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        // Centraliza o quad na tela
        model = glm::translate(model, glm::vec3(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f, 0.0f));
        // Escala para cobrir toda a tela
        model = glm::scale(model, glm::vec3((float)SCR_WIDTH, (float)SCR_HEIGHT, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 2. Desenha as 3 texturas menores na frente
        for (int i = 0; i < (int)textures.size(); i++)
        {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(150.0f + i * 200.0f, SCR_HEIGHT / 2.0f, 0.0f));
            model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// --- Funções auxiliares abaixo ---

std::string readFile(const char* filepath)
{
    std::string content;
    std::ifstream file(filepath, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Failed to read " << filepath << "\n";
        return "";
    }
    std::string line;
    while (std::getline(file, line))
        content += line + "\n";
    file.close();
    return content;
}

void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "| ERROR::SHADER-COMPILATION-ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "| ERROR::PROGRAM-LINKING-ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}

unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Filtros de textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture at path: " << path << "\n";
        stbi_image_free(data);
    }

    return textureID;
}
