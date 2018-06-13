#include "DCEL2DataRespond.h"
#include <mutex>
#include "Include/DFITCL2ApiDataType.h"
#include "LoadYamlFileConfig.h"
#include "Include/DFITCL2Api.h"

DCEL2DataRespond::DCEL2DataRespond()
:dce_l2_md_api_ptr_(nullptr)
{
}

DCEL2DataRespond::~DCEL2DataRespond()
{
	if (nullptr != dce_l2_md_api_ptr_) {
		DELETE_CONNECTOR((DFITCL2Api * &)dce_l2_md_api_ptr_);
		dce_l2_md_api_ptr_ = nullptr;
	}
}

DCEL2DataRespond* DCEL2DataRespond::GetInstance()
{
	static DCEL2DataRespond* pSingleSpi(nullptr);
	static std::mutex task_mutex;
	if (nullptr == pSingleSpi) {
		std::unique_lock<std::mutex> lock(task_mutex);
		if (nullptr == pSingleSpi) {
			static DCEL2DataRespond task;
			pSingleSpi = &task;
		}
	}
	return pSingleSpi;
}

bool DCEL2DataRespond::CreateDCEL2MdApi()
{
	if (nullptr != dce_l2_md_api_ptr_) {
		DELETE_CONNECTOR(dce_l2_md_api_ptr_);
		dce_l2_md_api_ptr_ = nullptr;
	}

	dce_l2_md_api_ptr_ = NEW_CONNECTOR();
	return nullptr != dce_l2_md_api_ptr_ ? true : false;
}

int DCEL2DataRespond::ConnectDCEL2Server(char * pszSvrAddr, unsigned int RecvQuoteType)
{
	if (nullptr == dce_l2_md_api_ptr_) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "DFITCL2Api have no create.");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "DFITCL2Api have no create.");
		return -1;
	}

	LOG_INFO_4(SPDLOG_CONSOLE_NAME,
		"{0}, connecting dce l2 service:{1}, quote type:{2}, LocalTime:{3}.",
		__FUNCTION__,
		pszSvrAddr,
		RecvQuoteType,
		CommonFunction::GetLocalSystemTime().c_str()
	);

	LOG_INFO_4(SPDLOG_DAILY_LOGGER_NAME,
		"{0}, connecting dce l2 service:{1}, quote type:{2}, LocalTime:{3}.",
		__FUNCTION__,
		pszSvrAddr,
		RecvQuoteType,
		CommonFunction::GetLocalSystemTime().c_str()
	);

	return dce_l2_md_api_ptr_->Connect(pszSvrAddr, this, RecvQuoteType);
}

int DCEL2DataRespond::SubscribeAllMarketData()
{
	if (nullptr == dce_l2_md_api_ptr_) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "DFITCL2Api have no create.");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "DFITCL2Api have no create.");
		return -1;
	}

	LOG_INFO_2(SPDLOG_CONSOLE_NAME,
		"{0}, subscribe all market data, LocalTime:{1}.",
		__FUNCTION__,
		CommonFunction::GetLocalSystemTime().c_str()
	);

	LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
		"{0}, subscribe all market data, LocalTime:{1}.",
		__FUNCTION__,
		CommonFunction::GetLocalSystemTime().c_str()
	);

	return dce_l2_md_api_ptr_->SubscribeAll();
}

