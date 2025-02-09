/*
 * Copyright (c) 2009-2023 Julien Nadeau Carriere <vedge@csoft.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Interface to keyboards.
 */

#include <agar/core/core.h>
#include <agar/gui/window.h>

#if defined(AG_WIDGETS) && defined(AG_DEBUG)
#include <agar/gui/box.h>
#include <agar/gui/label.h>
#include <agar/gui/tlist.h>
#endif

/* #define DEBUG_KEYBOARD */

/*
 * Agar internal keysyms
 */
static const struct {
	AG_KeySym ks;
	AG_KeyCategory cat;
	const char *_Nonnull name;
} agKeyNameTbl[] = {
	{ AG_KEY_NONE,			AG_KCAT_NONE,	  "None"	},
	{ AG_KEY_BACKSPACE,		AG_KCAT_CONTROL,  "BackSpace"	},
	{ AG_KEY_TAB,			AG_KCAT_SPACING,  "Tab"		},
	{ AG_KEY_CLEAR,			AG_KCAT_CONTROL,  "Clear"	},
	{ AG_KEY_RETURN,		AG_KCAT_RETURN,   "Return"	},
	{ AG_KEY_PAUSE,			AG_KCAT_CONTROL,  "Pause"	},
	{ AG_KEY_ESCAPE,		AG_KCAT_CONTROL,  "Escape"	},
	{ AG_KEY_SPACE,			AG_KCAT_SPACING,  "Space"	},
	{ AG_KEY_EXCLAIM,		AG_KCAT_PRINT,    "!"		},
	{ AG_KEY_QUOTEDBL,		AG_KCAT_PRINT,    "\""		},
	{ AG_KEY_HASH,			AG_KCAT_PRINT,    "#"		},
	{ AG_KEY_DOLLAR,		AG_KCAT_PRINT,    "$"		},
	{ AG_KEY_PERCENT,		AG_KCAT_PRINT,    "%"		},
	{ AG_KEY_AMPERSAND,		AG_KCAT_PRINT,    "&"		},
	{ AG_KEY_QUOTE,			AG_KCAT_PRINT,    "'"		},
	{ AG_KEY_LEFTPAREN,		AG_KCAT_PRINT,    "("		},
	{ AG_KEY_RIGHTPAREN,		AG_KCAT_PRINT,    ")"		},
	{ AG_KEY_ASTERISK,		AG_KCAT_PRINT,    "*"		},
	{ AG_KEY_PLUS,			AG_KCAT_PRINT,    "+"		},
	{ AG_KEY_COMMA,			AG_KCAT_PRINT,    ","		},
	{ AG_KEY_MINUS,			AG_KCAT_PRINT,    "-"		},
	{ AG_KEY_PERIOD,		AG_KCAT_PRINT,    "."		},
	{ AG_KEY_SLASH,			AG_KCAT_PRINT,    "/"		},
	{ AG_KEY_0,			AG_KCAT_PRINT,    "0"		},
	{ AG_KEY_1,			AG_KCAT_PRINT,    "1"		},
	{ AG_KEY_2,			AG_KCAT_PRINT,    "2"		},
	{ AG_KEY_3,			AG_KCAT_PRINT,    "3"		},
	{ AG_KEY_4,			AG_KCAT_PRINT,    "4"		},
	{ AG_KEY_5,			AG_KCAT_PRINT,    "5"		},
	{ AG_KEY_6,			AG_KCAT_PRINT,    "6"		},
	{ AG_KEY_7,			AG_KCAT_PRINT,    "7"		},
	{ AG_KEY_8,			AG_KCAT_PRINT,    "8"		},
	{ AG_KEY_9,			AG_KCAT_PRINT,    "9"		},
	{ AG_KEY_COLON,			AG_KCAT_PRINT,    ":"		},
	{ AG_KEY_SEMICOLON,		AG_KCAT_PRINT,    ";"		},
	{ AG_KEY_LESS,			AG_KCAT_PRINT,    "<"		},
	{ AG_KEY_EQUALS,		AG_KCAT_PRINT,    "="		},
	{ AG_KEY_GREATER,		AG_KCAT_PRINT,    ">"		},
	{ AG_KEY_QUESTION,		AG_KCAT_PRINT,    "?"		},
	{ AG_KEY_AT,			AG_KCAT_PRINT,    "@"		},
	{ AG_KEY_LEFTBRACKET,		AG_KCAT_PRINT,    "["		},
	{ AG_KEY_BACKSLASH,		AG_KCAT_PRINT,    "\\"		},
	{ AG_KEY_RIGHTBRACKET,		AG_KCAT_PRINT,    "]"		},
	{ AG_KEY_CARET,			AG_KCAT_PRINT,    "^"		},
	{ AG_KEY_UNDERSCORE,		AG_KCAT_PRINT,    "_"		},
	{ AG_KEY_BACKQUOTE,		AG_KCAT_PRINT,    "`"		},
	{ AG_KEY_A,			AG_KCAT_ALPHA,    "A"		},
	{ AG_KEY_B,			AG_KCAT_ALPHA,    "B"		},
	{ AG_KEY_C,			AG_KCAT_ALPHA,    "C"		},
	{ AG_KEY_D,			AG_KCAT_ALPHA,    "D"		},
	{ AG_KEY_E,			AG_KCAT_ALPHA,    "E"		},
	{ AG_KEY_F,			AG_KCAT_ALPHA,    "F"		},
	{ AG_KEY_G,			AG_KCAT_ALPHA,    "G"		},
	{ AG_KEY_H,			AG_KCAT_ALPHA,    "H"		},
	{ AG_KEY_I,			AG_KCAT_ALPHA,    "I"		},
	{ AG_KEY_J,			AG_KCAT_ALPHA,    "J"		},
	{ AG_KEY_K,			AG_KCAT_ALPHA,    "K"		},
	{ AG_KEY_L,			AG_KCAT_ALPHA,    "L"		},
	{ AG_KEY_M,			AG_KCAT_ALPHA,    "M"		},
	{ AG_KEY_N,			AG_KCAT_ALPHA,    "N"		},
	{ AG_KEY_O,			AG_KCAT_ALPHA,    "O"		},
	{ AG_KEY_P,			AG_KCAT_ALPHA,    "P"		},
	{ AG_KEY_Q,			AG_KCAT_ALPHA,    "Q"		},
	{ AG_KEY_R,			AG_KCAT_ALPHA,    "R"		},
	{ AG_KEY_S,			AG_KCAT_ALPHA,    "S"		},
	{ AG_KEY_T,			AG_KCAT_ALPHA,    "T"		},
	{ AG_KEY_U,			AG_KCAT_ALPHA,    "U"		},
	{ AG_KEY_V,			AG_KCAT_ALPHA,    "V"		},
	{ AG_KEY_W,			AG_KCAT_ALPHA,    "W"		},
	{ AG_KEY_X,			AG_KCAT_ALPHA,    "X"		},
	{ AG_KEY_Y,			AG_KCAT_ALPHA,    "Y"		},
	{ AG_KEY_Z,			AG_KCAT_ALPHA,    "Z"		},
	{ AG_KEY_DELETE,		AG_KCAT_CONTROL,  "Del"		},
	{ AG_KEY_KP0,			AG_KCAT_NUMBER,   "Kp0"		},
	{ AG_KEY_KP1,			AG_KCAT_NUMBER,   "Kp1"		},
	{ AG_KEY_KP2,			AG_KCAT_NUMBER,   "Kp2"		},
	{ AG_KEY_KP3,			AG_KCAT_NUMBER,   "Kp3"		},
	{ AG_KEY_KP4,			AG_KCAT_NUMBER,   "Kp4"		},
	{ AG_KEY_KP5,			AG_KCAT_NUMBER,   "Kp5"		},
	{ AG_KEY_KP6,			AG_KCAT_NUMBER,   "Kp6"		},
	{ AG_KEY_KP7,			AG_KCAT_NUMBER,   "Kp7"		},
	{ AG_KEY_KP8,			AG_KCAT_NUMBER,   "Kp8"		},
	{ AG_KEY_KP9,			AG_KCAT_NUMBER,   "Kp9"		},
	{ AG_KEY_KP_PERIOD,		AG_KCAT_NUMBER,   "KpPeriod"	},
	{ AG_KEY_KP_DIVIDE,		AG_KCAT_NUMBER,   "KpDiv"	},
	{ AG_KEY_KP_MULTIPLY,		AG_KCAT_NUMBER,   "KpMult"	},
	{ AG_KEY_KP_MINUS,		AG_KCAT_NUMBER,   "KpMinus"	},
	{ AG_KEY_KP_PLUS,		AG_KCAT_NUMBER,   "KpPlus"	},
	{ AG_KEY_KP_ENTER,		AG_KCAT_NUMBER,   "KpEnter"	},
	{ AG_KEY_KP_EQUALS,		AG_KCAT_NUMBER,   "KpEquals"	},
	{ AG_KEY_UP,			AG_KCAT_DIR,      "Up"		},
	{ AG_KEY_DOWN,			AG_KCAT_DIR,      "Down"	},
	{ AG_KEY_RIGHT,			AG_KCAT_DIR,      "Right"	},
	{ AG_KEY_LEFT,			AG_KCAT_DIR,      "Left"	},
	{ AG_KEY_INSERT,		AG_KCAT_CONTROL,  "Insert"	},
	{ AG_KEY_HOME,			AG_KCAT_CONTROL,  "Home"	},
	{ AG_KEY_END,			AG_KCAT_CONTROL,  "End"		},
	{ AG_KEY_PAGEUP,		AG_KCAT_CONTROL,  "PageUp"	},
	{ AG_KEY_PAGEDOWN,		AG_KCAT_CONTROL,  "PageDown"	},
	{ AG_KEY_F1,			AG_KCAT_FUNCTION, "F1"		},
	{ AG_KEY_F2,			AG_KCAT_FUNCTION, "F2"		},
	{ AG_KEY_F3,			AG_KCAT_FUNCTION, "F3"		},
	{ AG_KEY_F4,			AG_KCAT_FUNCTION, "F4"		},
	{ AG_KEY_F5,			AG_KCAT_FUNCTION, "F5"		},
	{ AG_KEY_F6,			AG_KCAT_FUNCTION, "F6"		},
	{ AG_KEY_F7,			AG_KCAT_FUNCTION, "F7"		},
	{ AG_KEY_F8,			AG_KCAT_FUNCTION, "F8"		},
	{ AG_KEY_F9,			AG_KCAT_FUNCTION, "F9"		},
	{ AG_KEY_F10,			AG_KCAT_FUNCTION, "F10"		},
	{ AG_KEY_F11,			AG_KCAT_FUNCTION, "F11"		},
	{ AG_KEY_F12,			AG_KCAT_FUNCTION, "F12"		},
	{ AG_KEY_F13,			AG_KCAT_FUNCTION, "F13"		},
	{ AG_KEY_F14,			AG_KCAT_FUNCTION, "F14"		},
	{ AG_KEY_F15,			AG_KCAT_FUNCTION, "F15"		},
	{ AG_KEY_F16,			AG_KCAT_FUNCTION, "F16"		},
	{ AG_KEY_F17,			AG_KCAT_FUNCTION, "F17"		},
	{ AG_KEY_F18,			AG_KCAT_FUNCTION, "F18"		},
	{ AG_KEY_F19,			AG_KCAT_FUNCTION, "F19"		},
	{ AG_KEY_F20,			AG_KCAT_FUNCTION, "F20"		},
	{ AG_KEY_F21,			AG_KCAT_FUNCTION, "F21"		},
	{ AG_KEY_F22,			AG_KCAT_FUNCTION, "F22"		},
	{ AG_KEY_F23,			AG_KCAT_FUNCTION, "F23"		},
	{ AG_KEY_F24,			AG_KCAT_FUNCTION, "F24"		},
	{ AG_KEY_F25,			AG_KCAT_FUNCTION, "F25"		},
	{ AG_KEY_F26,			AG_KCAT_FUNCTION, "F26"		},
	{ AG_KEY_F27,			AG_KCAT_FUNCTION, "F27"		},
	{ AG_KEY_F28,			AG_KCAT_FUNCTION, "F28"		},
	{ AG_KEY_F29,			AG_KCAT_FUNCTION, "F29"		},
	{ AG_KEY_F30,			AG_KCAT_FUNCTION, "F30"		},
	{ AG_KEY_F31,			AG_KCAT_FUNCTION, "F31"		},
	{ AG_KEY_F32,			AG_KCAT_FUNCTION, "F32"		},
	{ AG_KEY_F33,			AG_KCAT_FUNCTION, "F33"		},
	{ AG_KEY_F34,			AG_KCAT_FUNCTION, "F34"		},
	{ AG_KEY_F35,			AG_KCAT_FUNCTION, "F35"		},
	{ AG_KEY_NUMLOCK,		AG_KCAT_LOCK,     "NumLock"	},
	{ AG_KEY_CAPSLOCK,		AG_KCAT_LOCK,     "CapsLock"	},
	{ AG_KEY_SCROLLOCK,		AG_KCAT_LOCK,     "ScrollLock"	},
	{ AG_KEY_RSHIFT,		AG_KCAT_MODIFIER, "R-Shift"	},
	{ AG_KEY_LSHIFT,		AG_KCAT_MODIFIER, "L-Shift"	},
	{ AG_KEY_RCTRL,			AG_KCAT_MODIFIER, "R-Ctrl"	},
	{ AG_KEY_LCTRL,			AG_KCAT_MODIFIER, "L-Ctrl"	},
	{ AG_KEY_RALT,			AG_KCAT_MODIFIER, "R-Alt"	},
	{ AG_KEY_LALT,			AG_KCAT_MODIFIER, "L-Alt"	},
	{ AG_KEY_RMETA,			AG_KCAT_MODIFIER, "R-Meta"	},
	{ AG_KEY_LMETA,			AG_KCAT_MODIFIER, "L-Meta"	},
	{ AG_KEY_LSUPER,		AG_KCAT_MODIFIER, "L-Super"	},
	{ AG_KEY_RSUPER,		AG_KCAT_MODIFIER, "R-Super"	},
	{ AG_KEY_MODE,			AG_KCAT_MODIFIER, "Mode"	},
	{ AG_KEY_COMPOSE,		AG_KCAT_MODIFIER, "Compose"	},
	{ AG_KEY_HELP,			AG_KCAT_FUNCTION, "Help"	},
	{ AG_KEY_PRINT,			AG_KCAT_FUNCTION, "Print"	},
	{ AG_KEY_SYSREQ,		AG_KCAT_FUNCTION, "SysReq"	},
	{ AG_KEY_BREAK,			AG_KCAT_FUNCTION, "Break"	},
	{ AG_KEY_MENU,			AG_KCAT_FUNCTION, "Menu"	},
	{ AG_KEY_POWER,			AG_KCAT_FUNCTION, "Power"	},
	{ AG_KEY_EURO,			AG_KCAT_PRINT,    "Euro"	},
	{ AG_KEY_UNDO,			AG_KCAT_FUNCTION, "Undo"	},
	{ AG_KEY_GRAVE,			AG_KCAT_PRINT,    "Grave"	},
	{ AG_KEY_KP_CLEAR,		AG_KCAT_CONTROL,  "KpClear"	},
	{ AG_KEY_COMMAND,		AG_KCAT_FUNCTION, "Command"	},
	{ AG_KEY_FUNCTION,		AG_KCAT_FUNCTION, "Function"	},
	{ AG_KEY_VOLUME_UP,		AG_KCAT_FUNCTION, "VolumeUp"	},
	{ AG_KEY_VOLUME_DOWN,		AG_KCAT_FUNCTION, "VolumeDown"	},
	{ AG_KEY_VOLUME_MUTE,		AG_KCAT_FUNCTION, "VolumeMute"	},
	{ AG_KEY_BEGIN,			AG_KCAT_FUNCTION, "Begin"	},
	{ AG_KEY_RESET,			AG_KCAT_FUNCTION, "Reset"	},
	{ AG_KEY_STOP,			AG_KCAT_FUNCTION, "Stop"	},
	{ AG_KEY_USER,			AG_KCAT_FUNCTION, "User"	},
	{ AG_KEY_SYSTEM,		AG_KCAT_FUNCTION, "System"	},
	{ AG_KEY_PRINT_SCREEN,		AG_KCAT_FUNCTION, "PrintScreen"	},
	{ AG_KEY_CLEAR_LINE,		AG_KCAT_FUNCTION, "ClearLine"	},
	{ AG_KEY_CLEAR_DISPLAY,		AG_KCAT_FUNCTION, "ClearDisplay"},
	{ AG_KEY_INSERT_LINE,		AG_KCAT_FUNCTION, "InsertLine"	},
	{ AG_KEY_DELETE_LINE,		AG_KCAT_FUNCTION, "DeleteLine"	},
	{ AG_KEY_INSERT_CHAR,		AG_KCAT_FUNCTION, "InsertChar"	},
	{ AG_KEY_DELETE_CHAR,		AG_KCAT_FUNCTION, "DeleteChar"	},
	{ AG_KEY_PREV,			AG_KCAT_FUNCTION, "Prev"	},
	{ AG_KEY_NEXT,			AG_KCAT_FUNCTION, "Next"	},
	{ AG_KEY_SELECT,		AG_KCAT_FUNCTION, "Select"	},
	{ AG_KEY_EXECUTE,		AG_KCAT_FUNCTION, "Execute"	},
	{ AG_KEY_REDO,			AG_KCAT_FUNCTION, "Redo"	},
	{ AG_KEY_FIND,			AG_KCAT_FUNCTION, "Find"	},
	{ AG_KEY_MODE_SWITCH,		AG_KCAT_FUNCTION, "ModeSwitch"	},
	{ AG_KEY_NON_US_BACKSLASH,	AG_KCAT_PRINT,    "NonUsBackslash" },
	{ AG_KEY_APPLICATION,		AG_KCAT_FUNCTION, "Application"	},
	{ AG_KEY_AGAIN,			AG_KCAT_FUNCTION, "Again"	},
	{ AG_KEY_CUT,			AG_KCAT_FUNCTION, "Cut"		},
	{ AG_KEY_PASTE,			AG_KCAT_FUNCTION, "Paste"	},
	{ AG_KEY_KP_COMMA,		AG_KCAT_NUMBER,	  "KpComma"	},
	{ AG_KEY_KP_EQUALS_AS_400,	AG_KCAT_NUMBER,   "KpEqualsAs400" },
	{ AG_KEY_INTERNATIONAL_1,	AG_KCAT_FUNCTION, "International1" },
	{ AG_KEY_INTERNATIONAL_2,	AG_KCAT_FUNCTION, "International2" },
	{ AG_KEY_INTERNATIONAL_3,	AG_KCAT_FUNCTION, "International3" },
	{ AG_KEY_INTERNATIONAL_4,	AG_KCAT_FUNCTION, "International4" },
	{ AG_KEY_INTERNATIONAL_5,	AG_KCAT_FUNCTION, "International5" },
	{ AG_KEY_INTERNATIONAL_6,	AG_KCAT_FUNCTION, "International6" },
	{ AG_KEY_INTERNATIONAL_7,	AG_KCAT_FUNCTION, "International7" },
	{ AG_KEY_INTERNATIONAL_8,	AG_KCAT_FUNCTION, "International8" },
	{ AG_KEY_INTERNATIONAL_9,	AG_KCAT_FUNCTION, "International9" },
	{ AG_KEY_LANGUAGE_1,		AG_KCAT_FUNCTION, "Language1"	},
	{ AG_KEY_LANGUAGE_2,		AG_KCAT_FUNCTION, "Language2"	},
	{ AG_KEY_LANGUAGE_3,		AG_KCAT_FUNCTION, "Language3"	},
	{ AG_KEY_LANGUAGE_4,		AG_KCAT_FUNCTION, "Language4"	},
	{ AG_KEY_LANGUAGE_5,		AG_KCAT_FUNCTION, "Language5"	},
	{ AG_KEY_LANGUAGE_6,		AG_KCAT_FUNCTION, "Language6"	},
	{ AG_KEY_LANGUAGE_7,		AG_KCAT_FUNCTION, "Language7"	},
	{ AG_KEY_LANGUAGE_8,		AG_KCAT_FUNCTION, "Language8"	},
	{ AG_KEY_LANGUAGE_9,		AG_KCAT_FUNCTION, "Language9"	},
	{ AG_KEY_ALT_ERASE,		AG_KCAT_CONTROL,  "AltErase"	},
	{ AG_KEY_CANCEL,		AG_KCAT_FUNCTION, "Cancel"	},
	{ AG_KEY_PRIOR,			AG_KCAT_FUNCTION, "Prior"	},
	{ AG_KEY_RETURN2,		AG_KCAT_FUNCTION, "Return2"	},
	{ AG_KEY_SEPARATOR,		AG_KCAT_FUNCTION, "Separator"	},
	{ AG_KEY_OUT,			AG_KCAT_FUNCTION, "Out"		},
	{ AG_KEY_OPER,			AG_KCAT_FUNCTION, "Oper"	},
	{ AG_KEY_CLEAR_AGAIN,		AG_KCAT_FUNCTION, "ClearAgain"	},
	{ AG_KEY_CRSEL,			AG_KCAT_FUNCTION, "CrSel"	},
	{ AG_KEY_EXSEL,			AG_KCAT_FUNCTION, "ExSel"	},
	{ AG_KEY_KP_00,			AG_KCAT_NUMBER,   "Kp00"	},
	{ AG_KEY_KP_000,		AG_KCAT_NUMBER,   "Kp000"	},
	{ AG_KEY_THOUSANDS_SEPARATOR,	AG_KCAT_PRINT,    "ThousandsSep"},
	{ AG_KEY_DECIMALS_SEPARATOR,	AG_KCAT_PRINT,    "DecimalsSep" },
	{ AG_KEY_CURRENCY_UNIT,		AG_KCAT_PRINT,    "CurrencyUnit"},
	{ AG_KEY_KP_LEFT_PAREN,		AG_KCAT_PRINT,    "KpLeftParen" },
	{ AG_KEY_KP_RIGHT_PAREN,	AG_KCAT_PRINT,    "KpRightParen"},
	{ AG_KEY_KP_LEFT_BRACE,		AG_KCAT_PRINT,    "KpLeftBrace" },
	{ AG_KEY_KP_RIGHT_BRACE,	AG_KCAT_PRINT,    "KpRightBrace"},
	{ AG_KEY_KP_TAB,		AG_KCAT_SPACING,  "KpTab"	},
	{ AG_KEY_KP_BACKSPACE,		AG_KCAT_CONTROL,  "KpBackspace" },
	{ AG_KEY_KP_A,			AG_KCAT_NUMBER,   "KpA"		},
	{ AG_KEY_KP_B,			AG_KCAT_NUMBER,   "KpB"		},
	{ AG_KEY_KP_C,			AG_KCAT_NUMBER,   "KpC"		},
	{ AG_KEY_KP_D,			AG_KCAT_NUMBER,   "KpD"		},
	{ AG_KEY_KP_E,			AG_KCAT_NUMBER,   "KpE"		},
	{ AG_KEY_KP_F,			AG_KCAT_NUMBER,   "KpF"		},
	{ AG_KEY_KP_XOR,		AG_KCAT_FUNCTION, "KpXOR"	},
	{ AG_KEY_KP_POWER,		AG_KCAT_FUNCTION, "KpPower"	},
	{ AG_KEY_KP_PERCENT,		AG_KCAT_FUNCTION, "KpPercent"	},
	{ AG_KEY_KP_LESS,		AG_KCAT_FUNCTION, "KpLess"	},
	{ AG_KEY_KP_GREATER,		AG_KCAT_FUNCTION, "KpGreater"	},
	{ AG_KEY_KP_AMPERSAND,		AG_KCAT_PRINT,    "KpAmpersand"	},
	{ AG_KEY_KP_DBL_AMPERSAND,	AG_KCAT_PRINT,    "KpDblAmpersand"},
	{ AG_KEY_KP_VERTICAL_BAR,	AG_KCAT_PRINT,    "KpVerticalBar"},
	{ AG_KEY_KP_COLON,		AG_KCAT_PRINT,    "KpColon"	},
	{ AG_KEY_KP_HASH,		AG_KCAT_PRINT,    "KpHash"	},
	{ AG_KEY_KP_SPACE,		AG_KCAT_SPACING,  "KpSpace"	},
	{ AG_KEY_KP_AT,			AG_KCAT_PRINT,    "KpAt"	},
	{ AG_KEY_KP_EXCLAM,		AG_KCAT_PRINT,    "KpExclam"	},
	{ AG_KEY_KP_MEM_STORE,		AG_KCAT_FUNCTION, "KpMemStore"	},
	{ AG_KEY_KP_MEM_RECALL,		AG_KCAT_FUNCTION, "KpMemRecall"	},
	{ AG_KEY_KP_MEM_CLEAR,		AG_KCAT_FUNCTION, "KpMemClear"	},
	{ AG_KEY_KP_MEM_ADD,		AG_KCAT_FUNCTION, "KpMemAdd"	},
	{ AG_KEY_KP_MEM_SUBTRACT,	AG_KCAT_FUNCTION, "KpMemSubtract"},
	{ AG_KEY_KP_MEM_MULTIPLY,	AG_KCAT_FUNCTION, "KpMemMultiply"},
	{ AG_KEY_KP_MEM_DIVIDE,		AG_KCAT_FUNCTION, "KpMemDivide"	},
	{ AG_KEY_KP_PLUS_MINUS,		AG_KCAT_PRINT,    "KpPlusMinus"	},
	{ AG_KEY_KP_CLEAR_ENTRY,	AG_KCAT_FUNCTION, "KpClearEntry"},
	{ AG_KEY_KP_BINARY,		AG_KCAT_FUNCTION, "KpBinary"	},
	{ AG_KEY_KP_OCTAL,		AG_KCAT_FUNCTION, "KpOct"	},
	{ AG_KEY_KP_DECIMAL,		AG_KCAT_FUNCTION, "KpDec"	},
	{ AG_KEY_KP_HEXADECIMAL,	AG_KCAT_FUNCTION, "KpHex"	},
	{ AG_KEY_LGUI,			AG_KCAT_FUNCTION, "LeftGUI"	},
	{ AG_KEY_RGUI,			AG_KCAT_FUNCTION, "RightGUI"	},
	{ AG_KEY_AUDIO_NEXT,		AG_KCAT_FUNCTION, "AudioNext"	},
	{ AG_KEY_AUDIO_PREV,		AG_KCAT_FUNCTION, "AudioPrev"	},
	{ AG_KEY_AUDIO_STOP,		AG_KCAT_FUNCTION, "AudioStop"	},
	{ AG_KEY_AUDIO_PLAY,		AG_KCAT_FUNCTION, "AudioPlay"	},
	{ AG_KEY_AUDIO_MUTE,		AG_KCAT_FUNCTION, "AudioMute"	},
	{ AG_KEY_MEDIA_SELECT,		AG_KCAT_FUNCTION, "MediaSelect"	},
	{ AG_KEY_WWW,			AG_KCAT_FUNCTION, "WWW"		},
	{ AG_KEY_MAIL,			AG_KCAT_FUNCTION, "Mail"	},
	{ AG_KEY_CALCULATOR,		AG_KCAT_FUNCTION, "Calculator"	},
	{ AG_KEY_COMPUTER,		AG_KCAT_FUNCTION, "Computer"	},
	{ AG_KEY_AC_SEARCH,		AG_KCAT_FUNCTION, "AcSearch"	},
	{ AG_KEY_AC_HOME,		AG_KCAT_FUNCTION, "AcHome"	},
	{ AG_KEY_AC_BACK,		AG_KCAT_FUNCTION, "AcBack"	},
	{ AG_KEY_AC_FORWARD,		AG_KCAT_FUNCTION, "AcForward"	},
	{ AG_KEY_AC_STOP,		AG_KCAT_FUNCTION, "AcStop"	},
	{ AG_KEY_AC_REFRESH,		AG_KCAT_FUNCTION, "AcRefresh"	},
	{ AG_KEY_AC_BOOKMARKS,		AG_KCAT_FUNCTION, "AcBookmarks" },
	{ AG_KEY_AUDIO_REWIND,		AG_KCAT_FUNCTION, "AudioRewind"	},
	{ AG_KEY_AUDIO_FASTFORWARD,	AG_KCAT_FUNCTION, "AudioFastFwd"},
};
static const Uint agKeyNameTblSize = sizeof(agKeyNameTbl) /
                                     sizeof(agKeyNameTbl[0]);

