//
//  AppDelegate.m
//  LSYReader
//
//  Created by Labanotation on 16/5/30.
//  Copyright © 2016年 okwei. All rights reserved.
//

#import "AppDelegate.h"
#import "MainViewController.h"
#import "MMAlertView.h"
@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
    
    [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
   
//    [self checkVersion];
    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.window.backgroundColor = [UIColor whiteColor];
    self.window.rootViewController = [[MainViewController alloc] init];

    [self.window makeKeyAndVisible];
    
    return YES;
}

#pragma mark - 检查更新
- (void)checkVersion
{
    if ([AppID length] < 1) {
        return;
    }
    
    NSString *url = [NSString stringWithFormat:@"http://itunes.apple.com/lookup?id=%@",AppID];
    NSMutableURLRequest *urlRequest = [[NSMutableURLRequest alloc] init];
    [urlRequest setURL:[NSURL URLWithString:url]];
    [urlRequest setHTTPMethod:@"POST"];
    
    NSData *returnData = [NSURLConnection sendSynchronousRequest:urlRequest returningResponse:nil error:nil];
    
    if (returnData) {
        
        NSDictionary *jsonDic = [NSJSONSerialization JSONObjectWithData:returnData options:0 error:nil];
        
        NSArray *infoArr = [jsonDic objectForKey:@"results"];
        
        if ([infoArr count]) {
            
            NSDictionary *releaseInfo = [infoArr objectAtIndex:0];
            NSString *appStoreVersion = [releaseInfo objectForKey:@"version"];
            
            // 这里显示的是appstore 上面设置的号码.
            NSLog(@"appstore 版本为:%@ \n 当前版本为:%@",appStoreVersion,VERSION);
            
            NSInteger startTimes = [[NSUserDefaults standardUserDefaults] integerForKey:@"startTimesKey"];
            
            if ([VERSION compare: appStoreVersion] == NSOrderedAscending) {
                // 弹窗
                NSArray *items = @[MMItemMake(@"稍后提醒", MMItemTypeNormal, nil),MMItemMake(@"立即更新", MMItemTypeHighlight, ^(NSInteger index) {
                    
                    NSURL *url = [NSURL URLWithString:DownloadUrl];
                    NSLog(@"跳转下载地址 : %@", DownloadUrl);
                    [[UIApplication sharedApplication] openURL:url];
                    
                })];
                
                [[[MMAlertView alloc] initWithTitle:@"出新版本啦！" detail:releaseInfo[@"releaseNotes"] items:items]showWithBlock:nil];
                
            }
        }
    }
}
- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
