
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <gl/GL.h>
#include <GL/GLext.h>
#define WGL_WGLEXT_PROTOTYPES
#include <GL/wglext.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cassert>

//=========================================================================
// common
//=========================================================================
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef bool b32;

typedef enum  {
    TEK_KEY_SPACE = 0,
    TEK_KEY_APOSTROPHE,  /* ' */
    TEK_KEY_COMMA,  /* , */
    TEK_KEY_MINUS,  /* - */
    TEK_KEY_PERIOD,  /* . */
    TEK_KEY_SLASH,  /* / */
    TEK_KEY_0,
    TEK_KEY_1,
    TEK_KEY_2,
    TEK_KEY_3,
    TEK_KEY_4,
    TEK_KEY_5,
    TEK_KEY_6,
    TEK_KEY_7,
    TEK_KEY_8,
    TEK_KEY_9,
    TEK_KEY_SEMICOLON,  /* ; */
    TEK_KEY_EQUAL,  /* = */
    TEK_KEY_A,
    TEK_KEY_B,
    TEK_KEY_C,
    TEK_KEY_D,
    TEK_KEY_E,
    TEK_KEY_F,
    TEK_KEY_G,
    TEK_KEY_H,
    TEK_KEY_I,
    TEK_KEY_J,
    TEK_KEY_K,
    TEK_KEY_L,
    TEK_KEY_M,
    TEK_KEY_N,
    TEK_KEY_O,
    TEK_KEY_P,
    TEK_KEY_Q,
    TEK_KEY_R,
    TEK_KEY_S,
    TEK_KEY_T,
    TEK_KEY_U,
    TEK_KEY_V,
    TEK_KEY_W,
    TEK_KEY_X,
    TEK_KEY_Y,
    TEK_KEY_Z,
    TEK_KEY_LEFT_BRACKET,  /* [ */
    TEK_KEY_BACKSLASH,  /* \ */
    TEK_KEY_RIGHT_BRACKET,  /* ] */
    TEK_KEY_ESCAPE,
    TEK_KEY_ENTER,
    TEK_KEY_TAB,
    TEK_KEY_BACKSPACE,
    TEK_KEY_INSERT,
    TEK_KEY_DELETE,
    TEK_KEY_RIGHT,
    TEK_KEY_LEFT,
    TEK_KEY_DOWN,
    TEK_KEY_UP,
    TEK_KEY_PAGE_UP,
    TEK_KEY_PAGE_DOWN,
    TEK_KEY_HOME,
    TEK_KEY_END,
    TEK_KEY_CAPS_LOCK,
    TEK_KEY_SCROLL_LOCK,
    TEK_KEY_NUM_LOCK,
    TEK_KEY_PRINT_SCREEN,
    TEK_KEY_PAUSE,
    TEK_KEY_F1,
    TEK_KEY_F2,
    TEK_KEY_F3,
    TEK_KEY_F4,
    TEK_KEY_F5,
    TEK_KEY_F6,
    TEK_KEY_F7,
    TEK_KEY_F8,
    TEK_KEY_F9,
    TEK_KEY_F10,
    TEK_KEY_F11,
    TEK_KEY_F12,
    TEK_KEY_KP_0,
    TEK_KEY_KP_1,
    TEK_KEY_KP_2,
    TEK_KEY_KP_3,
    TEK_KEY_KP_4,
    TEK_KEY_KP_5,
    TEK_KEY_KP_6,
    TEK_KEY_KP_7,
    TEK_KEY_KP_8,
    TEK_KEY_KP_9,
    TEK_KEY_KP_DECIMAL,
    TEK_KEY_KP_DIVIDE,
    TEK_KEY_KP_MULTIPLY,
    TEK_KEY_KP_SUBTRACT,
    TEK_KEY_KP_ADD,
    TEK_KEY_LEFT_SHIFT,
    TEK_KEY_LEFT_CONTROL,
    TEK_KEY_LEFT_ALT,
    TEK_KEY_LEFT_SUPER,
    TEK_KEY_RIGHT_SHIFT,
    TEK_KEY_RIGHT_CONTROL,
    TEK_KEY_RIGHT_ALT,
    TEK_KEY_RIGHT_SUPER,
    TEK_KEY_MENU,
    TEK_KEY_NONE,
    TEK_KEY_COUNT
}tek_key;

