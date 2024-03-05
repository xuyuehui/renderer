#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "platform.h"
#include "../renderer/renderer.h"

using namespace std;

namespace CG {
    static const char *s_className = "Renderer";

    // Declare
    class Win32Application;
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    Win32Application *app;
    std::map<HWND, Window *> s_windows;

    static Window * GetWindow(HWND hwnd) {
        std::map<HWND, Window *>::iterator it = s_windows.find(hwnd);
        if (it == s_windows.end()) {
            return NULL;
        }

        return it->second;
    }

    class Win32Application : public Application {
    public:
        Win32Application(int argc, char **argv) : closing(false){
            instance = NULL;
            renderer = CreateRendererAPI(RAPI_SOFT);
        }

        ~Win32Application() {
            OnDeinitialize();
        }

        bool ShouldClosed() {
            return closing;
        }

        void PoolEvents() {
            static MSG msg;
            if (GetMessage(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        void Exit() {
            closing = true;
        }

        Renderer *GetRenderer() {
            return renderer;
        }

    public:
        bool OnInitialize() {
            WNDCLASSEX wc;
            instance = GetModuleHandle(NULL);

            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = 0;
            wc.lpfnWndProc = WndProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = instance;
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.lpszMenuName = NULL;
            wc.lpszClassName = s_className;
            wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

            if (!RegisterClassEx(&wc)) {
                MessageBox(NULL, "register class failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            }

            return true;
        }

        void OnDeinitialize() {
            for (std::map<HWND, Window *>::iterator it = s_windows.begin(); it != s_windows.end(); it++) {
                delete it->second;
            }

            s_windows.clear();
        }

        HINSTANCE GetInstance() { return instance; }
    private:
        bool		closing;
        HINSTANCE	instance;
        Renderer	*renderer;
    };

    Application * InitializeApplication(int argc, char **argv) {
        if (app != NULL) {
            return app;
        }

        app = new Win32Application(argc, argv);
        app->OnInitialize();

        return app;
    }

    Application *App() {
        return app;
    }

    static void Blit2ColorBuffer(const byte *src, byte *dst, int width, int height) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int srcIndex = (y * width + x) * 3;
                int dstIndex = ((height - y - 1) * width + x) * 3;

                dst[dstIndex + 0] = src[srcIndex + 2];
                dst[dstIndex + 1] = src[srcIndex + 1];
                dst[dstIndex + 2] = src[srcIndex + 0];
            }
        }
    }

    class Win32Window : public Window {
    public:
        Win32Window(HWND hwnd, int w, int h) : handle(hwnd), width(w), height(h), keyboardCallback(NULL), colorBuffer(NULL) {
            s_windows.insert(make_pair(hwnd, this));
        }

        ~Win32Window() {
            delete[] colorBuffer;
        }

        void SetKeyboardCallback(keyboardCallback_t callback) {
            keyboardCallback = callback;
        }

        void *GetHandle() const {
            return (void*)handle;
        }

        void GetSize(int &width, int &height) const{
            width = this->width;
            height = this->height;
        }

        void SwapBuffer(const byte *buffer, int width, int height) {
            HWND hwnd = (HWND)handle;

            if (colorBuffer == NULL) {
                colorBuffer = new byte[width * height * 3];
            }

            Blit2ColorBuffer(buffer, colorBuffer, width, height);

            BITMAPINFO bmi;
            HDC hdc = GetDC(hwnd);

            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biBitCount = 24;
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height;
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biClrUsed = 0;
            bmi.bmiHeader.biClrImportant = 0;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biSizeImage = 0;

            SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, colorBuffer, &bmi, DIB_RGB_COLORS);

            ReleaseDC(hwnd, hdc);
        }

    public:
        keyboardCallback_t keyboardCallback;
        byte *colorBuffer;
    private:
        HWND	handle;
        int		width;
        int		height;
    };

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_CLOSE:
            app->Exit();
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }


    Window * CreateRenderWindow(int width, int height, const char *title) {
        HWND hwnd = CreateWindowEx(
            0,
            s_className,
            title,
            WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, app->GetInstance(), NULL);

        if (hwnd == NULL) {
            MessageBox(NULL, "create window failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }

        RECT winRect, cliRect;
        GetWindowRect(hwnd, &winRect);
        GetClientRect(hwnd, &cliRect);

        long wDiff = width - cliRect.right;
        long hDiff = height - cliRect.bottom;

        MoveWindow(hwnd, winRect.left, winRect.top, width + wDiff, height + hDiff, TRUE);

        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~(WS_MAXIMIZEBOX);
        SetWindowLong(hwnd, GWL_STYLE, style);

        ShowWindow(hwnd, SW_SHOWNORMAL);
        UpdateWindow(hwnd);

        return new Win32Window(hwnd, width, height);
    }
}

#endif
