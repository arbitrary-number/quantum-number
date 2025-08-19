/*
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
 *
 * Copyright (c) Arbitrary Number Project Team. All rights reserved.
 */

/**
 * Quantum OS - Device Manager
 * 
 * Revolutionary device management system for mathematical computation devices,
 * quantum processors, and symbolic computation accelerators.
 * 
 * This system provides unified device abstraction for mathematical hardware,
 * including quantum processing units, mathematical coprocessors, and
 * symbolic computation engines.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_DEVICE_MANAGER_H
#define QUANTUM_DEVICE_MANAGER_H

#include "quantum_number.h"
#include "quantix_qfs.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Device types
typedef enum {
    QDM_DEVICE_QUANTUM_PROCESSOR = 1,    // Quantum Processing Unit
    QDM_DEVICE_MATH_COPROCESSOR = 2,     // Mathematical Coprocessor
    QDM_DEVICE_SYMBOLIC_ENGINE = 3,      // Symbolic Computation Engine
    QDM_DEVICE_MEMORY_CONTROLLER = 4,    // Quantum Memory Controller
    QDM_DEVICE_STORAGE_CONTROLLER = 5,   // QFS Storage Controller
    QDM_DEVICE_NETWORK_ADAPTER = 6,      // Quantum Network Adapter
    QDM_DEVICE_DISPLAY_CONTROLLER = 7,   // Mathematical Display Controller
    QDM_DEVICE_INPUT_CONTROLLER = 8,     // Mathematical Input Controller
    QDM_DEVICE_CRYPTO_ACCELERATOR = 9,   // Cryptographic Accelerator
    QDM_DEVICE_AI_PROCESSOR = 10         // AI/ML Processing Unit
} qdm_device_type_t;

// Device states
typedef enum {
    QDM_STATE_UNINITIALIZED = 0,
    QDM_STATE_INITIALIZING = 1,
    QDM_STATE_READY = 2,
    QDM_STATE_BUSY = 3,
    QDM_STATE_ERROR = 4,
    QDM_STATE_SUSPENDED = 5,
    QDM_STATE_OFFLINE = 6
} qdm_device_state_t;

// Device capabilities
#define QDM_CAP_QUANTUM_COMPUTATION     0x0001
#define QDM_CAP_SYMBOLIC_COMPUTATION    0x0002
#define QDM_CAP_MATHEMATICAL_FUNCTIONS  0x0004
#define QDM_CAP_PARALLEL_PROCESSING     0x0008
#define QDM_CAP_MEMORY_MAPPING          0x0010
#define QDM_CAP_DMA_SUPPORT             0x0020
#define QDM_CAP_INTERRUPT_SUPPORT       0x0040
#define QDM_CAP_POWER_MANAGEMENT        0x0080
#define QDM_CAP_HOT_PLUG                0x0100
#define QDM_CAP_VIRTUALIZATION          0x0200

// Device configuration
typedef struct {
    uint32_t device_id;                 // Unique device identifier
    qdm_device_type_t type;             // Device type
    char name[64];                      // Device name
    char vendor[32];                    // Vendor name
    char model[32];                     // Model name
    uint32_t version_major;             // Major version
    uint32_t version_minor;             // Minor version
    uint32_t capabilities;              // Device capabilities bitmask
    uint64_t memory_size;               // Device memory size
    uint32_t processing_units;          // Number of processing units
    uint32_t max_frequency_mhz;         // Maximum frequency in MHz
    uint32_t power_consumption_mw;      // Power consumption in mW
    bool supports_quantum_numbers;      // Supports Quantum Number operations
    bool supports_symbolic_math;        // Supports symbolic mathematics
    bool supports_parallel_ops;         // Supports parallel operations
} qdm_device_config_t;

// Device statistics
typedef struct {
    uint64_t operations_completed;      // Total operations completed
    uint64_t quantum_operations;        // Quantum operations performed
    uint64_t mathematical_operations;   // Mathematical operations performed
    uint64_t symbolic_operations;       // Symbolic operations performed
    uint64_t bytes_processed;           // Total bytes processed
    uint64_t errors_encountered;        // Total errors encountered
    uint32_t current_frequency_mhz;     // Current operating frequency
    uint32_t current_power_mw;          // Current power consumption
    double utilization_percent;         // Device utilization percentage
    double temperature_celsius;         // Device temperature
    uint64_t uptime_seconds;            // Device uptime in seconds
} qdm_device_stats_t;

// Device handle
typedef struct qdm_device {
    uint32_t device_id;                 // Device identifier
    qdm_device_type_t type;             // Device type
    qdm_device_state_t state;           // Current state
    qdm_device_config_t config;         // Device configuration
    qdm_device_stats_t stats;           // Device statistics
    
    // Memory mapping
    void *memory_base;                  // Base memory address
    size_t memory_size;                 // Memory size
    bool memory_mapped;                 // Memory is mapped
    
    // Interrupt handling
    uint32_t interrupt_vector;          // Interrupt vector
    void (*interrupt_handler)(struct qdm_device *device, uint32_t status);
    
    // Device operations
    int (*initialize)(struct qdm_device *device);
    int (*shutdown)(struct qdm_device *device);
    int (*reset)(struct qdm_device *device);
    int (*suspend)(struct qdm_device *device);
    int (*resume)(struct qdm_device *device);
    
    // Mathematical operations
    int (*compute_quantum)(struct qdm_device *device, const quantum_number_t *input, 
                          quantum_number_t *output);
    int (*compute_symbolic)(struct qdm_device *device, const char *expression, 
                           char **result);
    int (*compute_parallel)(struct qdm_device *device, void *input_array, 
                           size_t array_size, void *output_array);
    
    // I/O operations
    int (*read_register)(struct qdm_device *device, uint32_t offset, uint32_t *value);
    int (*write_register)(struct qdm_device *device, uint32_t offset, uint32_t value);
    int (*dma_transfer)(struct qdm_device *device, void *src, void *dst, size_t size);
    
    // Private device data
    void *private_data;                 // Device-specific private data
    
    // List management
    struct qdm_device *next;            // Next device in list
} qdm_device_t;

// Device manager result codes
typedef enum {
    QDM_SUCCESS = 0,
    QDM_ERROR_DEVICE_NOT_FOUND = -1,
    QDM_ERROR_DEVICE_BUSY = -2,
    QDM_ERROR_DEVICE_ERROR = -3,
    QDM_ERROR_INVALID_PARAMETER = -4,
    QDM_ERROR_OUT_OF_MEMORY = -5,
    QDM_ERROR_NOT_SUPPORTED = -6,
    QDM_ERROR_TIMEOUT = -7,
    QDM_ERROR_HARDWARE_FAULT = -8,
    QDM_ERROR_DRIVER_ERROR = -9,
    QDM_ERROR_PERMISSION_DENIED = -10
} qdm_result_t;

// Device manager initialization and cleanup
qdm_result_t qdm_initialize(void);
qdm_result_t qdm_cleanup(void);

// Device registration and management
qdm_result_t qdm_register_device(qdm_device_t *device);
qdm_result_t qdm_unregister_device(uint32_t device_id);
qdm_device_t* qdm_get_device(uint32_t device_id);
qdm_device_t* qdm_get_device_by_type(qdm_device_type_t type);
qdm_result_t qdm_enumerate_devices(qdm_device_type_t type, uint32_t *device_ids, 
                                   size_t *count);

// Device operations
qdm_result_t qdm_initialize_device(uint32_t device_id);
qdm_result_t qdm_shutdown_device(uint32_t device_id);
qdm_result_t qdm_reset_device(uint32_t device_id);
qdm_result_t qdm_suspend_device(uint32_t device_id);
qdm_result_t qdm_resume_device(uint32_t device_id);

// Device state management
qdm_device_state_t qdm_get_device_state(uint32_t device_id);
qdm_result_t qdm_set_device_state(uint32_t device_id, qdm_device_state_t state);
qdm_result_t qdm_wait_for_device_ready(uint32_t device_id, uint32_t timeout_ms);

// Mathematical computation operations
qdm_result_t qdm_compute_quantum_number(uint32_t device_id, 
                                        const quantum_number_t *input,
                                        quantum_number_t *output);
qdm_result_t qdm_compute_symbolic_expression(uint32_t device_id, 
                                             const char *expression,
                                             char **result);
qdm_result_t qdm_compute_parallel_array(uint32_t device_id, 
                                        void *input_array, size_t array_size,
                                        void *output_array);

// Device I/O operations
qdm_result_t qdm_read_device_register(uint32_t device_id, uint32_t offset, 
                                      uint32_t *value);
qdm_result_t qdm_write_device_register(uint32_t device_id, uint32_t offset, 
                                       uint32_t value);
qdm_result_t qdm_device_dma_transfer(uint32_t device_id, void *src, void *dst, 
                                     size_t size);

// Memory management
qdm_result_t qdm_map_device_memory(uint32_t device_id, uint64_t physical_addr, 
                                   size_t size, void **virtual_addr);
qdm_result_t qdm_unmap_device_memory(uint32_t device_id, void *virtual_addr, 
                                     size_t size);
qdm_result_t qdm_allocate_device_buffer(uint32_t device_id, size_t size, 
                                        void **buffer);
qdm_result_t qdm_free_device_buffer(uint32_t device_id, void *buffer);

// Interrupt management
qdm_result_t qdm_register_interrupt_handler(uint32_t device_id, 
                                            void (*handler)(qdm_device_t *device, uint32_t status));
qdm_result_t qdm_unregister_interrupt_handler(uint32_t device_id);
qdm_result_t qdm_enable_device_interrupts(uint32_t device_id);
qdm_result_t qdm_disable_device_interrupts(uint32_t device_id);

// Power management
qdm_result_t qdm_set_device_power_state(uint32_t device_id, uint32_t power_state);
qdm_result_t qdm_get_device_power_consumption(uint32_t device_id, uint32_t *power_mw);
qdm_result_t qdm_set_device_frequency(uint32_t device_id, uint32_t frequency_mhz);

// Device monitoring and statistics
qdm_result_t qdm_get_device_statistics(uint32_t device_id, qdm_device_stats_t *stats);
qdm_result_t qdm_reset_device_statistics(uint32_t device_id);
qdm_result_t qdm_get_device_temperature(uint32_t device_id, double *temperature);
qdm_result_t qdm_get_device_utilization(uint32_t device_id, double *utilization);

// Device configuration
qdm_result_t qdm_get_device_config(uint32_t device_id, qdm_device_config_t *config);
qdm_result_t qdm_set_device_config(uint32_t device_id, const qdm_device_config_t *config);
qdm_result_t qdm_get_device_capabilities(uint32_t device_id, uint32_t *capabilities);

// Hot-plug support
qdm_result_t qdm_scan_for_devices(void);
qdm_result_t qdm_handle_device_insertion(uint32_t device_id);
qdm_result_t qdm_handle_device_removal(uint32_t device_id);

// Virtualization support
qdm_result_t qdm_create_virtual_device(qdm_device_type_t type, uint32_t *virtual_device_id);
qdm_result_t qdm_destroy_virtual_device(uint32_t virtual_device_id);
qdm_result_t qdm_assign_device_to_vm(uint32_t device_id, uint32_t vm_id);

// Device driver interface
typedef struct {
    const char *name;                   // Driver name
    qdm_device_type_t supported_type;   // Supported device type
    int (*probe)(qdm_device_t *device); // Probe function
    int (*remove)(qdm_device_t *device); // Remove function
    int (*suspend)(qdm_device_t *device); // Suspend function
    int (*resume)(qdm_device_t *device); // Resume function
} qdm_driver_t;

qdm_result_t qdm_register_driver(const qdm_driver_t *driver);
qdm_result_t qdm_unregister_driver(const qdm_driver_t *driver);

// Quantum-specific device operations
qdm_result_t qdm_quantum_entangle_devices(uint32_t device1_id, uint32_t device2_id);
qdm_result_t qdm_quantum_measure_state(uint32_t device_id, quantum_number_t *state);
qdm_result_t qdm_quantum_apply_gate(uint32_t device_id, const char *gate_name, 
                                    const quantum_number_t *parameters);

// Mathematical coprocessor operations
qdm_result_t qdm_math_compute_function(uint32_t device_id, const char *function_name,
                                       const quantum_number_t *inputs, size_t input_count,
                                       quantum_number_t *output);
qdm_result_t qdm_math_solve_equation(uint32_t device_id, const char *equation,
                                     const char *variable, quantum_number_t *solution);
qdm_result_t qdm_math_optimize_expression(uint32_t device_id, const char *expression,
                                          char **optimized_expression);

// Symbolic computation engine operations
qdm_result_t qdm_symbolic_differentiate(uint32_t device_id, const char *expression,
                                        const char *variable, char **derivative);
qdm_result_t qdm_symbolic_integrate(uint32_t device_id, const char *expression,
                                    const char *variable, char **integral);
qdm_result_t qdm_symbolic_simplify(uint32_t device_id, const char *expression,
                                   char **simplified);

// Error handling and diagnostics
const char* qdm_result_to_string(qdm_result_t result);
qdm_result_t qdm_get_last_error(uint32_t device_id);
qdm_result_t qdm_run_device_diagnostics(uint32_t device_id, bool *passed);
qdm_result_t qdm_get_device_error_log(uint32_t device_id, char **error_log);

// Performance optimization
qdm_result_t qdm_optimize_device_performance(uint32_t device_id);
qdm_result_t qdm_balance_device_load(qdm_device_type_t type);
qdm_result_t qdm_predict_device_performance(uint32_t device_id, 
                                            const void *workload_description,
                                            double *predicted_time_ms);

// Device clustering and load balancing
qdm_result_t qdm_create_device_cluster(qdm_device_type_t type, uint32_t *device_ids,
                                       size_t device_count, uint32_t *cluster_id);
qdm_result_t qdm_destroy_device_cluster(uint32_t cluster_id);
qdm_result_t qdm_submit_cluster_job(uint32_t cluster_id, void *job_data,
                                    size_t job_size, uint32_t *job_id);

#ifdef __cplusplus
}
#endif

#endif // QUANTUM_DEVICE_MANAGER_H
