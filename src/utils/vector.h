#pragma once
#include  <SDL3/SDL.h>

#include "stringify.h"


#define VECTOR_DECL(type, name)\
typedef struct name {\
    type *internalArray;\
    size_t capacity;\
    size_t length;\
    \
    void (*elementFreeFunc)(type *self);\
    bool (*elementCopyFunc)(type *self, type *outCopy);\
    \
} name;\
\
\
bool name##_init(size_t initialCapacity, name *outVector);\
/* WARNING!: Borrowed pointers into elements will be invalidated, since the function directly frees the memory */\
void name##_free(name *vector);\
\
void name##_clear(name *vector);\
bool name##_get(const name *vector, size_t index, type *outElement);\
/* WARNING!: The pointer given will be invalid after a successful resizeTo function call that changes capacity */\
bool name##_borrow(name *vector, size_t index, type **outElement);\
/* WARNING!: A successful call of this function will invalidate any borrowed pointers into elements, since it calls resizeTo function */\
bool name##_set(name *vector, size_t index, type newElement);\
/* WARNING!: A successful call of this function will invalidate any borrowed pointers into elements, since it calls resizeTo function */\
bool name##_insert(name *vector, size_t index, type newElement);\
/* WARNING!: A successful call of this function will invalidate any borrowed pointers into elements, since it calls resizeTo function */\
bool name##_delete(name *vector, size_t index);\
/* WARNING!: A successful call of this function will invalidate any borrowed pointers into elements, since it calls resizeTo function */\
bool name##_push(name *vector, type newElement);\
/* WARNING!: A successful call of this function will invalidate any borrowed pointers into elements, since it calls resizeTo function */\
bool name##_pop(name *vector, type *outElement);\
/* NOTE: Elements are raw memory relocated */\
/* WARNING!: A successful call of this function will directly invalidate any borrowed pointers into elements */\
bool name##_resizeTo(name *vector, size_t newCapacity);\
/* WARNING!: This function will invalidate any borrowed pointers into elements if it successfully calls resizeTo function */\
bool name##_reserve(name *vector, size_t minimumCapacity);\


