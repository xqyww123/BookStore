/* StorageMng.h
 * 
 * by Xero Essential (–Ï∆Ù‘¥ 3130000950)
 */
#pragma once
#include "stdafx.h"
#include <boost\archive\binary_iarchive.hpp>
#include <boost\archive\binary_oarchive.hpp>

namespace BookStore{

	class StorageMng{
	public:
		typedef boost::archive::binary_iarchive iarchive;
		typedef boost::archive::binary_oarchive oarchive;

	private:
		
		iarchive* iar;
		oarchive* oar;
		fstream *fin, *fout;
	public:
	
		// check if db file exist
		static bool DbExist();
		// open the read access to the db
		boost::archive::binary_iarchive& LoadDb();
		// open the write access to the db
		boost::archive::binary_oarchive& SaveDb();
		void CloseDb(boost::archive::binary_iarchive& ar);
		void CloseDb(boost::archive::binary_oarchive& ar);
		void CloseLoadDb();
		void CloseSaveDb();
	};
}