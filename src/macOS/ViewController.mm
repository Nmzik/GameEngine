//
//  metalViewController.m
//  MyGameEngine
//
//  Created by nmzik on 13.08.2019.
//

#import "ViewController.h"

#include "../Game.h"
#include "../InputManager.h"
#include "../metal/MetalRenderer.h"

@implementation ViewController
{
    id <MTLDevice> device;
    MTKView* _view;
    
    Game* game;
}


-(void)viewDidLoad {
    [super viewDidLoad];
    
    [self initMetal];
    game = new Game("/Volumes/BOOTCAMP/Program Files/Rockstar Games/Grand Theft Auto V/");
    
    MetalRenderer* renderer = static_cast<MetalRenderer*>(game->getRenderer());
    renderer->device = device;
    renderer->mtkView = _view;
    
    game->initializeCamera(_view.frame.size.width, _view.frame.size.height);
    renderer->initializeRenderEngine();
    
    _view.delegate = self;
}

- (void)loadView {
    self.view = [[MTKView alloc] init];
}

-(void)initMetal {
    
    device = MTLCreateSystemDefaultDevice();
    _view = (MTKView *)self.view;
    
    if (!_view) {
        NSLog(@"Not supported");
        return;
    }
    
    _view.device = device;
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    _view.depthStencilPixelFormat = MTLPixelFormatDepth32Float;
    _view.framebufferOnly = true;
    //_view.delegate = self;
    
    CGRect newFrame = CGRectMake( _view.frame.origin.x, _view.frame.origin.y, 1024, 576);
    _view.frame = newFrame;
    
    NSLog(@"METAL RENDERER IS READY!");
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    game->frame();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    NSLog(@"HERE");
}

- (void)mouseMoved:(NSEvent *)event
{
    NSPoint locationInView = [_view convertPoint:[event locationInWindow] fromView:_view];
    game->getInput()->setMouseMovement(locationInView.x / 2, locationInView.y / 2);
    //NSLog(@"%f %f", test.x, test.y);
}

- (void)keyDown:(NSEvent *)event
{
    game->getInput()->processButton(event.keyCode, true);
}

- (void) flagsChanged:(NSEvent *) event {
    if ([event modifierFlags] & NSEventModifierFlagShift) {
        game->getInput()->processButton(event.keyCode, true);
    }
}

-(void)keyUp:(NSEvent *)event
{
    game->getInput()->processButton(event.keyCode, false);
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

@end
