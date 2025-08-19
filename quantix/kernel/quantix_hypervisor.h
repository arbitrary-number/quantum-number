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
 * Quantix OS Hypervisor Integration Layer
 * 
 * This header defines the hypervisor integration layer for Quantix OS,
 * providing optimized support for Hyper-V, VMware, VirtualBox, KVM/QEMU,
 * and other virtualization platforms with paravirtualization features.
 *
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTIX_HYPERVISOR_H
#define QUANTIX_HYPERVISOR_H

#include "quantix_x64_hal.h"
#include "quantix_qn_emulation.h"
#include <stdint.h>
#include <stdbool.h>

// Hypervisor Integration Version
#define QUANTIX_HYPERVISOR_VERSION_MAJOR 1
#define QUANTIX_HYPERVISOR_VERSION_MINOR 0
#define QUANTIX_HYPERVISOR_VERSION_PATCH 0
#define QUANTIX_HYPERVISOR_VERSION ((QUANTIX_HYPERVISOR_VERSION_MAJOR << 16) | \
                                   (QUANTIX_HYPERVISOR_VERSION_MINOR << 8) | \
                                   QUANTIX_HYPERVISOR_VERSION_PATCH)

// Hypervisor Types
typedef enum {
    QUANTIX_HV_TYPE_NONE,           // Bare metal
    QUANTIX_HV_TYPE_HYPER_V,        // Microsoft Hyper-V
    QUANTIX_HV_TYPE_VMWARE,         // VMware ESXi/Workstation
    QUANTIX_HV_TYPE_VIRTUALBOX,     // Oracle VirtualBox
    QUANTIX_HV_TYPE_KVM,            // Linux KVM/QEMU
    QUANTIX_HV_TYPE_XEN,            // Citrix Xen
    QUANTIX_HV_TYPE_PARALLELS,      // Parallels Desktop
    QUANTIX_HV_TYPE_UNKNOWN         // Unknown hypervisor
} quantix_hypervisor_type_t;

// Hypervisor Capabilities
#define QUANTIX_HV_CAP_PARAVIRT         (1ULL << 0)
#define QUANTIX_HV_CAP_ENLIGHTENMENTS   (1ULL << 1)
#define QUANTIX_HV_CAP_SYNTHETIC_TIMERS (1ULL << 2)
#define QUANTIX_HV_CAP_SYNTHETIC_INTS   (1ULL << 3)
#define QUANTIX_HV_CAP_HYPERCALLS       (1ULL << 4)
#define QUANTIX_HV_CAP_VP_ASSIST        (1ULL << 5)
#define QUANTIX_HV_CAP_MEMORY_BALLOON   (1ULL << 6)
#define QUANTIX_HV_CAP_GPU_PASSTHROUGH  (1ULL << 7)
#define QUANTIX_HV_CAP_NESTED_VIRT      (1ULL << 8)
#define QUANTIX_HV_CAP_SECURE_BOOT      (1ULL << 9)

// Error Codes
#define QUANTIX_HV_SUCCESS                  0
#define QUANTIX_HV_ERROR_NOT_VIRTUALIZED   -1
#define QUANTIX_HV_ERROR_UNSUPPORTED_HV    -2
#define QUANTIX_HV_ERROR_INVALID_PARAMETER -3
#define QUANTIX_HV_ERROR_HYPERCALL_FAILED  -4
#define QUANTIX_HV_ERROR_OUT_OF_MEMORY     -5
#define QUANTIX_HV_ERROR_TIMEOUT           -6
#define QUANTIX_HV_ERROR_ACCESS_DENIED     -7

// Hyper-V Specific Definitions
#define HYPERV_CPUID_VENDOR_SIGNATURE   0x40000000
#define HYPERV_CPUID_INTERFACE          0x40000001
#define HYPERV_CPUID_VERSION            0x40000002
#define HYPERV_CPUID_FEATURES           0x40000003
#define HYPERV_CPUID_ENLIGHTENMENTS     0x40000004
#define HYPERV_CPUID_IMPLEMENTATION     0x40000005

