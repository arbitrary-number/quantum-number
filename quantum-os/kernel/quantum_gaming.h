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
 * Quantum Gaming Performance Module
 * 
 * Provides gaming-optimized performance enhancements for Quantum OS,
 * including real-time scheduling, low-latency input handling, and
 * exact mathematical precision for superior gaming experience.
 * 
 * Copyright (C) 2025 Arbitrary Number Project Team
 * Licensed under Apache License 2.0
 */

#ifndef QUANTUM_GAMING_H
#define QUANTUM_GAMING_H

#include <stdint.h>
#include <stdbool.h>
#include "quantum_number.h"
#include "quantum_process_scheduler.h"
#include "qngpu_driver.h"

/* Gaming Performance Constants */
#define QG_MAX_GAMING_PROCESSES     64
#define QG_MAX_INPUT_DEVICES        16
#define QG_MAX_FRAME_BUFFERS        8
#define QG_TARGET_FPS_60            60
#define QG_TARGET_FPS_120           120
#define QG_TARGET_FPS_240           240
#define QG_MAX_LATENCY_US           1000    /* 1ms maximum latency */
#define QG_PRIORITY_BOOST_FACTOR    10

/* Gaming Process Types */
typedef enum {
    QG_PROCESS_UNKNOWN = 0,
    QG_PROCESS_GAME_ENGINE,
    QG_PROCESS_GAME_RENDERER,
    QG_PROCESS_GAME_AUDIO,
    QG_PROCESS_GAME_INPUT,
    QG_PROCESS_GAME_NETWORK,
    QG_PROCESS_GAME_PHYSICS,
    QG_PROCESS_ANTI_CHEAT,
    QG_PROCESS_GAME_LAUNCHER,
    QG_PROCESS_STREAMING
} qg_process_type_t;

/* Gaming Performance Modes */
typedef enum {
    QG_MODE_NORMAL = 0,
    QG_MODE_PERFORMANCE,
    QG_MODE_ULTRA_PERFORMANCE,
    QG_MODE_COMPETITIVE,
    QG_MODE_VR,
    QG_MODE_STREAMING
} qg_performance_mode_t;

/* Gaming Input Types */
typedef enum {
    QG_INPUT_KEYBOARD = 0,
    QG_INPUT_MOUSE,
    QG_INPUT_GAMEPAD,
    QG_INPUT_JOYSTICK,
    QG_INPUT_VR_CONTROLLER,
    QG_INPUT_RACING_WHEEL,
    QG_INPUT_FLIGHT_STICK,
    QG_INPUT_CUSTOM
} qg_input_type_t;

/* Gaming Frame Buffer Structure */
typedef struct {
    uint32_t buffer_id;
    uint32_t width;
    uint32_t height;
    uint32_t format;
    void *buffer_data;
    uint64_t buffer_size;
    quantum_number_t *quantum_pixels;  /* Exact pixel data */
    bool vsync_enabled;
    uint32_t refresh_rate;
    uint64_t frame_count;
    uint64_t last_frame_time;
    quantum_number_t frame_time_precision;
} qg_frame_buffer_t;

/* Gaming Input Device Structure */
typedef struct {
    uint32_t device_id;
    qg_input_type_t type;
    char device_name[64];
    bool connected;
    bool calibrated;
    uint32_t polling_rate;
    uint64_t last_input_time;
    quantum_number_t input_precision;
    
    /* Input state */
    union {
        struct {
            uint8_t keys[256];
            uint64_t key_timestamps[256];
        } keyboard;
        
        struct {
            int32_t x, y;
            int32_t delta_x, delta_y;
            uint8_t buttons;
            int32_t wheel;
            uint64_t movement_timestamp;
            quantum_number_t exact_position[2];
        } mouse;
        
        struct {
            float axes[16];
            uint32_t buttons;
            uint8_t hat_switches[4];
            uint64_t state_timestamp;
            quantum_number_t exact_axes[16];
        } gamepad;
    } state;
    
    /* Performance metrics */
    uint64_t total_inputs;
    uint64_t dropped_inputs;
    quantum_number_t average_latency;
    quantum_number_t max_latency;
} qg_input_device_t;

