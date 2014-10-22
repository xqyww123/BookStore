#include "stdafx.h"
#include "StorageMng.h"
#include "Config.h"

namespace BookStore{

	bool StorageMng::DbExist()
	{
		return boost::filesystem::is_regular_file(Config::DbAddress);
	}
	boost::archive::binary_iarchive& StorageMng::LoadDb()
	{
		if (!boost::filesystem::exists(Config::DbAddress) || !boost::filesystem::is_regular_file(Config::DbAddress))
			throw runtime_error("Db file not found");
		if (iar) return *iar;
		fin = new fstream(Config::DbAddress.c_str(), ios::in | ios::binary);
		if (!fin->good()) throw runtime_error("Db open failed");
		iar = new iarchive(*fin);
		return *iar;
	}
	// open the write access to the db
	boost::archive::binary_oarchive& StorageMng::SaveDb()
	{
		if (oar) return *oar;
		fout = new fstream(Config::DbAddress.c_str(), ios::out | ios::binary);
		if (!fout->good()) throw runtime_error("Db open failed");
		oar = new oarchive(*fout);
		return *oar;
	}
	void StorageMng::CloseDb(boost::archive::binary_iarchive& ar)
	{
		if (&ar != iar) throw logic_error("Trying to close a archive not created by StorageManager");
		delete iar;
		fin->close();
	}
	void StorageMng::CloseDb(boost::archive::binary_oarchive& ar)
	{
		if (&ar != oar) throw logic_error("Trying to close a archive not created by StorageManager");
		delete oar;
		fout->close();
	}
	void StorageMng::CloseLoadDb()
	{
		if (iar) CloseDb(*iar);
	}
	void StorageMng::CloseSaveDb()
	{
		if (oar) CloseDb(*oar);
	}
}