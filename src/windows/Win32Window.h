#pragma once
#include <Windows.h>
#include "../NativeWindow.h"

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
