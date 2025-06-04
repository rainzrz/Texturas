#include "gl_utils.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <glad/glad.h> // Carregamento dos ponteiros para funções OpenGL
#include <GLFW/glfw3.h> // Adicionado: Cabeçalho do GLFW

// Essas variáveis globais são acessadas por exemplo_05.cpp e devem ser globais.
// É uma boa prática declará-las como 'extern' em gl_utils.h se não forem definidas aqui.
// No seu caso, elas estão sendo usadas, então assumo que estão definidas em outro lugar (ex: exemplo_05.cpp)
// ou que o gl_utils.h as declara.
extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window; // Declaração global da janela GLFW

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
    // 1. Inicia o GLFW
    if (!glfwInit()) {
        fprintf(stderr, "ERRO: Não foi possível iniciar o GLFW3\n");
        return false;
    }

    // 2. Configura as dicas da janela GLFW (versão OpenGL, etc.)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessário para macOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3. Cria a janela
    // 'g_window' deve ser uma variável global declarada em outro lugar (ex: exemplo_05.cpp)
    // ou no gl_utils.h como 'extern GLFWwindow *g_window;'.
    // Caso contrário, você precisará definir 'GLFWwindow *g_window = NULL;' aqui.
    g_window = glfwCreateWindow(g_gl_width, g_gl_height, "PGCCHIB Example", NULL, NULL);
    if (!g_window) {
        fprintf(stderr, "ERRO: Não foi possível abrir a janela com o GLFW3\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(g_window); // Torna o contexto da janela atual

    // 4. INICIALIZA O GLAD - ESTA É A PARTE CRÍTICA!
    // Esta linha deve vir DEPOIS de glfwMakeContextCurrent(g_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "ERRO: Falha ao inicializar GLAD\n");
        return false;
    }

    // Opcional: Imprime informações do contexto OpenGL (bom para depuração)
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    printf("Renderizador: %s\n", renderer);
    printf("Versão OpenGL suportada %s\n", version);

    return true; // Sucesso
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