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
 * QNGPU Driver - Quantum Number Graphical Processing Unit Driver
 * 
 * This driver provides kernel-level support for the g256 QNGPU chip,
 * enabling exact mathematical graphics processing with Linux compatibility.
 * 
 * Copyright (C) 2025 Arbitrary Number Project Team
 * Licensed under Apache License 2.0
 */

#ifndef QNGPU_DRIVER_H
#define QNGPU_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "quantum_number.h"
#include "quantix_qfs.h"

/* QNGPU Hardware Constants */
#define QNGPU_MAX_DEVICES           8
#define QNGPU_CORES_PER_CLUSTER     1024
#define QNGPU_CLUSTERS_PER_DEVICE   5
#define QNGPU_TOTAL_CORES           (QNGPU_CORES_PER_CLUSTER * QNGPU_CLUSTERS_PER_DEVICE)
#define QNGPU_SHADER_UNITS          256
#define QNGPU_RASTERIZER_UNITS      128
#define QNGPU_TEXTURE_UNITS         256
#define QNGPU_MEMORY_SIZE_GB        32
#define QNGPU_MEMORY_BUS_WIDTH      512
#define QNGPU_PCIE_LANES            16

/* QNGPU Memory Management */
#define QNGPU_VRAM_BASE             0x1000000000ULL
#define QNGPU_VRAM_SIZE             (QNGPU_MEMORY_SIZE_GB * 1024ULL * 1024ULL * 1024ULL)
#define QNGPU_CACHE_L1_SIZE         (128 * 1024)  /* 128KB per core */
#define QNGPU_CACHE_L2_SIZE         (4 * 1024 * 1024)  /* 4MB per cluster */
#define QNGPU_CACHE_L3_SIZE         (16 * 1024 * 1024) /* 16MB shared */

/* QNGPU Device States */
typedef enum {
    QNGPU_STATE_UNINITIALIZED = 0,
    QNGPU_STATE_INITIALIZING,
    QNGPU_STATE_READY,
    QNGPU_STATE_ACTIVE,
    QNGPU_STATE_SUSPENDED,
    QNGPU_STATE_ERROR,
    QNGPU_STATE_SHUTDOWN
} qngpu_device_state_t;

/* QNGPU Power States */
typedef enum {
    QNGPU_POWER_OFF = 0,
    QNGPU_POWER_IDLE,
    QNGPU_POWER_LOW,
    QNGPU_POWER_MEDIUM,
    QNGPU_POWER_HIGH,
    QNGPU_POWER_BOOST
} qngpu_power_state_t;

/* QNGPU Core Structure */
typedef struct {
    uint32_t core_id;
    uint32_t cluster_id;
    uint32_t frequency_mhz;
    qngpu_power_state_t power_state;
    quantum_number_t registers[256];  /* 256 x 256-bit registers */
    uint64_t instruction_cache[QNGPU_CACHE_L1_SIZE / 8];
    uint64_t data_cache[QNGPU_CACHE_L1_SIZE / 8];
    bool active;
    uint64_t cycles_executed;
    uint64_t instructions_executed;
} qngpu_core_t;

/* QNGPU Cluster Structure */
typedef struct {
    uint32_t cluster_id;
    qngpu_core_t cores[QNGPU_CORES_PER_CLUSTER];
    uint64_t l2_cache[QNGPU_CACHE_L2_SIZE / 8];
    uint32_t active_cores;
    uint64_t total_cycles;
    uint64_t total_instructions;
    quantum_number_t shared_memory[1024];  /* Shared cluster memory */
} qngpu_cluster_t;

/* QNGPU Shader Unit */
typedef struct {
    uint32_t shader_id;
    uint32_t shader_type;  /* Vertex, Fragment, Geometry, Compute, Tessellation */
    quantum_number_t shader_registers[1024];
    uint64_t program_counter;
    bool active;
    uint32_t thread_count;
    uint64_t cycles_executed;
} qngpu_shader_unit_t;

/* QNGPU Rasterizer Unit */
typedef struct {
    uint32_t rasterizer_id;
    quantum_number_t triangle_vertices[9];  /* 3 vertices x 3 coordinates */
    quantum_number_t edge_functions[3];
    uint32_t pixel_coverage[64][64];  /* Coverage mask for 64x64 tile */
    bool active;
    uint64_t triangles_processed;
    uint64_t pixels_generated;
} qngpu_rasterizer_unit_t;

/* QNGPU Texture Unit */
typedef struct {
    uint32_t texture_id;
    uint64_t texture_base_address;
    uint32_t texture_width;
    uint32_t texture_height;
    uint32_t texture_format;
    quantum_number_t texture_coordinates[2];
    quantum_number_t sampled_color[4];  /* RGBA */
    bool active;
    uint64_t samples_processed;
} qngpu_texture_unit_t;

/* QNGPU Collapse Module for NVIDIA Integration */
typedef struct {
    bool enabled;
    uint32_t nvidia_device_id;
    uint64_t nvidia_memory_base;
    uint64_t nvidia_memory_size;
    uint32_t translation_buffer_size;
    uint64_t cuda_kernels_translated;
    uint64_t memory_transfers;
    bool fallback_active;
} qngpu_collapse_module_t;

