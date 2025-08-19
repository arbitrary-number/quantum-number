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
 * Quantum OS - Virtualization Manager
 * 
 * Revolutionary virtualization system for mathematical computation environments,
 * quantum containers, and symbolic computation isolation. This system enables
 * secure, isolated execution of mathematical workloads with quantum coherence
 * preservation across virtual boundaries.
 * 
 * Features quantum-native virtualization, mathematical namespace isolation,
 * and distributed quantum state management across virtual machines.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_VIRTUALIZATION_MANAGER_H
#define QUANTUM_VIRTUALIZATION_MANAGER_H

#include "quantum_number.h"
#include "quantix_qfs.h"
#include "quantum_device_manager.h"
#include "quantum_network_manager.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Virtualization types
typedef enum {
    QVM_TYPE_FULL_VIRTUALIZATION = 1,   // Full hardware virtualization
    QVM_TYPE_PARAVIRTUALIZATION = 2,    // Paravirtualization with quantum awareness
    QVM_TYPE_CONTAINER = 3,             // Quantum container (lightweight)
    QVM_TYPE_MATHEMATICAL_SANDBOX = 4,  // Mathematical computation sandbox
    QVM_TYPE_SYMBOLIC_ENVIRONMENT = 5,  // Symbolic computation environment
    QVM_TYPE_QUANTUM_SIMULATOR = 6,     // Quantum system simulator
    QVM_TYPE_DISTRIBUTED_NODE = 7,      // Distributed computation node
    QVM_TYPE_SECURE_ENCLAVE = 8         // Secure quantum enclave
} qvm_virtualization_type_t;

// Virtual machine states
typedef enum {
    QVM_STATE_STOPPED = 0,
    QVM_STATE_STARTING = 1,
    QVM_STATE_RUNNING = 2,
    QVM_STATE_PAUSED = 3,
    QVM_STATE_SUSPENDED = 4,
    QVM_STATE_STOPPING = 5,
    QVM_STATE_CRASHED = 6,
    QVM_STATE_MIGRATING = 7,
    QVM_STATE_CHECKPOINTING = 8,
    QVM_STATE_RESTORING = 9
} qvm_state_t;

// Virtual machine capabilities
#define QVM_CAP_QUANTUM_COMPUTATION     0x0001
#define QVM_CAP_SYMBOLIC_COMPUTATION    0x0002
#define QVM_CAP_MATHEMATICAL_FUNCTIONS  0x0004
#define QVM_CAP_DEVICE_PASSTHROUGH      0x0008
#define QVM_CAP_NETWORK_ISOLATION       0x0010
#define QVM_CAP_MEMORY_PROTECTION       0x0020
#define QVM_CAP_LIVE_MIGRATION          0x0040
#define QVM_CAP_CHECKPOINT_RESTORE      0x0080
#define QVM_CAP_QUANTUM_COHERENCE       0x0100
#define QVM_CAP_DISTRIBUTED_COMPUTE     0x0200

// Virtual machine configuration
typedef struct {
    uint32_t vm_id;                     // Unique VM identifier
    qvm_virtualization_type_t type;     // Virtualization type
    char name[64];                      // VM name
    char description[256];              // VM description
    
    // Resource allocation
    uint64_t memory_size_mb;            // Allocated memory in MB
    uint32_t cpu_cores;                 // Number of virtual CPU cores
    uint64_t storage_size_gb;           // Virtual storage size in GB
    uint32_t network_bandwidth_mbps;    // Network bandwidth limit
    
    // Quantum-specific configuration
    uint32_t quantum_qubits;            // Number of virtual qubits
    double quantum_coherence_time_ms;   // Quantum coherence time
    uint32_t quantum_gates_per_second;  // Quantum gate execution rate
    bool enable_quantum_error_correction; // Enable quantum error correction
    
    // Mathematical configuration
    uint32_t mathematical_precision;    // Mathematical precision level
    bool enable_symbolic_computation;   // Enable symbolic math
    bool enable_arbitrary_precision;    // Enable arbitrary precision arithmetic
    uint32_t max_expression_complexity; // Maximum expression complexity
    
    // Security and isolation
    bool enable_secure_boot;            // Enable secure boot
    bool enable_memory_encryption;      // Enable memory encryption
    bool enable_network_isolation;      // Enable network isolation
    uint32_t security_level;            // Security level (0-10)
    
    // Device assignment
    uint32_t *assigned_devices;         // Assigned device IDs
    size_t assigned_device_count;       // Number of assigned devices
    
    // Network configuration
    qnm_quantum_address_t network_address; // VM network address
    uint32_t network_vlan_id;           // Virtual LAN ID
    bool enable_quantum_networking;     // Enable quantum networking
    
    // Storage configuration
    char root_filesystem_path[512];     // Root filesystem path
    char *mount_points;                 // Additional mount points
    bool enable_qfs_integration;        // Enable QFS integration
    
    // Capabilities
    uint32_t capabilities;              // VM capabilities bitmask
} qvm_config_t;

