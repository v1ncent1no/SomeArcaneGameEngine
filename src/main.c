#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <HandmadeMath.h>

#include "kp_macroutils.h"
#include "gfx/models.h"

#define ERR_MSG_BUF_LEN 1024
struct {
    struct {
       i32 width;
       i32 height;
       GLFWwindow* handle;
    } window;
    kp_mesh_t mesh;
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

    kp_vertex_t vertices[] = {
        {{-0.5, -0.5, 0.0}, {0.0, 0.0, 1.0}},
        {{0.5, -0.5, 0.0}, {0.0, 1.0, 0.0}},
        {{0.0, 0.5, 0.0}, {1.0, 0.0, 0.0}},
    };
    u32 indices[] = {
        0, 1, 2
    };

    ////////////////////////////////////////////////////////////////////////////
    /// Mesh Creation

    kp_mesh_error_t mesh_err = kp_init_mesh(
        &state.mesh,
        vertices,
        KP_ARRAY_LENGTH(vertices),
        indices,
        KP_ARRAY_LENGTH(indices),
        GL_STATIC_DRAW);


    ////////////////////////////////////////////////////////////////////////////
    /// Shader Program Creation

    const char *vert_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "layout (location = 1) in vec3 color;\n"
        "uniform mat4 mvp;\n"
        "out vec3 vert_color;\n"
        "void main() {\n"
        "   gl_Position = mvp * vec4(pos, 1.0);\n"
        "   vert_color = color;\n"
        "}\n";
    const char *frag_src =
        "#version 330 core\n"
        "out vec4 frag_color;\n"
        "in vec3 vert_color;\n"
        "void main() {\n"
        "   frag_color = vec4(vert_color, 1.0);\n"
        "}\n";

    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vert_src, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, (int*) &state.ok);
    // KP_LOG_ASSERT(state.ok, "FAILED COMPILE VERTEX SHADER");

    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &frag_src, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, (int*) &state.ok);
    // KP_LOG_ASSERT(state.ok, "FAILED COMPILE FRAGMENT SHADER");

    u32 program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, (int*) &state.ok);
    // KP_LOG_ASSERT(state.ok, "FAILED TO LINK PROGRAM");

    glDetachShader(program, vertex);
    glDetachShader(program, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    state.shader_program = program;

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
}

void frame() {
    HMM_Mat4 proj = HMM_Perspective_RH_NO(
        HMM_ToRad(70),
        (f32)state.window.width / (f32)state.window.height,
        0.01,
        1000.0
    );
    HMM_Mat4 view = HMM_LookAt_RH(
        (HMM_Vec3){{0.0, 0.0, -3.0}},
        (HMM_Vec3){{0.0, 0.0, 0.0}},
        (HMM_Vec3){{0.0, 1.0, 0.0}}
    );
    HMM_Mat4 model = HMM_Rotate_RH(glfwGetTime(),
                                   (HMM_Vec3){{ 0.0, 1.0, 0.0 }});

    const HMM_Mat4 mvp = HMM_MulM4(HMM_MulM4(proj, view), model);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.shader_program);

    glUniformMatrix4fv(
        glGetUniformLocation(state.shader_program, "mvp"),
        1,
        GL_FALSE,
        (const float*) &mvp
    );

    kp_draw_mesh(&state.mesh);

    glfwPollEvents();
    glfwSwapBuffers(state.window.handle);
}

void cleanup() {
    glDeleteProgram(state.shader_program);
    kp_free_mesh(&state.mesh);

    glfwTerminate();
}
