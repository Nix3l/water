#include "entity.h"

mat4s entity_transformation_matrix(entity_s* entity) {
    mat4s transformation = MAT4_IDENTITY;
    transform_s* transform = &entity->transform;
    transformation = glms_translate(transformation, transform->position);
    transformation = glms_rotate_x(transformation, glm_rad(transform->rotation.x));
    transformation = glms_rotate_y(transformation, glm_rad(transform->rotation.y));
    transformation = glms_rotate_z(transformation, glm_rad(transform->rotation.z));
    transformation = glms_scale(transformation, transform->scale);
    return transformation;
}
