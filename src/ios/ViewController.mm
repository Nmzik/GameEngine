//
//  ViewController.m
//  tetdhff
//
//  Created by nmzik on 18.08.2019.
//  Copyright © 2019 nmzik. All rights reserved.
//

#import "ViewController.h"

#include "../Game.h"
#include "../metal/MetalRenderer.h"
#include "../InputManager.h"

@interface ViewController ()
{
    MTKView* _view;
    id <MTLDevice> device;
    Game* game;
    CGPoint _startTouch;
}
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    CGRect ScreenSize = UIScreen.mainScreen.bounds;
    _view = [[MTKView alloc]initWithFrame:CGRectMake(0, 0, ScreenSize.size.width, ScreenSize.size.height)];
    
    if (!_view) {
        NSLog(@"Not supported");
        return;
    }
    device = MTLCreateSystemDefaultDevice();
    _view.device = device;
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    _view.framebufferOnly = true;
    self.view = _view;
    
    NSString* mainPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject]stringByAppendingString:@"/"];
    
    NSString* executablePath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"];
    
    game = new Game([mainPath UTF8String], [executablePath UTF8String]);
    game->initializeCamera(_view.frame.size.width, _view.frame.size.height);
    MetalRenderer* renderer = static_cast<MetalRenderer*>(game->getRenderer());
    renderer->device = device;
    renderer->mtkView = _view;
    renderer->initializeRenderEngine();
    game->postLoad();
    _view.delegate = self;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    game->frame();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    
}

-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    _startTouch = [touches.anyObject locationInView:self.view];
    if (_startTouch.x < self.view.bounds.size.width * 0.15)
    {
        game->getInput()->processButton(0, true);
    }
    if (_startTouch.x > self.view.bounds.size.width * 0.85)
    {
        game->getInput()->processButton(2, true);
    }
    if (_startTouch.y < self.view.bounds.size.height * 0.20)
    {
        game->getInput()->processButton(13, true);
    }
    if (_startTouch.y > self.view.bounds.size.height * 0.80)
    {
        game->getInput()->processButton(1, true);
    }
    
    [super touchesBegan:touches withEvent:event];
}

-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    game->getInput()->processButton(0, false);
    game->getInput()->processButton(2, false);
    game->getInput()->processButton(13, false);
    game->getInput()->processButton(1, false);
    
    //printf("LAST MOVED with %f %f\n", lastMovedTouch.x, lastMovedTouch.y);
    //printf("ENDING with %f %f\n", _startTouch.x, _startTouch.y);
    
    [super touchesEnded:touches withEvent:event];
}

-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    
    CGPoint newTouch = [touches.anyObject locationInView:self.view];
    _startTouch = [touches.anyObject previousLocationInView:self.view];
    
    double deltaX = newTouch.x - _startTouch.x;
    double deltaY = newTouch.y - _startTouch.y;
    
    //CGPoint touch = [touches.anyObject locationInView:self.view];
    glm::vec2 prevPos = game->getInput()->getMouseMovement();
    
    game->getInput()->setMouseMovement(prevPos.x + deltaX, prevPos.y + deltaY);
    //}
    [super touchesMoved:touches withEvent:event];
}

@end
