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
 * Quantum Graphics Subsystem Kernel Module
 * 
 * This header defines the graphics subsystem for the Quantum OS, providing
 * comprehensive graphics management, window system integration, and exact
 * mathematical rendering capabilities with QNGPU/g256 integration.
 *
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_GRAPHICS_H
#define QUANTUM_GRAPHICS_H

#include "quantum_number.h"
#include "quantum_qngpu_driver.h"
#include "quantum_gaming.h"
#include <stdint.h>
#include <stdbool.h>

// Graphics Subsystem Version
#define QUANTUM_GRAPHICS_VERSION_MAJOR 1
#define QUANTUM_GRAPHICS_VERSION_MINOR 0
#define QUANTUM_GRAPHICS_VERSION_PATCH 0
#define QUANTUM_GRAPHICS_VERSION ((QUANTUM_GRAPHICS_VERSION_MAJOR << 16) | \
                                 (QUANTUM_GRAPHICS_VERSION_MINOR << 8) | \
                                 QUANTUM_GRAPHICS_VERSION_PATCH)

// Maximum Limits
#define MAX_GRAPHICS_CONTEXTS 256
#define MAX_WINDOWS 1024
#define MAX_DISPLAYS 16
#define MAX_GRAPHICS_PROCESSES 512
#define MAX_FONT_CACHE_SIZE 1024
#define MAX_CURSOR_CACHE_SIZE 64
#define MAX_ICON_CACHE_SIZE 512

// Graphics Capabilities
#define QUANTUM_GRAPHICS_CAP_EXACT_RENDERING     (1 << 0)
#define QUANTUM_GRAPHICS_CAP_WINDOW_MANAGER      (1 << 1)
#define QUANTUM_GRAPHICS_CAP_COMPOSITING         (1 << 2)
#define QUANTUM_GRAPHICS_CAP_MULTI_DISPLAY       (1 << 3)
#define QUANTUM_GRAPHICS_CAP_HDR_SUPPORT         (1 << 4)
#define QUANTUM_GRAPHICS_CAP_VR_INTEGRATION      (1 << 5)
#define QUANTUM_GRAPHICS_CAP_GAMING_OPTIMIZATION (1 << 6)
#define QUANTUM_GRAPHICS_CAP_OFFICE_ACCELERATION (1 << 7)
#define QUANTUM_GRAPHICS_CAP_VIDEO_DECODE        (1 << 8)
#define QUANTUM_GRAPHICS_CAP_VIDEO_ENCODE        (1 << 9)

// Error Codes
#define QUANTUM_GRAPHICS_SUCCESS                 0
#define QUANTUM_GRAPHICS_ERROR_NOT_INITIALIZED  -1
#define QUANTUM_GRAPHICS_ERROR_ALREADY_INITIALIZED -2
#define QUANTUM_GRAPHICS_ERROR_INVALID_PARAMETER -3
#define QUANTUM_GRAPHICS_ERROR_OUT_OF_MEMORY     -4
#define QUANTUM_GRAPHICS_ERROR_CONTEXT_NOT_FOUND -5
#define QUANTUM_GRAPHICS_ERROR_WINDOW_NOT_FOUND  -6
#define QUANTUM_GRAPHICS_ERROR_DISPLAY_NOT_FOUND -7
#define QUANTUM_GRAPHICS_ERROR_OPERATION_FAILED  -8
#define QUANTUM_GRAPHICS_ERROR_QNGPU_FAILURE     -9
#define QUANTUM_GRAPHICS_ERROR_RESOURCE_BUSY     -10

// Display Types
typedef enum {
    QUANTUM_DISPLAY_TYPE_LCD,
    QUANTUM_DISPLAY_TYPE_OLED,
    QUANTUM_DISPLAY_TYPE_EINK,
    QUANTUM_DISPLAY_TYPE_VR_HMD,
    QUANTUM_DISPLAY_TYPE_PROJECTOR,
    QUANTUM_DISPLAY_TYPE_VIRTUAL
} quantum_display_type_t;

// Window Types
typedef enum {
    QUANTUM_WINDOW_TYPE_NORMAL,
    QUANTUM_WINDOW_TYPE_DIALOG,
    QUANTUM_WINDOW_TYPE_POPUP,
    QUANTUM_WINDOW_TYPE_TOOLTIP,
    QUANTUM_WINDOW_TYPE_FULLSCREEN,
    QUANTUM_WINDOW_TYPE_GAMING,
    QUANTUM_WINDOW_TYPE_VR,
    QUANTUM_WINDOW_TYPE_OFFICE
} quantum_window_type_t;