AG_Keyboard *
AG_KeyboardNew(void *drv, const char *desc)
{
	AG_Keyboard *kbd;
	
	AG_OBJECT_ISA(drv, "AG_Driver:*");

	if ((kbd = TryMalloc(sizeof(AG_Keyboard))) == NULL) {
		return (NULL);
	}
	AG_ObjectInit(kbd, &agKeyboardClass);
	AGINPUTDEV(kbd)->drv = drv;
	if ((AGINPUTDEV(kbd)->desc = TryStrdup(desc)) == NULL) {
		goto fail;
	}
	AGDRIVER(drv)->kbd = kbd;
	AG_ObjectAttach(&agInputDevices, kbd);
	return (kbd);
fail:
	AG_ObjectDestroy(kbd);
	return (NULL);
}

static void
Init(void *_Nonnull obj)
{
	AG_Keyboard *kbd = obj;

	OBJECT(kbd)->flags |= AG_OBJECT_NAME_ONATTACH;
	kbd->keyCount = AG_KEY_LAST;
	kbd->keyState = Malloc(kbd->keyCount*sizeof(int));
	memset(kbd->keyState, 0, kbd->keyCount*sizeof(int));
	kbd->modState = AG_KEYMOD_NONE;
}

static void
Destroy(void *_Nonnull obj)
{
	AG_Keyboard *kbd = obj;

	free(kbd->keyState);
}

