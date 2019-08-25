#include "Win32Window.h"
#include <windowsx.h>

static Win32Window* GetWindowFromUserData(HWND wnd)
{
    return reinterpret_cast<Win32Window*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
            /* --- Common window events --- */

        case WM_CREATE:
        {
            return 0;
        }
        case WM_KEYDOWN:
        {
            if (auto window = GetWindowFromUserData(hwnd))
            {
                window->processButtonEvent(static_cast<uint32_t>(wParam), true);
            }
            return 0;
        }
        case WM_KEYUP:
        {
            if (auto window = GetWindowFromUserData(hwnd))
            {
                window->processButtonEvent(static_cast<uint32_t>(wParam), false);
            }
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            if (auto window = GetWindowFromUserData(hwnd))
            {
                /*Extract mouse position from event parameter */
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);

                /*Post local mouse motion event */
                window->processMouseEvents(x, y);
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Win32Window::Win32Window()
{
    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = getName();

    RegisterClass(&wc);

    ShowCursor(false);

    hwnd = CreateWindowEx(
        0,                    // Optional window styles.
        getName(),            // Window class
        L"GameEngine",        // Window text
        WS_OVERLAPPEDWINDOW,  // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,                   // Parent window
        NULL,                   // Menu
        GetModuleHandle(NULL),  // Instance handle
        NULL                    // Additional application data
    );

    if (hwnd == NULL)
    {
        return;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

Win32Window::~Win32Window()
{
    DestroyWindow(hwnd);
    UnregisterClass(getName(), GetModuleHandle(nullptr));
}

void Win32Window::initializeContext()
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    m_HDC = GetDC(hwnd);

    int nPixelFormat = ChoosePixelFormat(m_HDC, &pfd);

    if (nPixelFormat == 0)
        printf("SOMETHING WRONG");

    BOOL bResult = SetPixelFormat(m_HDC, nPixelFormat, &pfd);

    if (!bResult) printf("SOMETHING WRONG");

    HGLRC tempContext = wglCreateContext(m_HDC);
    wglMakeCurrent(m_HDC, tempContext);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("GLEW is not initialized!");
    }

    HGLRC m_hrc;
    int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_FLAGS_ARB, 0,
            0};

    if (wglewIsSupported("WGL_ARB_create_context") == 1)
    {
        m_hrc = wglCreateContextAttribsARB(m_HDC, 0, attribs);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tempContext);
        wglMakeCurrent(m_HDC, m_hrc);
    }
    else
    {  //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
        m_hrc = tempContext;
    }

    //Checking GL version
    const GLubyte* GLVersionString = glGetString(GL_VERSION);
    printf("%s\n", GLVersionString);
}

std::wstring get_utf16(const std::string_view& str, int codepage)
{
    if (str.empty()) return std::wstring();
    int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
    std::wstring res(sz, 0);
    MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
    return res;
}

void Win32Window::setTitle(std::string& osstr)
{
    SetWindowText(hwnd, get_utf16(osstr, CP_ACP).c_str());
}

void Win32Window::processEvents()
{
    MSG message;

    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void Win32Window::swapBuffers()
{
    ::SwapBuffers(m_HDC);
}