// Rendering Modes
typedef enum {
    QUANTUM_RENDER_MODE_EXACT,
    QUANTUM_RENDER_MODE_FAST,
    QUANTUM_RENDER_MODE_HYBRID,
    QUANTUM_RENDER_MODE_GAMING,
    QUANTUM_RENDER_MODE_OFFICE,
    QUANTUM_RENDER_MODE_VR
} quantum_render_mode_t;

// Color Spaces
typedef enum {
    QUANTUM_COLOR_SPACE_SRGB,
    QUANTUM_COLOR_SPACE_ADOBE_RGB,
    QUANTUM_COLOR_SPACE_REC2020,
    QUANTUM_COLOR_SPACE_DCI_P3,
    QUANTUM_COLOR_SPACE_QUANTUM_EXACT
} quantum_color_space_t;

// Font Rendering Modes
typedef enum {
    QUANTUM_FONT_RENDER_BITMAP,
    QUANTUM_FONT_RENDER_VECTOR,
    QUANTUM_FONT_RENDER_EXACT,
    QUANTUM_FONT_RENDER_SUBPIXEL
} quantum_font_render_mode_t;

// Display Information
typedef struct {
    uint32_t display_id;
    quantum_display_type_t display_type;
    uint32_t width;
    uint32_t height;
    uint32_t refresh_rate;
    uint32_t bit_depth;
    quantum_color_space_t color_space;
    bool hdr_supported;
    bool exact_precision_supported;
    char display_name[256];
    char manufacturer[128];
    char model[128];
} quantum_display_info_t;

// Window Configuration
typedef struct {
    quantum_window_type_t window_type;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t display_id;
    bool resizable;
    bool decorated;
    bool always_on_top;
    bool transparent;
    bool exact_rendering;
    quantum_render_mode_t render_mode;
    char title[256];
} quantum_window_config_t;

// Window Handle
typedef struct {
    uint32_t window_id;
    uint32_t process_id;
    quantum_window_type_t window_type;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t display_id;
    uint32_t qngpu_context_id;
    bool visible;
    bool focused;
    bool minimized;
    bool maximized;
    bool fullscreen;
    bool exact_rendering;
    quantum_render_mode_t render_mode;
    void* framebuffer;
    size_t framebuffer_size;
    char title[256];
} quantum_window_t;

// Graphics Context
typedef struct {
    uint32_t context_id;
    uint32_t window_id;
    uint32_t qngpu_context_id;
    quantum_render_mode_t render_mode;
    bool exact_precision_enabled;
    quantum_color_space_t color_space;
    uint32_t width;
    uint32_t height;
    void* private_data;
} quantum_graphics_context_t;

// Color with Exact Precision
typedef struct {
    quantum_number_t red;
    quantum_number_t green;
    quantum_number_t blue;
    quantum_number_t alpha;
} quantum_color_exact_t;

// Point with Exact Precision
typedef struct {
    quantum_number_t x;
    quantum_number_t y;
} quantum_point_exact_t;

// Rectangle with Exact Precision
typedef struct {
    quantum_number_t x;
    quantum_number_t y;
    quantum_number_t width;
    quantum_number_t height;
} quantum_rect_exact_t;

// Font Information
typedef struct {
    uint32_t font_id;
    char font_name[256];
    char font_family[128];
    uint32_t font_size;
    bool bold;
    bool italic;
    bool underline;
    quantum_font_render_mode_t render_mode;
    bool exact_precision;
    void* font_data;
    size_t font_data_size;
} quantum_font_t;

// Cursor Information
typedef struct {
    uint32_t cursor_id;
    uint32_t width;
    uint32_t height;
    uint32_t hotspot_x;
    uint32_t hotspot_y;
    void* cursor_data;
    size_t cursor_data_size;
} quantum_cursor_t;

// Icon Information
typedef struct {
    uint32_t icon_id;
    uint32_t width;
    uint32_t height;
    void* icon_data;
    size_t icon_data_size;
} quantum_icon_t;

// Graphics Performance Metrics
typedef struct {
    uint64_t frames_rendered;
    uint64_t windows_composited;
    uint64_t exact_operations_performed;
    uint64_t text_glyphs_rendered;
    uint64_t pixels_processed;
    uint32_t current_fps;
    uint32_t average_fps;
    float gpu_utilization;
    float memory_utilization;
    uint32_t active_windows;
    uint32_t active_contexts;
} quantum_graphics_metrics_t;

