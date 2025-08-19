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
 * Quantum OS - Network Manager
 * 
 * Revolutionary distributed quantum computing network system that enables
 * mathematical computation across quantum nodes, with native support for
 * quantum entanglement protocols and mathematical mesh networking.
 * 
 * This system provides the foundation for distributed quantum computation,
 * allowing Quantum Numbers to be processed across multiple quantum nodes
 * while maintaining mathematical coherence and cryptographic security.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_NETWORK_MANAGER_H
#define QUANTUM_NETWORK_MANAGER_H

#include "quantum_number.h"
#include "quantix_ecc.h"
#include "quantix_ultrascale_map_persistent.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Network protocol types
typedef enum {
    QNM_PROTOCOL_QUANTUM_TCP = 1,       // Quantum-enhanced TCP
    QNM_PROTOCOL_QUANTUM_UDP = 2,       // Quantum-enhanced UDP
    QNM_PROTOCOL_ENTANGLEMENT = 3,      // Quantum entanglement protocol
    QNM_PROTOCOL_MATHEMATICAL_MESH = 4, // Mathematical mesh networking
    QNM_PROTOCOL_SYMBOLIC_SYNC = 5,     // Symbolic computation synchronization
    QNM_PROTOCOL_DISTRIBUTED_COMPUTE = 6, // Distributed computation protocol
    QNM_PROTOCOL_QUANTUM_MULTICAST = 7, // Quantum multicast protocol
    QNM_PROTOCOL_SECURE_QUANTUM = 8     // Secure quantum communication
} qnm_protocol_type_t;

// Network node types
typedef enum {
    QNM_NODE_QUANTUM_PROCESSOR = 1,     // Quantum processing node
    QNM_NODE_MATHEMATICAL_COMPUTE = 2,  // Mathematical computation node
    QNM_NODE_SYMBOLIC_ENGINE = 3,       // Symbolic computation engine
    QNM_NODE_STORAGE_CLUSTER = 4,       // Distributed storage cluster
    QNM_NODE_COORDINATION_HUB = 5,      // Network coordination hub
    QNM_NODE_GATEWAY = 6,               // Network gateway
    QNM_NODE_RELAY = 7,                 // Network relay node
    QNM_NODE_EDGE_DEVICE = 8            // Edge computing device
} qnm_node_type_t;

// Network states
typedef enum {
    QNM_STATE_DISCONNECTED = 0,
    QNM_STATE_CONNECTING = 1,
    QNM_STATE_CONNECTED = 2,
    QNM_STATE_AUTHENTICATING = 3,
    QNM_STATE_AUTHENTICATED = 4,
    QNM_STATE_SYNCHRONIZED = 5,
    QNM_STATE_COMPUTING = 6,
    QNM_STATE_ERROR = 7,
    QNM_STATE_SUSPENDED = 8
} qnm_network_state_t;

// Quantum network address structure
typedef struct {
    uint8_t quantum_id[32];             // 256-bit quantum identifier
    uint8_t mathematical_domain[16];    // Mathematical domain identifier
    uint32_t node_type;                 // Node type identifier
    uint32_t capabilities;              // Node capabilities bitmask
    uint16_t port;                      // Network port
    uint8_t ipv6_addr[16];             // IPv6 address
    quantum_number_t entanglement_key;  // Quantum entanglement key
} qnm_quantum_address_t;

// Network capabilities
#define QNM_CAP_QUANTUM_COMPUTATION     0x0001
#define QNM_CAP_SYMBOLIC_COMPUTATION    0x0002
#define QNM_CAP_MATHEMATICAL_FUNCTIONS  0x0004
#define QNM_CAP_DISTRIBUTED_STORAGE     0x0008
#define QNM_CAP_ENTANGLEMENT_SUPPORT    0x0010
#define QNM_CAP_MESH_NETWORKING         0x0020
#define QNM_CAP_LOAD_BALANCING          0x0040
#define QNM_CAP_FAULT_TOLERANCE         0x0080
#define QNM_CAP_ENCRYPTION              0x0100
#define QNM_CAP_COMPRESSION             0x0200

