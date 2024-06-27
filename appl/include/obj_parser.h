#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int v_index;
    int vt_index;
    int vn_index;
} obj_vertex_info_t;

typedef struct
{
    float x;
    float y;
    float z;
} obj_float3_t;

typedef struct
{
    float x;
    float y;
} obj_float2_t;

typedef struct
{
    obj_float3_t position; // alias 'v'
    obj_float3_t normal;   // alias 'vn'
    obj_float2_t uv;       // alias 'vt'
} obj_vertex_t;

typedef struct
{
    obj_vertex_t v1;
    obj_vertex_t v2;
    obj_vertex_t v3;
} obj_triangle_t;

typedef struct
{
    int v_count;
    int vt_count;
    int vn_count;
    int f_count;
    obj_float3_t *v;
    obj_float2_t *vt;
    obj_float3_t *vn;
    obj_vertex_info_t *v_info;
    size_t triangle_count;
    obj_triangle_t *triangles;
} obj_t;

/// @param file_name Path of source file
/// @return Object structure
// Function declarations
obj_t *obj_parse(const char *file_name);


/// @brief destroy the obj
/// @param obj 
void obj_parse_destroy(obj_t *obj);

// Define declarations
#ifdef OBJ_IMPLEMENTATION

obj_t *obj_parse(const char *file_name)
{
    obj_t *obj = (obj_t *)malloc(sizeof(obj_t));
    obj->v_count = 0;
    obj->vt_count = 0;
    obj->vn_count = 0;
    obj->f_count = 0;

    FILE *file;
    fopen_s(&file, file_name, "r");

    if (file == NULL)
    {
        printf("ERROR READ FILE: %s\n", file_name);
        return NULL;
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strncmp(buffer, "v ", 2) == 0)
        {
            obj->v_count++;
        }

        if (strncmp(buffer, "vt ", 3) == 0)
        {
            obj->vt_count++;
        }

        if (strncmp(buffer, "vn ", 3) == 0)
        {
            obj->vn_count++;
        }

        if (strncmp(buffer, "f ", 2) == 0)
        {
            obj->f_count++;
        }
    }

    obj->v = (obj_float3_t *)malloc(obj->v_count * sizeof(obj_float3_t));
    obj->vt = (obj_float2_t *)malloc(obj->vt_count * sizeof(obj_float2_t));
    obj->vn = (obj_float3_t *)malloc(obj->vn_count * sizeof(obj_float3_t));
    obj->v_info = (obj_vertex_info_t *)malloc(obj->f_count * sizeof(obj_vertex_info_t) * 3);

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        perror("Error: trying to reset fseek");
        fclose(file);
        return NULL;
    }

    int v_index = 0;
    int vt_index = 0;
    int vn_index = 0;
    int f_index = 0;
    size_t triangle_count = 0;

    // Inside your loop:
    while (fgets(buffer, sizeof(buffer), file))
    {
        char *remaining_tokens;
        char *token = strtok_s(buffer, " ", &remaining_tokens);

        if (strcmp(token, "v") == 0)
        {
            // Get values from buffer and fill struct
            obj->v[v_index].x = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            obj->v[v_index].y = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            obj->v[v_index].z = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            v_index++;
        }

        else if (strcmp(token, "vt") == 0)
        {
            // Get values from buffer and fill struct
            obj->vt[vt_index].x = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            obj->vt[vt_index].y = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            vt_index++;
        }

        else if (strcmp(token, "vn") == 0)
        {
            // Get values from buffer and fill struct
            obj->vn[vn_index].x = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            obj->vn[vn_index].y = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            obj->vn[vn_index].z = strtof(strtok_s(NULL, " ", &remaining_tokens), NULL);
            vn_index++;
        }

        else if (strcmp(token, "f") == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                // Get values from buffer and fill struct
                obj->v_info[f_index + i].v_index = strtol(strtok_s(NULL, "/", &remaining_tokens), NULL, 10);
                obj->v_info[f_index + i].vt_index = strtol(strtok_s(NULL, "/", &remaining_tokens), NULL, 10);
                obj->v_info[f_index + i].vn_index = strtol(strtok_s(NULL, " ", &remaining_tokens), NULL, 10);
            }

            f_index += 3;
        }
    }

    fclose(file);

    // Set triangle count
    obj->triangle_count = obj->f_count;

    // Allocate triangles struct
    obj->triangles = (obj_triangle_t *)malloc(sizeof(obj_triangle_t) * obj->triangle_count);

    // Loop faces
    for (size_t i = 0; i < obj->triangle_count; i++)
    {
        // Set current face index for v_info
        const int f_index = i * 3;

        // Set indexes for vertex 1
        const int index_v1 = obj->v_info[f_index + 0].v_index - 1;
        const int index_vt1 = obj->v_info[f_index + 0].vt_index - 1;
        const int index_vn1 = obj->v_info[f_index + 0].vn_index - 1;

        // Set position, uv and normals
        obj->triangles[i].v1.position = obj->v[index_v1];
        obj->triangles[i].v1.uv = obj->vt[index_vt1];
        obj->triangles[i].v1.normal = obj->vn[index_vn1];

        // Set indexes for vertex 2
        const int index_v2 = obj->v_info[f_index + 1].v_index - 1;
        const int index_vt2 = obj->v_info[f_index + 1].vt_index - 1;
        const int index_vn2 = obj->v_info[f_index + 1].vn_index - 1;

        // Set position, uv and normals
        obj->triangles[i].v2.position = obj->v[index_v2];
        obj->triangles[i].v2.uv = obj->vt[index_vt2];
        obj->triangles[i].v2.normal = obj->vn[index_vn2];

        // Set indexes for vertex 3
        const int index_v3 = obj->v_info[f_index + 2].v_index - 1;
        const int index_vt3 = obj->v_info[f_index + 2].vt_index - 1;
        const int index_vn3 = obj->v_info[f_index + 2].vn_index - 1;

        // Set position, uv and normals
        obj->triangles[i].v3.position = obj->v[index_v3];
        obj->triangles[i].v3.uv = obj->vt[index_vt3];
        obj->triangles[i].v3.normal = obj->vn[index_vn3];
    }

    return obj;
}

void obj_parse_destroy(obj_t *obj)
{
    free(obj->v);
    free(obj->vt);
    free(obj->vn);
    free(obj->v_info);
    free(obj->triangles);
    free(obj);
}

#endif // OBJ_IMPLEMENTATION

#endif // end define OBJ_PARSER_H