// Window Manager State
typedef struct {
    uint32_t focused_window_id;
    uint32_t active_display_id;
    bool compositing_enabled;
    bool exact_rendering_enabled;
    quantum_render_mode_t default_render_mode;
    uint32_t window_count;
    uint32_t context_count;
} quantum_window_manager_state_t;

// Graphics Subsystem State
typedef struct {
    uint32_t version;
    uint32_t capabilities;
    bool initialized;
    uint32_t display_count;
    uint32_t window_count;
    uint32_t context_count;
    uint32_t process_count;
    quantum_window_manager_state_t window_manager;
    quantum_graphics_metrics_t metrics;
} quantum_graphics_subsystem_t;

// Function Declarations

// Subsystem Management
int quantum_graphics_init(void);
int quantum_graphics_shutdown(void);
int quantum_graphics_get_status(quantum_graphics_subsystem_t* status);
int quantum_graphics_get_metrics(quantum_graphics_metrics_t* metrics);

// Display Management
int quantum_graphics_enumerate_displays(quantum_display_info_t* displays, 
                                       uint32_t max_displays, 
                                       uint32_t* display_count);
int quantum_graphics_get_display_info(uint32_t display_id, 
                                     quantum_display_info_t* display_info);
int quantum_graphics_set_display_mode(uint32_t display_id, 
                                     uint32_t width, 
                                     uint32_t height, 
                                     uint32_t refresh_rate);
int quantum_graphics_enable_hdr(uint32_t display_id, bool enabled);

// Window Management
int quantum_graphics_create_window(quantum_window_config_t* config, 
                                  uint32_t* window_id);
int quantum_graphics_destroy_window(uint32_t window_id);
int quantum_graphics_show_window(uint32_t window_id);
int quantum_graphics_hide_window(uint32_t window_id);
int quantum_graphics_move_window(uint32_t window_id, uint32_t x, uint32_t y);
int quantum_graphics_resize_window(uint32_t window_id, 
                                  uint32_t width, 
                                  uint32_t height);
int quantum_graphics_set_window_title(uint32_t window_id, const char* title);
int quantum_graphics_focus_window(uint32_t window_id);
int quantum_graphics_minimize_window(uint32_t window_id);
int quantum_graphics_maximize_window(uint32_t window_id);
int quantum_graphics_fullscreen_window(uint32_t window_id, bool fullscreen);

// Graphics Context Management
int quantum_graphics_create_context(uint32_t window_id, 
                                   quantum_render_mode_t render_mode,
                                   uint32_t* context_id);
int quantum_graphics_destroy_context(uint32_t context_id);
int quantum_graphics_make_context_current(uint32_t context_id);
int quantum_graphics_swap_buffers(uint32_t context_id);
int quantum_graphics_set_render_mode(uint32_t context_id, 
                                    quantum_render_mode_t render_mode);

// Exact Precision Drawing
int quantum_graphics_clear_exact(uint32_t context_id, 
                                quantum_color_exact_t* clear_color);
int quantum_graphics_draw_point_exact(uint32_t context_id,
                                     quantum_point_exact_t* point,
                                     quantum_color_exact_t* color);
int quantum_graphics_draw_line_exact(uint32_t context_id,
                                    quantum_point_exact_t* start,
                                    quantum_point_exact_t* end,
                                    quantum_color_exact_t* color);
int quantum_graphics_draw_rectangle_exact(uint32_t context_id,
                                         quantum_rect_exact_t* rect,
                                         quantum_color_exact_t* color,
                                         bool filled);
int quantum_graphics_draw_circle_exact(uint32_t context_id,
                                      quantum_point_exact_t* center,
                                      quantum_number_t* radius,
                                      quantum_color_exact_t* color,
                                      bool filled);
int quantum_graphics_draw_polygon_exact(uint32_t context_id,
                                       quantum_point_exact_t* points,
                                       uint32_t point_count,
                                       quantum_color_exact_t* color,
                                       bool filled);

// Text Rendering
int quantum_graphics_load_font(const char* font_path, 
                              uint32_t font_size,
                              quantum_font_render_mode_t render_mode,
                              uint32_t* font_id);