/*
 * Update Agar's internal keyboard state following a key press/release
 * event. Drivers are only required to update this state for keys in the
 * AG_KEY_ASCII_START to AG_KEY_ASCII_END range, as well as modifier keys.
 */
int
AG_KeyboardUpdate(AG_Keyboard *kbd, AG_KeyboardAction action, AG_KeySym ks)
{
	Uint ms = kbd->modState;

	if (ks >= kbd->keyCount) {		/* Ignore bad keysym */
#ifdef AG_DEBUG
		Debug(kbd, "Bad keysym 0x%x\n", ks);
#endif
		return (0);
	}

	/* Update the keyboard state. */
	switch (action) {
	case AG_KEY_PRESSED:
		switch (ks) {
		case AG_KEY_NONE:
			return (0);
		case AG_KEY_NUMLOCK:
			ms ^= AG_KEYMOD_NUMLOCK;
			if ((ms & AG_KEYMOD_NUMLOCK) == 0) {
				action = AG_KEY_RELEASED;
			}
			break;
		case AG_KEY_CAPSLOCK:
			ms ^= AG_KEYMOD_CAPSLOCK;
			if ((ms & AG_KEYMOD_CAPSLOCK) == 0) {
				action = AG_KEY_RELEASED;
			}
			break;
		case AG_KEY_LCTRL:	ms |= AG_KEYMOD_LCTRL;		break;
		case AG_KEY_RCTRL:	ms |= AG_KEYMOD_RCTRL;		break;
		case AG_KEY_LSHIFT:	ms |= AG_KEYMOD_LSHIFT;		break;
		case AG_KEY_RSHIFT:	ms |= AG_KEYMOD_RSHIFT;		break;
		case AG_KEY_LALT:	ms |= AG_KEYMOD_LALT;		break;
		case AG_KEY_RALT:	ms |= AG_KEYMOD_RALT;		break;
		case AG_KEY_LMETA:	ms |= AG_KEYMOD_LMETA;		break;
		case AG_KEY_RMETA:	ms |= AG_KEYMOD_RMETA;		break;
		case AG_KEY_MODE:	ms |= AG_KEYMOD_MODE;		break;
		default:						break;
		}
		break;
	case AG_KEY_RELEASED:
		switch (ks) {
		case AG_KEY_NONE:
		case AG_KEY_NUMLOCK:
		case AG_KEY_CAPSLOCK:
			return (0);
		case AG_KEY_LCTRL:	ms &= ~AG_KEYMOD_LCTRL;		break;
		case AG_KEY_RCTRL:	ms &= ~AG_KEYMOD_RCTRL;		break;
		case AG_KEY_LSHIFT:	ms &= ~AG_KEYMOD_LSHIFT;	break;
		case AG_KEY_RSHIFT:	ms &= ~AG_KEYMOD_RSHIFT;	break;
		case AG_KEY_LALT:	ms &= ~AG_KEYMOD_LALT;		break;
		case AG_KEY_RALT:	ms &= ~AG_KEYMOD_RALT;		break;
		case AG_KEY_LMETA:	ms &= ~AG_KEYMOD_LMETA;		break;
		case AG_KEY_RMETA:	ms &= ~AG_KEYMOD_RMETA;		break;
		case AG_KEY_MODE:	ms &= ~AG_KEYMOD_MODE;		break;
		default:						break;
		}
		break;
	default:
		return (0);
	}
	kbd->modState = ms;
	if (kbd->keyState[ks] == (int)action) {
		return (0);
	}
	kbd->keyState[ks] = (int)action;
	return (1);
}

