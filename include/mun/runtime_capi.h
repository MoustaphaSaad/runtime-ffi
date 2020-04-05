#ifndef MUN_RUNTIME_BINDINGS_H_
#define MUN_RUNTIME_BINDINGS_H_

/* Generated with cbindgen:0.14.0 */

#include <stdbool.h>
#include <stdint.h>

/**
 * Represents the privacy level of modules, functions, or variables.
 */
enum MunPrivacy
#ifdef __cplusplus
  : uint8_t
#endif // __cplusplus
 {
    /**
     * Publicly (and privately) accessible
     */
    Public = 0,
    /**
     * Privately accessible
     */
    Private = 1,
};
#ifndef __cplusplus
typedef uint8_t MunPrivacy;
#endif // __cplusplus

/**
 * Represents the kind of memory management a struct uses.
 */
enum MunStructMemoryKind
#ifdef __cplusplus
  : uint8_t
#endif // __cplusplus
 {
    /**
     * A garbage collected struct is allocated on the heap and uses reference semantics when passed
     * around.
     */
    GC,
    /**
     * A value struct is allocated on the stack and uses value semantics when passed around.
     *
     * NOTE: When a value struct is used in an external API, a wrapper is created that _pins_ the
     * value on the heap. The heap-allocated value needs to be *manually deallocated*!
     */
    Value,
};
#ifndef __cplusplus
typedef uint8_t MunStructMemoryKind;
#endif // __cplusplus

/**
 * Represents a group of types that illicit the same characteristics.
 */
enum MunTypeGroup
#ifdef __cplusplus
  : uint8_t
#endif // __cplusplus
 {
    /**
     * Fundamental types (i.e. `()`, `bool`, `float`, `int`, etc.)
     */
    FundamentalTypes = 0,
    /**
     * Struct types (i.e. record, tuple, or unit structs)
     */
    StructTypes = 1,
};
#ifndef __cplusplus
typedef uint8_t MunTypeGroup;
#endif // __cplusplus

typedef uintptr_t MunToken;

/**
 * A C-style handle to an error.
 */
typedef struct {
    MunToken _0;
} MunErrorHandle;

/**
 * A C-style handle to a runtime.
 */
typedef struct {
    void *_0;
} MunRuntimeHandle;

/**
 * Represents a globally unique identifier (GUID).
 *
 * GUIDs are generated by taking the MD5 hash of a type's name.
 *
 * <div rustbindgen derive="Clone" derive="Copy" derive="Debug" derive="Eq" derive="PartialEq" derive="Ord" derive="PartialOrd"></div>
 */
typedef struct {
    /**
     * 16-byte MD5 hash
     */
    uint8_t b[16];
} MunGuid;

/**
 * Represents the type declaration for a value type.
 *
 * TODO: add support for structs, polymorphism, enumerations, type parameters, generic type definitions, and constructed generic types.
 *
 * <div rustbindgen derive="Debug"></div>
 */
typedef struct {
    /**
     * Type GUID
     */
    MunGuid guid;
    /**
     * Type name
     */
    const char *name;
    /**
     * The exact size of the type in bits without any padding
     */
    uint32_t size_in_bits;
    /**
     * The alignment of the type
     */
    uint8_t alignment;
    /**
     * Type group
     */
    MunTypeGroup group;
} MunTypeInfo;

/**
 * Represents a function signature.
 *
 * <div rustbindgen derive="Clone" derive="Debug"></div>
 */
typedef struct {
    /**
     * Function name
     */
    const char *name;
    /**
     * Argument types
     */
    const MunTypeInfo *const *arg_types;
    /**
     * Optional return type
     */
    const MunTypeInfo *return_type;
    /**
     * Number of argument types
     */
    uint16_t num_arg_types;
    /**
     * Function accessibility level
     */
    MunPrivacy privacy;
} MunFunctionSignature;

/**
 * Represents a function declaration.
 *
 * `fn_ptr` can be used to call the declared function.
 *
 * <div rustbindgen derive="Clone" derive="Debug"></div>
 */
