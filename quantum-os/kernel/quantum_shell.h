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
 * Quantum Shell Interface Kernel Module
 * 
 * This header defines the shell interface subsystem for the Quantum OS,
 * providing bash/zsh compatibility with Quantum Number extensions, POSIX
 * compliance, and advanced shell features for the Linux-like home OS.
 *
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_SHELL_H
#define QUANTUM_SHELL_H

#include "quantum_number.h"
#include "quantum_gaming.h"
#include "quantum_graphics.h"
#include "posix_compat.h"
#include <stdint.h>
#include <stdbool.h>

// Shell Subsystem Version
#define QUANTUM_SHELL_VERSION_MAJOR 1
#define QUANTUM_SHELL_VERSION_MINOR 0
#define QUANTUM_SHELL_VERSION_PATCH 0
#define QUANTUM_SHELL_VERSION ((QUANTUM_SHELL_VERSION_MAJOR << 16) | \
                              (QUANTUM_SHELL_VERSION_MINOR << 8) | \
                              QUANTUM_SHELL_VERSION_PATCH)

// Maximum Limits
#define MAX_SHELL_SESSIONS 256
#define MAX_COMMAND_LENGTH 8192
#define MAX_ENVIRONMENT_VARS 1024
#define MAX_ALIASES 512
#define MAX_FUNCTIONS 256
#define MAX_HISTORY_ENTRIES 10000
#define MAX_COMPLETION_ENTRIES 1000
#define MAX_PROMPT_LENGTH 512
#define MAX_PATH_LENGTH 4096

// Shell Capabilities
#define QUANTUM_SHELL_CAP_BASH_COMPAT        (1 << 0)
#define QUANTUM_SHELL_CAP_ZSH_COMPAT         (1 << 1)
#define QUANTUM_SHELL_CAP_QUANTUM_EXTENSIONS (1 << 2)
#define QUANTUM_SHELL_CAP_POSIX_COMPLIANCE   (1 << 3)
#define QUANTUM_SHELL_CAP_JOB_CONTROL        (1 << 4)
#define QUANTUM_SHELL_CAP_COMMAND_COMPLETION (1 << 5)
#define QUANTUM_SHELL_CAP_HISTORY_EXPANSION  (1 << 6)
#define QUANTUM_SHELL_CAP_PROGRAMMABLE_COMPLETION (1 << 7)
#define QUANTUM_SHELL_CAP_EXACT_ARITHMETIC   (1 << 8)
#define QUANTUM_SHELL_CAP_GAMING_INTEGRATION (1 << 9)

// Error Codes
#define QUANTUM_SHELL_SUCCESS                0
#define QUANTUM_SHELL_ERROR_NOT_INITIALIZED -1
#define QUANTUM_SHELL_ERROR_ALREADY_INITIALIZED -2
#define QUANTUM_SHELL_ERROR_INVALID_PARAMETER -3
#define QUANTUM_SHELL_ERROR_OUT_OF_MEMORY    -4
#define QUANTUM_SHELL_ERROR_SESSION_NOT_FOUND -5
#define QUANTUM_SHELL_ERROR_COMMAND_NOT_FOUND -6
#define QUANTUM_SHELL_ERROR_SYNTAX_ERROR     -7
#define QUANTUM_SHELL_ERROR_PERMISSION_DENIED -8
#define QUANTUM_SHELL_ERROR_EXECUTION_FAILED -9
#define QUANTUM_SHELL_ERROR_PIPE_FAILED      -10

// Shell Types
typedef enum {
    QUANTUM_SHELL_TYPE_BASH,
    QUANTUM_SHELL_TYPE_ZSH,
    QUANTUM_SHELL_TYPE_QUANTUM_BASH,
    QUANTUM_SHELL_TYPE_QUANTUM_ZSH,
    QUANTUM_SHELL_TYPE_POSIX_SH
} quantum_shell_type_t;

// Command Types
typedef enum {
    QUANTUM_COMMAND_TYPE_BUILTIN,
    QUANTUM_COMMAND_TYPE_EXTERNAL,
    QUANTUM_COMMAND_TYPE_FUNCTION,
    QUANTUM_COMMAND_TYPE_ALIAS,
    QUANTUM_COMMAND_TYPE_QUANTUM_BUILTIN
} quantum_command_type_t;

