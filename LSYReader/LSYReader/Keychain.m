//
//  Keychain.m
//  tech2real
//
//  Created by HappenMacMini on 2017/2/14.
//  Copyright © 2017年 SZUI. All rights reserved.
//

#import "Keychain.h"

@implementation Keychain

+ (NSMutableDictionary *)getKeychainQuery:(NSString *)service {
    return [NSMutableDictionary dictionaryWithObjectsAndKeys:
            (__bridge id)kSecClassGenericPassword,(id)kSecClass,// 标识符(kSecAttrGeneric通常值位密码)
            service, (__bridge id)kSecAttrService,// 服务(kSecAttrService)
            service, (__bridge id)kSecAttrAccount,// 账户(kSecAttrAccount)
            (__bridge id)kSecAttrAccessibleAfterFirstUnlock,(__bridge id)kSecAttrAccessible,// kSecAttrAccessiblein变量用来指定这个应用何时需要访问这个数据
            nil];
}

+ (void)addKeychainData:(id)data forKey:(NSString *)key {
    // 获取查询字典
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:key];
    // 在删除之前先删除旧数据
    SecItemDelete((__bridge CFDictionaryRef)keychainQuery);
    // 添加新的数据到字典
    [keychainQuery setObject:[NSKeyedArchiver archivedDataWithRootObject:data] forKey:(__bridge id)kSecValueData];
    // 将数据字典添加到钥匙串
    SecItemAdd((__bridge CFDictionaryRef)keychainQuery, NULL);
}

+ (id)getKeychainDataForKey:(NSString *)key {
    id ret = nil;
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:key];
    
    [keychainQuery setObject:(id)kCFBooleanTrue forKey:(__bridge id)kSecReturnData];
    [keychainQuery setObject:(__bridge id)kSecMatchLimitOne forKey:(__bridge id)kSecMatchLimit];
    CFDataRef keyData = NULL;
    if (SecItemCopyMatching((__bridge CFDictionaryRef)keychainQuery, (CFTypeRef *)&keyData) == noErr) {
        @try {
            ret = [NSKeyedUnarchiver unarchiveObjectWithData:(__bridge NSData *)keyData];
        } @catch (NSException *e) {
            NSLog(@"Unarchive of %@ failed: %@",key,e);
        } @finally {
            
        }
    }
    
    if (keyData) {
        CFRelease(keyData);
    }
    
    return ret;
}

+ (void)deleteKeychainDataForKey:(NSString *)key {
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:key];
    SecItemDelete((__bridge CFDictionaryRef)keychainQuery);
}

@end
