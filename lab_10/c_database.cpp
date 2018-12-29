#include "pch.h"
#include "c_database.h"

#include <iostream>
#include <exception>

#include "picosha2.h"


c_database::c_database()
{
}


c_database::~c_database()
{
}

void c_database::print_info(std::string &db_name)
{	
	std::cout << "---------------------- Content " << db_name << " ----------------------" << std::endl;
	map_t db_struct;
	get_db_struct(db_name, db_struct);
	for (auto &column_item : db_struct)
	{
		std::cout << "column: " << column_item.first.c_str() << std::endl;
		for (auto &key : column_item.second)
		{
			std::cout << "\tkey = " << key.first.c_str() << "\tvalue = " << key.second.c_str() << std::endl;
		}
	}
	std::cout << "---------------------------------------------------------------------" << std::endl << std::endl << std::endl << std::endl;
}

void c_database::generate_sha256_db(std::string &db_name_src, std::string &db_name_sha)
{
	print_info(db_name_src);
		
	map_t db_struct;
	get_db_struct(db_name_src, db_struct);

	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::DB* db_sha;
	rocksdb::Status s = rocksdb::DB::Open(options, db_name_sha, &db_sha);
	assert(s.ok());
	
	std::vector<rocksdb::ColumnFamilyDescriptor> column_families;
	column_families.push_back(rocksdb::ColumnFamilyDescriptor(rocksdb::kDefaultColumnFamilyName, rocksdb::ColumnFamilyOptions()));
	for (auto &column_item : db_struct)
	{
		if (column_item.first.find("default") != std::string::npos)
		{
			continue;
		}
		rocksdb::ColumnFamilyHandle* cf;
		s = db_sha->CreateColumnFamily(rocksdb::ColumnFamilyOptions(), column_item.first, &cf);
		assert(s.ok());
		delete cf;		
	
		column_families.push_back(rocksdb::ColumnFamilyDescriptor(column_item.first, rocksdb::ColumnFamilyOptions()));
	}
	delete db_sha;

	std::vector<rocksdb::ColumnFamilyHandle*> handles;
	s = rocksdb::DB::Open(rocksdb::DBOptions(), db_name_sha, column_families, &handles, &db_sha);
	assert(s.ok());
	
	for (auto &column_item : db_struct)
	{
		auto pos = std::distance(db_struct.begin(), db_struct.find(column_item.first));
		for (auto &key : column_item.second)
		{
			std::string src_key_value = key.first + "|" + key.second;
			std::string hash_hex_str = picosha2::hash256_hex_string(src_key_value);
			std::cout << column_item.first.c_str() << "::" << src_key_value << " = " << hash_hex_str << std::endl;
			s = db_sha->Put(rocksdb::WriteOptions(), handles[pos], rocksdb::Slice(key.first), rocksdb::Slice(hash_hex_str));
			assert(s.ok());
		}
	}
	
	for (auto handle : handles)
	{
		delete handle;
	}
	delete db_sha;
	std::cout << std::endl << std::endl << std::endl << std::endl;
	print_info(db_name_sha);
}

void c_database::get_db_struct(const std::string & name, map_t & db_struct)
{
	try
	{
		rocksdb::Options m_db_options;
		m_db_options.create_if_missing = true;
		rocksdb::DB *db;
		rocksdb::DBOptions db_options(m_db_options);
		rocksdb::ColumnFamilyOptions cf_options(m_db_options);
		std::vector<std::string> column_family_names;
		std::vector<rocksdb::ColumnFamilyDescriptor> descriptors;
		std::vector<rocksdb::ColumnFamilyHandle*> handles;

		rocksdb::Status s = rocksdb::DB::ListColumnFamilies(db_options, name, &column_family_names);
		assert(s.ok());

		for (auto& item : column_family_names)
		{
			descriptors.emplace_back(rocksdb::ColumnFamilyDescriptor(item, cf_options));
		}
		s = rocksdb::DB::OpenForReadOnly(db_options, name, descriptors, &handles, &db);
		assert(s.ok());

		for (auto& item : handles)
		{
			map_value_t list_keys;
			rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions(), item);
			for (it->SeekToFirst(); it->Valid(); it->Next())
			{
				list_keys[it->key().ToString()] = it->value().ToString();
			}
			assert(it->status().ok()); // Check for any errors found during the scan
			db_struct[item->GetName()] = list_keys;
			delete it;
			
		}		

		for (auto handle : handles)
		{
			delete handle;
		}
		delete db;
		//std::cout << "get_db_struct is success" << std::endl;
	}
	catch (std::exception &ex)
	{
		std::cerr << "Exception! Msg: " << ex.what() << std::endl;
	}
}