/* QNGPU Device Structure */
typedef struct {
    uint32_t device_id;
    uint32_t pci_bus;
    uint32_t pci_device;
    uint32_t pci_function;
    qngpu_device_state_t state;
    qngpu_power_state_t power_state;
    
    /* Hardware Components */
    qngpu_cluster_t clusters[QNGPU_CLUSTERS_PER_DEVICE];
    qngpu_shader_unit_t shader_units[QNGPU_SHADER_UNITS];
    qngpu_rasterizer_unit_t rasterizer_units[QNGPU_RASTERIZER_UNITS];
    qngpu_texture_unit_t texture_units[QNGPU_TEXTURE_UNITS];
    qngpu_collapse_module_t collapse_module;
    
    /* Memory Management */
    uint64_t vram_base_address;
    uint64_t vram_size;
    uint64_t vram_used;
    uint64_t l3_cache[QNGPU_CACHE_L3_SIZE / 8];
    
    /* Performance Counters */
    uint64_t frames_rendered;
    uint64_t triangles_processed;
    uint64_t pixels_rendered;
    uint64_t shader_invocations;
    uint64_t texture_samples;
    
    /* Power and Thermal */
    uint32_t temperature_celsius;
    uint32_t power_consumption_watts;
    uint32_t fan_speed_rpm;
    
    /* Driver State */
    void *driver_private;
    bool initialized;
    uint64_t uptime_seconds;
} qngpu_device_t;

/* QNGPU Graphics Context */
typedef struct {
    uint32_t context_id;
    uint32_t process_id;
    qngpu_device_t *device;
    
    /* Rendering State */
    quantum_number_t viewport[4];  /* x, y, width, height */
    quantum_number_t projection_matrix[16];
    quantum_number_t modelview_matrix[16];
    quantum_number_t clear_color[4];  /* RGBA */
    quantum_number_t clear_depth;
    
    /* Shader Programs */
    uint32_t vertex_shader_id;
    uint32_t fragment_shader_id;
    uint32_t geometry_shader_id;
    uint32_t compute_shader_id;
    
    /* Texture Bindings */
    uint32_t bound_textures[32];
    
    /* Buffer Objects */
    uint32_t vertex_buffer_id;
    uint32_t index_buffer_id;
    uint32_t uniform_buffer_id;
    
    /* Render Targets */
    uint32_t framebuffer_id;
    uint32_t color_attachment_id;
    uint32_t depth_attachment_id;
    
    bool active;
    uint64_t draw_calls;
    uint64_t state_changes;
} qngpu_graphics_context_t;

/* QNGPU Command Buffer */
typedef struct {
    uint32_t command_type;
    uint32_t command_size;
    uint64_t command_data[];
} qngpu_command_t;

typedef struct {
    uint32_t buffer_id;
    uint32_t size;
    uint32_t used;
    uint32_t read_offset;
    qngpu_command_t *commands;
    bool submitted;
    bool completed;
} qngpu_command_buffer_t;

/* Function Prototypes */

/* Device Management */
int qngpu_init_driver(void);
void qngpu_cleanup_driver(void);
int qngpu_probe_device(uint32_t pci_bus, uint32_t pci_device, uint32_t pci_function);
int qngpu_remove_device(uint32_t device_id);
qngpu_device_t* qngpu_get_device(uint32_t device_id);
int qngpu_reset_device(uint32_t device_id);

/* Power Management */
int qngpu_set_power_state(uint32_t device_id, qngpu_power_state_t state);
qngpu_power_state_t qngpu_get_power_state(uint32_t device_id);
int qngpu_suspend_device(uint32_t device_id);
int qngpu_resume_device(uint32_t device_id);

/* Memory Management */
uint64_t qngpu_alloc_vram(uint32_t device_id, uint64_t size, uint32_t alignment);
int qngpu_free_vram(uint32_t device_id, uint64_t address);
int qngpu_map_memory(uint32_t device_id, uint64_t vram_addr, void *cpu_addr, uint64_t size);
int qngpu_unmap_memory(uint32_t device_id, uint64_t vram_addr, uint64_t size);
int qngpu_copy_to_vram(uint32_t device_id, uint64_t vram_addr, const void *data, uint64_t size);
int qngpu_copy_from_vram(uint32_t device_id, void *data, uint64_t vram_addr, uint64_t size);

/* Graphics Context Management */
uint32_t qngpu_create_context(uint32_t device_id, uint32_t process_id);
int qngpu_destroy_context(uint32_t context_id);
int qngpu_bind_context(uint32_t context_id);
int qngpu_unbind_context(uint32_t context_id);
qngpu_graphics_context_t* qngpu_get_context(uint32_t context_id);

/* Shader Management */
uint32_t qngpu_create_shader(uint32_t context_id, uint32_t shader_type, const char *source);
int qngpu_compile_shader(uint32_t shader_id);
int qngpu_bind_shader(uint32_t context_id, uint32_t shader_id);
int qngpu_delete_shader(uint32_t shader_id);

/* Buffer Management */
uint32_t qngpu_create_buffer(uint32_t context_id, uint32_t buffer_type, uint64_t size);
int qngpu_bind_buffer(uint32_t context_id, uint32_t buffer_id);
int qngpu_upload_buffer_data(uint32_t buffer_id, const void *data, uint64_t size, uint64_t offset);
int qngpu_delete_buffer(uint32_t buffer_id);

/* Texture Management */
uint32_t qngpu_create_texture(uint32_t context_id, uint32_t width, uint32_t height, uint32_t format);
int qngpu_bind_texture(uint32_t context_id, uint32_t texture_id, uint32_t unit);
int qngpu_upload_texture_data(uint32_t texture_id, const void *data, uint64_t size);
int qngpu_delete_texture(uint32_t texture_id);

/* Rendering Commands */
int qngpu_clear(uint32_t context_id, uint32_t clear_flags);
int qngpu_draw_arrays(uint32_t context_id, uint32_t primitive_type, uint32_t first, uint32_t count);