// Virtual machine runtime information
typedef struct {
    uint32_t vm_id;                     // VM identifier
    qvm_state_t state;                  // Current state
    qvm_config_t config;                // VM configuration
    
    // Runtime statistics
    uint64_t uptime_seconds;            // VM uptime
    uint64_t cpu_time_ms;               // Total CPU time used
    uint64_t memory_used_mb;            // Current memory usage
    uint64_t storage_used_gb;           // Current storage usage
    uint64_t network_bytes_tx;          // Network bytes transmitted
    uint64_t network_bytes_rx;          // Network bytes received
    
    // Quantum statistics
    uint64_t quantum_operations;        // Quantum operations performed
    uint64_t quantum_gates_executed;    // Quantum gates executed
    double current_quantum_coherence;   // Current quantum coherence
    uint64_t quantum_errors;            // Quantum errors encountered
    
    // Mathematical statistics
    uint64_t mathematical_operations;   // Mathematical operations performed
    uint64_t symbolic_operations;       // Symbolic operations performed
    uint64_t expressions_evaluated;     // Expressions evaluated
    double average_computation_time_ms; // Average computation time
    
    // Performance metrics
    double cpu_utilization_percent;     // CPU utilization
    double memory_utilization_percent;  // Memory utilization
    double storage_utilization_percent; // Storage utilization
    double network_utilization_percent; // Network utilization
    
    // Host information
    char host_node[256];                // Host node identifier
    uint64_t creation_time;             // VM creation timestamp
    uint64_t last_checkpoint_time;      // Last checkpoint timestamp
} qvm_runtime_info_t;

// Virtual machine snapshot
typedef struct {
    uint32_t vm_id;                     // VM identifier
    uint64_t snapshot_id;               // Unique snapshot identifier
    char name[64];                      // Snapshot name
    char description[256];              // Snapshot description
    uint64_t creation_time;             // Creation timestamp
    uint64_t size_bytes;                // Snapshot size in bytes
    
    // Quantum state information
    quantum_number_t *quantum_state;    // Quantum state snapshot
    size_t quantum_state_size;          // Quantum state size
    double quantum_coherence_at_snapshot; // Quantum coherence at snapshot time
    
    // Mathematical state information
    char *mathematical_context;         // Mathematical computation context
    size_t mathematical_context_size;   // Mathematical context size
    
    // File system state
    char snapshot_path[512];            // Snapshot file path
    bool is_incremental;                // Is incremental snapshot
    uint64_t parent_snapshot_id;        // Parent snapshot ID (if incremental)
} qvm_snapshot_t;