typedef struct {
    /**
     * Function signature
     */
    MunFunctionSignature signature;
    /**
     * Function pointer
     */
    const void *fn_ptr;
} MunFunctionInfo;

/**
 * Represents a struct declaration.
 *
 * <div rustbindgen derive="Clone" derive="Debug"></div>
 */
typedef struct {
    /**
     * Struct name
     */
    const char *name;
    /**
     * Struct fields' names
     */
    const char *const *field_names;
    /**
     * Struct fields' information
     */
    const MunTypeInfo *const *field_types;
    /**
     * Struct fields' offsets
     */
    const uint16_t *field_offsets;
    /**
     * Number of fields
     */
    uint16_t num_fields;
    /**
     * Struct memory kind
     */
    MunStructMemoryKind memory_kind;
} MunStructInfo;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Destructs the error corresponding to `error_handle`.
 */
void mun_error_destroy(MunErrorHandle error_handle);

/**
 * Retrieves the error message corresponding to `error_handle`. If the `error_handle` exists, a
 * valid `char` pointer is returned, otherwise a null-pointer is returned.
 */
const char *mun_error_message(MunErrorHandle error_handle);

/**
 * Constructs a new runtime that loads the library at `library_path` and its dependencies. If
 * successful, the runtime `handle` is set, otherwise a non-zero error handle is returned.
 *
 * If a non-zero error handle is returned, it must be manually destructed using
 * [`mun_error_destroy`].
 *
 * The runtime must be manually destructed using [`mun_runtime_destroy`].
 *
 * TODO: expose interval at which the runtime's file watcher operates.
 *
 * # Safety
 *
 * This function receives raw pointers as parameters. If any of the arguments is a null pointer,
 * an error will be returned. Passing pointers to invalid data, will lead to undefined behavior.
 */
MunErrorHandle mun_runtime_create(const char *library_path, MunRuntimeHandle *handle);

/**
 * Destructs the runtime corresponding to `handle`.
 */
void mun_runtime_destroy(MunRuntimeHandle handle);

/**
 * Retrieves the [`FunctionInfo`] for `fn_name` from the runtime corresponding to `handle`. If
 * successful, `has_fn_info` and `fn_info` are set, otherwise a non-zero error handle is returned.
 *
 * If a non-zero error handle is returned, it must be manually destructed using
 * [`mun_error_destroy`].
 *
 * # Safety
 *
 * This function receives raw pointers as parameters. If any of the arguments is a null pointer,
 * an error will be returned. Passing pointers to invalid data, will lead to undefined behavior.
 */
MunErrorHandle mun_runtime_get_function_info(MunRuntimeHandle handle,
                                             const char *fn_name,
                                             bool *has_fn_info,
                                             MunFunctionInfo *fn_info);

/**
 * Updates the runtime corresponding to `handle`. If successful, `updated` is set, otherwise a
 * non-zero error handle is returned.
 *
 * If a non-zero error handle is returned, it must be manually destructed using
 * [`mun_error_destroy`].
 *
 * # Safety
 *
 * This function receives raw pointers as parameters. If any of the arguments is a null pointer,
 * an error will be returned. Passing pointers to invalid data, will lead to undefined behavior.
 */
MunErrorHandle mun_runtime_update(MunRuntimeHandle handle, bool *updated);

/**
 * Retrieves the [`StructInfo`] corresponding to `type_info`, if the type is a struct. If
 * successful, `struct_info` is set, otherwise a non-zero error handle is returned.
 *
 * If a non-zero error handle is returned, it must be manually destructed using
 * [`mun_error_destroy`].
 *
 * # Safety
 *
 * This function receives raw pointers as parameters. If any of the arguments is a null pointer,
 * an error will be returned. Passing pointers to invalid data, will lead to undefined behavior.
 */
MunErrorHandle mun_type_info_as_struct(const MunTypeInfo *type_info, MunStructInfo *struct_info);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* MUN_RUNTIME_BINDINGS_H_ */