/* Post a key-up event to widgets with the UNFOCUSED_KEYUP flag set. */
static void
PostUnfocusedKeyUp(AG_Widget *_Nonnull wid, AG_KeySym ks, AG_KeyMod kmod,
    AG_Char ch)
{
	AG_Widget *cwid;

	AG_ObjectLock(wid);

	if (wid->flags & AG_WIDGET_UNFOCUSED_KEYUP) {
		if (WIDGET_OPS(wid)->key_up != NULL) {
			WIDGET_OPS(wid)->key_up(wid, ks, kmod, ch);
		} else {
#ifdef AG_UNICODE
			AG_PostEvent(wid,  "key-up", "%i(key),%i(mod),%lu(ch)",
			    (int)ks, (int)kmod, (Ulong)ch);
#else
			AG_PostEvent(wid,  "key-up", "%i(key),%i(mod),%u(ch)",
			    (int)ks, (int)kmod, (Uint)ch);
#endif
		}
	}

	OBJECT_FOREACH_CHILD(cwid, wid, ag_widget)
		PostUnfocusedKeyUp(cwid, ks, kmod, ch);

	AG_ObjectUnlock(wid);
}

/* Post a key-down event to widgets with the UNFOCUSED_KEYDOWN flag set. */
static void
PostUnfocusedKeyDown(AG_Widget *_Nonnull wid, AG_KeySym ks, AG_KeyMod kmod,
    AG_Char ch)
{
	AG_Widget *cwid;

	AG_ObjectLock(wid);

	if (wid->flags & AG_WIDGET_UNFOCUSED_KEYDOWN) {
		if (WIDGET_OPS(wid)->key_down != NULL) {
			WIDGET_OPS(wid)->key_down(wid, ks, kmod, ch);
		} else {
#ifdef AG_UNICODE
			AG_PostEvent(wid,  "key-down", "%i(key),%i(mod),%lu(ch)",
			    (int)ks, (int)kmod, (Ulong)ch);
#else
			AG_PostEvent(wid,  "key-down", "%i(key),%i(mod),%u(ch)",
			    (int)ks, (int)kmod, (Uint)ch);
#endif
		}
	}

	OBJECT_FOREACH_CHILD(cwid, wid, ag_widget)
		PostUnfocusedKeyDown(cwid, ks, kmod, ch);

	AG_ObjectUnlock(wid);
}

