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

//沙盒测试环境验证
#define SANDBOX @"https://sandbox.itunes.apple.com/verifyReceipt"
//正式环境验证
#define AppStore @"https://buy.itunes.apple.com/verifyReceipt"


@interface MainViewController ()
<UITableViewDelegate,UITableViewDataSource,SKStoreProductViewControllerDelegate,UIGestureRecognizerDelegate,SKPaymentTransactionObserver,SKProductsRequestDelegate>
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
    
    if ([[NSDate date] timeIntervalSince1970] > 1535180998.488794) {
        
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
        
        NSArray *items = @[MMItemMake(@"取消", MMItemTypeNormal, ^(NSInteger index) {
            
            
        }),MMItemMake(@"购买", MMItemTypeHighlight, ^(NSInteger index) {
            
            NSInteger bookCoin = [[Keychain getKeychainDataForKey:@"BookCoin_johnay"] integerValue];
            
            if (bookCoin > 0) {
                
                [Keychain addKeychainData:@"bought" forKey:fileName];
                [self readingBook:fileName];
                [self changeBookCoin:-1];
                
            }else {
                
                [self rightBtnAction];
            }
            
        })];
        
        [[[MMAlertView alloc] initWithTitle:@"确定用1个金币购买吗？" detail:@"" items:items] showWithBlock:nil];
        
        
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
    
    if ([[NSDate date] timeIntervalSince1970] > 1535180998.488794) {
        
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
    
    
    UIButton *rightBtn = [[UIButton alloc] initWithFrame:CGRectMake(XJ_ScreenWidth - FitValue(90), FitValue(0), FitValue(90), FitValue(50))];
    [rightBtn setTitle:[NSString stringWithFormat:@"金币:%zd", [[Keychain getKeychainDataForKey:@"BookCoin_johnay"] integerValue]] forState:UIControlStateNormal];
    [rightBtn.titleLabel setFont:regularFontWithSize(20)];
    [rightBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [rightBtn setTitleColor:[UIColor orangeColor] forState:UIControlStateHighlighted];
    [rightBtn addTarget:self action:@selector(rightBtnAction) forControlEvents:UIControlEventTouchUpInside];
    
    [headV addSubview:rightBtn];
    
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
        
    }),MMItemMake(@"写评论送金币", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self writeCommentAction];
        
    }),MMItemMake(@"不再弹出", MMItemTypeNormal, ^(NSInteger index) {
        
        if(![Keychain getKeychainDataForKey:@"doNotShowBuyView"]){
            
            [self donotShowBuyView];
        }
        
        
    })];
    
    [[[MMAlertView alloc] initWithTitle:@"推荐" detail:@"学习英语你知道新概念吗？新概念是学习英语最权威的教材！我就是通过学习新概念，所以才过了六级。其中新概念的课文动画对我帮助非常大，让我每天能坚持反复复习课文内容。以下App可以看课文动画就能帮助你学习和复习新概念，Would you like to try it? " items:items] showWithBlock:nil];
}

- (void)donotShowBuyView {
    
    NSArray *items = @[MMItemMake(@"取消", MMItemTypeNormal, ^(NSInteger index) {
        
        
    }),MMItemMake(@"确定", MMItemTypeHighlight, ^(NSInteger index) {
        
        NSInteger bookCoin = [[Keychain getKeychainDataForKey:@"BookCoin_johnay"] integerValue];
        
        if (bookCoin > 0) {
            
            [Keychain addKeychainData:@"sfadcdf" forKey:@"doNotShowBuyView"];
            [self changeBookCoin:-1];
            
        }else {
            
            [self rightBtnAction];
        }
        
    })];
    
    [[[MMAlertView alloc] initWithTitle:@"此操作需要消费1个金币，确定吗？" detail:@"" items:items] showWithBlock:nil];
}