// Container configuration (lightweight virtualization)
typedef struct {
    uint32_t container_id;              // Unique container identifier
    char name[64];                      // Container name
    char image_name[128];               // Container image name
    
    // Resource limits
    uint64_t memory_limit_mb;           // Memory limit in MB
    uint32_t cpu_limit_percent;         // CPU limit percentage
    uint64_t storage_limit_gb;          // Storage limit in GB
    
    // Mathematical environment
    char mathematical_library[256];     // Mathematical library path
    char symbolic_engine[256];          // Symbolic computation engine
    uint32_t precision_level;           // Mathematical precision level
    
    // Quantum environment
    uint32_t virtual_qubits;            // Number of virtual qubits
    char quantum_simulator[256];        // Quantum simulator path
    bool enable_quantum_networking;     // Enable quantum networking
    
    // Namespace isolation
    bool isolate_network;               // Isolate network namespace
    bool isolate_filesystem;            // Isolate filesystem namespace
    bool isolate_process_tree;          // Isolate process tree
    bool isolate_mathematical_context;  // Isolate mathematical context
    
    // Security
    uint32_t user_id;                   // Container user ID
    uint32_t group_id;                  // Container group ID
    char security_profile[128];         // Security profile name
    bool read_only_filesystem;          // Read-only filesystem
} qvm_container_config_t;

// Virtualization manager result codes
typedef enum {
    QVM_SUCCESS = 0,
    QVM_ERROR_VM_NOT_FOUND = -1,
    QVM_ERROR_VM_ALREADY_EXISTS = -2,
    QVM_ERROR_INVALID_STATE = -3,
    QVM_ERROR_INSUFFICIENT_RESOURCES = -4,
    QVM_ERROR_CONFIGURATION_ERROR = -5,
    QVM_ERROR_QUANTUM_DECOHERENCE = -6,
    QVM_ERROR_MIGRATION_FAILED = -7,
    QVM_ERROR_CHECKPOINT_FAILED = -8,
    QVM_ERROR_RESTORE_FAILED = -9,
    QVM_ERROR_DEVICE_ASSIGNMENT_FAILED = -10,
    QVM_ERROR_NETWORK_CONFIGURATION_FAILED = -11,
    QVM_ERROR_SECURITY_VIOLATION = -12,
    QVM_ERROR_INVALID_PARAMETER = -13,
    QVM_ERROR_OUT_OF_MEMORY = -14,
    QVM_ERROR_NOT_SUPPORTED = -15,
    QVM_ERROR_PERMISSION_DENIED = -16
} qvm_result_t;

// Virtualization manager initialization and cleanup
qvm_result_t qvm_initialize(void);
qvm_result_t qvm_cleanup(void);

// Virtual machine lifecycle management
qvm_result_t qvm_create_vm(const qvm_config_t *config, uint32_t *vm_id);
qvm_result_t qvm_destroy_vm(uint32_t vm_id);
qvm_result_t qvm_start_vm(uint32_t vm_id);
qvm_result_t qvm_stop_vm(uint32_t vm_id, bool force);
qvm_result_t qvm_pause_vm(uint32_t vm_id);
qvm_result_t qvm_resume_vm(uint32_t vm_id);
qvm_result_t qvm_restart_vm(uint32_t vm_id);

// Virtual machine information and control
qvm_result_t qvm_get_vm_info(uint32_t vm_id, qvm_runtime_info_t *info);
qvm_result_t qvm_set_vm_config(uint32_t vm_id, const qvm_config_t *config);
qvm_result_t qvm_get_vm_state(uint32_t vm_id, qvm_state_t *state);
qvm_result_t qvm_list_vms(uint32_t **vm_ids, size_t *count);

// Resource management
qvm_result_t qvm_allocate_memory(uint32_t vm_id, uint64_t size_mb);
qvm_result_t qvm_deallocate_memory(uint32_t vm_id, uint64_t size_mb);
qvm_result_t qvm_assign_cpu_cores(uint32_t vm_id, uint32_t *core_ids, size_t core_count);
qvm_result_t qvm_assign_device(uint32_t vm_id, uint32_t device_id);
qvm_result_t qvm_unassign_device(uint32_t vm_id, uint32_t device_id);