/*
 * Process a key press or key release action, sending key-down/key-up
 * events to the appropriate Agar widget(s). This function is usually
 * invoked from the ProcessEvent() routine of a driver.
 *
 * Unicode characters without related keysym (or vice-versa) are allowed.
 * The ks argument can be AG_KEY_NONE, or the ch argument can be 0.
 * 
 * The agDrivers VFS must be locked.
 */
int
AG_ProcessKey(AG_Keyboard *kbd, AG_Window *win, AG_KeyboardAction action,
    AG_KeySym ks, AG_Char ch)
{
	AG_Driver *drv = AGINPUTDEV(kbd)->drv;
	AG_Widget *wFoc;
	const AG_KeyMod modState = kbd->modState;
	int tabCycle;

#ifdef DEBUG_KEYBOARD
	Debug(kbd, "ProcessKey(%s,%s,%s,0x%x)\n",
	    OBJECT(win)->name, (action == AG_KEY_PRESSED) ? "Down" : "Up",
	    AG_LookupKeyName(ks), ch);
#endif
	switch (action) {
	case AG_KEY_RELEASED:
		PostUnfocusedKeyUp(WIDGET(win), ks, modState, ch);
		break;
	case AG_KEY_PRESSED:
		if (AG_ExecGlobalKeys(ks, modState)) {
			return (1);              /* Break out of Window loop */
		}
		PostUnfocusedKeyDown(WIDGET(win), ks, modState, ch);
		break;
	}

	tabCycle = 1;

	if (AG_WindowIsFocused(win) &&                    /* Focused widget? */
	   (wFoc = AG_WidgetFindFocused(win)) != NULL) {
		AG_ObjectLock(wFoc);

		if (ks != AG_KEY_TAB || (wFoc->flags & AG_WIDGET_CATCH_TAB)) {
			if (wFoc->flags & AG_WIDGET_CATCH_TAB)
				tabCycle = 0;

			if (action == AG_KEY_RELEASED) {    /* Key Released */
				if (WIDGET_OPS(wFoc)->key_up != NULL) {
					WIDGET_OPS(wFoc)->key_up(wFoc, ks,
					    modState, ch);
				} else {
#ifdef AG_UNICODE
					AG_PostEvent(wFoc, "key-up",
					    "%i(key),%i(mod),%lu(ch)",
					    (int)ks, (int)modState, (Ulong)ch);
#else
					AG_PostEvent(wFoc, "key-up",
					    "%i(key),%i(mod),%u(ch)",
					    (int)ks, (int)modState, (Uint)ch);
#endif
				}
			} else {                             /* Key Pressed */
				if (WIDGET_OPS(wFoc)->key_down != NULL) {
					WIDGET_OPS(wFoc)->key_down(wFoc,
					    ks, modState, ch);
				} else {
#ifdef AG_UNICODE
					AG_PostEvent(wFoc, "key-down",
					    "%i(key),%i(mod),%lu(ch)",
					    (int)ks, (int)modState, (Ulong)ch);
#else
					AG_PostEvent(wFoc, "key-down",
					    "%i(key),%i(mod),%u(ch)",
					    (int)ks, (int)modState, (Uint)ch);
#endif
				}
			}
			if (AGDRIVER_SINGLE(drv)) {
				/*
				 * Once this key is released, ensure the key-up
				 * event will be delivered to this window (and
				 * not the newly focused one). Prevents issues
				 * where key-down event handlers cause changes
				 * in window focus.
				 */
				AGDRIVER_SW(drv)->winLastKeydown = win;
			}
		}
		AG_ObjectUnlock(wFoc);
	}

	if (AGDRIVER_MULTIPLE(drv) && tabCycle &&             /* Focus move */
	    (ks == AG_KEY_TAB && action == AG_KEY_RELEASED)) {
		AG_WindowCycleFocus(win, (modState & AG_KEYMOD_SHIFT) ? 1 : 0);
		/* Break out of Window loop */
		return (1);
	}
	return (0);
}

