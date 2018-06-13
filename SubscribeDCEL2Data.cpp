#include "DCEL2DataRespond.h"
#include <string>
#include "CommonFunction.h"
#include "TickDataContainer.h"
#include "LoadYamlFileConfig.h"

#if defined(__linux__)
#include <signal.h>
#include <unistd.h>
static void ExitProgram(int signal)
{
	LOG_INFO_2(SPDLOG_CONSOLE_NAME, "{0}:catch exit program signal:{1}", __FUNCTION__, signal);
	LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME, "{0}:catch exit program signal:{1}", __FUNCTION__, signal);
	if (SIGQUIT == signal) {
		//void TickDataContainer::Run() will get exit_program
		LoadYamlFileConfig::GetInstance()->SetExitProgram(true);
	}
	else {
		LOG_INFO(SPDLOG_CONSOLE_NAME, "the program has not safety closure");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "the program has not safety closure");
	}
}
#else
BOOL CALLBACK ExitProgram(DWORD events)
{
	LOG_INFO_2(SPDLOG_CONSOLE_NAME, "{0}:catch exit program signal:{1}", __FUNCTION__, events);
	LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME, "{0}:catch exit program signal:{1}", __FUNCTION__, events);

	if (CTRL_CLOSE_EVENT != events) { return false; }

	//the quote file name can be changed after closing  the file handle
	WriteTickDataFile::GetInstance()->Close();
	if (-1 == LoadYamlFileConfig::GetInstance()->RenameQuoteFile()) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "rename quote file name faild");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "rename quote file name faild");
	}

	LOG_INFO(SPDLOG_CONSOLE_NAME, "the program safety closure");
	LOG_INFO(SPDLOG_DAILY_LOGGER_NAME, "the program safety closure");

	CommonFunction::ApplyAllLogger();

	return true;
}
#endif

int main(int argc, char** argv)
{
	//read log directory path
	if (!LoadYamlFileConfig::GetInstance()->ReadLogConfig(argv[1])) {
		std::cout << "read log directory faild \n";
	}
	/////////////////////////////////////////

	//startup spdlog
	CommonFunction::CreateSpdLogger(LoadYamlFileConfig::GetInstance()->GetLogPath());
	/////////////////////////////////////////

	//read config info from yaml
	if (argc < 2) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "no parameter configuration for program!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "no parameter configuration for program!");
		LOG_ERR(SPDLOG_CONSOLE_NAME, "the program exit!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "the program exit!");

		CommonFunction::ApplyAllLogger();

#if defined(_WIN32) 
		system("PAUSE");
#endif
		return -1;
	}

	if (!LoadYamlFileConfig::GetInstance()->ReadConfigInfoFile(argv[1])) {
		LOG_ERR_1(SPDLOG_CONSOLE_NAME, "load {0} failed!", argv[1]);
		LOG_ERR_1(SPDLOG_DAILY_LOGGER_NAME, "load {0} failed!", argv[1]);
		LOG_ERR(SPDLOG_CONSOLE_NAME, "the program exit!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "the program exit!");

		CommonFunction::ApplyAllLogger();

#if defined(_WIN32)
		system("PAUSE");
#endif

		return -1;
	}
	/////////////////////////////////////////

	//init dce l2 api respond   
	DCEL2DataRespond* dce_l2_api(DCEL2DataRespond::GetInstance());
	//////////////////////////////////////////

	//create dce l2 api
	if (!dce_l2_api->CreateDCEL2MdApi()) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "create DFITCL2Api failed, the program exit!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "create DFITCL2Api failed, the program exit!");
		return -1;
	}
	/////////////////////////////////////////

	//start write file thread
	std::shared_ptr<TickDataContainer> tick_data_container_ptr(
		std::make_shared<TickDataContainer>(
			LoadYamlFileConfig::GetInstance()->GetQuotePath()));
	tick_data_container_ptr->Init();
	////////////////////////////////////////

	//connect login dce l2 server
	if (0 != dce_l2_api->ConnectDCEL2Server(
		const_cast<char*>(LoadYamlFileConfig::GetInstance()->GetMarketUrl().c_str()), 1)) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "connect dce l2 failed,the program exit!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "connect dce l2 failed, the program exit!");
#if defined(_WIN32)
		system("PAUSE");
#endif
		return -1;
	}
	///////////////////////////////////////

	//blocking waiting for dec l2 login success
	if (std::cv_status::timeout == dce_l2_api->WaitFor(30)) {
		LOG_WARN(SPDLOG_CONSOLE_NAME, "connect login dce l2 server time out!");
		LOG_WARN(SPDLOG_DAILY_LOGGER_NAME, "connect login dce l2 server time out!");

		LOG_ERR(SPDLOG_CONSOLE_NAME, "the program exit!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "the program exit!");
		CommonFunction::ApplyAllLogger();

#if defined(_WIN32)
		system("PAUSE");
#endif
		return -1;
	}
	///////////////////////////////////////

	//subscribe all
	if (0 != dce_l2_api->SubscribeAllMarketData()) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "subscribe all market data failed,the program exit!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "subscribe all market data failed,the program exit!");
#if defined(_WIN32)
		system("PAUSE");
#endif
		return -1;
	}
	///////////////////////////////////////

	//blocking waiting for dec 12 subscribe success
	if (std::cv_status::timeout == dce_l2_api->WaitFor(10)) {
		LOG_WARN(SPDLOG_CONSOLE_NAME, "subscribe all market data time out!");
		LOG_WARN(SPDLOG_DAILY_LOGGER_NAME, "subscribe all market data time out!");

		LOG_ERR(SPDLOG_CONSOLE_NAME, "the program exit!");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "the program exit!");
		CommonFunction::ApplyAllLogger();

#if defined(_WIN32)
		system("PAUSE");
#endif
		return -1;
	}
	////////////////////////////////////////


	//set program quit signal
#if defined(__linux__)
	struct sigaction sig;
	sig.sa_handler = ExitProgram;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	if (-1 == sigaction(SIGQUIT, &sig, 0)) {
		LOG_INFO(SPDLOG_CONSOLE_NAME, "the program can't catch SIGQUIT");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "the program can't catch SIGQUIT");
	}

	LOG_INFO_1(SPDLOG_CONSOLE_NAME, "the program pid : {0}", getpid());
#else
	if (!SetConsoleCtrlHandler(ExitProgram, TRUE)) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "the program can't catch SIGQUIT");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "the program can't catch SIGQUIT");
	}
#endif
	/////////////////////////////

	//block until the end of the sub thread
	tick_data_container_ptr->Join();
	////////////////////////////

	//the quote file name can be changed after closing the file handle
	WriteTickDataFile::GetInstance()->Close();
	if (-1 == LoadYamlFileConfig::GetInstance()->RenameQuoteFile()) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "rename quote file name faild");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "rename quote file name faild");
	}
	/////////////////////////////

	LOG_INFO(SPDLOG_CONSOLE_NAME, "the program safety closure");
	LOG_INFO(SPDLOG_DAILY_LOGGER_NAME, "the program safety closure");

	CommonFunction::ApplyAllLogger();

#if defined(_WIN32)
	system("PAUSE");
#endif
	return 0;
}

