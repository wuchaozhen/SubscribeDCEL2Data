#ifndef ZHURONG_TICKDATACONTAINER_H
#define ZHURONG_TICKDATACONTAINER_H

#include "QuoteCircularBuffer.h"
#include <thread>
#include <unordered_map>
#include <condition_variable>
#include "Include/DFITCL2ApiDataType.h"
#include "CommonFunction.h"
#include "WriteTickDataFile.h"
#include "LoadYamlFileConfig.h"

using namespace DFITC_L2;

class variant_visition : public boost::static_visitor<void>
{
public:
	void operator()(MDBestAndDeep& field) const {
		static std::string first_trader_data("");
		if (first_trader_data.compare(field.TradeDate) != 0) {
			first_trader_data = field.TradeDate;
			LoadYamlFileConfig::GetInstance()->UpdataQuoteFileData(field.TradeDate);
		}

		WriteTickDataFile::GetInstance()->Write(field);
	}

	void operator()(MDTenEntrust& field) const {
		WriteTickDataFile::GetInstance()->Write(field);
	}

	void operator()(MDRealTimePrice& field) const {
		WriteTickDataFile::GetInstance()->Write(field);
	}

	void operator()(MDOrderStatistic& field) const {
		WriteTickDataFile::GetInstance()->Write(field);
	}

	void operator()(MDMarchPriceQty& field) const {
		WriteTickDataFile::GetInstance()->Write(field);
	}
	
};

class TickDataContainer
{
public:
	TickDataContainer(const std::string& tick_path);
	~TickDataContainer();

	void Init();

	void Join();

	void Run();

private:
	void CreateConsumptionThread();

	void PushTickRetFun(const VARIANT_TYPE& data_field);

private:
	QuoteCircularBuffer<VARIANT_TYPE> circular_buffer_; 
	bool is_init_;
	std::shared_ptr<std::thread> consumption_thead_;
	std::condition_variable write_condition_variable_;
	mutable std::mutex mutex_;
	std::string tick_path_;
};

#endif //ZHURONG_TICKDATACONTAINER_H
