#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <HandmadeMath.h>

#define KP_USE_LIBC_ASSERT
#include "kp_macroutils.h"

#define ERR_MSG_BUF_LEN 1024
struct {
    struct {
       i32 width;
       i32 height;
       GLFWwindow* handle;
    } window;

    struct {
        u32 vao;
        u32 vbo;
        u32 ebo;

        u32 elem_count;
    } mesh;
    u32 shader_program;

    char err_msg_buf[ERR_MSG_BUF_LEN];
    bool ok;
} state = {
    .window = {
        .width = 1280,
        .height = 720,
    },
    .ok = true,
};

void init();
void frame();
void cleanup();

void glfw_window_resize_callback(GLFWwindow* win, i32 width, i32 height) {
    glViewport(0, 0, width, height);
    state.window.width = width;
    state.window.height = height;
}

int main(void) {
    init();
    if (!state.ok) {
        fprintf(stderr, "%s\n", state.err_msg_buf); // I think, i can do better
                                                    // than that...
                                                    // But I'm too lazy rn
        return EXIT_FAILURE;
    }

    while (!glfwWindowShouldClose(state.window.handle))
        frame();

    cleanup();
    return 0;
}

#define ERROR_BUFFER state.err_msg_buf
#define ERROR(MSG) \
    KP_STATEMENT(strcpy(ERROR_BUFFER, "error: " MSG));\
    KP_STATEMENT(state.ok = false);

void init() {
    if (!glfwInit()) {
        ERROR("failed to initialize GLFW");
        return;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    state.window.handle = glfwCreateWindow(
        state.window.width,
        state.window.height,
        "WINDOW",
        NULL,
        NULL
    );
    if (!state.window.handle) {
        ERROR("failed to create a window")
        return;
    }
    glfwMakeContextCurrent(state.window.handle);

    glfwSetFramebufferSizeCallback(
        state.window.handle,
        glfw_window_resize_callback
    );

    if (!gladLoadGL(glfwGetProcAddress)) {
        ERROR("failed to initialize OpenGL");
        return;
    }

    f32 vertices[] = {
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0,
         0.0,  0.5, 0.0
    };
    u32 indices[] = {
        0, 1, 2
    };

    ////////////////////////////////////////////////////////////////////////////
    /// Mesh Creation

    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    state.mesh.elem_count = sizeof(indices) / sizeof(indices[0]);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    const size_t stride = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    state.mesh.vao = vao;
    state.mesh.vbo = vbo;
    state.mesh.ebo = ebo;

    ////////////////////////////////////////////////////////////////////////////
    /// Shader Program Creation

    const char *vert_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "void main() {\n"
        "   gl_Position = vec4(pos, 1.0);\n"
        "}\n";
    const char *frag_src =
        "#version 330 core\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "   frag_color = vec4(1.0);\n"
        "}\n";

    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vert_src, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, (int*) &state.ok);
    KP_LOG_ASSERT(state.ok, "FAILED COMPILE VERTEX SHADER");

    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &frag_src, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, (int*) &state.ok);
    KP_LOG_ASSERT(state.ok, "FAILED COMPILE FRAGMENT SHADER");

    u32 program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, (int*) &state.ok);
    KP_LOG_ASSERT(state.ok, "FAILED TO LINK PROGRAM")

    glDetachShader(program, vertex);
    glDetachShader(program, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    state.shader_program = program;

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
}

void frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.shader_program);
    glBindVertexArray(state.mesh.vao);
    glDrawElements(GL_TRIANGLES, state.mesh.elem_count, GL_UNSIGNED_INT, NULL);

    glfwPollEvents();
    glfwSwapBuffers(state.window.handle);
}

void cleanup() {
    glDeleteProgram(state.shader_program);

    glDeleteBuffers(1, &state.mesh.vbo);
    glDeleteBuffers(1, &state.mesh.ebo);
    glDeleteVertexArrays(1, &state.mesh.vao);

    glfwTerminate();
}
