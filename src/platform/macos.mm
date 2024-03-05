//
//  macos.m
//  renderer
//
//  Created by xuyuehui on 2024/2/27.
//

#include <Cocoa/Cocoa.h>
#include <mach-o/dyld.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include "platform.h"
#include "../renderer/renderer.h"

#define PATH_SIZE 256

using namespace CG;

class MacosApplication : public Application {
public:
    MacosApplication(int argc, char **argv) : closing(false){
        renderer = CreateRendererAPI(RAPI_SOFT);
    }
    
    ~MacosApplication() {
    }
    
    bool ShouldClosed();
    void PoolEvents();
    void Exit();

    Renderer *GetRenderer();
public:
    void OnInitialize();
    void OnDeinitialize();
private:
    bool closing;
    Renderer *renderer;
};

MacosApplication *app;
NSAutoreleasePool *autoReleasePool;

inline bool MacosApplication::ShouldClosed() {
    return closing;
}

inline void MacosApplication::PoolEvents() {
    NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                            untilDate:[NSDate distantPast]
                            inMode:NSDefaultRunLoopMode dequeue:YES];
    
    if (event == nil) {
        return;
    }
    
    [NSApp sendEvent:event];
}

inline void MacosApplication::Exit() {
    closing = true;
}

inline Renderer *MacosApplication::GetRenderer() {
    return renderer;
}

class MacosWindow : public Window {
public:
    MacosWindow() :handle(NULL), width(1080), height(768) {
    }
    
    ~MacosWindow() {
    }
    
    void SetKeyboardCallback(keyboardCallback_t callback);
    void *GetHandle() const;
    void GetSize(int &width, int &height) const;
    void SwapBuffer();
public:
    NSWindow *handle;
    int width;
    int height;
    
    byte *colorBuffer;
};

void MacosWindow::SetKeyboardCallback(keyboardCallback_t callback) {
}

void *MacosWindow::GetHandle() const {
    return handle;
}

void MacosWindow::GetSize(int &width, int &height) const {
    width = this->width;
    height = this->height;
}

static void Blit2ColorBuffer(const byte *from, byte *to, int width, int height) {    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int srcIndex = (y * width + x) * 3;
            int dstIndex = ((height - y - 1) * width + x) * 3;

            dst[dstIndex + 0] = src[srcIndex + 0];
            dst[dstIndex + 1] = src[srcIndex + 1];
            dst[dstIndex + 2] = src[srcIndex + 2];
        }
    }
}

void MacosWindow::SwapBuffer() {
    if (colorBuffer == NULL) {
        colorBuffer = new byte[width * height * 3];
    }
    
    renderTargetDesc_t rtd;
    app->GetRenderer()->GetColorBufferDesc(rtd);
    
    Blit2ColorBuffer(rtd.data, colorBuffer, width, height);

    [[handle contentView] setNeedsDisplay:YES];
}

static void InitializeMenubar() {
    NSMenu *menuBar, *appMenu;
    NSMenuItem *appMenuItem, *quitMenuItem;
    NSString *appName, *quitTitle;
    
    menuBar = [[[NSMenu alloc] init] autorelease];
    [NSApp setMainMenu:menuBar];
    
    appMenuItem = [[[NSMenuItem alloc] init] autorelease];
    [menuBar addItem:appMenuItem];
    
    appMenu = [[[NSMenu alloc] init] autorelease];
    [appMenuItem setSubmenu:appMenu];
    
    appName = [[NSProcessInfo processInfo] processName];
    quitTitle = [@"Quit " stringByAppendingString:appName];
    quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
                                        action:@selector(terminate:)
                                        keyEquivalent:@"q"] autorelease];
    
    [appMenu addItem:quitMenuItem];
}

static void InitializeWorkingDir() {
    char path[PATH_SIZE];
    uint32_t size = PATH_SIZE;
    
    _NSGetExecutablePath(path, &size);
    *strrchr(path, '/') = '\0';
    chdir(path);
}

void MacosApplication::OnInitialize() {
    if (!NSApp) {
        autoReleasePool = [[NSAutoreleasePool alloc] init];
        
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        //InitializeWorkingDir();
        InitializeMenubar();
        
        [NSApp finishLaunching];
    }
}

void MacosApplication::OnDeinitialize() {
    [autoReleasePool drain];
}

@interface WindowDelegate : NSObject<NSWindowDelegate>
@end

@implementation WindowDelegate {
    MacosWindow *window;
}
- (instancetype)initWithWindow:(MacosWindow *)window {
    self = [super init];
    if (self != nil) {
        self->window = window;
    }
    
    return self;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    app->Exit();
    return NO;
}

@end

@interface ContentView : NSView
@end

@implementation ContentView {
    MacosWindow *window;
}

- (instancetype)initWithWindow:(MacosWindow *)window {
    self = [super init];
    if (self != nil) {
        self->window = window;
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    NSBitmapImageRep *rep = [[[NSBitmapImageRep alloc]
                              initWithBitmapDataPlanes:&window->colorBuffer
                              pixelsWide:window->width
                              pixelsHigh:window->height
                              bitsPerSample:8
                              samplesPerPixel:3
                              hasAlpha:NO
                              isPlanar:NO
                              colorSpaceName:NSCalibratedRGBColorSpace
                              bytesPerRow:window->width*3
                              bitsPerPixel:24] autorelease];
    
    NSImage *image = [[[NSImage alloc] init] autorelease];
    [image addRepresentation:rep];
    [image drawInRect:dirtyRect];
}

@end

ContentView *view;

Application *CG::InitializeApplication(int argc, char **argv) {
    app = new MacosApplication(argc, argv);
    app->OnInitialize();
    return app;
}

Window *CG::CreateRenderWindow(int width, int height, const char *title) {
    NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable;
    NSRect windowRect = NSMakeRect(0, 0, width, height);
    
    NSWindow *handle = [[NSWindow alloc] initWithContentRect:windowRect
                                         styleMask:windowStyle
                                         backing:NSBackingStoreBuffered
                                         defer:NO];
    [handle autorelease];
    
    MacosWindow *window = new MacosWindow();
    window->handle = handle;
    
    WindowDelegate *delegate = [[WindowDelegate alloc] initWithWindow:window];
    [handle setDelegate:delegate];
    
    view = [[[ContentView alloc] initWithWindow:window] autorelease];
    
    [handle setTitle:[NSString stringWithUTF8String:title]];
    [handle setColorSpace:[NSColorSpace genericRGBColorSpace]];
    [handle setContentView:view];
    [handle makeFirstResponder:view];
    [handle orderFrontRegardless];
    [handle makeKeyAndOrderFront:nil];
    
    return window;
}

Application *CG::App() {
    return app;
}
