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
    bool _isTouching;
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
    
    NSString* mainPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    NSString* resultString = [mainPath stringByAppendingString:@"/"];
    
    game = new Game([resultString UTF8String]);
    MetalRenderer* renderer = static_cast<MetalRenderer*>(game->getRenderer());
    renderer->device = device;
    renderer->mtkView = _view;
    renderer->initializeRenderEngine();
    _view.delegate = self;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    game->frame();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    
}

-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    _startTouch = [touches.anyObject locationInView:self.view];
    
    //NSLog(@"%f %f", _startTouch.x, _startTouch.y);
    if (_startTouch.x < 200)
    {
        game->getInput()->processButton(0, true);
        _isTouching = true;
    }
    if (_startTouch.x > 700)
    {
        game->getInput()->processButton(2, true);
        _isTouching = true;
    }
    if (_startTouch.y < 100)
    {
        game->getInput()->processButton(13, true);
        _isTouching = true;
    }
    if (_startTouch.y > 320)
    {
        game->getInput()->processButton(1, true);
        _isTouching = true;
    }
    
    [super touchesBegan:touches withEvent:event];
}

-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    _isTouching = false;
    
    game->getInput()->processButton(0, false);
    game->getInput()->processButton(2, false);
    game->getInput()->processButton(13, false);
    game->getInput()->processButton(1, false);
    
    [super touchesEnded:touches withEvent:event];
}

-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if(!_isTouching) {
        
        /*for(UITouch* touch in touches)
        {
            CGPoint location = [touch locationInView:self.view];
            //NSLog(@"%f %f", location.x, location.y);
            _startTouch = [touch previousLocationInView:self.view];
            
            double deltaX = location.x - _startTouch.x;
            double deltaY = location.y - _startTouch.y;
            
            if (deltaX > 8 && deltaY > 8)
            {
                game->getInput()->processButton(0, false);
                game->getInput()->processButton(2, false);
                game->getInput()->processButton(13, false);
                game->getInput()->processButton(1, false);
                break;
            }
        }*/
        CGPoint touch = [touches.anyObject locationInView:self.view];
        game->getInput()->setMouseMovement(touch.x, touch.y);
        
    }
    [super touchesMoved:touches withEvent:event];
}

@end
