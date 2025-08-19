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
 * Quantum Number Office Applications Kernel Module
 * 
 * This header defines the office applications subsystem for the Quantum OS,
 * providing hardware acceleration for document processing, spreadsheet
 * calculations, presentation rendering, and other office productivity tasks
 * using exact mathematical precision with QNGPU/g256 integration.
 *
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_OFFICE_H
#define QUANTUM_OFFICE_H

#include "quantum_number.h"
#include "quantum_qngpu_driver.h"
#include "quantum_graphics.h"
#include "quantum_gaming.h"
#include <stdint.h>
#include <stdbool.h>

// Office Subsystem Version
#define QUANTUM_OFFICE_VERSION_MAJOR 1
#define QUANTUM_OFFICE_VERSION_MINOR 0
#define QUANTUM_OFFICE_VERSION_PATCH 0
#define QUANTUM_OFFICE_VERSION ((QUANTUM_OFFICE_VERSION_MAJOR << 16) | \
                               (QUANTUM_OFFICE_VERSION_MINOR << 8) | \
                               QUANTUM_OFFICE_VERSION_PATCH)

// Maximum Limits
#define MAX_OFFICE_PROCESSES 128
#define MAX_DOCUMENTS 512
#define MAX_SPREADSHEETS 256
#define MAX_PRESENTATIONS 128
#define MAX_DOCUMENT_PAGES 10000
#define MAX_SPREADSHEET_CELLS 1000000
#define MAX_PRESENTATION_SLIDES 1000
#define MAX_FONTS 256
#define MAX_TEMPLATES 128
#define MAX_FORMULAS 10000

// Office Capabilities
#define QUANTUM_OFFICE_CAP_DOCUMENT_PROCESSING   (1 << 0)
#define QUANTUM_OFFICE_CAP_SPREADSHEET_CALC      (1 << 1)
#define QUANTUM_OFFICE_CAP_PRESENTATION_RENDER   (1 << 2)
#define QUANTUM_OFFICE_CAP_PDF_GENERATION        (1 << 3)
#define QUANTUM_OFFICE_CAP_EXACT_CALCULATIONS    (1 << 4)
#define QUANTUM_OFFICE_CAP_REAL_TIME_COLLAB      (1 << 5)
#define QUANTUM_OFFICE_CAP_ADVANCED_TYPOGRAPHY   (1 << 6)
#define QUANTUM_OFFICE_CAP_VECTOR_GRAPHICS       (1 << 7)
#define QUANTUM_OFFICE_CAP_CHART_GENERATION      (1 << 8)
#define QUANTUM_OFFICE_CAP_FORMULA_ACCELERATION  (1 << 9)

// Error Codes
#define QUANTUM_OFFICE_SUCCESS                   0
#define QUANTUM_OFFICE_ERROR_NOT_INITIALIZED    -1
#define QUANTUM_OFFICE_ERROR_ALREADY_INITIALIZED -2
#define QUANTUM_OFFICE_ERROR_INVALID_PARAMETER  -3
#define QUANTUM_OFFICE_ERROR_OUT_OF_MEMORY      -4
#define QUANTUM_OFFICE_ERROR_DOCUMENT_NOT_FOUND -5
#define QUANTUM_OFFICE_ERROR_PROCESS_NOT_FOUND  -6
#define QUANTUM_OFFICE_ERROR_OPERATION_FAILED   -7
#define QUANTUM_OFFICE_ERROR_FORMAT_UNSUPPORTED -8
#define QUANTUM_OFFICE_ERROR_CALCULATION_ERROR  -9
#define QUANTUM_OFFICE_ERROR_RENDERING_FAILED   -10

