#ifndef ZHURONG_WRITETICKDATAFILE_H
#define ZHURONG_WRITETICKDATAFILE_H

#include <fstream>  
#include "Include/DFITCL2Api.h"

using namespace DFITC_L2;

class WriteTickDataFile 
{
protected:
	WriteTickDataFile();

	~WriteTickDataFile();

public:
	static WriteTickDataFile* GetInstance();

	void Open(const std::string& instrument_name);

	void Close();

	void Write(const MDBestAndDeep& field);

	void Write(const MDTenEntrust& field);

	void Write(const MDRealTimePrice& field);

	void Write(const MDOrderStatistic& field);

	void Write(const MDMarchPriceQty& field);

	void Flush();

private:
	void WriteBestAndDeep(const MDBestAndDeep& field);

	void WriteArbi(const MDBestAndDeep& field);

private:
	std::ofstream out_file_;
	std::string instrument_name_;
};

#endif //ZHURONG_WRITETICKDATAFILE_H
