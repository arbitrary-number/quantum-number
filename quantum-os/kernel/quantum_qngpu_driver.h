/*
 * Copyright 2025 Arbitrary Number Project Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Quantum Number Graphical Processing Unit (QNGPU) Kernel Driver
 * 
 * This header defines the kernel driver interface for the QNGPU/g256 graphics
 * subsystem, providing exact mathematical graphics processing with integration
 * to the Collapse Module for hybrid NVIDIA GPU operation.
 *
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_QNGPU_DRIVER_H
#define QUANTUM_QNGPU_DRIVER_H

#include "quantum_number.h"
#include "quantum_gaming.h"
#include <stdint.h>
#include <stdbool.h>

// QNGPU Driver Version
#define QNGPU_DRIVER_VERSION_MAJOR 1
#define QNGPU_DRIVER_VERSION_MINOR 0
#define QNGPU_DRIVER_VERSION_PATCH 0
#define QNGPU_DRIVER_VERSION ((QNGPU_DRIVER_VERSION_MAJOR << 16) | \
                             (QNGPU_DRIVER_VERSION_MINOR << 8) | \
                             QNGPU_DRIVER_VERSION_PATCH)

// QNGPU Hardware Specifications
#define QNGPU_MAX_QGPU_CORES 4096
#define QNGPU_MEMORY_SIZE_GB 32
#define QNGPU_MEMORY_BANDWIDTH_TBPS 1.5
#define QNGPU_MAX_RENDER_TARGETS 16
#define QNGPU_MAX_TEXTURE_UNITS 256
#define QNGPU_MAX_SHADER_PROGRAMS 1024
#define QNGPU_MAX_VERTEX_BUFFERS 64
#define QNGPU_MAX_INDEX_BUFFERS 64
#define QNGPU_MAX_UNIFORM_BUFFERS 128

// QNGPU Capabilities
#define QNGPU_CAP_EXACT_RASTERIZATION    (1 << 0)
#define QNGPU_CAP_QUANTUM_SHADERS        (1 << 1)
#define QNGPU_CAP_COLLAPSE_MODULE        (1 << 2)
#define QNGPU_CAP_NVIDIA_HYBRID          (1 << 3)
#define QNGPU_CAP_REALTIME_RAYTRACING    (1 << 4)
#define QNGPU_CAP_EXACT_PHYSICS          (1 << 5)
#define QNGPU_CAP_VR_OPTIMIZATION        (1 << 6)
#define QNGPU_CAP_8K_RENDERING           (1 << 7)
#define QNGPU_CAP_HDR_EXACT              (1 << 8)
#define QNGPU_CAP_MULTI_GPU              (1 << 9)

// Error Codes
#define QNGPU_SUCCESS                    0
#define QNGPU_ERROR_NOT_INITIALIZED     -1
#define QNGPU_ERROR_ALREADY_INITIALIZED -2
#define QNGPU_ERROR_INVALID_PARAMETER   -3
#define QNGPU_ERROR_OUT_OF_MEMORY       -4
#define QNGPU_ERROR_HARDWARE_FAILURE    -5
#define QNGPU_ERROR_DRIVER_MISMATCH     -6
#define QNGPU_ERROR_CONTEXT_NOT_FOUND   -7
#define QNGPU_ERROR_RESOURCE_BUSY       -8
#define QNGPU_ERROR_OPERATION_FAILED    -9
#define QNGPU_ERROR_COLLAPSE_MODULE     -10
#define QNGPU_ERROR_NVIDIA_INTEGRATION  -11

// QNGPU Context Types
typedef enum {
    QNGPU_CONTEXT_GRAPHICS,
    QNGPU_CONTEXT_COMPUTE,
    QNGPU_CONTEXT_VR,
    QNGPU_CONTEXT_HYBRID_NVIDIA
} qngpu_context_type_t;

// QNGPU Render Modes
typedef enum {
    QNGPU_RENDER_MODE_EXACT,
    QNGPU_RENDER_MODE_HYBRID,
    QNGPU_RENDER_MODE_NVIDIA_FALLBACK
} qngpu_render_mode_t;

// QNGPU Shader Types
typedef enum {
    QNGPU_SHADER_VERTEX,
    QNGPU_SHADER_FRAGMENT,
    QNGPU_SHADER_GEOMETRY,
    QNGPU_SHADER_COMPUTE,
    QNGPU_SHADER_QUANTUM_EXACT
} qngpu_shader_type_t;

// QNGPU Texture Formats
typedef enum {
    QNGPU_TEXTURE_FORMAT_RGBA8,
    QNGPU_TEXTURE_FORMAT_RGBA16,
    QNGPU_TEXTURE_FORMAT_RGBA32F,
    QNGPU_TEXTURE_FORMAT_QUANTUM_EXACT,
    QNGPU_TEXTURE_FORMAT_HDR_EXACT
} qngpu_texture_format_t;

// QNGPU Buffer Types
typedef enum {
    QNGPU_BUFFER_VERTEX,
    QNGPU_BUFFER_INDEX,
    QNGPU_BUFFER_UNIFORM,
    QNGPU_BUFFER_STORAGE,
    QNGPU_BUFFER_QUANTUM_EXACT
} qngpu_buffer_type_t;

// QNGPU Hardware Information
typedef struct {
    uint32_t device_id;
    uint32_t vendor_id;
    uint32_t driver_version;
    uint32_t qgpu_core_count;
    uint64_t memory_size;
    uint64_t memory_bandwidth;
    uint32_t capabilities;
    bool collapse_module_present;
    bool nvidia_gpu_detected;
    char device_name[256];
    char driver_name[128];
} qngpu_device_info_t;

// QNGPU Context Configuration
typedef struct {
    qngpu_context_type_t context_type;
    qngpu_render_mode_t render_mode;
    uint32_t width;
    uint32_t height;
    uint32_t refresh_rate;
    bool vsync_enabled;
    bool exact_precision_enabled;
    bool collapse_module_enabled;
    bool nvidia_hybrid_enabled;
    uint32_t msaa_samples;
    uint32_t anisotropic_filtering;
} qngpu_context_config_t;

// QNGPU Context Handle
typedef struct {
    uint32_t context_id;
    qngpu_context_type_t context_type;
    qngpu_render_mode_t render_mode;
    bool active;
    uint32_t width;
    uint32_t height;
    uint32_t refresh_rate;
    bool exact_precision_enabled;
    bool collapse_module_active;
    bool nvidia_hybrid_active;
    void* private_data;
} qngpu_context_t;

// QNGPU Vertex Data with Exact Precision
typedef struct {
    quantum_number_t position_x;
    quantum_number_t position_y;
    quantum_number_t position_z;
    quantum_number_t position_w;
    quantum_number_t normal_x;
    quantum_number_t normal_y;
    quantum_number_t normal_z;
    quantum_number_t texture_u;
    quantum_number_t texture_v;
    quantum_number_t color_r;
    quantum_number_t color_g;
    quantum_number_t color_b;
    quantum_number_t color_a;
} qngpu_vertex_exact_t;

// QNGPU Shader Program
typedef struct {
    uint32_t program_id;
    qngpu_shader_type_t shader_type;
    bool active;
    bool exact_precision_enabled;
    char* source_code;
    size_t source_size;
    void* compiled_data;
    size_t compiled_size;
} qngpu_shader_program_t;

// QNGPU Texture Resource
typedef struct {
    uint32_t texture_id;
    qngpu_texture_format_t format;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t mip_levels;
    bool exact_precision_enabled;
    void* texture_data;
    size_t data_size;
} qngpu_texture_t;

// QNGPU Buffer Resource
typedef struct {
    uint32_t buffer_id;
    qngpu_buffer_type_t buffer_type;
    size_t buffer_size;
    bool exact_precision_enabled;
    void* buffer_data;
    bool gpu_resident;
} qngpu_buffer_t;

// QNGPU Render State
typedef struct {
    uint32_t active_context_id;
    uint32_t active_shader_program;
    uint32_t bound_textures[QNGPU_MAX_TEXTURE_UNITS];
    uint32_t bound_vertex_buffers[QNGPU_MAX_VERTEX_BUFFERS];
    uint32_t bound_index_buffer;
    uint32_t bound_uniform_buffers[QNGPU_MAX_UNIFORM_BUFFERS];
    qngpu_render_mode_t current_render_mode;
    bool exact_rasterization_enabled;
    bool collapse_module_active;
} qngpu_render_state_t;

// QNGPU Performance Metrics
typedef struct {
    uint64_t frames_rendered;
    uint64_t triangles_processed;
    uint64_t pixels_rendered;
    uint64_t shader_invocations;
    uint64_t memory_allocated;
    uint64_t memory_used;
    uint32_t current_fps;
    uint32_t average_fps;
    float gpu_utilization;
    float memory_utilization;
    uint64_t exact_calculations_performed;
    uint64_t collapse_module_operations;
} qngpu_performance_metrics_t;

// QNGPU Collapse Module Configuration
typedef struct {
    bool enabled;
    uint32_t nvidia_device_id;
    uint64_t nvidia_memory_size;
    bool hybrid_rendering_enabled;
    bool fallback_enabled;
    float load_balance_ratio; // 0.0 = all QNGPU, 1.0 = all NVIDIA
} qngpu_collapse_config_t;

// Function Declarations

// Driver Management
int qngpu_driver_init(void);
int qngpu_driver_shutdown(void);
int qngpu_get_device_info(qngpu_device_info_t* device_info);
int qngpu_get_performance_metrics(qngpu_performance_metrics_t* metrics);

// Context Management
int qngpu_create_context(qngpu_context_config_t* config, uint32_t* context_id);
int qngpu_destroy_context(uint32_t context_id);
int qngpu_make_context_current(uint32_t context_id);
int qngpu_swap_buffers(uint32_t context_id);

// Shader Management
int qngpu_create_shader_program(qngpu_shader_type_t shader_type, 
                               const char* source_code, 
                               size_t source_size,
                               uint32_t* program_id);
int qngpu_destroy_shader_program(uint32_t program_id);
int qngpu_use_shader_program(uint32_t program_id);
int qngpu_set_shader_uniform_quantum(uint32_t program_id, 
                                    const char* uniform_name,
                                    quantum_number_t* value);

// Buffer Management
int qngpu_create_buffer(qngpu_buffer_type_t buffer_type, 
                       size_t buffer_size,
                       bool exact_precision,
                       uint32_t* buffer_id);
int qngpu_destroy_buffer(uint32_t buffer_id);
int qngpu_bind_buffer(uint32_t buffer_id);
int qngpu_upload_buffer_data(uint32_t buffer_id, 
                            const void* data, 
                            size_t data_size,
                            size_t offset);
int qngpu_upload_vertex_data_exact(uint32_t buffer_id,
                                  qngpu_vertex_exact_t* vertices,
                                  uint32_t vertex_count);

// Texture Management
int qngpu_create_texture(qngpu_texture_format_t format,
                        uint32_t width,
                        uint32_t height,
                        uint32_t depth,
                        bool exact_precision,
                        uint32_t* texture_id);
int qngpu_destroy_texture(uint32_t texture_id);
int qngpu_bind_texture(uint32_t texture_id, uint32_t texture_unit);
int qngpu_upload_texture_data(uint32_t texture_id,
                             const void* data,
                             size_t data_size);
int qngpu_upload_texture_data_exact(uint32_t texture_id,
                                   quantum_number_t* pixel_data,
                                   uint32_t pixel_count);

// Rendering Operations
int qngpu_clear_framebuffer(quantum_number_t* clear_color);
int qngpu_draw_triangles(uint32_t vertex_count, uint32_t first_vertex);
int qngpu_draw_indexed_triangles(uint32_t index_count, uint32_t first_index);
int qngpu_draw_triangles_exact(qngpu_vertex_exact_t* vertices, uint32_t vertex_count);
int qngpu_set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
int qngpu_set_render_mode(qngpu_render_mode_t render_mode);

// Exact Rasterization
int qngpu_enable_exact_rasterization(bool enabled);
int qngpu_rasterize_triangle_exact(qngpu_vertex_exact_t* v0,
                                  qngpu_vertex_exact_t* v1,
                                  qngpu_vertex_exact_t* v2,
                                  uint32_t framebuffer_id);
int qngpu_rasterize_line_exact(qngpu_vertex_exact_t* v0,
                              qngpu_vertex_exact_t* v1,
                              uint32_t framebuffer_id);
int qngpu_rasterize_point_exact(qngpu_vertex_exact_t* vertex,
                               uint32_t framebuffer_id);

// Collapse Module Integration
int qngpu_collapse_module_init(qngpu_collapse_config_t* config);
int qngpu_collapse_module_shutdown(void);
int qngpu_collapse_module_set_load_balance(float ratio);
int qngpu_collapse_module_sync_nvidia(void);
int qngpu_collapse_module_fallback_to_nvidia(void);
int qngpu_collapse_module_restore_qngpu(void);

// VR Gaming Support
int qngpu_vr_init(uint32_t eye_width, uint32_t eye_height, uint32_t refresh_rate);
int qngpu_vr_shutdown(void);
int qngpu_vr_begin_frame(void);
int qngpu_vr_end_frame(void);
int qngpu_vr_render_eye(uint32_t eye_index, qngpu_vertex_exact_t* vertices, uint32_t vertex_count);
int qngpu_vr_set_tracking_data(quantum_number_t* head_position,
                              quantum_number_t* head_rotation,
                              quantum_number_t* controller_positions,
                              quantum_number_t* controller_rotations);

// Gaming Integration
int qngpu_gaming_register_process(uint32_t process_id);
int qngpu_gaming_unregister_process(uint32_t process_id);
int qngpu_gaming_set_priority(uint32_t process_id, uint32_t priority);
int qngpu_gaming_enable_low_latency(uint32_t process_id, bool enabled);
int qngpu_gaming_enable_exact_physics(uint32_t process_id, bool enabled);

// Debug and Profiling
int qngpu_debug_enable(bool enabled);
int qngpu_debug_capture_frame(const char* filename);
int qngpu_profile_begin(void);
int qngpu_profile_end(qngpu_performance_metrics_t* metrics);
int qngpu_validate_exact_precision(bool* validation_passed);

#endif // QUANTUM_QNGPU_DRIVER_H