// Office Application Types
typedef enum {
    QUANTUM_OFFICE_APP_WORD_PROCESSOR,
    QUANTUM_OFFICE_APP_SPREADSHEET,
    QUANTUM_OFFICE_APP_PRESENTATION,
    QUANTUM_OFFICE_APP_PDF_VIEWER,
    QUANTUM_OFFICE_APP_DIAGRAM_EDITOR,
    QUANTUM_OFFICE_APP_FORMULA_EDITOR,
    QUANTUM_OFFICE_APP_CHART_GENERATOR
} quantum_office_app_type_t;

// Document Types
typedef enum {
    QUANTUM_DOCUMENT_TYPE_TEXT,
    QUANTUM_DOCUMENT_TYPE_RICH_TEXT,
    QUANTUM_DOCUMENT_TYPE_MARKDOWN,
    QUANTUM_DOCUMENT_TYPE_PDF,
    QUANTUM_DOCUMENT_TYPE_SPREADSHEET,
    QUANTUM_DOCUMENT_TYPE_PRESENTATION,
    QUANTUM_DOCUMENT_TYPE_DIAGRAM
} quantum_document_type_t;

// Calculation Types
typedef enum {
    QUANTUM_CALC_TYPE_ARITHMETIC,
    QUANTUM_CALC_TYPE_STATISTICAL,
    QUANTUM_CALC_TYPE_FINANCIAL,
    QUANTUM_CALC_TYPE_ENGINEERING,
    QUANTUM_CALC_TYPE_SCIENTIFIC,
    QUANTUM_CALC_TYPE_EXACT_PRECISION
} quantum_calc_type_t;

// Rendering Modes
typedef enum {
    QUANTUM_RENDER_MODE_DRAFT,
    QUANTUM_RENDER_MODE_NORMAL,
    QUANTUM_RENDER_MODE_HIGH_QUALITY,
    QUANTUM_RENDER_MODE_EXACT_PRECISION,
    QUANTUM_RENDER_MODE_PRINT_PREVIEW
} quantum_office_render_mode_t;

// Font Types
typedef enum {
    QUANTUM_FONT_TYPE_SERIF,
    QUANTUM_FONT_TYPE_SANS_SERIF,
    QUANTUM_FONT_TYPE_MONOSPACE,
    QUANTUM_FONT_TYPE_SCRIPT,
    QUANTUM_FONT_TYPE_DECORATIVE,
    QUANTUM_FONT_TYPE_MATHEMATICAL
} quantum_font_type_t;

// Chart Types
typedef enum {
    QUANTUM_CHART_TYPE_LINE,
    QUANTUM_CHART_TYPE_BAR,
    QUANTUM_CHART_TYPE_PIE,
    QUANTUM_CHART_TYPE_SCATTER,
    QUANTUM_CHART_TYPE_AREA,
    QUANTUM_CHART_TYPE_HISTOGRAM,
    QUANTUM_CHART_TYPE_BOX_PLOT,
    QUANTUM_CHART_TYPE_3D_SURFACE
} quantum_chart_type_t;

// Office Process Configuration
typedef struct {
    quantum_office_app_type_t app_type;
    uint32_t process_id;
    uint32_t user_id;
    bool hardware_acceleration_enabled;
    bool exact_calculations_enabled;
    bool real_time_collaboration_enabled;
    quantum_office_render_mode_t default_render_mode;
    uint32_t max_memory_mb;
    uint32_t max_cpu_cores;
} quantum_office_process_config_t;

// Document Configuration
typedef struct {
    quantum_document_type_t document_type;
    uint32_t width;
    uint32_t height;
    uint32_t dpi;
    bool exact_positioning;
    bool hardware_rendering;
    quantum_color_space_t color_space;
    char template_name[256];
} quantum_document_config_t;

// Spreadsheet Cell
typedef struct {
    uint32_t row;
    uint32_t column;
    quantum_number_t value;
    char* formula;
    char* display_text;
    uint32_t format_id;
    bool has_formula;
    bool exact_precision;
} quantum_spreadsheet_cell_t;

// Document Page
typedef struct {
    uint32_t page_number;
    uint32_t width;
    uint32_t height;
    void* content_data;
    size_t content_size;
    bool rendered;
    uint32_t render_context_id;
} quantum_document_page_t;