// Quantum virtualization
qvm_result_t qvm_allocate_qubits(uint32_t vm_id, uint32_t qubit_count);
qvm_result_t qvm_deallocate_qubits(uint32_t vm_id, uint32_t qubit_count);
qvm_result_t qvm_measure_quantum_coherence(uint32_t vm_id, double *coherence);
qvm_result_t qvm_apply_quantum_error_correction(uint32_t vm_id);
qvm_result_t qvm_synchronize_quantum_state(uint32_t vm_id, const quantum_number_t *state);

// Mathematical environment management
qvm_result_t qvm_set_mathematical_precision(uint32_t vm_id, uint32_t precision);
qvm_result_t qvm_enable_symbolic_computation(uint32_t vm_id, bool enable);
qvm_result_t qvm_load_mathematical_library(uint32_t vm_id, const char *library_path);
qvm_result_t qvm_execute_mathematical_expression(uint32_t vm_id, const char *expression, quantum_number_t *result);

// Snapshot and checkpoint management
qvm_result_t qvm_create_snapshot(uint32_t vm_id, const char *name, uint64_t *snapshot_id);
qvm_result_t qvm_delete_snapshot(uint32_t vm_id, uint64_t snapshot_id);
qvm_result_t qvm_restore_snapshot(uint32_t vm_id, uint64_t snapshot_id);
qvm_result_t qvm_list_snapshots(uint32_t vm_id, qvm_snapshot_t **snapshots, size_t *count);
qvm_result_t qvm_create_checkpoint(uint32_t vm_id, const char *checkpoint_path);
qvm_result_t qvm_restore_checkpoint(uint32_t vm_id, const char *checkpoint_path);

// Live migration
qvm_result_t qvm_migrate_vm(uint32_t vm_id, const char *target_host, bool live_migration);
qvm_result_t qvm_prepare_migration(uint32_t vm_id, const char *target_host);
qvm_result_t qvm_complete_migration(uint32_t vm_id);
qvm_result_t qvm_cancel_migration(uint32_t vm_id);

// Container management (lightweight virtualization)
qvm_result_t qvm_create_container(const qvm_container_config_t *config, uint32_t *container_id);
qvm_result_t qvm_destroy_container(uint32_t container_id);
qvm_result_t qvm_start_container(uint32_t container_id);
qvm_result_t qvm_stop_container(uint32_t container_id);
qvm_result_t qvm_execute_in_container(uint32_t container_id, const char *command, char **output);

// Network virtualization
qvm_result_t qvm_create_virtual_network(const char *network_name, uint32_t *network_id);
qvm_result_t qvm_destroy_virtual_network(uint32_t network_id);
qvm_result_t qvm_attach_vm_to_network(uint32_t vm_id, uint32_t network_id);
qvm_result_t qvm_detach_vm_from_network(uint32_t vm_id, uint32_t network_id);
qvm_result_t qvm_configure_quantum_networking(uint32_t vm_id, const qnm_quantum_address_t *address);

// Storage virtualization
qvm_result_t qvm_create_virtual_disk(const char *disk_name, uint64_t size_gb, uint32_t *disk_id);
qvm_result_t qvm_destroy_virtual_disk(uint32_t disk_id);
qvm_result_t qvm_attach_disk_to_vm(uint32_t vm_id, uint32_t disk_id);
qvm_result_t qvm_detach_disk_from_vm(uint32_t vm_id, uint32_t disk_id);
qvm_result_t qvm_resize_virtual_disk(uint32_t disk_id, uint64_t new_size_gb);

// Security and isolation
qvm_result_t qvm_set_security_policy(uint32_t vm_id, const char *policy_name);
qvm_result_t qvm_enable_memory_encryption(uint32_t vm_id, bool enable);
qvm_result_t qvm_configure_secure_boot(uint32_t vm_id, const char *boot_key_path);
qvm_result_t qvm_isolate_vm_network(uint32_t vm_id, bool isolate);
qvm_result_t qvm_audit_vm_security(uint32_t vm_id, char **audit_report);