void DCEL2DataRespond::OnConnected()
{
	LOG_INFO_2(SPDLOG_CONSOLE_NAME,
		"{0}, connect dce l2 service success, LocalTime:{1}.",
		__FUNCTION__,
		CommonFunction::GetLocalSystemTime().c_str()
	);

	LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
		"{0}, connect dce l2 service success, LocalTime:{1}.",
		__FUNCTION__,
		CommonFunction::GetLocalSystemTime().c_str()
	);

	if (nullptr == dce_l2_md_api_ptr_) {
		LOG_ERR(SPDLOG_CONSOLE_NAME, "DFITCL2Api had no create.");
		LOG_ERR(SPDLOG_DAILY_LOGGER_NAME, "DFITCL2Api had no create.");
		return;
	}

	DFITC_L2::DFITCUserLoginField user_login_field = { 0 };
	strcpy(user_login_field.accountID, LoadYamlFileConfig::GetInstance()->GetUserID().c_str());
	strcpy(user_login_field.passwd, LoadYamlFileConfig::GetInstance()->GetUserPassword().c_str());
	dce_l2_md_api_ptr_->ReqUserLogin(&user_login_field);

	LOG_INFO_4(SPDLOG_CONSOLE_NAME,
		"{0}, login dce l2 server, accountID:{1}, passwd:{2}, LocalTime:{3}.",
		__FUNCTION__,
		user_login_field.accountID,
		user_login_field.passwd,
		CommonFunction::GetLocalSystemTime().c_str());

	LOG_INFO_4(SPDLOG_DAILY_LOGGER_NAME,
		"{0}, login dce l2 server, accountID:{1}, passwd:{2}, LocalTime:{3}.",
		__FUNCTION__,
		user_login_field.accountID,
		user_login_field.passwd,
		CommonFunction::GetLocalSystemTime().c_str());
}

void DCEL2DataRespond::OnDisconnected(int pReason)
{
	LOG_ERR_3(SPDLOG_DAILY_LOGGER_NAME,
		"{0}, disconnect md front service, Reason:{1}, LocalTime:{2}.",
		__FUNCTION__,
		CommonFunction::GetNetErrMsgForCode(pReason),
		CommonFunction::GetLocalSystemTime().c_str());
}

void DCEL2DataRespond::OnRspUserLogin(struct ErrorRtnField * pErrorField)
{
	if (nullptr == pErrorField) { return; }
	if (0 == pErrorField->ErrorID) {
		LOG_INFO_2(SPDLOG_CONSOLE_NAME,
			"{0}, user login ok,LocalTime:{1}",
			__FUNCTION__,
			CommonFunction::GetLocalSystemTime().c_str());

		LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, user login ok,LocalTime:{1}",
			__FUNCTION__,
			CommonFunction::GetLocalSystemTime().c_str());

		NotifyOne();
		return;
	}

	LOG_ERR_3(SPDLOG_CONSOLE_NAME,
		"{0}, user login failed,error msd:{1},LocalTime:{2}",
		__FUNCTION__,
		pErrorField->ErrorMsg,
		CommonFunction::GetLocalSystemTime().c_str());

	LOG_ERR_3(SPDLOG_DAILY_LOGGER_NAME,
		"{0}, user login failed,error msd:{1},LocalTime:{2}",
		__FUNCTION__,
		pErrorField->ErrorMsg,
		CommonFunction::GetLocalSystemTime().c_str());
}

void DCEL2DataRespond::OnRspUserLogout(struct ErrorRtnField * pErrorField)
{
	if (nullptr == pErrorField) { return; }
	if (0 == pErrorField->ErrorID) {
		LOG_INFO_2(SPDLOG_CONSOLE_NAME,
			"{0}, user logout,LocalTime:{1}",
			__FUNCTION__,
			CommonFunction::GetLocalSystemTime().c_str());

		LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, user logout,LocalTime:{1}",
			__FUNCTION__,
			CommonFunction::GetLocalSystemTime().c_str());
	}
	else {
		LOG_ERR_3(SPDLOG_CONSOLE_NAME,
			"{0}, user logout failed,error msd:{1},LocalTime:{2}",
			__FUNCTION__,
			pErrorField->ErrorMsg,
			CommonFunction::GetLocalSystemTime().c_str());

		LOG_ERR_3(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, user logout failed,error msd:{1},LocalTime:{2}",
			__FUNCTION__,
			pErrorField->ErrorMsg,
			CommonFunction::GetLocalSystemTime().c_str());
	}
}

