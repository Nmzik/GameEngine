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
    
    game = new Game("/Volumes/BOOTCAMP/Program\ Files/Rockstar\ Games/Grand\ Theft\ Auto\ V/");
    
    [self initMetal];
}

- (void)loadView {
    self.view = [[MTKView alloc] init];
}

-(void)initMetal {
    
    _view = (MTKView *)self.view;
    
    MetalRenderer* renderer = static_cast<MetalRenderer*>(game->getRenderer());
    _view.device = renderer->device;
    renderer->mtkView = _view;
    
    if (!_view) {
        NSLog(@"Not supported");
        return;
    }
    
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    _view.framebufferOnly = true;
    _view.delegate = self;
    
    CGRect newFrame = CGRectMake( _view.frame.origin.x, _view.frame.origin.y, 1024, 576);
    _view.frame = newFrame;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    NSPoint event_location = [NSEvent mouseLocation];
    //NSLog(@"%f %d", event_location.x, event_location.y);
    game->frame();
    game->getInput()->setMouseMovement(event_location.x, event_location.y);
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    NSLog(@"HERE");
}

- (void)keyDown:(NSEvent *)event
{
    game->getInput()->processButton(event.keyCode, true);
}

-(void)keyUp:(NSEvent *)event
{
    game->getInput()->processButton(event.keyCode, false);
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

@end