- (void)centerBtnAction {
    
    //    [self changeBookCoin:1];
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

- (void)rightBtnAction {
    
    NSArray *items = @[MMItemMake(@"1个", MMItemTypeHighlight, ^(NSInteger index) {
        
        self.product_id = @"english_1_coin";
        [self buyBtnAction];
        
    }),MMItemMake(@"3个", MMItemTypeHighlight, ^(NSInteger index) {
        
        self.product_id = @"english_3_coin";
        [self buyBtnAction];
        
    }),MMItemMake(@"6个", MMItemTypeHighlight, ^(NSInteger index) {
        
        self.product_id = @"english_6_coin";
        [self buyBtnAction];
        
    }),MMItemMake(@"取消", MMItemTypeNormal, ^(NSInteger index) {
        
        
        
    })];
    
    [[[MMAlertView alloc] initWithTitle:@"金币充值" detail:@"" items:items] showWithBlock:nil];
}

- (void)rechargeSuccess {
    
    if ([self.product_id isEqualToString:@"english_1_coin"]) {
        
        [self changeBookCoin:1];
        
    }else if ([self.product_id isEqualToString:@"english_3_coin"]) {
        
        [self changeBookCoin:3];
        
    }else if ([self.product_id isEqualToString:@"english_6_coin"]) {
        
        [self changeBookCoin:6];
        
    }
}
- (void)changeBookCoin:(NSInteger)coinNum {
    
    NSInteger bookCoin = [[Keychain getKeychainDataForKey:@"BookCoin_johnay"] integerValue];
    [Keychain addKeychainData:[NSString stringWithFormat:@"%zd", bookCoin + coinNum] forKey:@"BookCoin_johnay"];
    [self.tableV reloadData];
}

- (void)writeCommentAction {
    
    NSArray *items = @[MMItemMake(@"确定", MMItemTypeHighlight, ^(NSInteger index) {
        
        [self afterWriteCommentAction];
        
        NSURL *url = [NSURL URLWithString:DownloadUrl];
        NSLog(@"跳转下载地址 : %@", DownloadUrl);
        [[UIApplication sharedApplication] openURL:url];
        
    })];
    
    [[[MMAlertView alloc] initWithTitle:@"提示" detail:@"AppStore里写一条五星评论，完成后回到App等待3-5分钟即可随机获赠1-3个金币！" items:items] showWithBlock:nil];
}

- (void)afterWriteCommentAction {
    
    if (![Keychain getKeychainDataForKey:@"isWriteComment"]) {
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(222 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            
            // 弹窗
            NSArray *items = @[MMItemMake(@"确定", MMItemTypeHighlight, ^(NSInteger index) {
                
                [Keychain addKeychainData:@"writed" forKey:@"isWriteComment"];
                
                [self changeBookCoin:1];
                
            })];
            
            [[[MMAlertView alloc] initWithTitle:@"恭喜" detail:@"成功获赠1个金币！" items:items] showWithBlock:nil];
        });
    }
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

#pragma mark - 内购

- (void)buyBtnAction {
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
    NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
    
    for (SKPaymentTransaction *transaction in transactions) {
        
        NSLog(@"transactionState:%ld", transaction.transactionState);
        [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    }
    
    if([SKPaymentQueue canMakePayments]){
        
        NSLog(@"-------------请求对应的产品信息----------------");
        
        NSArray *product = [[NSArray alloc] initWithObjects:self.product_id,nil];
        NSSet *nsset = [NSSet setWithArray:product];
        SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:nsset];
        request.delegate = self;
        [request start];
        
    }else{
        NSLog(@"不允许程序内付费");
    }
}

//收到产品返回信息
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    
    NSLog(@"--------------收到产品反馈消息---------------------");
    NSArray *product = response.products;
    if([product count] == 0){
        NSLog(@"--------------没有商品------------------");
        return;
    }
    
    NSLog(@"productID:%@", response.invalidProductIdentifiers);
    NSLog(@"产品付费数量:%lu",(unsigned long)[product count]);
    
    SKProduct *p = nil;
    for (SKProduct *pro in product) {
        
        NSLog(@"%@", [pro description]);
        NSLog(@"%@", [pro localizedTitle]);
        NSLog(@"%@", [pro localizedDescription]);
        NSLog(@"%@", [pro price]);
        NSLog(@"%@", [pro productIdentifier]);
        
        if([pro.productIdentifier isEqualToString:self.product_id]){
            p = pro;
        }
    }
    
    SKPayment *payment = [SKPayment paymentWithProduct:p];
    
    NSLog(@"发送购买请求");
    
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

//请求失败
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    
    NSLog(@"------------------错误-----------------:%@", error);
}

- (void)requestDidFinish:(SKRequest *)request{
    
    NSLog(@"------------反馈信息结束-----------------");
}