/* Return a string describing a given Agar keysym. */
const char *
AG_LookupKeyName(AG_KeySym ks)
{
	int i;

	for (i = 0; i < agKeyNameTblSize; i++) {
		if (agKeyNameTbl[i].ks == ks) {
			return (agKeyNameTbl[i].name);
		}
	}
	return (NULL);
}

/* Return a string describing a given Agar keymod. */
char *
AG_LookupKeyMod(AG_KeyMod kmod)
{
	char buf[32];
	int nMods=0;

	if (kmod & AG_KEYMOD_CTRL_SHIFT) {
		Strlcpy(buf, "Ctrl-Shift", sizeof(buf));
		nMods++;
	} else if (kmod & AG_KEYMOD_CTRL_ALT) {
		Strlcpy(buf, "Ctrl-Alt", sizeof(buf));
		nMods++;
	} else {
		if (kmod & AG_KEYMOD_CTRL) {
			if ((kmod & AG_KEYMOD_LCTRL) && (kmod & AG_KEYMOD_RCTRL)==0) {
				Strlcpy(buf, "LCtrl", sizeof(buf));
			} else if ((kmod & AG_KEYMOD_LCTRL)==0 && (kmod & AG_KEYMOD_RCTRL)) {
				Strlcpy(buf, "RCtrl", sizeof(buf));
			} else {
				Strlcpy(buf, "Ctrl", sizeof(buf));
			}
			nMods++;
		} else if (kmod & AG_KEYMOD_META) {
			if ((kmod & AG_KEYMOD_LMETA) && (kmod & AG_KEYMOD_RMETA)==0) {
				Strlcpy(buf, "LMeta", sizeof(buf));
			} else if ((kmod & AG_KEYMOD_LMETA)==0 && (kmod & AG_KEYMOD_RMETA)) {
				Strlcpy(buf, "RMeta", sizeof(buf));
			} else {
				Strlcpy(buf, "Meta", sizeof(buf));
			}
			nMods++;
		} else {
			buf[0] = '\0';
		}

		if ((kmod & AG_KEYMOD_ALT) && ((kmod & AG_KEYMOD_CTRL_ALT) == 0)) {
			if (nMods > 0) {
				Strlcat(buf, "-", sizeof(buf));
				nMods = 0;
			}
			if ((kmod & AG_KEYMOD_LALT) && (kmod & AG_KEYMOD_RALT)==0) {
				Strlcat(buf, "LAlt", sizeof(buf));
			} else if ((kmod & AG_KEYMOD_LALT)==0 && (kmod & AG_KEYMOD_RALT)) {
				Strlcat(buf, "RAlt", sizeof(buf));
			} else {
				Strlcat(buf, "Alt", sizeof(buf));
			}
			nMods++;
		}
	
		if (kmod & AG_KEYMOD_SHIFT) {
			if (nMods > 0) {
				Strlcat(buf, "-", sizeof(buf));
				nMods--;
			}
			if ((kmod & AG_KEYMOD_LSHIFT) && (kmod & AG_KEYMOD_RSHIFT)==0) {
				Strlcat(buf, "LShift", sizeof(buf));
			} else if ((kmod & AG_KEYMOD_LSHIFT)==0 && (kmod & AG_KEYMOD_RSHIFT)) {
				Strlcat(buf, "RShift", sizeof(buf));
			} else {
				Strlcat(buf, "Shift", sizeof(buf));
			}
			nMods++;
		}
	}
#if 0
	if (kmod & AG_KEYMOD_MODE) {
		if (nMods > 0) {
			Strlcat(buf, "-", sizeof(buf));
		}
		Strlcat(buf, "Mode", sizeof(buf));
	} else if (kmod & AG_KEYMOD_NUMLOCK) {
		if (nMods > 0) {
			Strlcat(buf, "-", sizeof(buf));
		}
		Strlcat(buf, "NumLock", sizeof(buf));
	} else if (kmod & AG_KEYMOD_CAPSLOCK) {
		if (nMods > 0) {
			Strlcat(buf, "-", sizeof(buf));
		}
		Strlcat(buf, "CapsLock", sizeof(buf));
	}
#endif
	return AG_Strdup(buf);
}

