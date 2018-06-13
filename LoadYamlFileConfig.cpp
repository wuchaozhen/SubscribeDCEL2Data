#include "LoadYamlFileConfig.h"
#include "iostream"
#include <chrono>
#include <sys/stat.h>
#include "CommonFunction.h"
#include <string.h>
#include <fstream>

#if defined(_WIN32)
#include <direct.h>
#endif

LoadYamlFileConfig::LoadYamlFileConfig()
:dce_quote_dir_path_("")
,dce_quote_file_path_("")
,dce_log_dir_path_("")
,dce_log_file_path_("")
,dce_market_data_url_("")
,dce_user_id_("")
,dce_password_("")
,trader_day_("")
,exit_program_(false)
{
}

LoadYamlFileConfig::~LoadYamlFileConfig()
{
}

LoadYamlFileConfig* LoadYamlFileConfig::GetInstance()
{
	static LoadYamlFileConfig* pTask(nullptr);
	if (nullptr == pTask) {
		static LoadYamlFileConfig Task;
		pTask = &Task;
	}

	return pTask;
}

bool LoadYamlFileConfig::ReadConfigInfoFile(const std::string& config_path)
{
	YAML::Node config;

	try {
		config = YAML::LoadFile(config_path);
	}
	catch (std::exception& e) {
		std::cout << config_path << "is" << e.what() << std::endl;
		return false;
	}

	ReadTickDataFilePath(config);

	if (config["dce_login_info"]) {
		ReadDCEMdUrl(config);
		ReadDCEUserId(config);
		ReadDCEPassword(config);
	}

	return true;
}

bool LoadYamlFileConfig::ReadLogConfig(const std::string& config_path)
{
	YAML::Node config;

	try {
		config = YAML::LoadFile(config_path);
	}
	catch (std::exception& e) {
		std::cout << config_path << "is" << e.what() << std::endl;
		return false;
	}

	if (config["log_file_path"] &&
		config["log_file_path"]["dce_log_file_path"]) {
		dce_log_dir_path_ =
			config["log_file_path"]["dce_log_file_path"].as<std::string>();

		return true;
	}

	return false;
}

void LoadYamlFileConfig::ReadTickDataFilePath(const YAML::Node& config)
{
	if (config["tick_data_file_path"] &&
		config["tick_data_file_path"]["dce_tick_file_path"]) {
		dce_quote_dir_path_ =
			config["tick_data_file_path"]["dce_tick_file_path"].as<std::string>();

		LOG_INFO_2(SPDLOG_CONSOLE_NAME,
			"{0}, DCE tick file path:{1}.",
			__FUNCTION__,
			dce_quote_dir_path_);

		LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, DCE tick file path:{1}.",
			__FUNCTION__,
			dce_quote_dir_path_);
		return;
	}

	LOG_ERR_1(SPDLOG_CONSOLE_NAME,
		"{0},load tick_data_file_path::dce_tick_file_path failed.", __FUNCTION__);
	LOG_ERR_1(SPDLOG_DAILY_LOGGER_NAME,
		"{0},load tick_data_file_path::dce_tick_file_path failed.", __FUNCTION__);
}


void LoadYamlFileConfig::ReadDCEUserId(const YAML::Node& config)
{
	if (config["dce_login_info"]["user_id"]) {
		dce_user_id_ =
			config["dce_login_info"]["user_id"].as<std::string>();

		LOG_INFO_2(SPDLOG_CONSOLE_NAME,
			"{0}, DCE user id:{1}.",
			__FUNCTION__,
			dce_user_id_);

		LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, DCE brokder id:{1}.",
			__FUNCTION__,
			dce_user_id_);

		return;
	}

	LOG_ERR_1(SPDLOG_CONSOLE_NAME,
		"{0},load ctp_login_info::user_id failed.", __FUNCTION__);
	LOG_ERR_1(SPDLOG_DAILY_LOGGER_NAME,
		"{0},load ctp_login_info::user_id failed.", __FUNCTION__);
}

