//
//  MainViewController.m
//  知识
//
//  Created by Johnay  on 2018/2/11.
//  Copyright © 2018年 Johnay. All rights reserved.
//

#import "MainViewController.h"
#import "LSYReadViewController.h"
#import "LSYReadPageViewController.h"
#import "LSYReadUtilites.h"
#import "LSYReadModel.h"
#import "MMAlertView.h"
#import "RNCryptor iOS.h"
#import <StoreKit/StoreKit.h>
#import "Keychain.h"

@interface MainViewController ()
<UITableViewDelegate,UITableViewDataSource,SKStoreProductViewControllerDelegate>
{
    NSString *fileDataPath;
}

@property (nonatomic, strong) UIImageView *imgV;
@property (nonatomic, strong) UITableView *tableV;
@property (nonatomic, strong) NSMutableArray *names;
@property (nonatomic, strong) NSString *product_id;

@end

@implementation MainViewController

- (NSArray *)names {
    
    if (!_names) {
        
        _names = @[@"美国常用习语",@"中级美国英语1",@"中级美国英语2",@"中级美国英语3",@"中级美国英语4",@"最实用英语口语444句",@"常用英语口语900句1", @"常用英语口语900句2", @"常用英语口语900句3",@"7000雅思词汇用100个句子记完",
                   @"小学英语单词表",@"初中英语单词表",@"高中英语单词表",@"四级英语单词表",@"六级英语单词表",@"小学英语短语大全",@"初中英语短语大全",@"高中英语短语大全",@"大学英语短语大全",@"新概念英语全册单词表",@"新概念英语课文第1册",@"新概念英语课文第2册",@"新概念英语课文第3册",@"新概念英语课文第4册",@"新概念36篇必背文章",@"四级翻译",@"四级作文",@"走遍美国上",@"走遍美国下",@"化身博士_中英对照",@"谁动了我的奶酪",@"小王子"];
    }
    
    return _names;
}

- (void)viewDidAppear:(BOOL)animated {
    
    if (fileDataPath) {
        
        NSError *erro;
        [[NSFileManager defaultManager] removeItemAtPath:fileDataPath error:&erro];
        
        if (!erro) {
            
            fileDataPath = nil;
            NSLog(@"删除成功！");
        }
    }
}

//MARK: - 入口
- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.imgV = [[UIImageView alloc] initWithFrame:self.view.bounds];
    self.imgV.image = [UIImage imageNamed:@"bkImg4"];
    [self.view addSubview:self.imgV];
    

    self.tableV = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, XJ_ScreenWidth, XJ_ScreenHeight - XJ_safeBottomMargin)];
    self.tableV.backgroundColor = [UIColor clearColor];
    self.tableV.delegate = self;
    self.tableV.dataSource = self;
    self.tableV.separatorStyle = 0;
    self.tableV.estimatedRowHeight = 50.0;
    [self.tableV registerClass:[UITableViewCell class] forCellReuseIdentifier:@"UITableViewCell"];
    
    [self.view addSubview:self.tableV];
    
    
        NSTimeInterval timeInterval = [[NSDate date] timeIntervalSince1970] ;
    
        NSLog(@"timeInterval : %lf", timeInterval);
    
        NSLog(@"lastTime48 : %lf", timeInterval + 48 * 3600);
    
    if ([[NSDate date] timeIntervalSince1970] > 1536113423.803789) {
        
        if(![Keychain getKeychainDataForKey:@"doNotShowBuyView"]){
            
            [self leftBtnAction];
        }
        
    }
    
}

//MARK: - productViewControllerDidFinish
- (void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController {
    
    [self dismissViewControllerAnimated:YES completion:nil];
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    return self.names.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"UITableViewCell"];
    cell.textLabel.text = [NSString stringWithFormat:@"%zd. %@", indexPath.row + 1, self.names[indexPath.row]];
    cell.textLabel.font = regularFontWithSize(20);
    cell.textLabel.textAlignment = NSTextAlignmentCenter;
    cell.backgroundColor = [UIColor clearColor];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    if ([[Keychain getKeychainDataForKey:self.names[indexPath.row]] isEqualToString:@"readed"]) {
        
        cell.textLabel.textColor = [UIColor purpleColor];
        
    }else if ([[Keychain getKeychainDataForKey:self.names[indexPath.row]] isEqualToString:@"bought"]) {
        
        cell.textLabel.textColor = [UIColor orangeColor];
        
    }else{
        
        cell.textLabel.textColor = [UIColor whiteColor];
    }
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];

    
    NSString *fileName = [NSString stringWithFormat:@"%@", self.names[indexPath.row]];
    
    id keyData = [Keychain getKeychainDataForKey:fileName];
    if (!keyData) {
        
        [Keychain addKeychainData:@"readed" forKey:fileName];
        [self readingBook:fileName];
        
    }else if([keyData isEqualToString:@"readed"]) {
        
        [Keychain addKeychainData:@"bought" forKey:fileName];
        [self readingBook:fileName];
        
        
    }else if([keyData isEqualToString:@"bought"]) {
        
        [self readingBook:fileName];
    }
    
    
    [tableView reloadData];
}


- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {

    return FitValue(50);
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {

    UIView *headV = [[UIView alloc] initWithFrame:CGRectMake(0, 0, XJ_ScreenWidth, FitValue(50))];
    headV.backgroundColor = [UIColor clearColor];

    if ([[NSDate date] timeIntervalSince1970] > 1536113423.803789) {
        
        UIButton *leftBtn = [[UIButton alloc] initWithFrame:CGRectMake(FitValue(10), FitValue(0), FitValue(50), FitValue(50))];
        [leftBtn setTitle:@"推荐" forState:UIControlStateNormal];
        [leftBtn.titleLabel setFont:regularFontWithSize(20)];
        [leftBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        [leftBtn setTitleColor:[UIColor orangeColor] forState:UIControlStateHighlighted];
        [leftBtn addTarget:self action:@selector(leftBtnAction) forControlEvents:UIControlEventTouchUpInside];
        [headV addSubview:leftBtn];
    }

    UIButton *centerBtn = [[UIButton alloc] initWithFrame:CGRectMake(FitValue(60), FitValue(0), XJ_ScreenWidth - FitValue(120), FitValue(50))];
    [centerBtn setTitle:@"英语学习资料" forState:UIControlStateNormal];
    [centerBtn.titleLabel setFont:regularFontWithSize(28)];
    [centerBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [centerBtn setTitleColor:[UIColor orangeColor] forState:UIControlStateHighlighted];
    [centerBtn addTarget:self action:@selector(centerBtnAction) forControlEvents:UIControlEventTouchUpInside];
    [headV addSubview:centerBtn];

    return headV;
}

//MARK: - btnActions
- (void)leftBtnAction {
    
    NSArray *items = @[MMItemMake(@"第一册（美音）", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self buyAppAction:@"1358168911"];
        
    }),MMItemMake(@"第二册（美音）", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self buyAppAction:@"1359653579"];
        
    }),MMItemMake(@"第一册（英音）", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self buyAppAction:@"1383103801"];
        
    }),MMItemMake(@"第二册（英音）", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self buyAppAction:@"1384334078"];
        
    }),MMItemMake(@"第三册（英音）", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self buyAppAction:@"1386014907"];
        
    }),MMItemMake(@"第四册（英音）", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self buyAppAction:@"1413080436"];
        
    }),MMItemMake(@"确定", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self writeCommentAction];
        
    })];
    
    [[[MMAlertView alloc] initWithTitle:@"推荐" detail:@"学习英语你知道新概念吗？新概念是学习英语最权威的教材！我就是通过学习新概念，所以才过了六级。其中新概念的课文动画对我帮助非常大，让我每天能坚持反复复习课文内容。以下App可以看课文动画就能帮助你学习和复习新概念，Would you like to try it? " items:items] showWithBlock:nil];
}


- (void)centerBtnAction {
    
//    for (NSString *str in self.names) {
//
//        [Keychain deleteKeychainDataForKey:str];
//    }
//
//    [Keychain deleteKeychainDataForKey:@"BookCoin_johnay"];
//    [Keychain deleteKeychainDataForKey:@"isWriteComment"];
//    [Keychain deleteKeychainDataForKey:@"doNotShowBuyView"];
//
//    [self.tableV reloadData];
}



- (void)writeCommentAction {
    
    NSArray *items = @[MMItemMake(@"下次写", MMItemTypeHighlight, nil), MMItemMake(@"写评论", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self afterWriteCommentAction];
        
        NSURL *url = [NSURL URLWithString:DownloadUrl];
        NSLog(@"跳转下载地址 : %@", DownloadUrl);
        [[UIApplication sharedApplication] openURL:url];
        
    })];
    
    [[[MMAlertView alloc] initWithTitle:@"提示" detail:@"AppStore里写一条五星评论，完成后回到App等待3-5分钟即可不再弹出推荐！" items:items] showWithBlock:nil];
}

- (void)afterWriteCommentAction {
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(222 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        
        // 弹窗
        NSArray *items = @[MMItemMake(@"确定", MMItemTypeHighlight, ^(NSInteger index) {
            
            [Keychain addKeychainData:@"sfadcdf" forKey:@"doNotShowBuyView"];
            
        })];
        
        [[[MMAlertView alloc] initWithTitle:@"恭喜" detail:@"下次不再弹出推荐！" items:items] showWithBlock:nil];
    });
    
}

- (void)buyAppAction:(NSString *)appId {
    
    SKStoreProductViewController *appStore = [[SKStoreProductViewController alloc] init];
    appStore.delegate = self;
    // 先去跳转再去加载页面，体验感方面会好很多
    [appStore loadProductWithParameters:@{SKStoreProductParameterITunesItemIdentifier : appId} completionBlock:^(BOOL result, NSError * _Nullable error) {
        if (error) {
            NSLog(@"错误 %@",error);
        } else {
        }
    }];
    [self presentViewController:appStore animated:YES completion:nil];
}

- (void)readingBook:(NSString *)fileName {
    
    // 更改为本地视频文件
    NSString *urlStr= [[NSBundle mainBundle] pathForResource:fileName ofType:@"xjy"];
    
    NSData *encryptedData = [NSData dataWithContentsOfFile:urlStr];
    
    NSError *err;
    
    NSData *decryptedData = [RNDecryptor decryptData:encryptedData withPassword:@"johnayXiaoJian" error:&err];
    
    NSString *docPath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
    
    fileDataPath = [docPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.txt", fileName]];
    NSLog(@"fileDataPath:%@",fileDataPath);
    
    if ([decryptedData writeToFile:fileDataPath atomically:YES]) {
        
        LSYReadPageViewController *pageView = [[LSYReadPageViewController alloc] init];
        NSURL *fileURL = [NSURL fileURLWithPath:fileDataPath];
        //        NSURL *fileURL = [[NSBundle mainBundle] URLForResource:@"xjEnglish"withExtension:@"txt"];
        pageView.resourceURL = fileURL;    //文件位置
        
        dispatch_async(dispatch_get_global_queue(0, 0), ^{
            
            pageView.model = [LSYReadModel getLocalModelWithURL:fileURL];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                
                [self presentViewController:pageView animated:YES completion:nil];
            });
        });
        
    }
}

@end