typedef enum  {
    TEK_MOUSEBUTTON_NONE = 0,
    TEK_MOUSEBUTTON_LEFT,
    TEK_MOUSEBUTTON_MIDDLE,
    TEK_MOUSEBUTTON_RIGHT,
    TEK_MOUSEBUTTON_4,
    TEK_MOUSEBUTTON_5,
    TEK_MOUSEBUTTON_6,
    TEK_MOUSEBUTTON_7,
    TEK_MOUSEBUTTON_8,
    TEK_MOUSEBUTTON_9,
    TEK_MOUSEBUTTON_10,
    TEK_MOUSEBUTTON_11,
    TEK_MOUSEBUTTON_12,
    TEK_MOUSEBUTTON_13,
    TEK_MOUSEBUTTON_14,
    TEK_MOUSEBUTTON_15,
    TEK_MOUSEBUTTON_COUNT
}tek_mousebutton;

typedef enum  {
    TEK_GAMEPADBUTTON_A,
    TEK_GAMEPADBUTTON_B,
    TEK_GAMEPADBUTTON_X,
    TEK_GAMEPADBUTTON_Y,
    TEK_GAMEPADBUTTON_LSHOULDER,
    TEK_GAMEPADBUTTON_RSHOULDER,
    TEK_GAMEPADBUTTON_LTHUMB,
    TEK_GAMEPADBUTTON_RTHUMB,
    TEK_GAMEPADBUTTON_START,
    TEK_GAMEPADBUTTON_SELECT,
    TEK_GAMEPADBUTTON_DPAD_LEFT,
    TEK_GAMEPADBUTTON_DPAD_RIGHT,
    TEK_GAMEPADBUTTON_DPAD_UP,
    TEK_GAMEPADBUTTON_DPAD_DOWN,
    TEK_GAMEPADBUTTON_COUNT
}tek_gamepad_button;

typedef enum  {
    TEK_GAMEPADAXIS_LEFTX,
    TEK_GAMEPADAXIS_LEFTY,
    TEK_GAMEPADAXIS_RIGHTX,
    TEK_GAMEPADAXIS_RIGHTY,
    TEK_GAMEPADAXIS_LTRIGGER,
    TEK_GAMEPADAXIS_RTRIGGER,
    TEK_GAMEPADAXIS_COUNT
}tek_gamepad_axis;


//=========================================================================
// window
//=========================================================================

static const u32 g_window_width = 1280;
static const u32 g_window_height = 720;

static b32 g_close_requested = false;
static b32 g_fullscreen = false;

static HDC g_hdc;
static HWND g_hwnd;
static HGLRC g_hglrc;

static b32 g_keys[512];
static b32 g_buttons[24];

static LRESULT CALLBACK WndProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);
static int get_scancode(tek_key key);

b32 window_open(HINSTANCE h_instance, u32 width, u32 height, const char* title, b32 fullscreen)
{
    RECT window_rect = {};
    window_rect.left   = 0;
    window_rect.right  = width;
    window_rect.top    = 0;
    window_rect.bottom = height;

    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszClassName = title;
    if(!RegisterClass(&wc))
    {
        return false;
    }

    DEVMODE dev = {};
    dev.dmSize = sizeof(dev);
    dev.dmPelsWidth = width;
    dev.dmPelsHeight = height;
    dev.dmBitsPerPel = 32;
    dev.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
    DWORD dw_ex_style = 0;
    DWORD dw_style = 0;
    if(fullscreen)
    {
        dw_ex_style = WS_EX_APPWINDOW;
        dw_style = WS_POPUP;
        ChangeDisplaySettings(&dev,CDS_FULLSCREEN);
    } else{
        dw_ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dw_style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
    }

    AdjustWindowRectEx(&window_rect, dw_style, FALSE, dw_ex_style);

    g_hwnd = CreateWindowEx(
            dw_ex_style,
            title,title,
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dw_style,
            CW_USEDEFAULT, 0,
            window_rect.right - window_rect.left,
            window_rect.bottom - window_rect.top,
            nullptr,nullptr,
            h_instance,
            nullptr);

    if(!g_hwnd)
    {
        return false;
    }

    //opengl context
    g_hdc = GetDC(g_hwnd);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixel_format = ChoosePixelFormat(g_hdc, &pfd);
    if(pixel_format == 0)
    {
        return false;
    }

    int result = SetPixelFormat(g_hdc, pixel_format, &pfd);
    if(!result)
    {
        return false;
    }

    int attributes[] ={
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
    };

    HGLRC temp_context = wglCreateContext(g_hdc);
    wglMakeCurrent(g_hdc, temp_context);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
    if(wglCreateContextAttribsARB)
    {
        g_hglrc = wglCreateContextAttribsARB(g_hdc, nullptr, attributes);
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(temp_context);
        wglMakeCurrent(g_hdc, g_hglrc);

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
        if(wglSwapIntervalEXT)
        {
            //no vsync
            wglSwapIntervalEXT(0);
        }
    }
    else
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(temp_context);
        return false;
    }

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);

    return true;
}

