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
 * @file kernel.h
 * @brief Quantum OS - Main Kernel Header
 * 
 * Revolutionary operating system kernel designed for symbolic mathematical
 * computation and Quantum Number operations.
 * 
 * @author Arbitrary Number Project Team
 * @date August 18, 2025
 */

#ifndef QUANTUM_KERNEL_H
#define QUANTUM_KERNEL_H

#include "quantum_number.h"
#include "memory.h"
#include "process.h"
#include "syscalls.h"
#include <stdint.h>
#include <stddef.h>

// Kernel version information
#define QUANTUM_OS_VERSION_MAJOR    1
#define QUANTUM_OS_VERSION_MINOR    0
#define QUANTUM_OS_VERSION_PATCH    0
#define QUANTUM_OS_VERSION_STRING   "1.0.0"
#define QUANTUM_OS_BUILD_DATE       "August 18, 2025"

// Kernel initialization phases
typedef enum {
    KERNEL_PHASE_EARLY_INIT,        // Early initialization
    KERNEL_PHASE_MEMORY_INIT,       // Memory management setup
    KERNEL_PHASE_PROCESS_INIT,      // Process management setup
    KERNEL_PHASE_SYSCALL_INIT,      // System call interface setup
    KERNEL_PHASE_DEVICE_INIT,       // Device driver initialization
    KERNEL_PHASE_FS_INIT,           // File system initialization
    KERNEL_PHASE_USER_INIT,         // User space initialization
    KERNEL_PHASE_COMPLETE           // Kernel fully initialized
} kernel_phase_t;

// Kernel state structure
typedef struct kernel_state {
    kernel_phase_t current_phase;       // Current initialization phase
    uint64_t boot_time;                 // System boot time
    uint64_t uptime;                    // System uptime in ticks
    size_t total_memory;                // Total system memory
    uint32_t cpu_count;                 // Number of CPU cores
    uint32_t quantum_units_processed;   // Total quantum operations
    uint32_t mathematical_operations;   // Mathematical operations count
    uint32_t symbolic_computations;     // Symbolic computations count
    char version_string[64];            // Kernel version string
    uint32_t kernel_flags;              // Kernel configuration flags
} kernel_state_t;

// Kernel configuration flags
#define KERNEL_FLAG_DEBUG           0x01    // Debug mode enabled
#define KERNEL_FLAG_MATHEMATICAL    0x02    // Mathematical optimization
#define KERNEL_FLAG_SYMBOLIC        0x04    // Symbolic computation support
#define KERNEL_FLAG_MULTICORE       0x08    // Multi-core support
#define KERNEL_FLAG_REAL_TIME       0x10    // Real-time scheduling
#define KERNEL_FLAG_SECURE          0x20    // Security features enabled

// Kernel panic codes
typedef enum {
    KERNEL_PANIC_GENERAL = 0,
    KERNEL_PANIC_OUT_OF_MEMORY,
    KERNEL_PANIC_INVALID_STATE,
    KERNEL_PANIC_HARDWARE_FAULT,
    KERNEL_PANIC_MATHEMATICAL_ERROR,
    KERNEL_PANIC_CORRUPTION,
    KERNEL_PANIC_ASSERTION_FAILED
} kernel_panic_code_t;

// Main kernel functions
int kernel_init(size_t memory_size, uint32_t flags);
void kernel_main(void);
void kernel_shutdown(void);
kernel_state_t* kernel_get_state(void);

// Kernel initialization functions
int kernel_early_init(void);
int kernel_memory_init(size_t memory_size);
int kernel_process_init(void);
int kernel_syscall_init(void);
int kernel_device_init(void);
int kernel_fs_init(void);
int kernel_user_init(void);

// Kernel utility functions
void kernel_print_banner(void);
void kernel_print_statistics(void);
uint64_t kernel_get_uptime(void);
uint64_t kernel_get_boot_time(void);
const char* kernel_get_version(void);

// Kernel panic and error handling
void kernel_panic(kernel_panic_code_t code, const char *message);
void kernel_assert(int condition, const char *message);
void kernel_log(const char *level, const char *format, ...);

// Kernel debugging
#ifdef KERNEL_DEBUG
void kernel_debug_print(const char *format, ...);
void kernel_debug_dump_state(void);
void kernel_debug_memory_stats(void);
void kernel_debug_process_list(void);
#define KERNEL_DEBUG_PRINT(fmt, ...) kernel_debug_print(fmt, ##__VA_ARGS__)
#define KERNEL_ASSERT(cond, msg) kernel_assert(cond, msg)
#else
#define KERNEL_DEBUG_PRINT(fmt, ...)
#define KERNEL_ASSERT(cond, msg)
#endif

// Kernel logging levels
#define KERNEL_LOG_EMERGENCY    "EMERG"
#define KERNEL_LOG_ALERT        "ALERT"
#define KERNEL_LOG_CRITICAL     "CRIT"
#define KERNEL_LOG_ERROR        "ERROR"
#define KERNEL_LOG_WARNING      "WARN"
#define KERNEL_LOG_NOTICE       "NOTICE"
#define KERNEL_LOG_INFO         "INFO"
#define KERNEL_LOG_DEBUG        "DEBUG"

// Kernel timer and scheduling
void kernel_timer_init(void);
void kernel_timer_tick(void);
uint64_t kernel_get_ticks(void);
void kernel_schedule(void);

// Kernel interrupt handling
void kernel_interrupt_init(void);
void kernel_interrupt_handler(uint32_t interrupt_number);
int kernel_register_interrupt_handler(uint32_t interrupt_number, 
                                     void (*handler)(uint32_t));

// Kernel mathematical operations tracking
void kernel_track_quantum_operation(void);
void kernel_track_mathematical_operation(void);
void kernel_track_symbolic_computation(void);

// Kernel error codes
typedef enum {
    KERNEL_SUCCESS = 0,
    KERNEL_ERROR_INIT_FAILED = -1,
    KERNEL_ERROR_OUT_OF_MEMORY = -2,
    KERNEL_ERROR_INVALID_PARAMETER = -3,
    KERNEL_ERROR_HARDWARE_FAULT = -4,
    KERNEL_ERROR_MATHEMATICAL_ERROR = -5,
    KERNEL_ERROR_CORRUPTION = -6,
    KERNEL_ERROR_NOT_IMPLEMENTED = -7
} kernel_error_t;

const char* kernel_error_string(kernel_error_t error);

// Kernel boot parameters
typedef struct kernel_boot_params {
    size_t memory_size;             // Total system memory
    uint32_t cpu_count;             // Number of CPU cores
    uint32_t flags;                 // Kernel configuration flags
    void *initrd_start;             // Initial ramdisk start
    size_t initrd_size;             // Initial ramdisk size
    char cmdline[256];              // Kernel command line
} kernel_boot_params_t;

// Kernel command line parsing
int kernel_parse_cmdline(const char *cmdline, kernel_boot_params_t *params);
int kernel_get_cmdline_param(const char *name, char *value, size_t value_size);

#endif // QUANTUM_KERNEL_H
