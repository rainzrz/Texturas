#include "gl_utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

// Definições das variáveis globais (apenas aqui!)
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = nullptr;

bool restart_gl_log() {
    FILE* file = fopen("gl.log", "w");
    if (!file) {
        fprintf(stderr, "ERROR: could not open GL log file for writing\n");
        return false;
    }
    time_t now = time(NULL);
    fprintf(file, "GL_LOG started. Local time: %s\n", ctime(&now));
    fclose(file);
    return true;
}

bool start_gl() {
    if (!glfwInit()) {
        fprintf(stderr, "ERRO: Não foi possível iniciar o GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window = glfwCreateWindow(g_gl_width, g_gl_height, "PGCCHIB Example", NULL, NULL);
    if (!g_window) {
        fprintf(stderr, "ERRO: Não foi possível abrir a janela com o GLFW3\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(g_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "ERRO: Falha ao inicializar GLAD\n");
        return false;
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderizador: %s\n", renderer);
    printf("Versão OpenGL suportada %s\n", version);

    return true;
}

void _update_fps_counter(GLFWwindow* window) {
    static double previous_seconds = 0.0;
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        char tmp[128];
        double fps = (double)frame_count / elapsed_seconds;
        sprintf(tmp, "OpenGL @ FPS: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

bool parse_file_into_str(const char* file_name, char* shader_str, int max_len) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "ERROR: could not open shader file %s\n", file_name);
        return false;
    }
    size_t cnt = fread(shader_str, 1, max_len - 1, file);
    if (ferror(file)) {
        fprintf(stderr, "ERROR: reading shader file %s\n", file_name);
        fclose(file);
        return false;
    }
    shader_str[cnt] = '\0';
    fclose(file);
    return true;
}

void print_shader_info_log(GLuint shader) {
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetShaderInfoLog(shader, max_length, &actual_length, log);
    printf("Shader info log for GL index %u:\n%s\n", shader, log);
}
