#ifndef _INFO_TREE_
#define _INFO_TREE_

/*

	Copyright (C) 2015 and beyond by Jeremiah Morris
	and the "Aleph One" developers.
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	This license is contained in the file "COPYING",
	which is included with this source code; it is available online at
	http://www.gnu.org/licenses/gpl.html

	Boost.PropertyTree-based structured-data reader and writer
*/


#include "cseries.h"
#include "FileHandler.h"
#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

class InfoTree : public boost::property_tree::ptree
{
public:
	typedef boost::property_tree::xml_parser_error parse_error;
	typedef boost::property_tree::ini_parser_error ini_error;
	typedef boost::property_tree::ptree_bad_path path_error;
	typedef boost::property_tree::ptree_bad_data data_error;
	typedef boost::property_tree::ptree_error unexpected_error;
	
	InfoTree() {}
	explicit InfoTree(const data_type &data) : boost::property_tree::ptree(data) {}
	InfoTree(const boost::property_tree::ptree &rhs) : boost::property_tree::ptree(rhs) {}
	
	static InfoTree load_xml(FileSpecifier filename);
	static InfoTree load_xml(std::istringstream& stream);
	void save_xml(FileSpecifier filename) const;
	void save_xml(std::ostringstream& stream) const;
	
	static InfoTree load_ini(FileSpecifier filename);
	static InfoTree load_ini(std::istringstream& stream);
	void save_ini(FileSpecifier filename) const;
	void save_ini(std::ostringstream& stream) const;

	template<typename T> bool read(std::string path, T& value) const
	{
		try {
			value = get_child(path).get_value<T>();
			return true;
		} catch (path_error ep) {} catch (data_error ed) {}
		return false;
	}

	template<typename T> bool read_attr(std::string path, T& value) const
	{
		return read(std::string("<xmlattr>.") + path, value);
	}

	template<typename T> bool read_attr_bounded(std::string path, T& value, const T min, const T max) const
	{
		T temp;
		if (read_attr(path, temp) && temp >= min && temp <= max)
		{
			value = temp;
			return true;
		}
		return false;
	}

	template<typename T> void put_attr(std::string path, const T value)
	{
		put(std::string("<xmlattr>.") + path, value);
	}
	

	bool read_color(RGBColor& color) const;
	bool read_color(rgb_color& color) const;
	bool read_path(std::string key, char *dest) const;
	bool read_cstr(std::string key, char *dest, int maxlen) const;
	
	void add_color(std::string path, const RGBColor& color);
	void add_color(std::string path, const RGBColor& color, size_t index);
	void add_color(std::string path, const rgb_color& color);
	void add_color(std::string path, const rgb_color& color, size_t index);
	
	void put_attr_path(std::string path, std::string filepath);
	void put_cstr(std::string path, std::string cstr);
	void put_attr_cstr(std::string path, std::string cstr);
};

#endif