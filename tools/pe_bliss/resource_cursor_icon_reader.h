/*************************************************************************/
/* Copyright (c) 2015 dx, http://kaimi.ru                                */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person           */
/* obtaining a copy of this software and associated documentation        */
/* files (the "Software"), to deal in the Software without               */
/* restriction, including without limitation the rights to use,          */
/* copy, modify, merge, publish, distribute, sublicense, and/or          */
/* sell copies of the Software, and to permit persons to whom the        */
/* Software is furnished to do so, subject to the following conditions:  */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#pragma once
#include <string>
#include "stdint_defs.h"

namespace pe_bliss
{
class pe_resource_viewer;

class resource_cursor_icon_reader
{
public:
	resource_cursor_icon_reader(const pe_resource_viewer& res);

	//Returns single icon data by ID and language (minimum checks of format correctness)
	const std::string get_single_icon_by_id_lang(uint32_t language, uint32_t id) const;
	//Returns single icon data by ID and index in language directory (instead of language) (minimum checks of format correctness)
	const std::string get_single_icon_by_id(uint32_t id, uint32_t index = 0) const;

	//Returns icon data of group of icons by name and language (minimum checks of format correctness)
	const std::string get_icon_by_name(uint32_t language, const std::wstring& icon_group_name) const;
	//Returns icon data of group of icons by name and index in language directory (instead of language) (minimum checks of format correctness)
	const std::string get_icon_by_name(const std::wstring& icon_group_name, uint32_t index = 0) const;
	//Returns icon data of group of icons by ID and language (minimum checks of format correctness)
	const std::string get_icon_by_id_lang(uint32_t language, uint32_t icon_group_id) const;
	//Returns icon data of group of icons by ID and index in language directory (instead of language) (minimum checks of format correctness)
	const std::string get_icon_by_id(uint32_t icon_group_id, uint32_t index = 0) const;
	
	//Returns single cursor data by ID and language (minimum checks of format correctness)
	const std::string get_single_cursor_by_id_lang(uint32_t language, uint32_t id) const;
	//Returns single cursor data by ID and index in language directory (instead of language) (minimum checks of format correctness)
	const std::string get_single_cursor_by_id(uint32_t id, uint32_t index = 0) const;

	//Returns cursor data by name and language (minimum checks of format correctness)
	const std::string get_cursor_by_name(uint32_t language, const std::wstring& cursor_group_name) const;
	//Returns cursor data by name and index in language directory (instead of language) (minimum checks of format correctness)
	const std::string get_cursor_by_name(const std::wstring& cursor_group_name, uint32_t index = 0) const;
	//Returns cursor data by ID and language (minimum checks of format correctness)
	const std::string get_cursor_by_id_lang(uint32_t language, uint32_t cursor_group_id) const;
	//Returns cursor data by ID and index in language directory (instead of language) (minimum checks of format correctness)
	const std::string get_cursor_by_id(uint32_t cursor_group_id, uint32_t index = 0) const;

private:
	const pe_resource_viewer& res_;

	//Helper function of creating icon headers from ICON_GROUP resource data
	//Returns icon count
	static uint16_t format_icon_headers(std::string& ico_data, const std::string& resource_data);
	
	//Helper function of creating cursor headers from CURSOR_GROUP resource data
	//Returns cursor count
	uint16_t format_cursor_headers(std::string& cur_data, const std::string& resource_data, uint32_t language, uint32_t index = 0xFFFFFFFF) const;

	//Looks up icon group by icon id and returns full icon headers if found
	const std::string lookup_icon_group_data_by_icon(uint32_t icon_id, uint32_t language) const;
	//Checks for icon presence inside icon group, fills icon headers if found
	static bool check_icon_presence(const std::string& icon_group_resource_data, uint32_t icon_id, std::string& ico_data);

	//Looks up cursor group by cursor id and returns full cursor headers if found
	const std::string lookup_cursor_group_data_by_cursor(uint32_t cursor_id, uint32_t language, const std::string& raw_cursor_data) const;
	//Checks for cursor presence inside cursor group, fills cursor headers if found
	static bool check_cursor_presence(const std::string& icon_group_resource_data, uint32_t cursor_id, std::string& cur_header_data, const std::string& raw_cursor_data);
};
}
