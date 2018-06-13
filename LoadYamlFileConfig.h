#ifndef ZHURONG_LOADYAMLFILECONFIG_H
#define ZHURONG_LOADYAMLFILECONFIG_H

#include <string>
#include <set>
#include "Include/yaml-cpp/yaml.h"

class LoadYamlFileConfig
{
private:
	LoadYamlFileConfig();
	~LoadYamlFileConfig();

public:
	static LoadYamlFileConfig* GetInstance();

	bool ReadConfigInfoFile(const std::string& config_path);

	bool ReadLogConfig(const std::string& config_path);
	
	const std::string& GetLogPath();

	const std::string& MakeQuoteDirectory();

	const std::string& GetQuotePath();

	const std::string& GetMarketUrl() { return dce_market_data_url_; }

	const std::string& GetUserID() { return dce_user_id_;  };

	const std::string& GetUserPassword() { return dce_password_;  };

	void UpdataQuoteFileData(std::string trader_day) { trader_day_ = trader_day;  };

	int RenameQuoteFile();

	void SetExitProgram(bool exit) { exit_program_ = exit; }

	bool GetExitProgram() { return exit_program_; };
	
private:
	void ReadTickDataFilePath(const YAML::Node& config);

	void ReadDCEMdUrl(const YAML::Node& config);

	void ReadDCEUserId(const YAML::Node& config);

	void ReadDCEPassword(const YAML::Node& config);

	void MakeLogDirectory();


private:
	std::string dce_quote_dir_path_;
	std::string dce_quote_file_path_;
	std::string dce_log_dir_path_;
	std::string dce_log_file_path_;
	std::string dce_market_data_url_;
	std::string dce_user_id_;
	std::string dce_password_;
	std::string trader_day_;
	bool exit_program_;
};

#endif //ZHURONG_LOADYAMLFILECONFIG_H
