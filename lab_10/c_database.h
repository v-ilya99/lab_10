#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <map>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include "rocksdb/env.h"
#include "rocksdb/iterator.h"

//#include "options/options_parser.h"

typedef std::map<std::string, std::string> map_value_t;
typedef std::map<std::string, map_value_t> map_t;

class c_database
{
public:
	c_database();
	~c_database();
	void print_info(std::string &db_name);
	void create_db(std::string &db_name);
	void generate_sha256_db(std::string &db_name_src, std::string &db_name_sha);
private:
	void get_db_struct(const std::string &name, map_t &db_struct);
private:	
	//std::vector<rocksdb::ColumnFamilyHandle*> m_handles;
	//std::vector<std::string> m_names;
	//std::vector<std::set<std::string>> m_keys;
	//rocksdb::ColumnFamilyOptions m_column_family_options;
	//rocksdb::Options m_db_options;
	
};

