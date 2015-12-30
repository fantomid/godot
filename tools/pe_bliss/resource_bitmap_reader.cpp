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
#include <cmath>
#include "resource_bitmap_reader.h"
#include "pe_resource_viewer.h"
#include "pe_structures.h"

namespace pe_bliss
{
using namespace pe_win;

resource_bitmap_reader::resource_bitmap_reader(const pe_resource_viewer& res)
	:res_(res)
{}

//Returns bitmap data by name and index in language directory (instead of language) (minimum checks of format correctness)
const std::string resource_bitmap_reader::get_bitmap_by_name(const std::wstring& name, uint32_t index) const
{
	return create_bitmap(res_.get_resource_data_by_name(pe_resource_viewer::resource_bitmap, name, index).get_data());
}

//Returns bitmap data by name and language (minimum checks of format correctness)
const std::string resource_bitmap_reader::get_bitmap_by_name(uint32_t language, const std::wstring& name) const
{
	return create_bitmap(res_.get_resource_data_by_name(language, pe_resource_viewer::resource_bitmap, name).get_data());
}

//Returns bitmap data by ID and language (minimum checks of format correctness)
const std::string resource_bitmap_reader::get_bitmap_by_id_lang(uint32_t language, uint32_t id) const
{
	return create_bitmap(res_.get_resource_data_by_id(language, pe_resource_viewer::resource_bitmap, id).get_data());
}

//Returns bitmap data by ID and index in language directory (instead of language) (minimum checks of format correctness)
const std::string resource_bitmap_reader::get_bitmap_by_id(uint32_t id, uint32_t index) const
{
	return create_bitmap(res_.get_resource_data_by_id(pe_resource_viewer::resource_bitmap, id, index).get_data());
}

//Helper function of creating bitmap header
const std::string resource_bitmap_reader::create_bitmap(const std::string& resource_data)
{
	//Create bitmap file header
	bitmapfileheader header = {0};
	header.bfType = 0x4d42; //Signature "BM"
	header.bfOffBits = sizeof(bitmapfileheader) + sizeof(bitmapinfoheader); //Offset to bitmap bits
	header.bfSize = static_cast<uint32_t>(sizeof(bitmapfileheader) + resource_data.length()); //Size of bitmap

	//Check size of resource data
	if(resource_data.length() < sizeof(bitmapinfoheader))
		throw pe_exception("Incorrect resource bitmap", pe_exception::resource_incorrect_bitmap);

	{
		//Get bitmap info header
		const bitmapinfoheader* info = reinterpret_cast<const bitmapinfoheader*>(resource_data.data());

		//If color table is present, skip it
		if(info->biClrUsed != 0)
			header.bfOffBits += 4 * info->biClrUsed; //Add this size to offset to bitmap bits
		else if(info->biBitCount <= 8)
			header.bfOffBits += 4 * static_cast<uint32_t>(std::pow(2.f, info->biBitCount)); //Add this size to offset to bitmap bits
	}

	//Return final bitmap data
	return std::string(reinterpret_cast<const char*>(&header), sizeof(bitmapfileheader)) + resource_data;
}
}
