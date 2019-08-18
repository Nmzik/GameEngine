//
//  AppDelegate.m
//  testgame
//
//  Created by nmzik on 14.08.2019.
//  Copyright © 2019 nmzik. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)aNotification {
    NSViewController* controller = [[ViewController alloc] init];
    self.window = [NSWindow windowWithContentViewController:controller];
    [self.window makeFirstResponder:controller];
    [self.window makeKeyAndOrderFront:self];
    [self.window setAcceptsMouseMovedEvents:YES];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    //[self.window makeKeyAndOrderFront:self];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end