void DCEL2DataRespond::OnRspSubscribeMarketData(struct ErrorRtnField * pErrorField)
{

}

void DCEL2DataRespond::OnRspUnSubscribeMarketData(struct ErrorRtnField * pErrorField)
{

}

void DCEL2DataRespond::OnRspSubscribeAll(struct ErrorRtnField * pErrorField)
{
	if (nullptr == pErrorField) { return; }
	if (0 == pErrorField->ErrorID) {
		LOG_INFO_2(SPDLOG_CONSOLE_NAME,
			"{0}, subscribe all market data success,LocalTime:{1}",
			__FUNCTION__,
			CommonFunction::GetLocalSystemTime().c_str());

		LOG_INFO_2(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, subscribe all market data success,LocalTime:{1}",
			__FUNCTION__,
			CommonFunction::GetLocalSystemTime().c_str());

		NotifyOne();
	}
	else {
		LOG_ERR_3(SPDLOG_CONSOLE_NAME,
			"{0}, subscribe all market data failed,error msd:{1},LocalTime:{2}",
			__FUNCTION__,
			pErrorField->ErrorMsg,
			CommonFunction::GetLocalSystemTime().c_str());

		LOG_ERR_3(SPDLOG_DAILY_LOGGER_NAME,
			"{0}, subscribe all market data failed,error msd:{1},LocalTime:{2}",
			__FUNCTION__,
			pErrorField->ErrorMsg,
			CommonFunction::GetLocalSystemTime().c_str());
	}
}

void DCEL2DataRespond::OnRspUnSubscribeAll(struct ErrorRtnField * pErrorField)
{
}

void DCEL2DataRespond::OnBestAndDeep(struct MDBestAndDeep * const pQuote)
{
	if (nullptr == pQuote) { return; }
	MDBestAndDeep field = { 0 };
	memcpy(&field, pQuote, sizeof(MDBestAndDeep));
	if (subscribe_func_) {
		subscribe_func_(field);
	}
}

void DCEL2DataRespond::OnArbi(struct MDBestAndDeep * const pQuote)
{
	if (nullptr == pQuote) { return; }
	MDBestAndDeep field = { 0 };
	memcpy(&field, pQuote, sizeof(MDBestAndDeep));
	if (subscribe_func_) {
		subscribe_func_(field);
	}
}

void DCEL2DataRespond::OnTenEntrust(struct MDTenEntrust * const pQuote)
{
	if (nullptr == pQuote) { return; }
	MDTenEntrust field = { 0 };
	memcpy(&field, pQuote, sizeof(MDTenEntrust));
	if (subscribe_func_) {
		subscribe_func_(field);
	}
}

void DCEL2DataRespond::OnRealtime(struct MDRealTimePrice * const pQuote)
{
	if (nullptr == pQuote) { return; }
	MDRealTimePrice field = { 0 };
	memcpy(&field, pQuote, sizeof(MDRealTimePrice));
	if (subscribe_func_) {
		subscribe_func_(field);
	}
}

void DCEL2DataRespond::OnOrderStatistic(struct MDOrderStatistic * const pQuote)
{
	if (nullptr == pQuote) { return; }
	MDOrderStatistic field = { 0 };
	memcpy(&field, pQuote, sizeof(MDOrderStatistic));
	if (subscribe_func_) {
		subscribe_func_(field);
	}
}

void DCEL2DataRespond::OnMarchPrice(struct MDMarchPriceQty * const pQuote)
{
	if (nullptr == pQuote) { return; }
	MDMarchPriceQty field = { 0 };
	memcpy(&field, pQuote, sizeof(MDMarchPriceQty));
	if (subscribe_func_) {
		subscribe_func_(field);
	}
}

void DCEL2DataRespond::OnHeartBeatLost()
{
	LOG_WARN(SPDLOG_CONSOLE_NAME, "hreat beat lost!");
}
