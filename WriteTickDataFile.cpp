#include "WriteTickDataFile.h"
#include <chrono>
#include <iostream>
#include <iosfwd>
#include <string.h>
#include <float.h>
#include "CommonFunction.h"


WriteTickDataFile::WriteTickDataFile()
{
}

WriteTickDataFile::~WriteTickDataFile()
{
	Close();
}

WriteTickDataFile* WriteTickDataFile::GetInstance()
{
	static WriteTickDataFile* pSingleSpi(nullptr);
	if (nullptr == pSingleSpi) {
		static WriteTickDataFile task;
		pSingleSpi = &task;
	}
	return pSingleSpi;
}

void WriteTickDataFile::Open(const std::string& instrument_name)
{
	out_file_.open(instrument_name, std::ios::out | std::ios::binary | std::ios::app);
}

void WriteTickDataFile::Close()
{
	if (out_file_.is_open()) {
		out_file_.close();
	}
}

void WriteTickDataFile::WriteBestAndDeep(const MDBestAndDeep& field)
{
	const int size = sizeof(MDBestAndDeep) + 60;
	char data[size] = { 0 };

	sprintf(data, "%c;%s,%s,%d,%s,\
%s,%s,%s,%d,%d,\
%s,%d,%d,%d,%s,\
%s,%s,%s,%s,%s,\
%s,%s,%d,%d,%s,\
%d,%d,%s,%s,%s,\
%s;\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s;%s\n",
	CommonFunction::GetQuoteMark(field.Type),
	field.TradeDate,
	field.Contract,
	0,     //事务编号
	"",    //合约名称

	CommonFunction::ValidDoubleValue(field.LastPrice).c_str(),
	CommonFunction::ValidDoubleValue(field.HighPrice).c_str(),
	CommonFunction::ValidDoubleValue(field.LowPrice).c_str(),
	field.LastMatchQty,
	field.MatchTotQty,

	CommonFunction::ValidDoubleValue(field.Turnover).c_str(),
	field.LastOpenInterest,
	field.OpenInterest,
	field.InterestChg,
	CommonFunction::ValidDoubleValue(field.ClearPrice).c_str(),

	CommonFunction::ValidDoubleValue(field.LifeLow).c_str(),
	CommonFunction::ValidDoubleValue(field.LifeHigh).c_str(),
	CommonFunction::ValidDoubleValue(field.RiseLimit).c_str(),
	CommonFunction::ValidDoubleValue(field.FallLimit).c_str(),
	CommonFunction::ValidDoubleValue(field.LastClearPrice).c_str(),

	CommonFunction::ValidDoubleValue(field.LastClose).c_str(),
	"N/A",
	0,
	0,
	"N/A",

	0,
	0,
	CommonFunction::ValidDoubleValue(field.AvgPrice).c_str(),
	field.GenTime,
	CommonFunction::ValidDoubleValue(field.OpenPrice).c_str(),

	CommonFunction::ValidDoubleValue(field.Close).c_str(),

	CommonFunction::ValidDoubleValue(field.BuyPriceOne).c_str(),
	field.BuyQtyOne,
	field.BuyImplyQtyOne,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceTwo).c_str(),
	field.BuyQtyTwo,
	field.BuyImplyQtyTwo,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceThree).c_str(),
	field.BuyQtyThree,
	field.BuyImplyQtyThree,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceFour).c_str(),
	field.BuyQtyFour,
	field.BuyImplyQtyFour,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceFive).c_str(),
	field.BuyQtyFive,
	field.BuyImplyQtyFive,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceOne).c_str(),
	field.SellQtyOne,
	field.SellImplyQtyOne,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceTwo).c_str(),
	field.SellQtyTwo,
	field.SellImplyQtyTwo,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceThree).c_str(),
	field.SellQtyThree,
	field.SellImplyQtyThree,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceFour).c_str(),
	field.SellQtyFour,
	field.SellImplyQtyFour,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceFive).c_str(),
	field.SellQtyFive,
	field.SellImplyQtyFive,
	0,
	field.GenTime,
	CommonFunction::GetLocalSystemTime().c_str());

	out_file_.write(data, strlen(data));
	out_file_.flush();
}

void WriteTickDataFile::WriteArbi(const MDBestAndDeep& field)
{
	const int size = sizeof(MDBestAndDeep) + 60;
	char data[size] = { 0 };

	sprintf(data, "%c;%s,%s,%d,%s,\
%s,%s,%s,%s,%s,\
%s,%s,%d,%s,%d,\
%s;\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s|\
%s,%d,%d,%d,%s;%s\n",
	CommonFunction::GetQuoteMark(field.Type),
	field.TradeDate,
	field.Contract,
	0,     //事务编号
	CommonFunction::ValidDoubleValue(field.LastPrice).c_str(),

	CommonFunction::ValidDoubleValue(field.LowPrice).c_str(),
	CommonFunction::ValidDoubleValue(field.HighPrice).c_str(),
	CommonFunction::ValidDoubleValue(field.LifeLow).c_str(),
	CommonFunction::ValidDoubleValue(field.LifeHigh).c_str(),
	CommonFunction::ValidDoubleValue(field.RiseLimit).c_str(),
		
	CommonFunction::ValidDoubleValue(field.FallLimit).c_str(),
	"N/A",
	0,
	"N/A",
	0,

	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceOne).c_str(),
	field.BuyQtyOne,
	field.BuyImplyQtyOne,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceTwo).c_str(),
	field.BuyQtyTwo,
	field.BuyImplyQtyTwo,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceThree).c_str(),
	field.BuyQtyThree,
	field.BuyImplyQtyThree,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceFour).c_str(),
	field.BuyQtyFour,
	field.BuyImplyQtyFour,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.BuyPriceFive).c_str(),
	field.BuyQtyFive,
	field.BuyImplyQtyFive,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceOne).c_str(),
	field.SellQtyOne,
	field.SellImplyQtyOne,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceTwo).c_str(),
	field.SellQtyTwo,
	field.SellImplyQtyTwo,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceThree).c_str(),
	field.SellQtyThree,
	field.SellImplyQtyThree,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceFour).c_str(),
	field.SellQtyFour,
	field.SellImplyQtyFour,
	0,
	field.GenTime,

	CommonFunction::ValidDoubleValue(field.SellPriceFive).c_str(),
	field.SellQtyFive,
	field.SellImplyQtyFive,
	0,
	field.GenTime,
	CommonFunction::GetLocalSystemTime().c_str());

	out_file_.write(data, strlen(data));
	out_file_.flush();
}