// Network node information
typedef struct {
    qnm_quantum_address_t address;      // Node quantum address
    qnm_node_type_t type;               // Node type
    qnm_network_state_t state;          // Current state
    uint32_t capabilities;              // Node capabilities
    char hostname[256];                 // Node hostname
    char description[512];              // Node description
    
    // Performance metrics
    uint64_t total_operations;          // Total operations performed
    uint64_t quantum_operations;        // Quantum operations performed
    uint64_t mathematical_operations;   // Mathematical operations performed
    uint64_t bytes_transmitted;         // Total bytes transmitted
    uint64_t bytes_received;            // Total bytes received
    double cpu_utilization;             // CPU utilization percentage
    double memory_utilization;          // Memory utilization percentage
    double network_latency_ms;          // Network latency in milliseconds
    double bandwidth_mbps;              // Available bandwidth in Mbps
    
    // Quantum-specific metrics
    double quantum_coherence;           // Quantum coherence level (0.0-1.0)
    double entanglement_fidelity;       // Entanglement fidelity (0.0-1.0)
    uint32_t entangled_nodes;           // Number of entangled nodes
    uint64_t quantum_errors;            // Quantum error count
    
    // Timing information
    uint64_t last_seen;                 // Last communication timestamp
    uint64_t uptime;                    // Node uptime in seconds
    uint64_t connection_time;           // Connection establishment time
} qnm_node_info_t;

// Quantum computation job
typedef struct {
    uint64_t job_id;                    // Unique job identifier
    qnm_protocol_type_t protocol;       // Communication protocol
    quantum_number_t *input_data;       // Input quantum numbers
    size_t input_count;                 // Number of input elements
    quantum_number_t *output_data;      // Output quantum numbers
    size_t output_count;                // Number of output elements
    char *computation_expression;       // Mathematical expression
    mathematical_metadata_t metadata;   // Computation metadata
    
    // Job control
    uint32_t priority;                  // Job priority (0-255)
    uint32_t timeout_ms;                // Job timeout in milliseconds
    uint32_t retry_count;               // Number of retries allowed
    bool requires_entanglement;         // Requires quantum entanglement
    bool requires_synchronization;      // Requires node synchronization
    
    // Target nodes
    qnm_quantum_address_t *target_nodes; // Target node addresses
    size_t target_node_count;           // Number of target nodes
    
    // Status tracking
    qnm_network_state_t status;         // Job status
    double progress_percent;            // Job progress percentage
    uint64_t start_time;                // Job start timestamp
    uint64_t completion_time;           // Job completion timestamp
    char error_message[512];            // Error message if failed
} qnm_computation_job_t;

// Network statistics
typedef struct {
    uint64_t total_nodes;               // Total nodes in network
    uint64_t active_nodes;              // Currently active nodes
    uint64_t quantum_nodes;             // Quantum processing nodes
    uint64_t mathematical_nodes;        // Mathematical computation nodes
    uint64_t storage_nodes;             // Storage cluster nodes
    
    uint64_t total_jobs_submitted;      // Total jobs submitted
    uint64_t jobs_completed;            // Jobs completed successfully
    uint64_t jobs_failed;               // Jobs that failed
    uint64_t jobs_in_progress;          // Jobs currently in progress
    
    uint64_t total_bytes_transmitted;   // Total network bytes transmitted
    uint64_t total_bytes_received;      // Total network bytes received
    uint64_t quantum_operations_distributed; // Distributed quantum operations
    uint64_t mathematical_operations_distributed; // Distributed math operations
    
    double average_latency_ms;          // Average network latency
    double total_bandwidth_gbps;        // Total network bandwidth
    double network_utilization_percent; // Network utilization percentage
    double quantum_coherence_average;   // Average quantum coherence
    double fault_tolerance_level;       // Network fault tolerance level
} qnm_network_stats_t;

// Network manager result codes
typedef enum {
    QNM_SUCCESS = 0,
    QNM_ERROR_NODE_NOT_FOUND = -1,
    QNM_ERROR_CONNECTION_FAILED = -2,
    QNM_ERROR_AUTHENTICATION_FAILED = -3,
    QNM_ERROR_PROTOCOL_ERROR = -4,
    QNM_ERROR_TIMEOUT = -5,
    QNM_ERROR_QUANTUM_DECOHERENCE = -6,
    QNM_ERROR_ENTANGLEMENT_FAILED = -7,
    QNM_ERROR_COMPUTATION_FAILED = -8,
    QNM_ERROR_NETWORK_CONGESTION = -9,
    QNM_ERROR_INSUFFICIENT_RESOURCES = -10,
    QNM_ERROR_INVALID_PARAMETER = -11,
    QNM_ERROR_OUT_OF_MEMORY = -12,
    QNM_ERROR_NOT_SUPPORTED = -13,
    QNM_ERROR_PERMISSION_DENIED = -14,
    QNM_ERROR_NETWORK_PARTITION = -15
} qnm_result_t;