// Hyper-V MSRs
#define HV_MSR_GUEST_OS_ID              0x40000000
#define HV_MSR_HYPERCALL                0x40000001
#define HV_MSR_VP_INDEX                 0x40000002
#define HV_MSR_RESET                    0x40000003
#define HV_MSR_VP_RUNTIME               0x40000010
#define HV_MSR_TIME_REF_COUNT           0x40000020
#define HV_MSR_REFERENCE_TSC            0x40000021
#define HV_MSR_TSC_FREQUENCY            0x40000022
#define HV_MSR_APIC_FREQUENCY           0x40000023

// Hyper-V Hypercall Codes
#define HVCALL_POST_MESSAGE             0x005C
#define HVCALL_SIGNAL_EVENT             0x005D
#define HVCALL_POST_DEBUG_DATA          0x0069
#define HVCALL_RETRIEVE_DEBUG_DATA      0x006A
#define HVCALL_RESET_DEBUG_SESSION      0x006B

// VMware Specific Definitions
#define VMWARE_CPUID_VENDOR_SIGNATURE   0x40000000
#define VMWARE_HYPERVISOR_MAGIC         0x564D5868
#define VMWARE_HYPERVISOR_PORT          0x5658

// VMware Commands
#define VMWARE_CMD_GETVERSION           10
#define VMWARE_CMD_GETHZ                45
#define VMWARE_CMD_GETTIME              23

// Hypervisor Information Structure
typedef struct {
    quantix_hypervisor_type_t type;
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t build_number;
    uint64_t capabilities;
    char vendor_signature[13];
    char product_name[64];
    bool enlightenments_available;
    bool paravirt_available;
    uint32_t max_virtual_processors;
    uint64_t memory_balloon_size;
} quantix_hypervisor_info_t;

// Hyper-V Enlightenments Structure
typedef struct {
    bool vp_runtime_available;
    bool time_ref_count_available;
    bool synic_available;
    bool synthetic_timers_available;
    bool apic_access_available;
    bool hypercall_available;
    bool vp_index_available;
    bool reset_available;
    bool stats_available;
    bool partition_ref_counter_available;
    bool guest_idle_available;
    bool frequency_regs_available;
} quantix_hyperv_enlightenments_t;

// Virtual Processor Assist Page
typedef struct {
    uint32_t apic_assist;
    uint32_t reserved1;
    uint64_t vtl_control;
    uint64_t nested_enlightenments_control;
    bool enlighten_vmentry;
    bool enlighten_vmexit;
    bool enlighten_tlb_flush;
    uint8_t reserved2[5];
    uint64_t current_nested_vmcs;
    bool synthetic_time_unhalted_timer_expired;
    uint8_t reserved3[7];
    uint64_t virtualization_fault_information;
    uint8_t reserved4[40];
} __attribute__((packed)) quantix_vp_assist_page_t;

// Synthetic Interrupt Controller
typedef struct {
    uint64_t control;
    uint64_t event_flags_page;
    uint64_t message_page;
    uint32_t sint[16];
    uint64_t eom;
    uint64_t irr;
    uint64_t icr;
} quantix_synic_t;

// Memory Balloon Information
typedef struct {
    uint64_t current_size;
    uint64_t target_size;
    uint64_t max_size;
    uint32_t page_size;
    bool dynamic_memory_enabled;
    bool smart_paging_enabled;
    uint64_t startup_memory;
    uint64_t minimum_memory;
    uint64_t maximum_memory;
} quantix_memory_balloon_t;

// GPU Passthrough Information
typedef struct {
    bool discrete_device_assignment;
    bool remotefx_available;
    bool gpu_partitioning_available;
    uint32_t assigned_gpu_count;
    char assigned_gpu_names[4][64];
    uint64_t gpu_memory_sizes[4];
} quantix_gpu_passthrough_t;

// Performance Metrics
typedef struct {
    uint64_t hypercalls_made;
    uint64_t enlightenment_operations;
    uint64_t synthetic_interrupts;
    uint64_t memory_balloon_operations;
    uint64_t vp_assist_operations;
    uint32_t average_hypercall_time_ns;
    uint32_t virtualization_overhead_percent;
} quantix_hypervisor_metrics_t;