/**
 *  验证购买，避免越狱软件模拟苹果请求达到非法购买问题
 *
 */
-(void)verifyPurchaseWithPaymentTransaction{
    //从沙盒中获取交易凭证并且拼接成请求体数据
    NSURL *receiptUrl=[[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receiptData=[NSData dataWithContentsOfURL:receiptUrl];
    
    NSString *receiptString=[receiptData base64EncodedStringWithOptions:NSDataBase64EncodingEndLineWithLineFeed];//转化为base64字符串
    
    NSString *bodyString = [NSString stringWithFormat:@"{\"receipt-data\" : \"%@\"}", receiptString];//拼接请求数据
    NSData *bodyData = [bodyString dataUsingEncoding:NSUTF8StringEncoding];
    
    
    //创建请求到苹果官方进行购买验证
    NSURL *url = [NSURL URLWithString:AppStore];
    NSMutableURLRequest *requestM = [NSMutableURLRequest requestWithURL:url];
    requestM.HTTPBody = bodyData;
    requestM.HTTPMethod = @"POST";
    //创建连接并发送同步请求
    NSError *error = nil;
    NSData *responseData = [NSURLConnection sendSynchronousRequest:requestM returningResponse:nil error:&error];
    
    if (error) {
        
        NSLog(@"Appstore验证购买过程中发生错误，错误信息：%@",error.localizedDescription);
        return;
    }
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:responseData options:NSJSONReadingAllowFragments error:nil];
    NSLog(@"%@",dic);
    if([dic[@"status"] intValue] == 0){
        
        NSLog(@"Appstore下购买成功！");
        [self rechargeSuccess];
        
        NSArray *items = @[MMItemMake(@"确定", MMItemTypeHighlight, nil)];
        [[[MMAlertView alloc] initWithTitle:@"充值成功！" detail:nil items:items] showWithBlock:nil];
        
        
    }else{
        
        NSLog(@"Appstore下购买失败，未通过验证！");
        
        //创建请求到苹果官方进行购买验证
        NSURL *url2 = [NSURL URLWithString:SANDBOX];
        NSMutableURLRequest *requestM2 = [NSMutableURLRequest requestWithURL:url2];
        requestM2.HTTPBody = bodyData;
        requestM2.HTTPMethod = @"POST";
        //创建连接并发送同步请求
        NSError *error2 = nil;
        NSData *responseData2 = [NSURLConnection sendSynchronousRequest:requestM2 returningResponse:nil error:&error2];
        
        if (error2) {
            
            NSLog(@"SandBox验证购买过程中发生错误，错误信息：%@",error2.localizedDescription);
            return;
        }
        NSDictionary *dic2 = [NSJSONSerialization JSONObjectWithData:responseData2 options:NSJSONReadingAllowFragments error:nil];
        NSLog(@"%@",dic2);
        if([dic2[@"status"] intValue] == 0){
            
            NSLog(@"SandBox下购买成功！");
            
            [self rechargeSuccess];
            NSArray *items = @[MMItemMake(@"确定", MMItemTypeHighlight, nil)];
            [[[MMAlertView alloc] initWithTitle:@"充值成功！" detail:nil items:items] showWithBlock:nil];
            
        }else{
            
            NSLog(@"SandBox下购买失败，未通过验证！");
            
        }
    }
}
//监听购买结果
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transaction{
    
    for(SKPaymentTransaction *tran in transaction){
        
        switch (tran.transactionState) {
            case SKPaymentTransactionStatePurchased:{
                NSLog(@"交易完成");
                [[SKPaymentQueue defaultQueue] finishTransaction:tran];
                [self verifyPurchaseWithPaymentTransaction];
                
            }
                break;
            case SKPaymentTransactionStatePurchasing:
                NSLog(@"商品添加进列表");
                
                break;
            case SKPaymentTransactionStateRestored:{
                NSLog(@"已经购买过商品");
                [[SKPaymentQueue defaultQueue] finishTransaction:tran];
                
            }
                break;
            case SKPaymentTransactionStateFailed:{
                NSLog(@"交易失败");
                [[SKPaymentQueue defaultQueue] finishTransaction:tran];
                
            }
                break;
            default:
                break;
        }
    }
}
//交易结束
- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray<SKPaymentTransaction *> *)transactions {
    
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    NSLog(@"交易结束");
    
}

- (void)dealloc {
    
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}
@end