void window_close()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(g_hglrc);
}

//=========================================================================
// input
//=========================================================================

struct tek_keyboard_state
{
    b32 m_down[TEK_KEY_COUNT];
    b32 m_pressed[TEK_KEY_COUNT];
    b32 m_released[TEK_KEY_COUNT];
};

struct tek_mouse_state
{
    b32 m_down[TEK_MOUSEBUTTON_COUNT];
    b32 m_pressed[TEK_MOUSEBUTTON_COUNT];
    b32 m_released[TEK_MOUSEBUTTON_COUNT];
};

struct tek_gamepad_state
{
    b32 m_connected;
    b32 m_down[TEK_GAMEPADBUTTON_COUNT];
    b32 m_pressed[TEK_GAMEPADBUTTON_COUNT];
    b32 m_released[TEK_GAMEPADBUTTON_COUNT];

    int m_axis[TEK_GAMEPADAXIS_COUNT];
};

struct tek_input_state
{
    tek_keyboard_state m_keyboard_state;
    tek_mouse_state m_mouse_state;
    tek_gamepad_state m_gamepad_state[4];
};

static tek_input_state g_input_state;

void input_update()
{
    for(u32 i = 0; i < TEK_KEY_COUNT; ++i)
    {
        int scancode = get_scancode(static_cast<tek_key>(i));
        if(scancode < 0 || scancode >= 512)
            continue;

        b32 down = g_keys[scancode];

        if(down)
        {

            if(g_input_state.m_keyboard_state.m_down[i])
            {
                g_input_state.m_keyboard_state.m_pressed[i] = false;
            }
            else
            {
                g_input_state.m_keyboard_state.m_pressed[i] = true;
            }
            g_input_state.m_keyboard_state.m_down[i] = true;
            g_input_state.m_keyboard_state.m_released[i] = false;
        }
        else
        {
            if(g_input_state.m_keyboard_state.m_down[i])
            {
                g_input_state.m_keyboard_state.m_released[i] = true;
            }
            else
            {
                g_input_state.m_keyboard_state.m_released[i] = false;
            }
            g_input_state.m_keyboard_state.m_down[i] = false;
            g_input_state.m_keyboard_state.m_pressed[i] = false;
        }

    }

}

//=========================================================================
// timing
//=========================================================================

static DWORD g_start_ticks;

void time_init()
{
    g_start_ticks = GetTickCount64();
}

f64 time_get_seconds()
{
    DWORD ticks = GetTickCount64();
    DWORD new_ticks = ticks - g_start_ticks;
    f64 seconds = new_ticks / 1000.0;
    return seconds;
}

void time_sleep(i32 milliseconds)
{
    Sleep(milliseconds);
}

//=========================================================================
// opengl
//=========================================================================

#define GL_FUNCTION(x, y)PFN## x ##PROC y = NULL;

#include "gl_functions.inl"

static void load_gl_extensions()
{
#undef GL_FUNCTION
#ifdef TEK_PLATFORM_WIN32
#define GL_FUNCTION(x,y)y = (PFN## x ##PROC)wglGetProcAddress(#y);
#elif TEK_PLATFORM_LINUX
#define GL_FUNCTION(x, y)y = (PFN## x ##PROC)glXGetProcAddress((const GLubyte*)#y);
#endif

#include "gl_functions.inl"

    //TODO: some kind of check if the functions pointers are correct
}

static std::string shader_load_source(const char* filename)
{
    std::ifstream in_file(filename);
    if(!in_file.is_open()) return "";

    std::stringstream buffer;
    buffer << in_file.rdbuf();

    return buffer.str();
}