// Hypervisor State
typedef struct {
    quantix_hypervisor_type_t type;
    quantix_hypervisor_info_t info;
    quantix_hyperv_enlightenments_t hyperv_enlightenments;
    quantix_vp_assist_page_t* vp_assist_page;
    quantix_synic_t* synic;
    quantix_memory_balloon_t memory_balloon;
    quantix_gpu_passthrough_t gpu_passthrough;
    quantix_hypervisor_metrics_t metrics;
    bool initialized;
    void* private_data;
} quantix_hypervisor_state_t;

// Hypercall Input/Output Structures
typedef struct {
    uint64_t control;
    uint64_t input_params_gpa;
    uint64_t output_params_gpa;
} quantix_hypercall_input_t;

typedef struct {
    uint16_t call_status;
    uint16_t reserved1;
    uint32_t elements_processed;
    uint64_t reserved2;
} quantix_hypercall_output_t;

// Function Declarations

// Hypervisor Detection and Initialization
int quantix_hypervisor_detect(quantix_hypervisor_type_t* type);
int quantix_hypervisor_init(void);
int quantix_hypervisor_shutdown(void);
int quantix_hypervisor_get_info(quantix_hypervisor_info_t* info);
bool quantix_hypervisor_is_virtualized(void);

// Hyper-V Specific Functions
int quantix_hyperv_init_enlightenments(void);
int quantix_hyperv_setup_hypercall_page(void);
int quantix_hyperv_setup_vp_assist_page(void);
int quantix_hyperv_init_synic(void);
int quantix_hyperv_shutdown_synic(void);
int quantix_hyperv_post_message(uint32_t connection_id, 
                               uint32_t message_type,
                               const void* payload,
                               uint32_t payload_size);
int quantix_hyperv_signal_event(uint32_t connection_id, uint16_t flag_number);

// Hyper-V Hypercalls
uint64_t quantix_hyperv_hypercall(uint64_t control, 
                                 uint64_t input_params_gpa,
                                 uint64_t output_params_gpa);
int quantix_hyperv_get_partition_id(uint64_t* partition_id);
int quantix_hyperv_get_vp_index(uint32_t* vp_index);
uint64_t quantix_hyperv_get_time_ref_count(void);
uint64_t quantix_hyperv_get_vp_runtime(void);

// VMware Specific Functions
int quantix_vmware_init(void);
int quantix_vmware_get_version(uint32_t* version);
int quantix_vmware_get_hz(uint64_t* hz);
int quantix_vmware_get_time(uint64_t* time);
int quantix_vmware_backdoor_call(uint32_t cmd, 
                                uint32_t* eax, 
                                uint32_t* ebx,
                                uint32_t* ecx, 
                                uint32_t* edx);

// VirtualBox Specific Functions
int quantix_vbox_init(void);
int quantix_vbox_detect_guest_additions(bool* installed);
int quantix_vbox_get_host_version(char* version, size_t version_size);

// KVM/QEMU Specific Functions
int quantix_kvm_init(void);
int quantix_kvm_setup_pv_eoi(void);
int quantix_kvm_setup_pv_unhalt(void);
int quantix_kvm_setup_async_pf(void);

// Memory Balloon Management
int quantix_hypervisor_init_memory_balloon(void);
int quantix_hypervisor_set_balloon_target(uint64_t target_size);
int quantix_hypervisor_get_balloon_info(quantix_memory_balloon_t* info);
int quantix_hypervisor_balloon_inflate(uint64_t pages);
int quantix_hypervisor_balloon_deflate(uint64_t pages);

// GPU Passthrough Support
int quantix_hypervisor_init_gpu_passthrough(void);
int quantix_hypervisor_enumerate_assigned_gpus(void);
int quantix_hypervisor_get_gpu_info(quantix_gpu_passthrough_t* info);
int quantix_hypervisor_setup_collapse_module_vm(uint32_t gpu_id);

// Performance Optimization
int quantix_hypervisor_optimize_for_vm(void);
int quantix_hypervisor_enable_enlightenments(void);
int quantix_hypervisor_setup_synthetic_timers(void);
int quantix_hypervisor_setup_synthetic_interrupts(void);
int quantix_hypervisor_tune_scheduler(void);

// Time and Clock Management
uint64_t quantix_hypervisor_get_tsc_frequency(void);
uint64_t quantix_hypervisor_get_apic_frequency(void);
int quantix_hypervisor_setup_reference_tsc(void);
int quantix_hypervisor_calibrate_timers(void);