/* Lookup an Agar keysym by name. */
AG_KeySym
AG_LookupKeySym(const char *name)
{
	int i;

	for (i = 0; i < agKeyNameTblSize; i++) {
		if (strcmp(agKeyNameTbl[i].name,name) == 0) {
			return (agKeyNameTbl[i].ks);
		}
	}
	return (AG_KEY_NONE);
}

/* Compare unsided modifier state against a string of flags. */
int
AG_CompareKeyMods(Uint modState, const char *flags)
{
	const char *c;

	for (c = &flags[0]; *c != '\0'; c++) {
		switch (*c) {
		case 'C':
			if (modState & AG_KEYMOD_LCTRL ||
			    modState & AG_KEYMOD_RCTRL) {
				return (1);
			}
			break;
		case 'A':
			if (modState & AG_KEYMOD_LALT ||
			    modState & AG_KEYMOD_RALT) {
				return (1);
			}
			break;
		case 'S':
			if (modState & AG_KEYMOD_LSHIFT ||
			    modState & AG_KEYMOD_RSHIFT) {
				return (1);
			}
			break;
		case 'M':
			if (modState & AG_KEYMOD_LMETA ||
			    modState & AG_KEYMOD_RMETA) {
				return (1);
			}
			break;
		}
	}
	return (0);
}