// Network manager initialization and cleanup
qnm_result_t qnm_initialize(const qnm_quantum_address_t *local_address);
qnm_result_t qnm_cleanup(void);

// Node discovery and management
qnm_result_t qnm_discover_nodes(qnm_node_type_t type, qnm_node_info_t **nodes, size_t *count);
qnm_result_t qnm_register_node(const qnm_node_info_t *node_info);
qnm_result_t qnm_unregister_node(const qnm_quantum_address_t *address);
qnm_result_t qnm_get_node_info(const qnm_quantum_address_t *address, qnm_node_info_t *info);
qnm_result_t qnm_update_node_status(const qnm_quantum_address_t *address, qnm_network_state_t state);

// Connection management
qnm_result_t qnm_connect_to_node(const qnm_quantum_address_t *address, qnm_protocol_type_t protocol);
qnm_result_t qnm_disconnect_from_node(const qnm_quantum_address_t *address);
qnm_result_t qnm_authenticate_node(const qnm_quantum_address_t *address, const uint8_t *credentials, size_t cred_size);
qnm_result_t qnm_establish_secure_channel(const qnm_quantum_address_t *address, uint8_t *session_key);

// Quantum entanglement operations
qnm_result_t qnm_establish_entanglement(const qnm_quantum_address_t *node1, const qnm_quantum_address_t *node2);
qnm_result_t qnm_break_entanglement(const qnm_quantum_address_t *node1, const qnm_quantum_address_t *node2);
qnm_result_t qnm_measure_entanglement_fidelity(const qnm_quantum_address_t *node1, const qnm_quantum_address_t *node2, double *fidelity);
qnm_result_t qnm_synchronize_quantum_states(const qnm_quantum_address_t *nodes, size_t node_count);

// Distributed computation
qnm_result_t qnm_submit_computation_job(const qnm_computation_job_t *job, uint64_t *job_id);
qnm_result_t qnm_cancel_computation_job(uint64_t job_id);
qnm_result_t qnm_get_job_status(uint64_t job_id, qnm_computation_job_t *job_status);
qnm_result_t qnm_wait_for_job_completion(uint64_t job_id, uint32_t timeout_ms);

// Mathematical mesh networking
qnm_result_t qnm_create_mathematical_mesh(const qnm_quantum_address_t *nodes, size_t node_count, uint32_t *mesh_id);
qnm_result_t qnm_destroy_mathematical_mesh(uint32_t mesh_id);
qnm_result_t qnm_add_node_to_mesh(uint32_t mesh_id, const qnm_quantum_address_t *address);
qnm_result_t qnm_remove_node_from_mesh(uint32_t mesh_id, const qnm_quantum_address_t *address);
qnm_result_t qnm_broadcast_to_mesh(uint32_t mesh_id, const void *data, size_t data_size);

// Load balancing and optimization
qnm_result_t qnm_balance_computation_load(qnm_node_type_t node_type);
qnm_result_t qnm_optimize_network_topology(void);
qnm_result_t qnm_predict_optimal_nodes(const qnm_computation_job_t *job, qnm_quantum_address_t **optimal_nodes, size_t *node_count);
qnm_result_t qnm_migrate_computation(uint64_t job_id, const qnm_quantum_address_t *target_node);

// Fault tolerance and recovery
qnm_result_t qnm_detect_network_partitions(qnm_quantum_address_t **partitioned_nodes, size_t *count);
qnm_result_t qnm_recover_from_node_failure(const qnm_quantum_address_t *failed_node);
qnm_result_t qnm_replicate_critical_data(const qnm_quantum_address_t *source_node, const qnm_quantum_address_t *target_nodes, size_t target_count);
qnm_result_t qnm_perform_network_healing(void);

