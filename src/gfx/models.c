#include "models.h"

#define VBO_CURRENT_COUNT 1

kp_mesh_error_t kp_init_mesh(kp_mesh_t* mesh, kp_vertex_t* vertices,
			  size_t vertex_count, u32 *indices,
			  size_t index_count, GLenum use) {
    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    if (!vao) return KP_MESH_ERROR_VAO_NULL;

    glGenBuffers(VBO_CURRENT_COUNT, &vbo);
    if (!vbo) return KP_MESH_ERROR_VBO_NULL;
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    if (!ebo) return KP_MESH_ERROR_EBO_NULL;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(
	GL_ARRAY_BUFFER,
	sizeof(vertices) * vertex_count,
	vertices,
	use
    );

    glBufferData(
	GL_ELEMENT_ARRAY_BUFFER,
	sizeof(indices[0]) * index_count,
	indices,
	use
    );

    const size_t stride = sizeof(kp_vertex_t);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
			  (void*) KP_OFFSET_OF(kp_vertex_t, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
			  (void*) KP_OFFSET_OF(kp_vertex_t, color));
    glEnableVertexAttribArray(1);

    /*
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
			  (void*) KP_OFFSET_OF(kp_vertex_t, uv_coord));
    glEnableVertexAttribArray(2);
    */

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->vao = vao;
    mesh->vbo[0] = vbo;
    mesh->ebo = ebo;
    mesh->index_count = index_count;
    mesh->vbos_count = VBO_CURRENT_COUNT;

    return KP_MESH_ERROR_OK;
}

void kp_free_mesh(kp_mesh_t *mesh) {
    glDeleteBuffers(VBO_CURRENT_COUNT, mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteVertexArrays(1, &mesh->vao);

}

void kp_draw_mesh(kp_mesh_t *mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, NULL);
}
