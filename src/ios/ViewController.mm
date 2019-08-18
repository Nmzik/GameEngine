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
    Game* game;
    
    CGPoint _startTouch;
    bool _isTouching;
}
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSString* mainPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    NSString* resultString = [mainPath stringByAppendingString:@"/"];
    
    game = new Game([resultString UTF8String]);
    
    CGRect ScreenSize = UIScreen.mainScreen.bounds;
    _view = [[MTKView alloc]initWithFrame:CGRectMake(0, 0, ScreenSize.size.width, ScreenSize.size.height)];
    
    if (!_view) {
        NSLog(@"Not supported");
        return;
    }
    
    MetalRenderer* renderer = static_cast<MetalRenderer*>(game->getRenderer());
    _view.device = renderer->device;
    renderer->mtkView = _view;
    
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    _view.framebufferOnly = true;
    _view.delegate = self;
    NSLog(@"HERE");
    
    self.view = _view;
    // Do any additional setup after loading the view.
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    game->frame();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    
}

-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    _isTouching = true;
    _startTouch = [touches.anyObject locationInView:self.view];
    
    NSLog(@"%f %f", _startTouch.x, _startTouch.y);
    if (_startTouch.x < 250)
        game->getInput()->processButton(0, true);
    if (_startTouch.x > 650)
        game->getInput()->processButton(2, true);
    if (_startTouch.y < 130)
        game->getInput()->processButton(13, true);
    if (_startTouch.y > 320)
        game->getInput()->processButton(1, true);
    
    [super touchesBegan:touches withEvent:event];
}

-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    _isTouching = false;
    
    game->getInput()->processButton(0, false);
    game->getInput()->processButton(2, false);
    game->getInput()->processButton(13, false);
    game->getInput()->processButton(1, false);
    
    game->getInput()->setMouseMovement(0, 0);
    
    [super touchesEnded:touches withEvent:event];
}

-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    if(_isTouching) {
        
        game->getInput()->processButton(0, false);
        game->getInput()->processButton(2, false);
        game->getInput()->processButton(13, false);
        game->getInput()->processButton(1, false);
        
    for(UITouch* touch in touches)
    {
        CGPoint location = [touch locationInView:self.view];
        //NSLog(@"%f %f", location.x, location.y);
        _startTouch = [touch previousLocationInView:self.view];
        
        double deltaX = location.x - _startTouch.x;
        double deltaY = location.y - _startTouch.y;
        
        game->getInput()->setMouseMovement(deltaX, deltaY);
    }
    }
    [super touchesMoved:touches withEvent:event];
}

@end