// Performance monitoring and optimization
qvm_result_t qvm_get_performance_metrics(uint32_t vm_id, qvm_runtime_info_t *metrics);
qvm_result_t qvm_optimize_vm_performance(uint32_t vm_id);
qvm_result_t qvm_balance_vm_load(void);
qvm_result_t qvm_predict_resource_usage(uint32_t vm_id, uint64_t *predicted_memory_mb, uint32_t *predicted_cpu_percent);

// Distributed virtualization
qvm_result_t qvm_create_distributed_vm(const qvm_config_t *config, const char **host_nodes, size_t host_count, uint32_t *vm_id);
qvm_result_t qvm_scale_distributed_vm(uint32_t vm_id, uint32_t new_node_count);
qvm_result_t qvm_rebalance_distributed_vm(uint32_t vm_id);
qvm_result_t qvm_get_distributed_vm_topology(uint32_t vm_id, char **topology_info);

// Quantum coherence management across VMs
qvm_result_t qvm_establish_quantum_entanglement_between_vms(uint32_t vm1_id, uint32_t vm2_id);
qvm_result_t qvm_break_quantum_entanglement_between_vms(uint32_t vm1_id, uint32_t vm2_id);
qvm_result_t qvm_synchronize_quantum_states_across_vms(uint32_t *vm_ids, size_t vm_count);
qvm_result_t qvm_measure_inter_vm_quantum_coherence(uint32_t vm1_id, uint32_t vm2_id, double *coherence);

// Mathematical computation orchestration
qvm_result_t qvm_distribute_mathematical_computation(const char *expression, uint32_t *vm_ids, size_t vm_count, quantum_number_t *result);
qvm_result_t qvm_parallel_symbolic_computation(const char *expression, uint32_t *vm_ids, size_t vm_count, char **result);
qvm_result_t qvm_coordinate_distributed_solving(const char *equation, uint32_t *vm_ids, size_t vm_count, quantum_number_t **solutions, size_t *solution_count);

// Template and image management
qvm_result_t qvm_create_vm_template(uint32_t vm_id, const char *template_name);
qvm_result_t qvm_delete_vm_template(const char *template_name);
qvm_result_t qvm_create_vm_from_template(const char *template_name, const char *vm_name, uint32_t *vm_id);
qvm_result_t qvm_list_vm_templates(char ***template_names, size_t *count);

// Event handling and monitoring
typedef void (*qvm_state_change_callback_t)(uint32_t vm_id, qvm_state_t old_state, qvm_state_t new_state);
typedef void (*qvm_resource_alert_callback_t)(uint32_t vm_id, const char *resource_type, double utilization_percent);
typedef void (*qvm_quantum_decoherence_callback_t)(uint32_t vm_id, double coherence_level);

qvm_result_t qvm_register_state_change_callback(qvm_state_change_callback_t callback);
qvm_result_t qvm_register_resource_alert_callback(qvm_resource_alert_callback_t callback);
qvm_result_t qvm_register_quantum_decoherence_callback(qvm_quantum_decoherence_callback_t callback);

// Debugging and diagnostics
qvm_result_t qvm_dump_vm_state(uint32_t vm_id, const char *dump_path);
qvm_result_t qvm_trace_vm_execution(uint32_t vm_id, bool enable_tracing);
qvm_result_t qvm_analyze_vm_performance(uint32_t vm_id, char **analysis_report);
qvm_result_t qvm_test_vm_quantum_coherence(uint32_t vm_id, double *coherence_test_result);

// Configuration and utilities
qvm_result_t qvm_create_default_config(qvm_virtualization_type_t type, qvm_config_t *config);
qvm_result_t qvm_validate_config(const qvm_config_t *config);
qvm_result_t qvm_estimate_resource_requirements(const qvm_config_t *config, uint64_t *memory_mb, uint32_t *cpu_cores);

// Error handling
const char* qvm_result_to_string(qvm_result_t result);
qvm_result_t qvm_get_last_error(void);
qvm_result_t qvm_clear_errors(void);

#ifdef __cplusplus
}
#endif

#endif // QUANTUM_VIRTUALIZATION_MANAGER_H