static GLuint shader_create(const char* src, GLenum type)
{
    switch(type)
    {
        case GL_VERTEX_SHADER:
        case GL_FRAGMENT_SHADER:
        case GL_GEOMETRY_SHADER:
            break;
        default:
        {
            printf("ERROR: Invalid Shader type\n");
            return 0;
        }
    }

    GLuint id = glCreateShader(type);

    const char* c_src = src;
    glShaderSource(id, 1, &c_src, NULL);
    glCompileShader(id);

    GLint result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        int info_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length);
        char* error_message = new char[info_length + 1];
        glGetShaderInfoLog(id, info_length, NULL, &error_message[0]);
        printf("ERROR: Shader compilation error: %s \n", error_message);
        glDeleteShader(id);
        delete[] error_message;
        return 0;
    }

    return id;
}

static GLuint shader_link(GLuint vert_id, GLuint frag_id, GLuint geom_id)
{
    GLuint program = glCreateProgram();

    glAttachShader(program, vert_id);
    glAttachShader(program, frag_id);
    if(geom_id > 0)
    {
        glAttachShader(program, geom_id);
    }

    glLinkProgram(program);
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if(!result)
    {
        int info_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
        char* error_message = new char[info_length + 1];
        glGetProgramInfoLog(program, info_length, NULL, &error_message[0]);
        printf("ERROR: Shader linking error: %s\n", error_message);
        glDeleteProgram(program);
        delete[] error_message;
        return 0;
    }

    return program;
}

static u32 shader_load(const char* vertex_filename, const char* fragment_filename, const char* geometry_filename)
{
    assert(vertex_filename && fragment_filename);
    std::string vertex_source = shader_load_source(vertex_filename);
    std::string fragment_source = shader_load_source(fragment_filename);
    u32 vertex_id = shader_create(vertex_source.c_str(),GL_VERTEX_SHADER);
    u32 fragment_id = shader_create(fragment_source.c_str(),GL_FRAGMENT_SHADER);
    u32 geometry_id = 0;
    if(geometry_filename)
    {
        std::string geometry_source = shader_load_source(geometry_filename);
        geometry_id = shader_create(geometry_source.c_str(),GL_GEOMETRY_SHADER);
    }
    u32 result = shader_link(vertex_id, fragment_id, geometry_id);
    return result;
}

//=========================================================================
// main
//=========================================================================

INT WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PSTR p_cmd_line, int n_cmd_show)
{
    //platform initialization
    time_init();
    if(!window_open(h_instance,g_window_width, g_window_height, "argos", false))
    {
        return 1;
    }

    //graphics initialization
    load_gl_extensions();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    //main loop
    b32 is_running = true;
    MSG msg = {};
    while(is_running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        is_running = !g_close_requested;
        input_update();
        if(g_input_state.m_keyboard_state.m_pressed[TEK_KEY_ESCAPE])
        {
            is_running = false;
        }

        //render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SwapBuffers(g_hdc);
    }

    window_close();
    return 0;
}

static LRESULT CALLBACK WndProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
    switch (message) {
        case WM_SIZE: {
            int w = LOWORD(l_param);
            int h = HIWORD(l_param);
        }break;
        case WM_DESTROY:
            g_close_requested = true;
            //PostQuitMessage(0);
            break;
        case WM_KEYDOWN:{
            g_keys[w_param] = true;
        }break;
        case WM_KEYUP:{
            g_keys[w_param] = false;
        }break;
        case WM_LBUTTONDOWN: {
            g_buttons[0] = true;
        }break;
        case WM_LBUTTONUP: {
            g_buttons[0] = false;
        }break;
        case WM_MBUTTONDOWN: {
            g_buttons[1] = true;
        }break;
        case WM_MBUTTONUP: {
            g_buttons[1] = false;
        }break;
        case WM_RBUTTONDOWN: {
            g_buttons[2] = true;
        }break;
        case WM_RBUTTONUP: {
            g_buttons[2] = false;
        }break;
        default:
            break;
    }
    return DefWindowProc(h_wnd, message, w_param, l_param);
}