// Job States
typedef enum {
    QUANTUM_JOB_STATE_RUNNING,
    QUANTUM_JOB_STATE_STOPPED,
    QUANTUM_JOB_STATE_DONE,
    QUANTUM_JOB_STATE_TERMINATED
} quantum_job_state_t;

// Redirection Types
typedef enum {
    QUANTUM_REDIRECT_INPUT,
    QUANTUM_REDIRECT_OUTPUT,
    QUANTUM_REDIRECT_APPEND,
    QUANTUM_REDIRECT_ERROR,
    QUANTUM_REDIRECT_PIPE,
    QUANTUM_REDIRECT_HERE_DOC,
    QUANTUM_REDIRECT_HERE_STRING
} quantum_redirect_type_t;

// Shell Session Configuration
typedef struct {
    quantum_shell_type_t shell_type;
    uint32_t process_id;
    uint32_t user_id;
    uint32_t group_id;
    bool interactive;
    bool login_shell;
    bool quantum_extensions_enabled;
    bool exact_arithmetic_enabled;
    bool gaming_integration_enabled;
    char working_directory[MAX_PATH_LENGTH];
    char home_directory[MAX_PATH_LENGTH];
    char shell_path[MAX_PATH_LENGTH];
} quantum_shell_config_t;

// Environment Variable
typedef struct {
    char name[256];
    char value[1024];
    bool exported;
    bool readonly;
    quantum_number_t* quantum_value; // For exact arithmetic variables
} quantum_env_var_t;

// Shell Alias
typedef struct {
    char name[256];
    char expansion[1024];
    bool active;
} quantum_shell_alias_t;

// Shell Function
typedef struct {
    char name[256];
    char* body;
    size_t body_size;
    bool active;
} quantum_shell_function_t;

// Command History Entry
typedef struct {
    uint32_t entry_id;
    char command[MAX_COMMAND_LENGTH];
    uint64_t timestamp;
    uint32_t exit_code;
    uint64_t execution_time_ns;
} quantum_history_entry_t;

// Command Completion Entry
typedef struct {
    char completion[256];
    char description[512];
    quantum_command_type_t command_type;
} quantum_completion_entry_t;

// Job Control
typedef struct {
    uint32_t job_id;
    uint32_t process_group_id;
    quantum_job_state_t state;
    char command[MAX_COMMAND_LENGTH];
    uint64_t start_time;
    uint64_t end_time;
    uint32_t exit_code;
    bool background;
} quantum_job_t;

// Command Redirection
typedef struct {
    quantum_redirect_type_t type;
    int source_fd;
    int target_fd;
    char filename[MAX_PATH_LENGTH];
    char* here_doc_content;
    size_t here_doc_size;
} quantum_redirect_t;

// Parsed Command
typedef struct {
    char** argv;
    int argc;
    quantum_command_type_t command_type;
    quantum_redirect_t* redirections;
    int redirection_count;
    bool background;
    char* input_file;
    char* output_file;
    char* error_file;
    bool pipe_next;
} quantum_parsed_command_t;

// Command Pipeline
typedef struct {
    quantum_parsed_command_t* commands;
    int command_count;
    bool background;
    uint32_t job_id;
} quantum_command_pipeline_t;

// Shell Session
typedef struct {
    uint32_t session_id;
    quantum_shell_type_t shell_type;
    uint32_t process_id;
    uint32_t user_id;
    uint32_t group_id;
    bool active;
    bool interactive;
    bool login_shell;
    bool quantum_extensions_enabled;
    bool exact_arithmetic_enabled;
    
    // Working state
    char working_directory[MAX_PATH_LENGTH];
    char home_directory[MAX_PATH_LENGTH];
    char prompt[MAX_PROMPT_LENGTH];
    uint32_t exit_code;
    
    // Environment
    quantum_env_var_t environment_vars[MAX_ENVIRONMENT_VARS];
    int env_var_count;
    
    // Aliases and functions
    quantum_shell_alias_t aliases[MAX_ALIASES];
    int alias_count;
    quantum_shell_function_t functions[MAX_FUNCTIONS];
    int function_count;
    
    // History
    quantum_history_entry_t history[MAX_HISTORY_ENTRIES];
    int history_count;
    int history_index;
    
    // Job control
    quantum_job_t jobs[256];
    int job_count;
    uint32_t next_job_id;
    
    // Completion
    quantum_completion_entry_t completions[MAX_COMPLETION_ENTRIES];
    int completion_count;
    
    // Terminal settings
    int terminal_fd;
    bool echo_enabled;
    bool raw_mode;
    
    // Private data
    void* private_data;
} quantum_shell_session_t;

