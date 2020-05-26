//
// Created by nmzik on 5/21/2020.
//
#include <unistd.h>
#include <android_native_app_glue.h>
#include <jni.h>
#include <cstdio>
#include <android/log.h>
#include "../../../../../GameEngine/src/Game.h"
#include "../../../../../GameEngine/src/android/AndroidWindow.h"
#include "../../../../../GameEngine/src/vulkan/VulkanRenderer.h"

Game* game = nullptr;
bool isReady = false;

extern "C" {
void handle_cmd(android_app *pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            AndroidWindow *window = (AndroidWindow *) game->getNativeWindow();
            window->setNativeWindow(pApp->window);

            int screenWidth = ANativeWindow_getWidth(pApp->window);
            int screenHeight = ANativeWindow_getHeight(pApp->window);
            game->initializeCamera(screenWidth, screenHeight);

            VulkanRenderer *renderer = (VulkanRenderer *) game->getRenderer();
            std::string shadersPath = pApp->activity->internalDataPath;
            shadersPath += "/";
            renderer->setShadersPath(shadersPath);

            renderer->initialize();
            game->postLoad();
            isReady = true;
            break;
        }
        case APP_CMD_TERM_WINDOW:
            printf("EXIT");
            break;
    }
}

extern "C" {
int32_t handleInput(android_app *app, AInputEvent *event) {

}
}

static int pfd[2];
static pthread_t thr;
static const char *tag = "myapp";

static void *thread_func(void*)
{
    ssize_t rdsz;
    char buf[128];
    while((rdsz = read(pfd[0], buf, sizeof buf - 1)) > 0) {
        if(buf[rdsz - 1] == '\n') --rdsz;
        buf[rdsz] = 0;  /* add null-terminator */
        __android_log_write(ANDROID_LOG_DEBUG, tag, buf);
    }
    return 0;
}

int start_logger(const char *app_name)
{

    /* make stdout line-buffered and stderr unbuffered */
    setvbuf(stdout, 0, _IOLBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);

    /* create the pipe and redirect stdout and stderr */
    pipe(pfd);
    dup2(pfd[1], 1);
    dup2(pfd[1], 2);

    /* spawn the logging thread */
    if(pthread_create(&thr, 0, thread_func, 0) == -1)
        return -1;
    pthread_detach(thr);
    return 0;
}

void android_main(struct android_app *pApp) {
    start_logger("MapViewer");

    pApp->onAppCmd = handle_cmd;
    //pApp->onInputEvent = handleInput;

    const char* internal = pApp->activity->internalDataPath;
    const char* external = pApp->activity->externalDataPath;

    std::string keyPath = internal;
    keyPath += "/assets/compressed.dat";

    std::string mainPath = internal;
    mainPath += "/";

    game = new Game(mainPath.c_str(), keyPath.c_str());

    int events;
    android_poll_source *pSource;
    do {
        while (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(pApp, pSource);
            }
        }

        if (isReady)
            game->frame();

    } while (!pApp->destroyRequested);
}
}