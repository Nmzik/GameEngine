//
//  main.m
//  testgame
//
//  Created by nmzik on 14.08.2019.
//  Copyright © 2019 nmzik. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

int main(int argc, const char * argv[]) {
    NSApplication* app = [NSApplication sharedApplication];
    [app setDelegate:[[AppDelegate alloc] init]];
    [app run];
}