void LoadYamlFileConfig::ReadDCEPassword(const YAML::Node& config)
{
	if (config["dce_login_info"]["password"]) {
		dce_password_ =
			config["dce_login_info"]["password"].as<std::string>();
		return;
	}

	LOG_ERR_1(SPDLOG_CONSOLE_NAME,
		"{0},load dce_login_info::password failed.", __FUNCTION__);
	LOG_ERR_1(SPDLOG_DAILY_LOGGER_NAME,
		"{0},load dce_login_info::password failed.", __FUNCTION__);
}

void LoadYamlFileConfig::ReadDCEMdUrl(const YAML::Node& config)
{
	if (config["dce_login_info"]["market_data_url"]) {
		dce_market_data_url_ =
			config["dce_login_info"]["market_data_url"].as<std::string>();

		LOG_INFO_2(SPDLOG_CONSOLE_NAME,
			"{0}, DCE md url:{1}.",
			__FUNCTION__,
			dce_market_data_url_);

		LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, DCE md url:{1}.",
			__FUNCTION__,
			dce_market_data_url_);
		return;
	}

	LOG_ERR_1(SPDLOG_CONSOLE_NAME,
		"{0},load dce_login_info::market_data_url failed.", __FUNCTION__);
	LOG_ERR_1(SPDLOG_DAILY_LOGGER_NAME,
		"{0},load dce_login_info::market_data_url failed.", __FUNCTION__);
}

const std::string& LoadYamlFileConfig::GetLogPath()
{
	MakeLogDirectory();

#if defined(__linux__)
	dce_log_file_path_ += "/";
#elif defined(_WIN32)
	dce_log_file_path_ += "\\";
#endif

	dce_log_file_path_ += "DCE.txt";

	return dce_log_file_path_;
}

void LoadYamlFileConfig::MakeLogDirectory()
{
	dce_log_file_path_ = dce_log_dir_path_;
	dce_log_file_path_.append("dce_log");

#if defined(__linux__)
	mkdir(dce_log_file_path_.c_str(), S_IRWXU);
#elif defined(_WIN32)
	_mkdir(dce_log_file_path_.c_str());
#endif
}

const std::string& LoadYamlFileConfig::MakeQuoteDirectory()
{
#if defined(__linux__)
	mkdir(dce_quote_dir_path_.c_str(), S_IRWXU);
#elif defined(_WIN32)
	_mkdir(dce_quote_dir_path_.c_str());
#endif

	LOG_INFO_2(SPDLOG_CONSOLE_NAME,
		"{0}, create quote directory:{1}.",
		__FUNCTION__,
		dce_quote_dir_path_);

	LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
		"{0}, create quote directory:{1}.",
		__FUNCTION__,
		dce_quote_dir_path_);

	return dce_quote_dir_path_;
}

const std::string& LoadYamlFileConfig::GetQuotePath()
{
	dce_quote_file_path_ = dce_quote_dir_path_;
#if defined(__linux__)
	dce_quote_file_path_ += "level2Quot_unknow.csv";
#elif defined(_WIN32)
	dce_quote_file_path_ += "level2Quot_unknow.csv";
#endif
	return dce_quote_file_path_;
}

int LoadYamlFileConfig::RenameQuoteFile()
{
	std::string init_quote_file_path(dce_quote_file_path_);

	dce_quote_file_path_ = dce_quote_dir_path_;
#if defined(__linux__)
	dce_quote_file_path_ += "level2Quot_";
	dce_quote_file_path_ += trader_day_;
#elif defined(_WIN32)
	dce_quote_file_path_ += "level2Quot_";
	dce_quote_file_path_ += trader_day_;
#endif
	dce_quote_file_path_ += ".csv";

	return rename(init_quote_file_path.c_str(), dce_quote_file_path_.c_str());
}