#if defined(AG_WIDGETS) && defined(AG_DEBUG)
static void
PollKeyboardState(AG_Event *_Nonnull event)
{
	AG_Tlist *tl = AG_TLIST_SELF();
	AG_Keyboard *kbd = AG_PTR(1);
	AG_TlistItem *it;
	Uint i;

	if (!AG_OBJECT_VALID(kbd) ||
	    !AG_OfClass(kbd, "AG_InputDevice:AG_Keyboard:*"))
		return;

	AG_TlistBegin(tl);

	for (i = 0; i < kbd->keyCount; i++) {
		const char *keyName;

		if (kbd->keyState[i] == 0) {
			continue;
		}
		if ((keyName = AG_LookupKeyName(i)) != NULL) {
			it = AG_TlistAddS(tl, NULL, keyName);
		} else {
			it = AG_TlistAdd(tl, NULL, _("Unknown Key %d"), i);
		}
		it->p1 = &kbd->keyState[i];
	}

	AG_TlistEnd(tl);
}

static void *
Edit(void *obj)
{
	AG_Keyboard *kbd = obj;
	AG_Box *box;
	AG_Label *lbl;
	AG_Tlist *tl;

	box = AG_BoxNewVert(NULL, AG_BOX_EXPAND);

	lbl = AG_LabelNewS(box, 0, AGINPUTDEV(kbd)->desc);
	AG_SetFontFamily(lbl, "league-spartan");
	AG_SetFontSize(lbl, "130%");

	lbl = AG_LabelNewPolledMT(box, AG_LABEL_HFILL, &OBJECT(kbd)->lock,
	    _("Key Count: %u\n"
	      "Modifier State: 0x%x"),
	    &kbd->keyCount, &kbd->modState);
	AG_RedrawOnTick(lbl, 120);

	tl = AG_TlistNewPolled(box, AG_TLIST_EXPAND, PollKeyboardState,"%p", kbd);
	AG_SetFontWeight(tl, "bold");
	AG_SetFontSize(tl, "120%");
	AG_TlistSizeHint(tl, "<AG_KEY_THOUSANDS_SEPARATOR>", 10);
	AG_TlistSetRefresh(tl, 120);

	return (box);
}
#endif /* AG_WIDGETS and AG_DEBUG */

AG_ObjectClass agKeyboardClass = {
	"Agar(InputDevice:Keyboard)",
	sizeof(AG_Keyboard),
	{ 0,0 },
	Init,
	NULL,		/* reset */
	Destroy,
	NULL,		/* load */
	NULL,		/* save */
#if defined(AG_WIDGETS) && defined(AG_DEBUG)
	Edit
#else
	NULL		/* edit */
#endif
};
