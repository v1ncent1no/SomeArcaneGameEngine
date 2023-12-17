#pragma once

#include "../kp_macroutils.h"
#include <glad/gl.h>

#define KP_MAX_MESH_VBO_COUNT 8

typedef struct kp_mesh {
    u32 vao;
    u32 vbo[KP_MAX_MESH_VBO_COUNT];
    u32 ebo;

    size_t index_count;
    size_t vbos_count;
} kp_mesh_t;

typedef enum kp_mesh_error {
    KP_MESH_ERROR_OK,
    KP_MESH_ERROR_VAO_NULL,
    KP_MESH_ERROR_VBO_NULL,
    KP_MESH_ERROR_EBO_NULL,
} kp_mesh_error_t;

typedef struct kp_vertex {
    f32 position[3];
    f32 color[3];
    f32 uv_coord[2];
} kp_vertex_t;

kp_mesh_error_t kp_init_mesh(kp_mesh_t* mesh, kp_vertex_t* vertices,
			  size_t vertex_count, u32 *indices,
			  size_t index_count, GLenum use);

void kp_free_mesh(kp_mesh_t *mesh);
void kp_draw_mesh(kp_mesh_t *mesh);
