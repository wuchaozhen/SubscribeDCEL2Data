/**
 * 版权所有(C)2012-2016, 大连飞创信息技术有限公司
 * 文件名称：DFITCL2ApiDataType.h
 * 文件说明：定义接口所需的数据类型的头文件
 * 当前版本：1.1.5.2
 * 作者：Datafeed项目组
 * 发布日期：2017年12月16日
 */             
#if !defined(AFX_HYQUOTE_H__INCLUDED_)
#define AFX_HYQUOTE_H__INCLUDED_
#include <limits>
#include "BasicDataType.h"
#pragma pack(4)
namespace DFITC_L2 {
////////////////////////////////////////////////
///MDBestAndDeep：最优与五档深度行情
////////////////////////////////////////////////
typedef struct MDBestAndDeep{
    INT1        Type;                                 //行情域标识
    UINT4       Length;                               //报文长度
    UINT4       Version;                              //版本从1开始
    UINT4       Time;                                 //预留字段
    INT1        Exchange[3];                          //交易所
    INT1        Contract[80];                         //合约代码
    BOOL        SuspensionSign;                       //停牌标志
    REAL8       LastClearPrice;                       //昨结算价
    REAL8       ClearPrice;                           //今结算价
    REAL8       AvgPrice;                             //成交均价
    REAL8       LastClose;                            //昨收盘
    REAL8       Close;                                //今收盘
    REAL8       OpenPrice;                            //今开盘
    UINT4       LastOpenInterest;                     //昨持仓量
    UINT4       OpenInterest;                         //持仓量
    REAL8       LastPrice;                            //最新价
    UINT4       MatchTotQty;                          //成交数量
    REAL8       Turnover;                             //成交金额
    REAL8       RiseLimit;                            //最高报价
    REAL8       FallLimit;                            //最低报价
    REAL8       HighPrice;                            //最高价
    REAL8       LowPrice;                             //最低价
    REAL8       PreDelta;                             //昨虚实度
    REAL8       CurrDelta;                            //今虚实度
    REAL8       BuyPriceOne;                          //买入价格1
    UINT4       BuyQtyOne;                            //买入数量1
    UINT4       BuyImplyQtyOne;                       //买1推导量
    REAL8       BuyPriceTwo;                          //买入价格2
    UINT4       BuyQtyTwo;                            //买入数量2
    UINT4       BuyImplyQtyTwo;                       //买2推导量
    REAL8       BuyPriceThree;                        //买入价格3
    UINT4       BuyQtyThree;                          //买入数量3
    UINT4       BuyImplyQtyThree;                     //买3推导量
    REAL8       BuyPriceFour;                         //买入价格4
    UINT4       BuyQtyFour;                           //买入数量4
    UINT4       BuyImplyQtyFour;                      //买4推导量
    REAL8       BuyPriceFive;                         //买入价格5
    UINT4       BuyQtyFive;                           //买入数量5
    UINT4       BuyImplyQtyFive;                      //买5推导量
    REAL8       SellPriceOne;                         //卖出价格1
    UINT4       SellQtyOne;                           //卖出数量1
    UINT4       SellImplyQtyOne;                      //卖1推导量
    REAL8       SellPriceTwo;                         //卖出价格2
    UINT4       SellQtyTwo;                           //卖出数量2
    UINT4       SellImplyQtyTwo;                      //卖2推导量
    REAL8       SellPriceThree;                       //卖出价格3
    UINT4       SellQtyThree;                         //卖出数量3
    UINT4       SellImplyQtyThree;                    //卖3推导量
    REAL8       SellPriceFour;                        //卖出价格4
    UINT4       SellQtyFour;                          //卖出数量4
    UINT4       SellImplyQtyFour;                     //卖4推导量
    REAL8       SellPriceFive;                        //卖出价格5
    UINT4       SellQtyFive;                          //卖出数量5
    UINT4       SellImplyQtyFive;                     //卖5推导量
    INT1        GenTime[13];                          //行情产生时间
    UINT4       LastMatchQty;                         //最新成交量
    INT4        InterestChg;                          //持仓量变化
    REAL8       LifeLow;                              //历史最低价
    REAL8       LifeHigh;                             //历史最高价
    REAL8       Delta;                                //delta
    REAL8       Gamma;                                //gama
    REAL8       Rho;                                  //rho
    REAL8       Theta;                                //theta
    REAL8       Vega;                                 //vega
    INT1        TradeDate[9];                         //行情日期
    INT1        LocalDate[9];                         //本地日期，预留字段
}THYQuote;

////////////////////////////////////////////////
///MDTenEntrust：最优价位上十笔委托
////////////////////////////////////////////////
typedef struct MDTenEntrust{
    INT1        Type;                                 //行情域标识
    UINT4       Len;                                  //报文长度
    INT1        Contract[80];                         //合约代码
    REAL8       BestBuyOrderPrice;                    //最优买价格
    UINT4       BestBuyOrderQtyOne;                   //委托量1
    UINT4       BestBuyOrderQtyTwo;                   //委托量2
    UINT4       BestBuyOrderQtyThree;                 //委托量3
    UINT4       BestBuyOrderQtyFour;                  //委托量4
    UINT4       BestBuyOrderQtyFive;                  //委托量5
    UINT4       BestBuyOrderQtySix;                   //委托量6
    UINT4       BestBuyOrderQtySeven;                 //委托量7
    UINT4       BestBuyOrderQtyEight;                 //委托量8
    UINT4       BestBuyOrderQtyNine;                  //委托量9
    UINT4       BestBuyOrderQtyTen;                   //委托量10
    REAL8       BestSellOrderPrice;                   //最优卖价格
    UINT4       BestSellOrderQtyOne;                  //委托量1
    UINT4       BestSellOrderQtyTwo;                  //委托量2
    UINT4       BestSellOrderQtyThree;                //委托量3
    UINT4       BestSellOrderQtyFour;                 //委托量4
    UINT4       BestSellOrderQtyFive;                 //委托量5
    UINT4       BestSellOrderQtySix;                  //委托量6
    UINT4       BestSellOrderQtySeven;                //委托量7
    UINT4       BestSellOrderQtyEight;                //委托量8
    UINT4       BestSellOrderQtyNine;                 //委托量9
    UINT4       BestSellOrderQtyTen;                  //委托量10
    INT1        GenTime[13];                          //生成时间
}TENENTRUST;

////////////////////////////////////////////////
///MDOrderStatistic：加权平均以及委托总量行情
////////////////////////////////////////////////
typedef struct MDOrderStatistic{
    INT1        Type;                                 //行情域标识
    UINT4       Len;                                  //报文长度
    INT1        ContractID[80];                       //合约号
    UINT4       TotalBuyOrderNum;                     //买委托总量
    UINT4       TotalSellOrderNum;                    //卖委托总量
    REAL8       WeightedAverageBuyOrderPrice;         //加权平均委买价格
    REAL8       WeightedAverageSellOrderPrice;        //加权平均委卖价格
}ORDERSTATISTIC;

////////////////////////////////////////////////
///MDRealTimePrice：实时结算价
////////////////////////////////////////////////
typedef struct MDRealTimePrice{
    INT1        Type;                                 //行情域标识
    UINT4       Len;                                  //报文长度
    INT1        ContractID[80];                       //合约号
    REAL8       RealTimePrice;                        //实时结算价
}REALTIMEPRICE;

////////////////////////////////////////////////
///MDMarchPriceQty：分价位成交
////////////////////////////////////////////////
typedef struct MDMarchPriceQty{
    INT1        Type;                                 //行情域标识
    UINT4       Len;                                  //报文长度
    INT1        ContractID[80];                       //合约号
    REAL8       PriceOne;                             //价格
    UINT4       PriceOneBOQty;                        //买开数量
    UINT4       PriceOneBEQty;                        //买平数量
    UINT4       PriceOneSOQty;                        //卖开数量
    UINT4       PriceOneSEQty;                        //卖平数量
    REAL8       PriceTwo;                             //价格
    UINT4       PriceTwoBOQty;                        //买开数量
    UINT4       PriceTwoBEQty;                        //买平数量
    UINT4       PriceTwoSOQty;                        //卖开数量
    UINT4       PriceTwoSEQty;                        //卖平数量
    REAL8       PriceThree;                           //价格
    UINT4       PriceThreeBOQty;                      //买开数量
    UINT4       PriceThreeBEQty;                      //买平数量
    UINT4       PriceThreeSOQty;                      //卖开数量
    UINT4       PriceThreeSEQty;                      //卖平数量
    REAL8       PriceFour;                            //价格
    UINT4       PriceFourBOQty;                       //买开数量
    UINT4       PriceFourBEQty;                       //买平数量
    UINT4       PriceFourSOQty;                       //卖开数量
    UINT4       PriceFourSEQty;                       //卖平数量
    REAL8       PriceFive;                            //价格
    UINT4       PriceFiveBOQty;                       //买开数量
    UINT4       PriceFiveBEQty;                       //买平数量
    UINT4       PriceFiveSOQty;                       //卖开数量
    UINT4       PriceFiveSEQty;                       //卖平数量
}MARCHPRICEQTY;

////////////////////////////////////////////////
///DFITCUserLoginField：登录信息
////////////////////////////////////////////////
struct DFITCUserLoginField
{
    LONG        lRequestID;                           //保留
    INT1        accountID[30];                        //用户名
    INT1        passwd[30];                           //密码
};

////////////////////////////////////////////////
///DFITCPasswdChangeField：修改密码信息
////////////////////////////////////////////////
struct DFITCPasswdChangeField
{
    LONG        RequestID;                            //保留
    INT1        AccountID[30];                        //用户名
    INT1        OldPassword[30];                      //旧密码
    INT1        NewPassword[30];                      //新密码
};

////////////////////////////////////////////////
///DFITCUserLogoutField：登出信息
////////////////////////////////////////////////
struct DFITCUserLogoutField
{
    LONG        RequestID;                            //保留
    INT1        AccountID[30];                        //用户名
};

////////////////////////////////////////////////
///ErrorRtnField：登录，登出，修改密码返回信息
////////////////////////////////////////////////
struct ErrorRtnField
{
    LONG        LocalOrderID;                         //保留
    INT4        ErrorID;                              //0为成功 非0为失败
    INT1        ErrorMsg[128];                        //失败时，返回错误信息
};

////////////////////////////////////////////////
///MAX_DBL：行情中出现的无意义double类型值的表示
//////////////////////////////////////////////// 
double const MAX_DBL =  (std::numeric_limits<double>::max)();
}
#pragma pack()
#endif // !defined(AFX_HYQUOTE_H__INCLUDED_)