/* Gaming Process Structure */
typedef struct {
    uint32_t process_id;
    qg_process_type_t type;
    qg_performance_mode_t mode;
    char process_name[64];
    
    /* Performance settings */
    uint32_t target_fps;
    uint32_t cpu_affinity_mask;
    int32_t priority_boost;
    bool realtime_scheduling;
    bool memory_locked;
    bool preemption_disabled;
    
    /* Resource allocation */
    uint64_t allocated_memory;
    uint64_t locked_memory;
    uint32_t assigned_cpu_cores;
    uint32_t qngpu_context_id;
    
    /* Performance metrics */
    uint64_t frame_count;
    quantum_number_t average_frame_time;
    quantum_number_t min_frame_time;
    quantum_number_t max_frame_time;
    uint64_t dropped_frames;
    uint64_t input_events_processed;
    
    /* Anti-cheat integration */
    bool anti_cheat_enabled;
    uint64_t anti_cheat_hash;
    quantum_number_t process_integrity_hash;
    
    /* Mathematical precision context */
    quantum_number_t mathematical_precision;
    bool exact_physics_enabled;
    bool exact_collision_detection;
} qg_gaming_process_t;

/* Gaming Performance Statistics */
typedef struct {
    uint64_t total_gaming_processes;
    uint64_t active_gaming_processes;
    uint64_t total_frames_rendered;
    uint64_t total_input_events;
    quantum_number_t system_average_fps;
    quantum_number_t system_average_latency;
    uint64_t memory_usage_gaming;
    uint32_t cpu_usage_gaming;
    uint32_t gpu_usage_gaming;
    
    /* Performance counters */
    uint64_t scheduler_preemptions;
    uint64_t context_switches;
    uint64_t cache_misses;
    uint64_t memory_allocations;
    uint64_t network_packets_gaming;
    
    /* Quality metrics */
    uint64_t frame_drops;
    uint64_t input_lag_events;
    uint64_t stuttering_events;
    quantum_number_t frame_time_variance;
} qg_performance_stats_t;

/* Gaming Network Packet Structure */
typedef struct {
    uint32_t packet_id;
    uint32_t game_session_id;
    uint64_t timestamp;
    uint32_t packet_size;
    uint8_t priority;
    bool requires_exact_timing;
    quantum_number_t exact_timestamp;
    void *packet_data;
} qg_network_packet_t;

/* Function Prototypes */

/* Module initialization and cleanup */
int qg_init(void);
void qg_cleanup(void);

/* Gaming process management */
int qg_register_gaming_process(uint32_t process_id, qg_process_type_t type, const char *name);
int qg_unregister_gaming_process(uint32_t process_id);
int qg_set_performance_mode(uint32_t process_id, qg_performance_mode_t mode);
int qg_set_target_fps(uint32_t process_id, uint32_t target_fps);
int qg_enable_realtime_scheduling(uint32_t process_id, bool enable);
int qg_set_cpu_affinity(uint32_t process_id, uint32_t cpu_mask);
int qg_lock_process_memory(uint32_t process_id, bool lock);

/* Input handling */
int qg_register_input_device(qg_input_type_t type, const char *device_name);
int qg_unregister_input_device(uint32_t device_id);
int qg_set_input_polling_rate(uint32_t device_id, uint32_t polling_rate);
int qg_process_input_event(uint32_t device_id, void *event_data, size_t data_size);
int qg_get_input_state(uint32_t device_id, void *state_buffer, size_t buffer_size);
int qg_calibrate_input_device(uint32_t device_id);

/* Frame buffer management */
int qg_create_frame_buffer(uint32_t width, uint32_t height, uint32_t format);
int qg_destroy_frame_buffer(uint32_t buffer_id);
int qg_set_vsync(uint32_t buffer_id, bool enable);
int qg_set_refresh_rate(uint32_t buffer_id, uint32_t refresh_rate);
int qg_swap_buffers(uint32_t buffer_id);
int qg_get_frame_time(uint32_t buffer_id, quantum_number_t *frame_time);

