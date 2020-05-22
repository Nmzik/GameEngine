#pragma once

#include <android/native_window.h>
#include "../NativeWindow.h"

class AndroidWindow : public NativeWindow
{
    ANativeWindow* nativeWindow;

public:
    AndroidWindow();
    ~AndroidWindow();

    virtual void initializeContext();
    virtual void setTitle(std::string& osstr);
    virtual void processEvents();
    virtual void swapBuffers();

    void setNativeWindow(ANativeWindow* window)
    {
        nativeWindow = window;
    }

    ANativeWindow* getNativeWindow()
    {
        return nativeWindow;
    }
};
