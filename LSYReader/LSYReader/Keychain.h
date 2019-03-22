//
//  Keychain.h
//  tech2real
//
//  Created by HappenMacMini on 2017/2/14.
//  Copyright © 2017年 SZUI. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Security/Security.h>
@interface Keychain : NSObject
// 添加数据
+ (void)addKeychainData:(id)data forKey:(NSString *)key;
// 根据key获取相应的数据
+ (id)getKeychainDataForKey:(NSString *)key;
// 删除数据
+ (void)deleteKeychainDataForKey:(NSString *)key;

@end