// Presentation Slide
typedef struct {
    uint32_t slide_number;
    uint32_t width;
    uint32_t height;
    void* slide_data;
    size_t slide_size;
    uint32_t animation_count;
    bool has_transitions;
    uint32_t render_context_id;
} quantum_presentation_slide_t;

// Font Information
typedef struct {
    uint32_t font_id;
    char font_name[256];
    char font_family[128];
    quantum_font_type_t font_type;
    uint32_t font_size;
    bool bold;
    bool italic;
    bool underline;
    bool exact_metrics;
    void* font_data;
    size_t font_data_size;
} quantum_office_font_t;

// Formula Context
typedef struct {
    char formula[1024];
    quantum_number_t result;
    quantum_calc_type_t calculation_type;
    bool exact_precision;
    uint32_t dependency_count;
    uint32_t* dependencies; // Cell references
    bool needs_recalculation;
} quantum_formula_context_t;

// Chart Data Point
typedef struct {
    quantum_number_t x_value;
    quantum_number_t y_value;
    quantum_number_t z_value; // For 3D charts
    char label[256];
    quantum_color_exact_t color;
} quantum_chart_data_point_t;

// Chart Configuration
typedef struct {
    quantum_chart_type_t chart_type;
    uint32_t width;
    uint32_t height;
    char title[256];
    char x_axis_label[128];
    char y_axis_label[128];
    char z_axis_label[128]; // For 3D charts
    bool exact_scaling;
    bool hardware_rendering;
    quantum_color_exact_t background_color;
    uint32_t data_point_count;
    quantum_chart_data_point_t* data_points;
} quantum_chart_config_t;

// Office Document Handle
typedef struct {
    uint32_t document_id;
    uint32_t process_id;
    quantum_document_type_t document_type;
    char filename[512];
    bool modified;
    bool read_only;
    uint32_t page_count;
    quantum_document_page_t* pages;
    uint32_t render_context_id;
    void* private_data;
} quantum_office_document_t;

// Spreadsheet Handle
typedef struct {
    uint32_t spreadsheet_id;
    uint32_t process_id;
    char filename[512];
    bool modified;
    uint32_t row_count;
    uint32_t column_count;
    quantum_spreadsheet_cell_t* cells;
    quantum_formula_context_t* formulas;
    uint32_t formula_count;
    bool exact_calculations;
    void* private_data;
} quantum_office_spreadsheet_t;

// Presentation Handle
typedef struct {
    uint32_t presentation_id;
    uint32_t process_id;
    char filename[512];
    bool modified;
    uint32_t slide_count;
    quantum_presentation_slide_t* slides;
    uint32_t current_slide;
    bool slideshow_mode;
    uint32_t render_context_id;
    void* private_data;
} quantum_office_presentation_t;

// Office Process Handle
typedef struct {
    uint32_t process_id;
    quantum_office_app_type_t app_type;
    bool active;
    bool hardware_acceleration_enabled;
    bool exact_calculations_enabled;
    uint32_t document_count;
    uint32_t* document_ids;
    uint32_t qngpu_context_id;
    uint32_t graphics_context_id;
    void* private_data;
} quantum_office_process_t;

// Office Performance Metrics
typedef struct {
    uint64_t documents_processed;
    uint64_t pages_rendered;
    uint64_t formulas_calculated;
    uint64_t charts_generated;
    uint64_t exact_calculations_performed;
    uint64_t hardware_operations;
    uint32_t active_processes;
    uint32_t active_documents;
    float average_render_time_ms;
    float average_calculation_time_ms;
} quantum_office_metrics_t;

// Office Subsystem State
typedef struct {
    uint32_t version;
    uint32_t capabilities;
    bool initialized;
    uint32_t process_count;
    uint32_t document_count;
    uint32_t spreadsheet_count;
    uint32_t presentation_count;
    quantum_office_metrics_t metrics;
} quantum_office_subsystem_t;

