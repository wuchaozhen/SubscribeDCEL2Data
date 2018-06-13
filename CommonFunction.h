#ifndef ZHURONG_COMMONFUNCTION_H
#define ZHURONG_COMMONFUNCTION_H

#include <string>
#include <chrono>
#include <iostream>
#include "Include/spdlog/spdlog.h"
#include "boost/variant.hpp"
#include "Include/DFITCL2Api.h"
#include <sstream>
#include "boost/format.hpp"

//spd log macro definition
#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

#define SPDLOG_CONSOLE_NAME "console"
#define SPDLOG_DAILY_LOGGER_NAME "daily_logger"

namespace spd = spdlog;

#define LOG_TRACE(name, infos) spd::get(name)->trace(infos);

#define LOG_DEBUG(name, infos) spd::get(name)->debug(infos);

#define LOG_INFO(name, infos) spd::get(name)->info(infos);
#define LOG_INFO_1(name, infos, arg1) spd::get(name)->info(infos, arg1);
#define LOG_INFO_2(name, infos, arg1, arg2) spd::get(name)->info(infos, arg1, arg2);
#define LOG_INFO_3(name, infos, arg1, arg2, arg3) spd::get(name)->info(infos, arg1, arg2, arg3);
#define LOG_INFO_4(name, infos, arg1, arg2, arg3, arg4) spd::get(name)->info(infos, arg1, arg2, arg3, arg4);
#define LOG_INFO_5(name, infos, arg1, arg2, arg3, arg4, arg5) spd::get(name)->info(infos, arg1, arg2, arg3, arg4, arg5);

#define LOG_WARN(name, infos) spd::get(name)->warn(infos);
#define LOG_WARN_2(name, infos, arg1, arg2) spd::get(name)->warn(infos, arg1, arg2);

#define LOG_ERR(name, infos) spd::get(name)->error(infos);
#define LOG_ERR_1(name, infos, arg1) spd::get(name)->error(infos, arg1);
#define LOG_ERR_2(name, infos, arg1, arg2) spd::get(name)->error(infos, arg1, arg2);
#define LOG_ERR_3(name, infos, arg1, arg2, arg3) spd::get(name)->error(infos, arg1, arg2, arg3);
#define LOG_ERR_5(name, infos, arg1, arg2, arg3, arg4, arg5) spd::get(name)->error(infos, arg1, arg2, arg3, arg4, arg5);

#define LOG_CRITICAL(name, infos) spd::get(name)->critical(infos);

#define LOG_OFF(name, infos) spd::get(name)->off(infos);
///////////////////////////

//dce l2 market data struct
using namespace DFITC_L2;
typedef boost::variant<
	MDBestAndDeep,
	MDTenEntrust,
	MDRealTimePrice,
	MDOrderStatistic,
	MDMarchPriceQty> VARIANT_TYPE;
//////////////////////////

class CommonFunction
{
public:

	static std::string ValidDoubleValue(double value)
	{
		std::string strs("N/A");
		if (MAX_DBL != value) {
			strs = str(boost::format("%.2lf") % value);
		}
		return strs;
	}

	static char GetQuoteMark(int type)
	{
		char mark;
		switch (type)
		{
		case '1':
			mark = 'b';
			break;
		case '2':
			mark = 'e';
			break;
		case '4':
			mark = 'c';
			break;
		case '5':
			mark = 'h';
			break;
		case '6':
			mark = 'f';
			break;
		case '7':
			mark = 'g';
			break;
		default:
			mark = '-';
			break;
		}
		return mark;
	}

	static std::string GetNetErrMsgForCode(int nReason)
	{
		std::string strErrorMsg("");
		switch (nReason)
		{
		case 1:
			strErrorMsg = "headbeat timeout";
			break;
		case 2:
			strErrorMsg = "network disconnection";
			break;
		case 3:
			strErrorMsg = "receive false message";
			break;
		case 4:
			strErrorMsg = "in the multicast market mode,notify customers when destrorying instances";
			break;
		case 5:
			strErrorMsg = "send message too long";
			break;
		case 6:
			strErrorMsg = "internal error";
			break;
		default:
			//strErrorMsg = "unknown error";
			break;
		}
		return strErrorMsg;
	}

	static std::string GetErrMsgForReqCode(int nRetReqCode)
	{
		std::string strErrorMsg("");
		switch (nRetReqCode)
		{
		case 0:
			strErrorMsg = "send request success";
			break;
		case -1:
			strErrorMsg = "network connection failure";
			break;
		case -2:
			strErrorMsg = "unprocessed request exceed the number of licenses";
			break;
		case -3:
			strErrorMsg = "transmission request exceeding restriction pre second";
			break;
		default:
			strErrorMsg = "unknown error";
			break;
		}

		return strErrorMsg;
	}

	
	static std::string GetLocalSystemTime()
	{
		using namespace std::chrono;
		auto now = system_clock::now();
		auto mill = now.time_since_epoch();
		auto diff = duration_cast<milliseconds>(mill).count();
		auto const msecs = diff % 1000000;

		auto nows = system_clock::to_time_t(now);
		struct tm* ptm = localtime(&nows);

		char time[60] = { 0 };
		sprintf(time, "%02d:%02d:%02d.%06d",
			(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, (int)msecs);

		return std::string(time);
	}

	static bool CreateSpdLogger(const std::string& file_name)
	{
		try {
			auto console = spd::stdout_color_mt(SPDLOG_CONSOLE_NAME);
			console->info("spdlog startup!");

			auto daily_logger = spd::daily_logger_mt(SPDLOG_DAILY_LOGGER_NAME,
				/*"daily.txt"*/file_name, 0, 0);
			daily_logger->flush_on(spd::level::err);
			daily_logger->flush_on(spd::level::warn);
			daily_logger->flush_on(spd::level::info);
			daily_logger->info("spdlog startup!");

			return true;
		}
		catch (const spd::spdlog_ex &ex) {
			std::cout << "spdlog init failed: " << ex.what() << std::endl;
			return false;
		}
	}

	static void ApplyAllLogger()
	{
		spd::apply_all([&](std::shared_ptr<spdlog::logger> l) {l->info("End of SubscribeDCEL2Data."); });

		spdlog::drop_all();
	}
};


#endif //ZHURONG_COMMONFUNCTION_H