static int get_scancode(tek_key key)
{
    //TODO: change to a table lookup
    switch (key)
    {
        case TEK_KEY_SPACE: {
            return VK_SPACE;
        }break;
        case TEK_KEY_APOSTROPHE: {
            return VK_OEM_7;
        }break;
        case TEK_KEY_COMMA: {  /* , */
            return VK_OEM_COMMA;
        }break;
        case TEK_KEY_MINUS: {  /* - */
            return VK_OEM_MINUS;
        }break;
        case TEK_KEY_PERIOD: {  /* . */
            return VK_OEM_PERIOD;
        }break;
        case TEK_KEY_SLASH: {  /* / */
            return VK_OEM_2;
        }break;
        case TEK_KEY_0: {
            return 0x30;
        }break;
        case TEK_KEY_1: {
            return 0x31;
        }break;
        case TEK_KEY_2: {
            return 0x32;
        }break;
        case TEK_KEY_3: {
            return 0x33;
        }break;
        case TEK_KEY_4: {
            return 0x34;
        }break;
        case TEK_KEY_5: {
            return 0x35;
        }break;
        case TEK_KEY_6: {
            return 0x36;
        }break;
        case TEK_KEY_7: {
            return 0x37;
        }break;
        case TEK_KEY_8: {
            return 0x38;
        }break;
        case TEK_KEY_9: {
            return 0x39;
        }break;
        case TEK_KEY_SEMICOLON: {
            return VK_OEM_1;
        }break;
        case TEK_KEY_EQUAL: {
            return 187;
        }break; //TODO: find scancode
        case TEK_KEY_A: {
            return 0x41;
        }break;
        case TEK_KEY_B: {
            return 0x42;
        }break;
        case TEK_KEY_C: {
            return 0x43;
        }break;
        case TEK_KEY_D: {
            return 0x44;
        }break;
        case TEK_KEY_E: {
            return 0x45;
        }break;
        case TEK_KEY_F: {
            return 0x46;
        }break;
        case TEK_KEY_G: {
            return 0x47;
        }break;
        case TEK_KEY_H: {
            return 0x48;
        }break;
        case TEK_KEY_I: {
            return 0x49;
        }break;
        case TEK_KEY_J: {
            return 0x4A;
        }break;
        case TEK_KEY_K: {
            return 0x4B;
        }break;
        case TEK_KEY_L: {
            return 0x4C;
        }break;
        case TEK_KEY_M: {
            return 0x4D;
        }break;
        case TEK_KEY_N: {
            return 0x4E;
        }break;
        case TEK_KEY_O: {
            return 0x4F;
        }break;
        case TEK_KEY_P: {
            return 0x50;
        }break;
        case TEK_KEY_Q: {
            return 0x51;
        }break;
        case TEK_KEY_R: {
            return 0x52;
        }break;
        case TEK_KEY_S: {
            return 0x53;
        }break;
        case TEK_KEY_T: {
            return 0x54;
        }break;
        case TEK_KEY_U: {
            return 0x55;
        }break;
        case TEK_KEY_V: {
            return 0x56;
        }break;
        case TEK_KEY_W: {
            return 0x57;
        }break;
        case TEK_KEY_X: {
            return 0x58;
        }break;
        case TEK_KEY_Y: {
            return 0x59;
        }break;
        case TEK_KEY_Z: {
            return 0x5A;
        }break;
        case TEK_KEY_LEFT_BRACKET: {
            return VK_OEM_4;
        }break;
        case TEK_KEY_BACKSLASH: {
            return VK_OEM_5;
        }break;
        case TEK_KEY_RIGHT_BRACKET: {  /* ] */
            return VK_OEM_6;
        }break;
        case TEK_KEY_ESCAPE: {
            return VK_ESCAPE;
        }break;
        case TEK_KEY_ENTER: {
            return VK_RETURN;
        }break;
        case TEK_KEY_TAB: {
            return VK_TAB;
        }break;
        case TEK_KEY_BACKSPACE: {
            return VK_BACK;
        }break;
        case TEK_KEY_INSERT: {
            return VK_INSERT;
        }break;
        case TEK_KEY_DELETE: {
            return VK_DELETE;
        }break;
        case TEK_KEY_RIGHT: {
            return VK_RIGHT;
        }break;
        case TEK_KEY_LEFT: {
            return VK_LEFT;
        }break;
        case TEK_KEY_DOWN: {
            return VK_DOWN;
        }break;
        case TEK_KEY_UP: {
            return VK_UP;
        }break;
        case TEK_KEY_PAGE_UP: {
            return VK_PRIOR;
        }break;
        case TEK_KEY_PAGE_DOWN: {
            return VK_NEXT;
        }break;
        case TEK_KEY_HOME: {
            return VK_HOME;
        }break;
        case TEK_KEY_END: {
            return VK_END;
        }break;
        case TEK_KEY_CAPS_LOCK: {
            return VK_CAPITAL;
        }break;
        case TEK_KEY_SCROLL_LOCK: {
            return VK_SCROLL;
        }break;
            /*case 0x41: {
            return KEY_NUM_LOCK;
            }break;*///TODO: find scancode
        case TEK_KEY_PRINT_SCREEN: {
            return VK_PRINT;
        }break;
        case TEK_KEY_PAUSE: {
            return VK_PAUSE;
        }break;
        case TEK_KEY_F1: {
            return VK_F1;
        }break;
        case TEK_KEY_F2: {
            return VK_F2;
        }break;
        case TEK_KEY_F3: {
            return VK_F3;
        }break;
        case TEK_KEY_F4: {
            return VK_F4;
        }break;
        case TEK_KEY_F5: {
            return VK_F5;
        }break;
        case TEK_KEY_F6: {
            return VK_F6;
        }break;
        case TEK_KEY_F7: {
            return VK_F7;
        }break;
        case TEK_KEY_F8: {
            return VK_F8;
        }break;
        case TEK_KEY_F9: {
            return VK_F9;
        }break;
        case TEK_KEY_F10: {
            return VK_F10;
        }break;
        case TEK_KEY_F11: {
            return VK_F11;
        }break;
        case TEK_KEY_F12: {
            return VK_F12;
        }break;
        case TEK_KEY_KP_0: {
            return VK_NUMPAD0;
        }break;
        case TEK_KEY_KP_1: {
            return VK_NUMPAD1;
        }break;
        case TEK_KEY_KP_2: {
            return VK_NUMPAD2;
        }break;
        case TEK_KEY_KP_3: {
            return VK_NUMPAD3;
        }break;
        case TEK_KEY_KP_4: {
            return VK_NUMPAD4;
        }break;
        case TEK_KEY_KP_5: {
            return VK_NUMPAD5;
        }break;
        case TEK_KEY_KP_6: {
            return VK_NUMPAD6;
        }break;
        case TEK_KEY_KP_7: {
            return VK_NUMPAD7;
        }break;
        case TEK_KEY_KP_8: {
            return VK_NUMPAD8;
        }break;
        case TEK_KEY_KP_9: {
            return VK_NUMPAD9;
        }break;
        case TEK_KEY_KP_DECIMAL: {
            return VK_DECIMAL;
        }break;
        case TEK_KEY_KP_DIVIDE: {
            return VK_DIVIDE;
        }break;
        case TEK_KEY_KP_MULTIPLY: {
            return VK_MULTIPLY;
        }break;
        case TEK_KEY_KP_SUBTRACT: {
            return VK_SUBTRACT;
        }break;
        case TEK_KEY_KP_ADD: {
            return VK_ADD;
        }break;
            /*case KEY_LEFT_SHIFT: {
                return VK_LSHIFT;
            }break;*/
        case TEK_KEY_LEFT_SHIFT: {
            return 16;
        }break;
        case TEK_KEY_LEFT_CONTROL: {
            return 17;
        }break;
            /*case KEY_LEFT_CONTROL: {
                return VK_LCONTROL;
            }break;*/
        case TEK_KEY_LEFT_ALT: {
            return VK_LMENU;
        }break;
        case TEK_KEY_LEFT_SUPER: {
            return VK_LWIN;
        }break;
            /*case KEY_RIGHT_SHIFT: {
                return VK_RSHIFT;
            }break;*/
        case TEK_KEY_RIGHT_SHIFT: {
            return 16;
        }break;
        case TEK_KEY_RIGHT_CONTROL: {
            return 17;
        }break;
            /*case KEY_RIGHT_CONTROL: {
                return VK_RCONTROL;
            }break;*/
        case TEK_KEY_RIGHT_ALT: {
            return VK_RMENU;
        }break;
        case TEK_KEY_RIGHT_SUPER: {
            return VK_RWIN;
        }break;
        case TEK_KEY_MENU: {
            return VK_APPS;
        }break;
        default:
            break;
    }
    return -1;
}