/* Performance optimization */
int qg_optimize_for_gaming(void);
int qg_disable_unnecessary_services(void);
int qg_set_cpu_governor(const char *governor);
int qg_set_gpu_performance_mode(qg_performance_mode_t mode);
int qg_optimize_memory_allocation(void);
int qg_reduce_system_latency(void);

/* Anti-cheat integration */
int qg_enable_anti_cheat(uint32_t process_id, bool enable);
int qg_verify_process_integrity(uint32_t process_id);
int qg_detect_cheating_behavior(uint32_t process_id);
int qg_report_suspicious_activity(uint32_t process_id, const char *description);

/* Mathematical precision for gaming */
int qg_enable_exact_physics(uint32_t process_id, bool enable);
int qg_enable_exact_collision_detection(uint32_t process_id, bool enable);
int qg_set_mathematical_precision(uint32_t process_id, const quantum_number_t *precision);
int qg_calculate_exact_trajectory(const quantum_number_t *initial_pos, 
                                 const quantum_number_t *velocity,
                                 const quantum_number_t *acceleration,
                                 const quantum_number_t *time,
                                 quantum_number_t *final_pos);

/* Network gaming optimization */
int qg_optimize_network_for_gaming(void);
int qg_set_packet_priority(uint32_t game_session_id, uint8_t priority);
int qg_reduce_network_latency(void);
int qg_enable_packet_batching(bool enable);
int qg_process_gaming_packet(qg_network_packet_t *packet);

/* VR gaming support */
int qg_enable_vr_mode(uint32_t process_id, bool enable);
int qg_set_vr_refresh_rate(uint32_t target_fps);
int qg_optimize_vr_latency(void);
int qg_track_vr_headset_position(quantum_number_t *position, quantum_number_t *rotation);

/* Streaming optimization */
int qg_enable_streaming_mode(uint32_t process_id, bool enable);
int qg_optimize_for_streaming(uint32_t bitrate, uint32_t target_fps);
int qg_encode_frame_for_streaming(uint32_t buffer_id, void *encoded_data, size_t *data_size);

/* Performance monitoring */
int qg_get_performance_stats(qg_performance_stats_t *stats);
int qg_get_process_performance(uint32_t process_id, qg_gaming_process_t *process_info);
int qg_monitor_frame_times(uint32_t process_id, quantum_number_t *frame_times, size_t count);
int qg_detect_performance_issues(uint32_t process_id);
int qg_generate_performance_report(char *report_buffer, size_t buffer_size);

/* Gaming-specific system calls */
int qg_syscall_set_gaming_mode(uint32_t process_id, qg_performance_mode_t mode);
int qg_syscall_get_exact_time(quantum_number_t *exact_time);
int qg_syscall_wait_for_vblank(uint32_t display_id);
int qg_syscall_lock_fps(uint32_t target_fps);
int qg_syscall_get_input_latency(uint32_t device_id, quantum_number_t *latency);

/* Compatibility with existing games */
int qg_enable_compatibility_mode(const char *game_name);
int qg_apply_game_specific_optimizations(const char *game_name);
int qg_load_game_profile(const char *profile_path);
int qg_save_game_profile(const char *profile_path, uint32_t process_id);

/* Utility functions */
qg_gaming_process_t *qg_get_gaming_process(uint32_t process_id);
qg_input_device_t *qg_get_input_device(uint32_t device_id);
qg_frame_buffer_t *qg_get_frame_buffer(uint32_t buffer_id);
bool qg_is_gaming_process(uint32_t process_id);
uint32_t qg_get_optimal_cpu_affinity(qg_process_type_t type);
uint32_t qg_calculate_optimal_polling_rate(qg_input_type_t type);

/* Debug and diagnostics */
void qg_print_performance_stats(void);
void qg_print_gaming_processes(void);
void qg_print_input_devices(void);
void qg_dump_frame_buffer_info(uint32_t buffer_id);
int qg_run_performance_benchmark(void);

#endif /* QUANTUM_GAMING_H */