// Shell Performance Metrics
typedef struct {
    uint64_t commands_executed;
    uint64_t quantum_operations_performed;
    uint64_t exact_calculations_performed;
    uint64_t history_lookups;
    uint64_t completions_generated;
    uint64_t jobs_created;
    uint64_t pipes_created;
    uint32_t active_sessions;
    uint32_t active_jobs;
    float average_command_time_ms;
} quantum_shell_metrics_t;

// Shell Subsystem State
typedef struct {
    uint32_t version;
    uint32_t capabilities;
    bool initialized;
    uint32_t session_count;
    uint32_t active_jobs;
    quantum_shell_metrics_t metrics;
} quantum_shell_subsystem_t;

// Quantum Number Shell Extensions
typedef struct {
    quantum_number_t result;
    char expression[1024];
    bool exact_mode;
} quantum_arithmetic_context_t;

// Function Declarations

// Subsystem Management
int quantum_shell_init(void);
int quantum_shell_shutdown(void);
int quantum_shell_get_status(quantum_shell_subsystem_t* status);
int quantum_shell_get_metrics(quantum_shell_metrics_t* metrics);

// Session Management
int quantum_shell_create_session(quantum_shell_config_t* config, 
                                uint32_t* session_id);
int quantum_shell_destroy_session(uint32_t session_id);
int quantum_shell_get_session(uint32_t session_id, 
                             quantum_shell_session_t** session);
int quantum_shell_set_working_directory(uint32_t session_id, 
                                       const char* directory);
int quantum_shell_get_working_directory(uint32_t session_id, 
                                       char* directory, 
                                       size_t directory_size);

// Command Execution
int quantum_shell_execute_command(uint32_t session_id, 
                                 const char* command_line);
int quantum_shell_execute_pipeline(uint32_t session_id, 
                                  quantum_command_pipeline_t* pipeline);
int quantum_shell_parse_command(const char* command_line, 
                               quantum_command_pipeline_t* pipeline);
int quantum_shell_free_pipeline(quantum_command_pipeline_t* pipeline);