int quantum_graphics_unload_font(uint32_t font_id);
int quantum_graphics_draw_text(uint32_t context_id,
                              uint32_t font_id,
                              const char* text,
                              quantum_point_exact_t* position,
                              quantum_color_exact_t* color);
int quantum_graphics_draw_text_exact(uint32_t context_id,
                                    uint32_t font_id,
                                    const char* text,
                                    quantum_point_exact_t* position,
                                    quantum_color_exact_t* color);
int quantum_graphics_measure_text(uint32_t font_id,
                                 const char* text,
                                 quantum_rect_exact_t* bounds);

// Image and Texture Operations
int quantum_graphics_load_image(const char* image_path, uint32_t* image_id);
int quantum_graphics_unload_image(uint32_t image_id);
int quantum_graphics_draw_image(uint32_t context_id,
                               uint32_t image_id,
                               quantum_point_exact_t* position);
int quantum_graphics_draw_image_scaled(uint32_t context_id,
                                      uint32_t image_id,
                                      quantum_rect_exact_t* dest_rect);
int quantum_graphics_create_texture_from_pixels(uint32_t width,
                                               uint32_t height,
                                               quantum_color_exact_t* pixels,
                                               uint32_t* texture_id);

// Cursor Management
int quantum_graphics_load_cursor(const char* cursor_path, 
                                uint32_t hotspot_x, 
                                uint32_t hotspot_y,
                                uint32_t* cursor_id);
int quantum_graphics_unload_cursor(uint32_t cursor_id);
int quantum_graphics_set_cursor(uint32_t window_id, uint32_t cursor_id);
int quantum_graphics_show_cursor(bool visible);

// Icon Management
int quantum_graphics_load_icon(const char* icon_path, uint32_t* icon_id);
int quantum_graphics_unload_icon(uint32_t icon_id);
int quantum_graphics_set_window_icon(uint32_t window_id, uint32_t icon_id);

// Compositing and Effects
int quantum_graphics_enable_compositing(bool enabled);
int quantum_graphics_set_window_opacity(uint32_t window_id, 
                                       quantum_number_t* opacity);
int quantum_graphics_set_window_blur(uint32_t window_id, 
                                    quantum_number_t* blur_radius);
int quantum_graphics_add_drop_shadow(uint32_t window_id,
                                    quantum_number_t* offset_x,
                                    quantum_number_t* offset_y,
                                    quantum_number_t* blur_radius,
                                    quantum_color_exact_t* shadow_color);

// Gaming Integration
int quantum_graphics_register_gaming_process(uint32_t process_id);
int quantum_graphics_unregister_gaming_process(uint32_t process_id);
int quantum_graphics_enable_gaming_mode(uint32_t window_id, bool enabled);
int quantum_graphics_set_gaming_priority(uint32_t window_id, uint32_t priority);

// Office Application Acceleration
int quantum_graphics_register_office_process(uint32_t process_id);
int quantum_graphics_unregister_office_process(uint32_t process_id);
int quantum_graphics_enable_office_acceleration(uint32_t window_id, bool enabled);
int quantum_graphics_accelerate_document_rendering(uint32_t context_id,
                                                  const void* document_data,
                                                  size_t data_size);

// VR Integration
int quantum_graphics_vr_init(uint32_t eye_width, 
                            uint32_t eye_height, 
                            uint32_t refresh_rate);
int quantum_graphics_vr_shutdown(void);
int quantum_graphics_vr_create_window(uint32_t* window_id);
int quantum_graphics_vr_render_eye(uint32_t window_id, 
                                  uint32_t eye_index,
                                  quantum_point_exact_t* head_position,
                                  quantum_point_exact_t* head_rotation);

// Video Decode/Encode
int quantum_graphics_video_decoder_init(void);
int quantum_graphics_video_decoder_shutdown(void);
int quantum_graphics_decode_frame(const void* encoded_data,
                                 size_t data_size,
                                 uint32_t* texture_id);
int quantum_graphics_encode_frame(uint32_t texture_id,
                                 void** encoded_data,
                                 size_t* data_size);

// Debug and Profiling
int quantum_graphics_debug_enable(bool enabled);
int quantum_graphics_debug_capture_frame(uint32_t context_id, 
                                        const char* filename);
int quantum_graphics_profile_begin(void);
int quantum_graphics_profile_end(quantum_graphics_metrics_t* metrics);
int quantum_graphics_validate_exact_precision(uint32_t context_id, 
                                             bool* validation_passed);

#endif // QUANTUM_GRAPHICS_H
