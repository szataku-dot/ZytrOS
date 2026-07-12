#pragma once
#include <cstdint>

extern const char* CMD_TEXT_WHITE;
extern const char* CMD_TEXT_RED;
extern const char* CMD_TEXT_GREEN;
extern const char* CMD_TEXT_BLUE;
extern const char* CMD_TEXT_YELLOW;
extern const char* CMD_TEXT_GRAY;

// --- PALETA KOLORÓW ---

// ===== BASIC =====
inline constexpr uint32_t COLOR_BLACK               = 0xFF000000;
inline constexpr uint32_t COLOR_WHITE               = 0xFFFFFFFF;
inline constexpr uint32_t COLOR_RED                 = 0xFFFF5555;
inline constexpr uint32_t COLOR_GREEN               = 0xFF59E81B;
inline constexpr uint32_t COLOR_BLUE                = 0xFF59AEED;
inline constexpr uint32_t COLOR_YELLOW              = 0xFFFFD966;
inline constexpr uint32_t COLOR_GRAY                = 0xFFAAAAAA;
inline constexpr uint32_t COLOR_DARK_GRAY           = 0xFF555555;
inline constexpr uint32_t COLOR_LIGHT_GRAY          = 0xFFCCCCCC;

// ===== NasuaOS UI CORE =====
inline constexpr uint32_t COLOR_NASUA_BG            = 0xFF121826;  // ciemny granat
inline constexpr uint32_t COLOR_NASUA_TASKBAR       = 0xFF1F2937;  // grafit
inline constexpr uint32_t COLOR_NASUA_START_MENU    = 0xFF243142;  // grafit
inline constexpr uint32_t COLOR_NASUA_DARK          = 0xFF808080; // cienie 3D
inline constexpr uint32_t COLOR_NASUA_DARKER        = 0xFF404040;
inline constexpr uint32_t COLOR_NASUA_LIGHT         = 0xFFE0E0E0; // highlight
inline constexpr uint32_t COLOR_NASUA_HIGHLIGHT     = 0xFFFFFFFF;
inline constexpr uint32_t COLOR_MOUSE               = 0xFFFFFFFF;

// ===== WINDOW ELEMENTS =====
inline constexpr uint32_t COLOR_TITLEBAR            = 0xFF4A4A4A; // klasyczny niebieski
inline constexpr uint32_t COLOR_TITLEBAR_INACTIVE   = 0xFF6D6D6D;

inline constexpr uint32_t COLOR_WINDOW              = 0xFFC0C0C0;
inline constexpr uint32_t COLOR_BORDER              = 0xFF000000;

inline constexpr uint32_t COLOR_BUTTON_FACE         = 0xFFC0C0C0;
inline constexpr uint32_t COLOR_BUTTON_SHADOW       = 0xFF808080;
inline constexpr uint32_t COLOR_BUTTON_HL           = 0xFFFFFFFF;

// ===== TERMINAL COLORS =====
inline constexpr uint32_t COLOR_TERM_BG             = 0xFF000000;
inline constexpr uint32_t COLOR_TERM_FG             = 0xFF00FF00; // zielony matrix
inline constexpr uint32_t COLOR_TERM_WARN           = 0xFFFFFF00;
inline constexpr uint32_t COLOR_TERM_ERROR          = 0xFFFF4444;
inline constexpr uint32_t COLOR_TERM_INFO           = 0xFF59AEED;

// ===== SHADOWS / DEPTH =====
inline constexpr uint32_t COLOR_SHADOW              = 0xAA000000;
inline constexpr uint32_t COLOR_HARD_SHADOW         = 0xFF202020;