// Built-in Commands
int quantum_shell_builtin_cd(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_pwd(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_echo(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_export(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_unset(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_alias(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_unalias(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_history(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_jobs(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_fg(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_bg(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_kill(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_exit(uint32_t session_id, int argc, char** argv);

// Quantum Number Extensions
int quantum_shell_builtin_qcalc(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_qset(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_qget(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_qprecision(uint32_t session_id, int argc, char** argv);
int quantum_shell_evaluate_quantum_expression(uint32_t session_id,
                                             const char* expression,
                                             quantum_number_t* result);

// Environment Variables
int quantum_shell_set_env_var(uint32_t session_id, 
                              const char* name, 
                              const char* value,
                              bool exported);
int quantum_shell_get_env_var(uint32_t session_id, 
                              const char* name, 
                              char* value, 
                              size_t value_size);
int quantum_shell_unset_env_var(uint32_t session_id, const char* name);
int quantum_shell_set_quantum_var(uint32_t session_id,
                                 const char* name,
                                 quantum_number_t* value,
                                 bool exported);
int quantum_shell_get_quantum_var(uint32_t session_id,
                                 const char* name,
                                 quantum_number_t* value);

// Aliases and Functions
int quantum_shell_set_alias(uint32_t session_id, 
                           const char* name, 
                           const char* expansion);
int quantum_shell_get_alias(uint32_t session_id, 
                           const char* name, 
                           char* expansion, 
                           size_t expansion_size);
int quantum_shell_unset_alias(uint32_t session_id, const char* name);
int quantum_shell_set_function(uint32_t session_id,
                              const char* name,
                              const char* body);
int quantum_shell_get_function(uint32_t session_id,
                              const char* name,
                              char** body,
                              size_t* body_size);
int quantum_shell_unset_function(uint32_t session_id, const char* name);

// History Management
int quantum_shell_add_history(uint32_t session_id, 
                             const char* command,
                             uint32_t exit_code,
                             uint64_t execution_time_ns);
int quantum_shell_get_history(uint32_t session_id,
                             quantum_history_entry_t* entries,
                             int max_entries,
                             int* entry_count);
int quantum_shell_search_history(uint32_t session_id,
                                const char* pattern,
                                quantum_history_entry_t* entries,
                                int max_entries,
                                int* entry_count);
int quantum_shell_clear_history(uint32_t session_id);

// Job Control
int quantum_shell_create_job(uint32_t session_id,
                            const char* command,
                            uint32_t process_group_id,
                            bool background,
                            uint32_t* job_id);
int quantum_shell_get_job(uint32_t session_id,
                         uint32_t job_id,
                         quantum_job_t* job);
int quantum_shell_set_job_state(uint32_t session_id,
                               uint32_t job_id,
                               quantum_job_state_t state);
int quantum_shell_bring_job_foreground(uint32_t session_id, uint32_t job_id);
int quantum_shell_send_job_background(uint32_t session_id, uint32_t job_id);
int quantum_shell_kill_job(uint32_t session_id, uint32_t job_id, int signal);

// Command Completion
int quantum_shell_generate_completions(uint32_t session_id,
                                      const char* partial_command,
                                      quantum_completion_entry_t* completions,
                                      int max_completions,
                                      int* completion_count);
int quantum_shell_add_completion(uint32_t session_id,
                                const char* completion,
                                const char* description,
                                quantum_command_type_t command_type);
int quantum_shell_complete_filename(const char* partial_path,
                                   char** completions,
                                   int max_completions,
                                   int* completion_count);
int quantum_shell_complete_command(uint32_t session_id,
                                  const char* partial_command,
                                  char** completions,
                                  int max_completions,
                                  int* completion_count);

// Terminal Interface
int quantum_shell_set_prompt(uint32_t session_id, const char* prompt);
int quantum_shell_get_prompt(uint32_t session_id, 
                            char* prompt, 
                            size_t prompt_size);
int quantum_shell_read_line(uint32_t session_id, 
                           char* buffer, 
                           size_t buffer_size);
int quantum_shell_write_output(uint32_t session_id, 
                              const char* output, 
                              size_t output_size);
int quantum_shell_write_error(uint32_t session_id, 
                             const char* error, 
                             size_t error_size);

// Gaming Integration
int quantum_shell_register_gaming_commands(void);
int quantum_shell_builtin_qgame(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_qgpu(uint32_t session_id, int argc, char** argv);
int quantum_shell_builtin_qperf(uint32_t session_id, int argc, char** argv);

// POSIX Compatibility
int quantum_shell_posix_glob(const char* pattern, 
                            char** matches, 
                            int max_matches, 
                            int* match_count);
int quantum_shell_posix_expand_variables(uint32_t session_id,
                                        const char* input,
                                        char* output,
                                        size_t output_size);
int quantum_shell_posix_quote_removal(const char* input,
                                     char* output,
                                     size_t output_size);

// Signal Handling
int quantum_shell_setup_signal_handlers(uint32_t session_id);
int quantum_shell_handle_signal(uint32_t session_id, int signal);
int quantum_shell_ignore_signal(int signal);
int quantum_shell_default_signal(int signal);

// Debug and Profiling
int quantum_shell_debug_enable(bool enabled);
int quantum_shell_debug_trace_command(uint32_t session_id, 
                                     const char* command);
int quantum_shell_profile_begin(uint32_t session_id);
int quantum_shell_profile_end(uint32_t session_id, 
                             quantum_shell_metrics_t* metrics);

#endif // QUANTUM_SHELL_H
