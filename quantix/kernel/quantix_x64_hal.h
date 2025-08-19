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
/*
 * Quantix OS x64 Hardware Abstraction Layer
 * 
 * This header defines the x64 Hardware Abstraction Layer for Quantix OS,
 * providing low-level hardware interface for x64 processors while maintaining
 * Quantum Number mathematical precision through software emulation.
 *
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTIX_X64_HAL_H
#define QUANTIX_X64_HAL_H

#include "../quantum-os/kernel/quantum_number.h"
#include <stdint.h>
#include <stdbool.h>

// x64 HAL Version
#define QUANTIX_X64_HAL_VERSION_MAJOR 1
#define QUANTIX_X64_HAL_VERSION_MINOR 0
#define QUANTIX_X64_HAL_VERSION_PATCH 0
#define QUANTIX_X64_HAL_VERSION ((QUANTIX_X64_HAL_VERSION_MAJOR << 16) | \
                                (QUANTIX_X64_HAL_VERSION_MINOR << 8) | \
                                QUANTIX_X64_HAL_VERSION_PATCH)

// CPU Feature Detection Flags
#define X64_FEATURE_SSE         (1ULL << 0)
#define X64_FEATURE_SSE2        (1ULL << 1)
#define X64_FEATURE_SSE3        (1ULL << 2)
#define X64_FEATURE_SSSE3       (1ULL << 3)
#define X64_FEATURE_SSE4_1      (1ULL << 4)
#define X64_FEATURE_SSE4_2      (1ULL << 5)
#define X64_FEATURE_AVX         (1ULL << 6)
#define X64_FEATURE_AVX2        (1ULL << 7)
#define X64_FEATURE_AVX512F     (1ULL << 8)
#define X64_FEATURE_AVX512DQ    (1ULL << 9)
#define X64_FEATURE_AVX512CD    (1ULL << 10)
#define X64_FEATURE_AVX512BW    (1ULL << 11)
#define X64_FEATURE_AVX512VL    (1ULL << 12)
#define X64_FEATURE_FMA         (1ULL << 13)
#define X64_FEATURE_BMI1        (1ULL << 14)
#define X64_FEATURE_BMI2        (1ULL << 15)
#define X64_FEATURE_POPCNT      (1ULL << 16)
#define X64_FEATURE_LZCNT       (1ULL << 17)
#define X64_FEATURE_RDRAND      (1ULL << 18)
#define X64_FEATURE_RDSEED      (1ULL << 19)
#define X64_FEATURE_AES         (1ULL << 20)
#define X64_FEATURE_PCLMULQDQ   (1ULL << 21)
#define X64_FEATURE_TSX         (1ULL << 22)
#define X64_FEATURE_MPX         (1ULL << 23)
#define X64_FEATURE_SHA         (1ULL << 24)
#define X64_FEATURE_CET         (1ULL << 25)
#define X64_FEATURE_VAES        (1ULL << 26)
#define X64_FEATURE_VPCLMULQDQ  (1ULL << 27)

// Memory Protection Flags
#define X64_MEM_READ            (1 << 0)
#define X64_MEM_WRITE           (1 << 1)
#define X64_MEM_EXECUTE         (1 << 2)
#define X64_MEM_USER            (1 << 3)
#define X64_MEM_GLOBAL          (1 << 4)
#define X64_MEM_NOCACHE         (1 << 5)
#define X64_MEM_WRITETHROUGH    (1 << 6)
#define X64_MEM_LARGE_PAGE      (1 << 7)

// Interrupt Vector Definitions
#define X64_INT_DIVIDE_ERROR    0
#define X64_INT_DEBUG           1
#define X64_INT_NMI             2
#define X64_INT_BREAKPOINT      3
#define X64_INT_OVERFLOW        4
#define X64_INT_BOUND_RANGE     5
#define X64_INT_INVALID_OPCODE  6
#define X64_INT_DEVICE_NA       7
#define X64_INT_DOUBLE_FAULT    8
#define X64_INT_INVALID_TSS     10
#define X64_INT_SEGMENT_NP      11
#define X64_INT_STACK_FAULT     12
#define X64_INT_GENERAL_PROT    13
#define X64_INT_PAGE_FAULT      14
#define X64_INT_FPU_ERROR       16
#define X64_INT_ALIGNMENT       17
#define X64_INT_MACHINE_CHECK   18
#define X64_INT_SIMD_FP         19
#define X64_INT_VIRT_EXCEPTION  20
#define X64_INT_SECURITY        30

// APIC Definitions
#define X64_APIC_BASE_MSR       0x1B
#define X64_APIC_ENABLE         (1 << 11)
#define X64_APIC_X2APIC_ENABLE  (1 << 10)

// Error Codes
#define X64_HAL_SUCCESS                 0
#define X64_HAL_ERROR_NOT_INITIALIZED  -1
#define X64_HAL_ERROR_INVALID_PARAMETER -2
#define X64_HAL_ERROR_OUT_OF_MEMORY    -3
#define X64_HAL_ERROR_HARDWARE_FAILURE -4
#define X64_HAL_ERROR_UNSUPPORTED      -5
#define X64_HAL_ERROR_TIMEOUT          -6

// CPU Vendor IDs
typedef enum {
    X64_VENDOR_UNKNOWN,
    X64_VENDOR_INTEL,
    X64_VENDOR_AMD,
    X64_VENDOR_VIA,
    X64_VENDOR_HYGON
} x64_cpu_vendor_t;

// CPU Information Structure
typedef struct {
    x64_cpu_vendor_t vendor;
    uint32_t family;
    uint32_t model;
    uint32_t stepping;
    uint32_t signature;
    uint64_t features;
    uint32_t cache_line_size;
    uint32_t logical_processors;
    uint32_t physical_cores;
    uint32_t max_cpuid_leaf;
    uint32_t max_extended_leaf;
    char brand_string[49];
    char vendor_string[13];
} x64_cpu_info_t;

// Memory Information Structure
typedef struct {
    uint64_t total_physical_memory;
    uint64_t available_physical_memory;
    uint64_t total_virtual_memory;
    uint64_t available_virtual_memory;
    uint64_t kernel_virtual_start;
    uint64_t kernel_virtual_end;
    uint64_t user_virtual_start;
    uint64_t user_virtual_end;
    uint32_t page_size;
    uint32_t large_page_size;
    uint32_t huge_page_size;
    bool large_pages_supported;
    bool huge_pages_supported;
    bool nx_bit_supported;
    bool smep_supported;
    bool smap_supported;
} x64_memory_info_t;

// APIC Information Structure
typedef struct {
    bool local_apic_present;
    bool x2apic_supported;
    bool x2apic_enabled;
    uint32_t local_apic_id;
    uint64_t local_apic_base;
    uint32_t max_lvt_entries;
    bool tsc_deadline_supported;
} x64_apic_info_t;

// Timer Information Structure
typedef struct {
    uint64_t tsc_frequency;
    uint64_t apic_timer_frequency;
    uint64_t hpet_frequency;
    bool tsc_invariant;
    bool tsc_reliable;
    bool apic_timer_present;
    bool hpet_present;
    bool pit_present;
} x64_timer_info_t;

// Interrupt Handler Function Type
typedef void (*x64_interrupt_handler_t)(uint64_t vector, uint64_t error_code, void* context);

// System Information Structure
typedef struct {
    uint32_t hal_version;
    x64_cpu_info_t cpu;
    x64_memory_info_t memory;
    x64_apic_info_t apic;
    x64_timer_info_t timer;
    bool hypervisor_present;
    char hypervisor_signature[13];
    bool secure_boot_enabled;
    bool virtualization_enabled;
} x64_system_info_t;

// Page Table Entry Structure
typedef struct {
    uint64_t present : 1;
    uint64_t writable : 1;
    uint64_t user : 1;
    uint64_t write_through : 1;
    uint64_t cache_disable : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t large_page : 1;
    uint64_t global : 1;
    uint64_t available1 : 3;
    uint64_t physical_address : 40;
    uint64_t available2 : 11;
    uint64_t nx : 1;
} __attribute__((packed)) x64_page_table_entry_t;

// GDT Entry Structure
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) x64_gdt_entry_t;

// IDT Entry Structure
typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed)) x64_idt_entry_t;

// Function Declarations

// System Initialization
int x64_hal_init(void);
int x64_hal_shutdown(void);
int x64_hal_get_system_info(x64_system_info_t* info);

// CPU Management
int x64_hal_detect_cpu(x64_cpu_info_t* cpu_info);
int x64_hal_get_cpu_features(uint64_t* features);
int x64_hal_enable_cpu_feature(uint64_t feature);
int x64_hal_disable_cpu_feature(uint64_t feature);
uint32_t x64_hal_get_cpu_count(void);
uint32_t x64_hal_get_current_cpu(void);

// Memory Management
int x64_hal_init_memory(x64_memory_info_t* memory_info);
int x64_hal_map_physical_memory(uint64_t physical_addr, 
                               uint64_t virtual_addr, 
                               uint64_t size, 
                               uint32_t flags);
int x64_hal_unmap_memory(uint64_t virtual_addr, uint64_t size);
int x64_hal_set_memory_protection(uint64_t virtual_addr, 
                                 uint64_t size, 
                                 uint32_t flags);
uint64_t x64_hal_get_physical_address(uint64_t virtual_addr);
void* x64_hal_allocate_physical_pages(uint64_t page_count);
int x64_hal_free_physical_pages(void* physical_addr, uint64_t page_count);

// Interrupt Management
int x64_hal_init_interrupts(void);
int x64_hal_register_interrupt_handler(uint32_t vector, 
                                      x64_interrupt_handler_t handler);
int x64_hal_unregister_interrupt_handler(uint32_t vector);
int x64_hal_enable_interrupts(void);
int x64_hal_disable_interrupts(void);
int x64_hal_send_ipi(uint32_t target_cpu, uint32_t vector);
int x64_hal_send_ipi_all(uint32_t vector);

// APIC Management
int x64_hal_init_apic(void);
int x64_hal_enable_x2apic(void);
int x64_hal_disable_x2apic(void);
uint32_t x64_hal_get_apic_id(void);
int x64_hal_set_apic_timer(uint64_t initial_count, 
                          uint32_t divide_config, 
                          bool periodic);
int x64_hal_stop_apic_timer(void);

// Timer Management
int x64_hal_init_timers(void);
uint64_t x64_hal_read_tsc(void);
uint64_t x64_hal_get_tsc_frequency(void);
uint64_t x64_hal_get_system_time_ns(void);
int x64_hal_set_timer_interrupt(uint64_t nanoseconds);
int x64_hal_cancel_timer_interrupt(void);

// I/O Operations
uint8_t x64_hal_inb(uint16_t port);
uint16_t x64_hal_inw(uint16_t port);
uint32_t x64_hal_inl(uint16_t port);
void x64_hal_outb(uint16_t port, uint8_t value);
void x64_hal_outw(uint16_t port, uint16_t value);
void x64_hal_outl(uint16_t port, uint32_t value);

// MSR Operations
uint64_t x64_hal_read_msr(uint32_t msr);
int x64_hal_write_msr(uint32_t msr, uint64_t value);

// Control Register Operations
uint64_t x64_hal_read_cr0(void);
uint64_t x64_hal_read_cr2(void);
uint64_t x64_hal_read_cr3(void);
uint64_t x64_hal_read_cr4(void);
int x64_hal_write_cr0(uint64_t value);
int x64_hal_write_cr3(uint64_t value);
int x64_hal_write_cr4(uint64_t value);

// Cache Management
int x64_hal_flush_cache(void);
int x64_hal_invalidate_cache(void);
int x64_hal_flush_tlb(void);
int x64_hal_invalidate_tlb_entry(uint64_t virtual_addr);
int x64_hal_flush_tlb_global(void);

// Power Management
int x64_hal_halt_cpu(void);
int x64_hal_enter_sleep_state(uint32_t state);
int x64_hal_set_cpu_frequency(uint32_t frequency_mhz);
int x64_hal_get_cpu_temperature(uint32_t* temperature);

// Hypervisor Detection
int x64_hal_detect_hypervisor(char* signature, size_t signature_size);
bool x64_hal_is_virtualized(void);
int x64_hal_get_hypervisor_info(char* info, size_t info_size);

// Security Features
int x64_hal_enable_smep(void);
int x64_hal_enable_smap(void);
int x64_hal_enable_cet(void);
int x64_hal_setup_stack_canary(void);
bool x64_hal_is_secure_boot_enabled(void);

// Performance Monitoring
int x64_hal_init_performance_counters(void);
int x64_hal_start_performance_counter(uint32_t counter, uint32_t event);
int x64_hal_stop_performance_counter(uint32_t counter);
uint64_t x64_hal_read_performance_counter(uint32_t counter);

// Debug Support
int x64_hal_enable_debug_mode(void);
int x64_hal_disable_debug_mode(void);
int x64_hal_set_breakpoint(uint64_t address, uint32_t type);
int x64_hal_clear_breakpoint(uint32_t breakpoint_id);
int x64_hal_single_step_enable(void);
int x64_hal_single_step_disable(void);

// CPUID Wrapper Functions
void x64_hal_cpuid(uint32_t leaf, 
                   uint32_t subleaf, 
                   uint32_t* eax, 
                   uint32_t* ebx, 
                   uint32_t* ecx, 
                   uint32_t* edx);
bool x64_hal_cpuid_supported(void);

// Atomic Operations
uint64_t x64_hal_atomic_add(volatile uint64_t* ptr, uint64_t value);
uint64_t x64_hal_atomic_sub(volatile uint64_t* ptr, uint64_t value);
uint64_t x64_hal_atomic_exchange(volatile uint64_t* ptr, uint64_t value);
bool x64_hal_atomic_compare_exchange(volatile uint64_t* ptr, 
                                    uint64_t expected, 
                                    uint64_t desired);
uint64_t x64_hal_atomic_increment(volatile uint64_t* ptr);
uint64_t x64_hal_atomic_decrement(volatile uint64_t* ptr);

// Memory Barriers
void x64_hal_memory_barrier(void);
void x64_hal_read_barrier(void);
void x64_hal_write_barrier(void);
void x64_hal_compiler_barrier(void);

// NUMA Support
int x64_hal_get_numa_node_count(void);
int x64_hal_get_current_numa_node(void);
int x64_hal_get_cpu_numa_node(uint32_t cpu_id);
int x64_hal_allocate_numa_memory(uint32_t node, 
                                uint64_t size, 
                                void** memory);
int x64_hal_free_numa_memory(void* memory, uint64_t size);

// Quantum Number Hardware Acceleration Detection
bool x64_hal_has_quantum_acceleration(void);
int x64_hal_init_quantum_acceleration(void);
int x64_hal_shutdown_quantum_acceleration(void);

// Collapse Module Support
int x64_hal_detect_collapse_modules(void);
int x64_hal_enumerate_gpus(void);
int x64_hal_init_gpu_collapse_module(uint32_t gpu_id);

// Inline Assembly Helpers
static inline void x64_hal_cli(void) {
    __asm__ volatile ("cli" ::: "memory");
}

static inline void x64_hal_sti(void) {
    __asm__ volatile ("sti" ::: "memory");
}

static inline void x64_hal_hlt(void) {
    __asm__ volatile ("hlt");
}

static inline void x64_hal_pause(void) {
    __asm__ volatile ("pause");
}

static inline void x64_hal_nop(void) {
    __asm__ volatile ("nop");
}

static inline void x64_hal_wbinvd(void) {
    __asm__ volatile ("wbinvd" ::: "memory");
}

static inline void x64_hal_invlpg(uint64_t addr) {
    __asm__ volatile ("invlpg (%0)" :: "r" (addr) : "memory");
}

static inline uint64_t x64_hal_rdtsc(void) {
    uint32_t low, high;
    __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
    return ((uint64_t)high << 32) | low;
}

static inline uint64_t x64_hal_rdtscp(uint32_t* aux) {
    uint32_t low, high;
    __asm__ volatile ("rdtscp" : "=a" (low), "=d" (high), "=c" (*aux));
    return ((uint64_t)high << 32) | low;
}

#endif // QUANTIX_X64_HAL_H