#define VECTOR_IMPL(type, name, typeFreeFunc, typeCopyFunc)\
bool name##_init(size_t initialCapacity, name *outVector) {\
    name vector; SDL_zero(vector);\
    vector.elementFreeFunc = typeFreeFunc;\
    vector.elementCopyFunc = typeCopyFunc;\
    \
    if (!name##_resizeTo(&vector, initialCapacity)) {\
        return false;\
    }\
    \
    *outVector = vector;\
    return true;\
}\
\
void name##_free(name *vector) {\
    name##_clear(vector);\
    \
    if (vector->internalArray) SDL_free(vector->internalArray);\
    vector->internalArray = NULL;\
    vector->capacity = 0;\
    vector->length = 0;\
}\
\
void name##_clear(name *vector) {\
    if (vector->internalArray) {\
        if (vector->elementFreeFunc) {\
            for (size_t i = 0; i < vector->length; i++) {\
                vector->elementFreeFunc(&vector->internalArray[i]);\
            }\
        }\
        \
        vector->length = 0;\
    }\
}\
\
bool name##_get(const name *vector, size_t index, type *outElement) {\
    if (index >= vector->length) {\
        return SDL_SetError("%s vector: Can't get element at an index of %zu that is out of bounds", STRINGIFY(name), index);\
    }\
    \
    type copy;\
    if (vector->elementCopyFunc) {\
        if (!vector->elementCopyFunc(&vector->internalArray[index], &copy)) {\
            return SDL_SetError("%s vector: Can't get element due to failure of copying it", STRINGIFY(name));\
        }\
    }\
    else {\
        copy = vector->internalArray[index];\
    }\
    \
    *outElement = copy;\
    return true;\
}\
\
bool name##_borrow(name *vector, size_t index, type **outBorrowedElement) {\
    if (index >= vector->length) {\
        return SDL_SetError("%s vector: Can't borrow element at an index of %zu that is out of bounds", STRINGIFY(name), index);\
    }\
    \
    *outBorrowedElement = &vector->internalArray[index];\
    return true;\
}\
\
bool name##_set(name *vector, size_t index, type newElement) {\
    if (index >= vector->length) {\
        return SDL_SetError("%s vector: Can't set element at an index of %zu that is out of bounds", STRINGIFY(name), index);\
    }\
    \
    type copy;\
    if (vector->elementCopyFunc) {\
        if (!vector->elementCopyFunc(&newElement, &copy)) {\
            return SDL_SetError("%s vector: Can't set element due to failure of copying newElement parameter", STRINGIFY(name));\
        }\
    }\
    else {\
        copy = newElement;\
    }\
    \
    if (vector->elementFreeFunc) {\
        vector->elementFreeFunc(&vector->internalArray[index]);\
    }\
    \
    vector->internalArray[index] = copy;\
    return true;\
}\
\
bool name##_insert(name *vector, size_t index, type newElement) {\
    if (index > vector->length) {\
        return SDL_SetError("%s vector: Can't insert element at an index of %zu that is out of bounds", STRINGIFY(name), index);\
    }\
    \
    type copy;\
    if (vector->elementCopyFunc) {\
        if (!vector->elementCopyFunc(&newElement, &copy)) {\
            return SDL_SetError("%s vector: Can't insert element due to failure of copying newElement parameter", STRINGIFY(name));\
        }\
    }\
    else {\
        copy = newElement;\
    }\
    \
    if (vector->length >= vector->capacity) {\
        if (vector->capacity > SDL_SIZE_MAX / sizeof(type) / 2) {\
            return SDL_SetError("%s vector: Can't insert newElement due to vector's capacity being TOO big", STRINGIFY(name));\
        }\
        \
        if (!name##_resizeTo(vector, vector->capacity < 1 ? 1 : vector->capacity * 2)) {\
            if (vector->elementFreeFunc) {\
                vector->elementFreeFunc(&copy);\
            }\
            return false;\
        }\
    }\
    \
    SDL_memmove(\
        &vector->internalArray[index + 1],\
        &vector->internalArray[index],\
        sizeof(type) * (vector->length - index)\
    );\
    \
    vector->internalArray[index] = copy;\
    vector->length++;\
    \
    return true;\
}\
\
bool name##_delete(name *vector, size_t index) {\
    if (index >= vector->length) {\
        return SDL_SetError("%s vector: Can't delete element at an index of %zu that is out of bounds", STRINGIFY(name), index);\
    }\
    \
    if (vector->elementFreeFunc) {\
        vector->elementFreeFunc(&vector->internalArray[index]);\
    }\
    \
    vector->length--; /* This is intentional */\
    \
    SDL_memmove(\
        &vector->internalArray[index],\
        &vector->internalArray[index + 1],\
        sizeof(type) * (vector->length - index)\
    );\
    \
    return true;\
}\
\
bool name##_push(name *vector, type newElement) {\
    return name##_insert(vector, vector->length, newElement);\
}\
\
bool name##_pop(name *vector, type *outElement) {\
    if (vector->length == 0) {\
        return SDL_SetError("%s vector: Can't pop a element from a empty vector", STRINGIFY(name));\
    }\
    \
    if (outElement) {\
        if (!name##_get(vector, vector->length - 1, outElement)) {\
            return false;\
        }\
    }\
    \
    return name##_delete(vector, vector->length - 1);\
}\
\
bool name##_resizeTo(name *vector, size_t newCapacity) {\
    newCapacity = newCapacity < 1 ? 1 : newCapacity;\
    \
    if (newCapacity == vector->capacity) {\
        return true;\
    }\
    \
    if (newCapacity > SDL_SIZE_MAX / sizeof(type)) {\
        return SDL_SetError("%s vector: newCapacity is too big", STRINGIFY(name));\
    }\
    \
    if (newCapacity < vector->length) {\
        return SDL_SetError("%s vector: Resizing to capacity of %zu will delete %zu elements", STRINGIFY(name), newCapacity, vector->length - newCapacity);\
    }\
    \
    type *newInternalArray = SDL_malloc(newCapacity * sizeof(type));\
    if (!newInternalArray) {\
        return SDL_SetError("%s vector: Failed to resize due to allocation failure of the new resized internal array", STRINGIFY(name));\
    }\
    \
    if (vector->internalArray) {\
        SDL_memcpy(newInternalArray, vector->internalArray, sizeof(type) * vector->length);\
        SDL_free(vector->internalArray);\
    }\
    \
    vector->internalArray = newInternalArray;\
    vector->capacity = newCapacity;\
    return true;\
}\
\
bool name##_reserve(name *vector, size_t minimumCapacity) {\
    if (minimumCapacity <= vector->capacity) {\
        return true;\
    }\
    \
    return name##_resizeTo(vector, minimumCapacity);\
}\