// Function Declarations

// Subsystem Management
int quantum_office_init(void);
int quantum_office_shutdown(void);
int quantum_office_get_status(quantum_office_subsystem_t* status);
int quantum_office_get_metrics(quantum_office_metrics_t* metrics);

// Process Management
int quantum_office_register_process(quantum_office_process_config_t* config);
int quantum_office_unregister_process(uint32_t process_id);
int quantum_office_get_process(uint32_t process_id, 
                              quantum_office_process_t** process);
int quantum_office_enable_hardware_acceleration(uint32_t process_id, 
                                               bool enabled);
int quantum_office_enable_exact_calculations(uint32_t process_id, 
                                            bool enabled);

// Document Management
int quantum_office_create_document(uint32_t process_id,
                                  quantum_document_config_t* config,
                                  uint32_t* document_id);
int quantum_office_open_document(uint32_t process_id,
                                const char* filename,
                                uint32_t* document_id);
int quantum_office_save_document(uint32_t document_id, 
                                const char* filename);
int quantum_office_close_document(uint32_t document_id);
int quantum_office_get_document(uint32_t document_id,
                               quantum_office_document_t** document);

// Document Rendering
int quantum_office_render_page(uint32_t document_id,
                              uint32_t page_number,
                              quantum_office_render_mode_t render_mode);
int quantum_office_render_page_to_texture(uint32_t document_id,
                                         uint32_t page_number,
                                         uint32_t* texture_id);
int quantum_office_export_to_pdf(uint32_t document_id,
                                const char* pdf_filename);
int quantum_office_print_document(uint32_t document_id,
                                 const char* printer_name);

// Text Processing
int quantum_office_insert_text(uint32_t document_id,
                              uint32_t page_number,
                              quantum_point_exact_t* position,
                              const char* text,
                              uint32_t font_id);
int quantum_office_format_text(uint32_t document_id,
                              uint32_t page_number,
                              quantum_rect_exact_t* selection,
                              uint32_t font_id,
                              quantum_color_exact_t* color);
int quantum_office_find_replace_text(uint32_t document_id,
                                    const char* find_text,
                                    const char* replace_text,
                                    bool case_sensitive,
                                    uint32_t* replacement_count);

// Spreadsheet Management
int quantum_office_create_spreadsheet(uint32_t process_id,
                                     uint32_t rows,
                                     uint32_t columns,
                                     uint32_t* spreadsheet_id);
int quantum_office_open_spreadsheet(uint32_t process_id,
                                   const char* filename,
                                   uint32_t* spreadsheet_id);
int quantum_office_save_spreadsheet(uint32_t spreadsheet_id,
                                   const char* filename);
int quantum_office_close_spreadsheet(uint32_t spreadsheet_id);

// Spreadsheet Operations
int quantum_office_set_cell_value(uint32_t spreadsheet_id,
                                 uint32_t row,
                                 uint32_t column,
                                 quantum_number_t* value);
int quantum_office_get_cell_value(uint32_t spreadsheet_id,
                                 uint32_t row,
                                 uint32_t column,
                                 quantum_number_t* value);
int quantum_office_set_cell_formula(uint32_t spreadsheet_id,
                                   uint32_t row,
                                   uint32_t column,
                                   const char* formula);
int quantum_office_get_cell_formula(uint32_t spreadsheet_id,
                                   uint32_t row,
                                   uint32_t column,
                                   char* formula,
                                   size_t formula_size);
int quantum_office_recalculate_spreadsheet(uint32_t spreadsheet_id);
int quantum_office_recalculate_cell(uint32_t spreadsheet_id,
                                   uint32_t row,
                                   uint32_t column);

// Formula Processing
int quantum_office_evaluate_formula(const char* formula,
                                   quantum_spreadsheet_cell_t* cells,
                                   uint32_t cell_count,
                                   quantum_number_t* result);