// Security and encryption
qnm_result_t qnm_generate_quantum_key_pair(uint8_t *public_key, uint8_t *private_key);
qnm_result_t qnm_encrypt_quantum_data(const quantum_number_t *data, size_t data_count, const uint8_t *key, void **encrypted_data, size_t *encrypted_size);
qnm_result_t qnm_decrypt_quantum_data(const void *encrypted_data, size_t encrypted_size, const uint8_t *key, quantum_number_t **data, size_t *data_count);
qnm_result_t qnm_verify_node_identity(const qnm_quantum_address_t *address, const uint8_t *signature, size_t sig_size);

// Network monitoring and statistics
qnm_result_t qnm_get_network_statistics(qnm_network_stats_t *stats);
qnm_result_t qnm_monitor_network_health(double *health_score);
qnm_result_t qnm_get_node_performance_metrics(const qnm_quantum_address_t *address, qnm_node_info_t *metrics);
qnm_result_t qnm_analyze_network_bottlenecks(char **analysis_report);

// Protocol-specific operations
qnm_result_t qnm_send_quantum_tcp(const qnm_quantum_address_t *target, const void *data, size_t data_size);
qnm_result_t qnm_send_quantum_udp(const qnm_quantum_address_t *target, const void *data, size_t data_size);
qnm_result_t qnm_multicast_quantum_data(const qnm_quantum_address_t *targets, size_t target_count, const void *data, size_t data_size);
qnm_result_t qnm_broadcast_network_announcement(const char *message);

// Advanced mathematical operations
qnm_result_t qnm_distributed_matrix_multiplication(const quantum_number_t *matrix_a, const quantum_number_t *matrix_b, size_t dimensions, quantum_number_t **result);
qnm_result_t qnm_distributed_fourier_transform(const quantum_number_t *input, size_t input_size, quantum_number_t **output);
qnm_result_t qnm_distributed_symbolic_integration(const char *expression, const char *variable, char **result);
qnm_result_t qnm_distributed_equation_solving(const char *equation, const char *variables, quantum_number_t **solutions, size_t *solution_count);

// Quantum-specific distributed operations
qnm_result_t qnm_distributed_quantum_simulation(const quantum_number_t *initial_state, const char *hamiltonian, double time, quantum_number_t **final_state);
qnm_result_t qnm_distributed_quantum_algorithm(const char *algorithm_name, const quantum_number_t *parameters, size_t param_count, quantum_number_t **result);
qnm_result_t qnm_distributed_quantum_error_correction(const quantum_number_t *noisy_state, quantum_number_t **corrected_state);

// Network configuration and tuning
qnm_result_t qnm_set_network_parameters(uint32_t max_connections, uint32_t timeout_ms, uint32_t retry_count);
qnm_result_t qnm_configure_protocol(qnm_protocol_type_t protocol, const void *config, size_t config_size);
qnm_result_t qnm_set_quality_of_service(qnm_node_type_t node_type, uint32_t priority, uint32_t bandwidth_limit);
qnm_result_t qnm_enable_compression(bool enable, uint32_t compression_level);

// Event handling and callbacks
typedef void (*qnm_node_event_callback_t)(const qnm_quantum_address_t *address, qnm_network_state_t old_state, qnm_network_state_t new_state);
typedef void (*qnm_job_completion_callback_t)(uint64_t job_id, qnm_result_t result, const qnm_computation_job_t *job);
typedef void (*qnm_network_error_callback_t)(qnm_result_t error, const char *error_message);

qnm_result_t qnm_register_node_event_callback(qnm_node_event_callback_t callback);
qnm_result_t qnm_register_job_completion_callback(qnm_job_completion_callback_t callback);
qnm_result_t qnm_register_network_error_callback(qnm_network_error_callback_t callback);

// Debugging and diagnostics
qnm_result_t qnm_dump_network_topology(const char *output_file);
qnm_result_t qnm_trace_computation_path(uint64_t job_id, char **trace_log);
qnm_result_t qnm_test_network_connectivity(const qnm_quantum_address_t *target, double *latency_ms);
qnm_result_t qnm_benchmark_network_performance(const qnm_quantum_address_t *target, double *throughput_mbps);

// Error handling
const char* qnm_result_to_string(qnm_result_t result);
qnm_result_t qnm_get_last_network_error(void);
qnm_result_t qnm_clear_network_errors(void);

#ifdef __cplusplus
}
#endif

#endif // QUANTUM_NETWORK_MANAGER_H
