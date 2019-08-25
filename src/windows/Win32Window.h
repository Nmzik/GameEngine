#pragma once
#include "../NativeWindow.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>

class Win32Window : public NativeWindow
{
    HWND hwnd;
    HDC m_HDC;

public:
    Win32Window();
    ~Win32Window();

    const wchar_t* getName() const
    {
        return L"OpenGL Program";
    }
    virtual void initializeContext();
    virtual void setTitle(std::string& osstr);
    virtual void processEvents();
    virtual void swapBuffers();
};
