#ifndef ZHURONG_DCEL2DATARESPOND_H
#define ZHURONG_DCEL2DATARESPOND_H

#include "Include/DFITCL2Api.h"
#include <mutex>
#include <condition_variable>
#include "CommonFunction.h"

using namespace DFITC_L2;

class DCEL2DataRespond : public DFITC_L2::DFITCL2Spi
{
protected:
	DCEL2DataRespond();
	virtual ~DCEL2DataRespond();

public:
	static DCEL2DataRespond* GetInstance();

	bool CreateDCEL2MdApi();

	int ConnectDCEL2Server(char * pszSvrAddr, unsigned int RecvQuoteType = 1);

	int SubscribeAllMarketData();

	void RegisterSubscribeL2CallBack(std::function<void(VARIANT_TYPE)> func) {
		subscribe_func_ = func;
	}

	void NotifyOne() { condition_variable_.notify_one(); }

	std::cv_status WaitFor(int second) {
		std::unique_lock<std::mutex> lock(mutex_);
		return condition_variable_.wait_for(lock, std::chrono::seconds(second));
	}

private:
	virtual void OnConnected();
	
	virtual void OnDisconnected(int pReason);

	virtual void OnRspUserLogin(struct ErrorRtnField * pErrorField);

	virtual void OnRspUserLogout(struct ErrorRtnField * pErrorField);

	virtual void OnRspSubscribeMarketData(struct ErrorRtnField * pErrorField);

	virtual void OnRspUnSubscribeMarketData(struct ErrorRtnField * pErrorField);

	virtual void OnRspSubscribeAll(struct ErrorRtnField * pErrorField);

	virtual void OnRspUnSubscribeAll(struct ErrorRtnField * pErrorField);

	virtual void OnBestAndDeep(struct MDBestAndDeep * const pQuote);

	virtual void OnArbi(struct MDBestAndDeep * const pQuote);

	virtual void OnTenEntrust(struct MDTenEntrust * const pQuote);

	virtual void OnRealtime(struct MDRealTimePrice * const pQuote);

	virtual void OnOrderStatistic(struct MDOrderStatistic * const pQuote);

    virtual void OnMarchPrice(struct MDMarchPriceQty * const pQuote);

	virtual void OnHeartBeatLost();

private:
	DFITC_L2::DFITCL2Api* dce_l2_md_api_ptr_;
	std::condition_variable condition_variable_;
	std::mutex mutex_;
	std::function<void(VARIANT_TYPE)>  subscribe_func_;
};

#endif  //ZHURONG_DCEL2DATARESPOND_H