// Nested Virtualization Support
bool quantix_hypervisor_supports_nested_virt(void);
int quantix_hypervisor_enable_nested_virt(void);
int quantix_hypervisor_setup_nested_enlightenments(void);

// Security Features
bool quantix_hypervisor_secure_boot_enabled(void);
int quantix_hypervisor_setup_secure_boot(void);
int quantix_hypervisor_validate_integrity(void);
int quantix_hypervisor_setup_hvci(void);

// Debug and Diagnostics
int quantix_hypervisor_enable_debug_mode(void);
int quantix_hypervisor_post_debug_data(const void* data, size_t size);
int quantix_hypervisor_retrieve_debug_data(void* buffer, size_t buffer_size);
int quantix_hypervisor_dump_state(char* buffer, size_t buffer_size);

// Performance Monitoring
int quantix_hypervisor_get_metrics(quantix_hypervisor_metrics_t* metrics);
int quantix_hypervisor_reset_metrics(void);
int quantix_hypervisor_benchmark_hypercalls(uint32_t iterations,
                                           uint64_t* average_time_ns);

// Configuration Management
int quantix_hypervisor_get_vm_config(char* config, size_t config_size);
int quantix_hypervisor_optimize_quantum_numbers(void);
int quantix_hypervisor_setup_exact_arithmetic(void);

// Integration with Quantum Number System
int quantix_hypervisor_accelerate_qn_operations(void);
int quantix_hypervisor_setup_qn_cache_coherency(void);
int quantix_hypervisor_optimize_qn_memory_access(void);

// Utility Functions
const char* quantix_hypervisor_type_to_string(quantix_hypervisor_type_t type);
int quantix_hypervisor_parse_cpuid_signature(const char* signature,
                                            quantix_hypervisor_type_t* type);
bool quantix_hypervisor_feature_available(uint64_t feature);

// Inline Helper Functions
static inline bool quantix_hypervisor_is_hyper_v(void) {
    uint32_t eax, ebx, ecx, edx;
    x64_hal_cpuid(HYPERV_CPUID_VENDOR_SIGNATURE, 0, &eax, &ebx, &ecx, &edx);
    return (ebx == 0x7263694D && ecx == 0x666F736F && edx == 0x76482074); // "Microsoft Hv"
}

static inline bool quantix_hypervisor_is_vmware(void) {
    uint32_t eax, ebx, ecx, edx;
    x64_hal_cpuid(VMWARE_CPUID_VENDOR_SIGNATURE, 0, &eax, &ebx, &ecx, &edx);
    return (ebx == 0x61774D56 && ecx == 0x4D566572 && edx == 0x65726177); // "VMwareVMware"
}

static inline bool quantix_hypervisor_is_kvm(void) {
    uint32_t eax, ebx, ecx, edx;
    x64_hal_cpuid(0x40000000, 0, &eax, &ebx, &ecx, &edx);
    return (ebx == 0x4b4d564b && ecx == 0x564b4d56 && edx == 0x0000004d); // "KVMKVMKVM\0\0\0"
}

static inline bool quantix_hypervisor_is_virtualbox(void) {
    uint32_t eax, ebx, ecx, edx;
    x64_hal_cpuid(0x40000000, 0, &eax, &ebx, &ecx, &edx);
    return (ebx == 0x786f4256 && ecx == 0x786f4256 && edx == 0x786f4256); // "VBoxVBoxVBox"
}

static inline uint64_t quantix_hypervisor_rdmsr_safe(uint32_t msr, uint64_t default_value) {
    uint64_t value;
    __try {
        value = x64_hal_read_msr(msr);
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        value = default_value;
    }
    return value;
}

static inline void quantix_hypervisor_wrmsr_safe(uint32_t msr, uint64_t value) {
    __try {
        x64_hal_write_msr(msr, value);
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        // Ignore MSR write failures in virtualized environments
    }
}

// Constants
extern const char* QUANTIX_HYPERVISOR_SIGNATURES[];
extern const quantix_hypervisor_info_t QUANTIX_DEFAULT_HYPERVISOR_INFO;

#endif // QUANTIX_HYPERVISOR_H