void WriteTickDataFile::Write(const MDBestAndDeep& field)
{
	if ('1' == field.Type) {
		WriteBestAndDeep(field);
	}

	if ('2' == field.Type) {
		WriteArbi(field);
	}
}

void WriteTickDataFile::Write(const MDTenEntrust& field)
{
	const int size = sizeof(MDTenEntrust) + 60;
	char data[size] = { 0 };

	sprintf(data, "%c;%s,\
%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
%s;%s\n",
	CommonFunction::GetQuoteMark(field.Type),
	field.Contract,
	CommonFunction::ValidDoubleValue(field.BestBuyOrderPrice).c_str(),
	field.BestBuyOrderQtyOne,
	field.BestBuyOrderQtyTwo,
	field.BestBuyOrderQtyThree,
	field.BestBuyOrderQtyFour,
	field.BestBuyOrderQtyFive,
	field.BestBuyOrderQtySix,
	field.BestBuyOrderQtySeven,
	field.BestBuyOrderQtyEight,
	field.BestBuyOrderQtyNine,
	field.BestBuyOrderQtyTen,
	CommonFunction::ValidDoubleValue(field.BestSellOrderPrice).c_str(),
	field.BestSellOrderQtyOne,
	field.BestSellOrderQtyTwo,
	field.BestSellOrderQtyThree,
	field.BestSellOrderQtyFour,
	field.BestSellOrderQtyFive,
	field.BestSellOrderQtySix,
	field.BestSellOrderQtySeven,
	field.BestSellOrderQtyEight,
	field.BestSellOrderQtyNine,
	field.BestSellOrderQtyTen,
	field.GenTime,
	CommonFunction::GetLocalSystemTime().c_str());

	out_file_.write(data, strlen(data));
	out_file_.flush();
}

void WriteTickDataFile::Write(const MDRealTimePrice& field)
{
	const int size = sizeof(MDRealTimePrice) + 60;
	char data[size] = { 0 };

	sprintf(data, "%c;%s,%s;%s;\n",
		CommonFunction::GetQuoteMark(field.Type),
		field.ContractID,
		CommonFunction::ValidDoubleValue(field.RealTimePrice).c_str(),
		CommonFunction::GetLocalSystemTime().c_str());

	out_file_.write(data, strlen(data));
	out_file_.flush();
}

void WriteTickDataFile::Write(const MDOrderStatistic& field)
{
	const int size = sizeof(MDOrderStatistic) + 60;
	char data[size] = { 0 };

	sprintf(data, "%c;%s,%d,%d,%s,%s;%s;\n",
		CommonFunction::GetQuoteMark(field.Type),
		field.ContractID,
		field.TotalBuyOrderNum,
		field.TotalSellOrderNum,
		CommonFunction::ValidDoubleValue(field.WeightedAverageBuyOrderPrice).c_str(),
		CommonFunction::ValidDoubleValue(field.WeightedAverageSellOrderPrice).c_str(),
		CommonFunction::GetLocalSystemTime().c_str());

	out_file_.write(data, strlen(data));
	out_file_.flush();
}

void WriteTickDataFile::Write(const MDMarchPriceQty& field)
{
	const int size = sizeof(MDMarchPriceQty) + 60;
	char data[size] = { 0 };

	sprintf(data, "%c;%s,\
%s,%d,%d,%d,%d,\
%s,%d,%d,%d,%d,\
%s,%d,%d,%d,%d,\
%s,%d,%d,%d,%d,\
%s,%d,%d,%d,%d;\
%s;\n",
	CommonFunction::GetQuoteMark(field.Type),
	field.ContractID,
	CommonFunction::ValidDoubleValue(field.PriceOne).c_str(),
	field.PriceOneBOQty,
	field.PriceOneBEQty,
	field.PriceOneSOQty,
	field.PriceOneSEQty,

	CommonFunction::ValidDoubleValue(field.PriceTwo).c_str(),
	field.PriceTwoBOQty,
	field.PriceTwoBEQty,
	field.PriceTwoSOQty,
	field.PriceTwoSEQty,

	CommonFunction::ValidDoubleValue(field.PriceThree).c_str(),
	field.PriceThreeBOQty,
	field.PriceThreeBEQty,
	field.PriceThreeSOQty,
	field.PriceThreeSEQty,

	CommonFunction::ValidDoubleValue(field.PriceFour).c_str(),
	field.PriceFourBOQty,
	field.PriceFourBEQty,
	field.PriceFourSOQty,
	field.PriceFourSEQty,

	CommonFunction::ValidDoubleValue(field.PriceFive).c_str(),
	field.PriceFiveBOQty,
	field.PriceFiveBEQty,
	field.PriceFiveSOQty,
	field.PriceFiveSEQty,

	CommonFunction::GetLocalSystemTime().c_str());

	out_file_.write(data, strlen(data));
	out_file_.flush();
}

void WriteTickDataFile::Flush()
{
	out_file_.flush();
}



