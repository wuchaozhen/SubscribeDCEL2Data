#include "TickDataContainer.h"
#include "boost/bind.hpp"
#include "DCEL2DataRespond.h"

const int CIRCULAR_BUFFER_SIZE = 1000000; 

TickDataContainer::TickDataContainer(const std::string& tick_path)
:circular_buffer_(CIRCULAR_BUFFER_SIZE)
,is_init_(false)
,tick_path_(tick_path)
{
	WriteTickDataFile::GetInstance()->Open(tick_path);
	CreateConsumptionThread();
}

TickDataContainer::~TickDataContainer()
{
}

void TickDataContainer::CreateConsumptionThread()
{
	is_init_ = true;
	try {
		consumption_thead_ =
			(std::shared_ptr<std::thread>)new std::thread(&TickDataContainer::Run, this);
	}
	catch (std::exception& e) {
		LOG_ERR_2(SPDLOG_DAILY_LOGGER_NAME, "{0}, Error:{1}", __FUNCTION__, e.what());

		WriteTickDataFile::GetInstance()->Close();
		if (-1 == LoadYamlFileConfig::GetInstance()->RenameQuoteFile()) {
			LOG_ERR(SPDLOG_CONSOLE_NAME, "rename quote file name faild");
			LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "rename quote file name faild");
		}

		CommonFunction::ApplyAllLogger();
	}
}

void TickDataContainer::Run()
{
	unsigned int item_count(0);
	while (is_init_) {

#if defined(__linux__)
		if (LoadYamlFileConfig::GetInstance()->GetExitProgram()) {
			//exit the current thread, and the program will exit 		
			break;
		}
#endif

		if (circular_buffer_.empty()) {
			std::unique_lock<std::mutex> lock(mutex_);
			write_condition_variable_.wait_for(lock, std::chrono::seconds(1));
		}

		item_count = circular_buffer_.size();
		if (0 == item_count) { continue; }

		if (item_count > (circular_buffer_.capacity() - 100)) {
			LOG_WARN(SPDLOG_DAILY_LOGGER_NAME, 
				"the circular buffer container has overflow, max_size > item_count.");
		}

		//debug begin
		using namespace std::chrono;
		static auto last_minute_time = std::chrono::system_clock::now();
		auto cur_minute_time = std::chrono::system_clock::now();
		int time = int(duration_cast<minutes>(cur_minute_time - last_minute_time).count()) *
			minutes::period::num / minutes::period::den;
		if (60 <= time) {
			last_minute_time = cur_minute_time;
			LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
				"current time:{0}, circular buffer size:{1}",
				CommonFunction::GetLocalSystemTime(), circular_buffer_.size());
		}
		//debug end

		VARIANT_TYPE field = circular_buffer_.front();
		circular_buffer_.pop_front();

		//write file
		boost::apply_visitor(variant_visition(), field);
	}
}

void TickDataContainer::Init()
{
	DCEL2DataRespond::GetInstance()->RegisterSubscribeL2CallBack(
		boost::bind(&TickDataContainer::PushTickRetFun, this, _1));
}

void TickDataContainer::Join()
{
	if (nullptr != consumption_thead_) {
		consumption_thead_->join();
	}
}

void TickDataContainer::PushTickRetFun(const VARIANT_TYPE& data_field)
{
	write_condition_variable_.notify_one();
	circular_buffer_.push_back(data_field);
}
