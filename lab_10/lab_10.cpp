// lab_10.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>

#include <cstdio>
#include <string>
#include <vector>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

#include <boost/program_options.hpp>

#include "c_database.h"

std::string kDBPath = "D:\\work\\lab_10\\db\\src_1";

using namespace rocksdb;

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print usage message")
		//("print-info,p", po::value<std::string>()->default_value("D:\\work\\lab_10\\db\\src_1"), "print info from database")
		("input,i", po::value<std::string>()->default_value("D:\\work\\lab_10\\db\\src_1"), "source database name")
		("output,o", po::value<std::string>()->default_value("D:\\work\\lab_10\\db\\output"), "output sha256 database name")
		("generate,g", po::value<std::string>()->default_value("D:\\work\\lab_10\\db\\output"), "output sha256 database name")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	
	try
	{
		po::notify(vm);
	}
	catch (std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		return EXIT_FAILURE;
	}
	
	std::string in_db_name;
	std::string out_db_name;
	if (vm.count("help"))
	{
		std::cout << desc << "\n";
		return EXIT_SUCCESS;
	}
	//else if (vm.count("print-info"))
	//{
	//	std::string database_name = vm["print-info"].as<std::string>();
	//	std::cout << "database_name: " << database_name.c_str() << "\n";
	//	c_database database;
	//	database.print_info(database_name);
	//
	//	return EXIT_SUCCESS;
	//}	
		
	if (vm.count("input"))
	{
		in_db_name = vm["input"].as<std::string>();
		std::cout << "input database name: " << in_db_name.c_str() << std::endl;
	}
	
	if (vm.count("output"))
	{
		out_db_name = vm["output"].as<std::string>();
		std::cout << "output database name: " << out_db_name.c_str() << std::endl;
	}	

	try
	{
		c_database database;
		database.generate_sha256_db(in_db_name, out_db_name);
	}
	catch (std::exception &ex)
	{
		std::cerr << "Exception! Msg: " << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Exception! Unknown!" << std::endl;
	}
	return EXIT_SUCCESS;

	//rocksdb::Options options;
	//options.create_if_missing = true;
	//rocksdb::DB* db;
	//Status s = DB::Open(options, kDBPath, &db);
	//assert(s.ok());
	//
	//// create column family
	//ColumnFamilyHandle* cf;
	//ColumnFamilyHandle* cf1;
	//s = db->CreateColumnFamily(ColumnFamilyOptions(), "new_cf", &cf);
	//assert(s.ok());
	//s = db->CreateColumnFamily(ColumnFamilyOptions(), "new_cf2", &cf1);
	//assert(s.ok());
	//
	//// close DB
	//delete cf;
	//delete cf1;
	//delete db;
	//
	//// open DB with two column families
	//std::vector<ColumnFamilyDescriptor> column_families;
	//// have to open default column family
	//column_families.push_back(ColumnFamilyDescriptor(kDefaultColumnFamilyName, ColumnFamilyOptions()));
	//// open the new one, too
	//column_families.push_back(ColumnFamilyDescriptor("new_cf", ColumnFamilyOptions()));
	//column_families.push_back(ColumnFamilyDescriptor("new_cf2", ColumnFamilyOptions()));
	//std::vector<ColumnFamilyHandle*> handles;
	//s = DB::Open(DBOptions(), kDBPath, column_families, &handles, &db);
	//assert(s.ok());
	//
	//// put and get from non-default column family
	//s = db->Put(WriteOptions(), handles[1], Slice("key"), Slice("value"));
	//assert(s.ok());
	//std::string value;
	//s = db->Get(ReadOptions(), handles[1], Slice("key"), &value);
	//assert(s.ok());
	//
	////// atomic write
	////WriteBatch batch;
	////batch.Put(handles[0], Slice("key2"), Slice("value2"));
	////batch.Put(handles[1], Slice("key3"), Slice("value3"));
	////batch.Delete(handles[0], Slice("key"));
	////s = db->Write(WriteOptions(), &batch);
	////assert(s.ok());
	//
	//s = db->Put(WriteOptions(), handles[2], Slice("2_key_1"), Slice("value_1"));
	//assert(s.ok());
	//s = db->Put(WriteOptions(), handles[2], Slice("2_key_2"), Slice("value_2"));
	//assert(s.ok());
	//s = db->Put(WriteOptions(), handles[2], Slice("2_key_3"), Slice("value_3"));
	//assert(s.ok());
	//s = db->Put(WriteOptions(), handles[2], Slice("2_key_4"), Slice("value_4"));
	//assert(s.ok());
	//
	////// drop column family
	////s = db->DropColumnFamily(handles[1]);
	////assert(s.ok());
	//
	//// close db
	//for (auto handle : handles) {
	//	delete handle;
	//}
	//delete db;
	
	//return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