int quantum_office_parse_formula(const char* formula,
                                quantum_formula_context_t* context);
int quantum_office_register_formula_function(const char* function_name,
                                            void* function_ptr);

// Chart Generation
int quantum_office_create_chart(quantum_chart_config_t* config,
                               uint32_t* chart_id);
int quantum_office_render_chart(uint32_t chart_id,
                               uint32_t* texture_id);
int quantum_office_update_chart_data(uint32_t chart_id,
                                    quantum_chart_data_point_t* data_points,
                                    uint32_t data_point_count);
int quantum_office_export_chart(uint32_t chart_id,
                               const char* filename,
                               const char* format);

// Presentation Management
int quantum_office_create_presentation(uint32_t process_id,
                                      uint32_t width,
                                      uint32_t height,
                                      uint32_t* presentation_id);
int quantum_office_open_presentation(uint32_t process_id,
                                    const char* filename,
                                    uint32_t* presentation_id);
int quantum_office_save_presentation(uint32_t presentation_id,
                                    const char* filename);
int quantum_office_close_presentation(uint32_t presentation_id);

// Presentation Operations
int quantum_office_add_slide(uint32_t presentation_id,
                            uint32_t slide_number,
                            const char* template_name);
int quantum_office_delete_slide(uint32_t presentation_id,
                               uint32_t slide_number);
int quantum_office_render_slide(uint32_t presentation_id,
                               uint32_t slide_number,
                               uint32_t* texture_id);
int quantum_office_start_slideshow(uint32_t presentation_id);
int quantum_office_stop_slideshow(uint32_t presentation_id);
int quantum_office_next_slide(uint32_t presentation_id);
int quantum_office_previous_slide(uint32_t presentation_id);

// Font Management
int quantum_office_load_font(const char* font_path,
                           uint32_t font_size,
                           quantum_font_type_t font_type,
                           uint32_t* font_id);
int quantum_office_unload_font(uint32_t font_id);
int quantum_office_get_font_metrics(uint32_t font_id,
                                   const char* text,
                                   quantum_rect_exact_t* bounds);
int quantum_office_render_text_exact(uint32_t font_id,
                                    const char* text,
                                    quantum_point_exact_t* position,
                                    quantum_color_exact_t* color,
                                    uint32_t* texture_id);

// Template Management
int quantum_office_load_template(const char* template_path,
                               quantum_document_type_t document_type,
                               uint32_t* template_id);
int quantum_office_unload_template(uint32_t template_id);
int quantum_office_apply_template(uint32_t document_id,
                                 uint32_t template_id);

// Collaboration Features
int quantum_office_enable_collaboration(uint32_t document_id,
                                       const char* session_id);
int quantum_office_disable_collaboration(uint32_t document_id);
int quantum_office_sync_changes(uint32_t document_id);
int quantum_office_get_collaborators(uint32_t document_id,
                                    char** collaborator_list,
                                    uint32_t* collaborator_count);

// Import/Export
int quantum_office_import_document(const char* filename,
                                  quantum_document_type_t source_type,
                                  quantum_document_type_t target_type,
                                  uint32_t* document_id);
int quantum_office_export_document(uint32_t document_id,
                                  const char* filename,
                                  quantum_document_type_t target_type);

// Hardware Acceleration
int quantum_office_accelerate_calculation(quantum_formula_context_t* formula,
                                         quantum_number_t* result);
int quantum_office_accelerate_rendering(uint32_t document_id,
                                       uint32_t page_number);
int quantum_office_accelerate_chart_generation(uint32_t chart_id);

// Debug and Profiling
int quantum_office_debug_enable(bool enabled);
int quantum_office_debug_dump_document(uint32_t document_id,
                                      const char* dump_filename);
int quantum_office_profile_begin(uint32_t process_id);
int quantum_office_profile_end(uint32_t process_id,
                              quantum_office_metrics_t* metrics);

#endif // QUANTUM_OFFICE_H
