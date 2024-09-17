/**
 * File name: Base64EncoderDecoder.cpp
 * Project: Geonkick (A percussion synthesizer)
F *
 * Copyright (C) 2023 Iurie Nistor 
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "Base64EncoderDecoder.h"

#include <cstring>

std::string Base64EncoderDecoder::encode(const std::vector<float> &data)
{
	constexpr std::array<char, 64> base64Chars = {{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		}};

    std::string result;
    size_t i = 0;
    auto bytes = reinterpret_cast<const unsigned char*>(data.data());
    auto bytesSize = data.size() * sizeof(float);
    while (i < bytesSize) {
        unsigned char b1 = (i < bytesSize) ? bytes[i++] : 0;
        unsigned char b2 = (i < bytesSize) ? bytes[i++] : 0;
        unsigned char b3 = (i < bytesSize) ? bytes[i++] : 0;
        result += base64Chars[(b1 & 0xfc) >> 2];
        result += base64Chars[((b1 & 0x03) << 4) | ((b2 & 0xf0) >> 4)];
        result += base64Chars[((b2 & 0x0f) << 2) | ((b3 & 0xc0) >> 6)];
        result += base64Chars[b3 & 0x3f];
    }

    // Pad with '=' if necessary.
    while (result.size() % 4 != 0)
        result += '=';
    return result;
}

std::vector<float> Base64EncoderDecoder::decode(const std::string &input)
{
	auto decodeChar = [&](char c) {
		constexpr std::array<signed char, 256> base64CharMap = {{
				/* Initialize with -1 for characters not in base64Chars */
				-1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, 62, -1, -1, -1, 63,
				52, 53, 54, 55, 56, 57, 58, 59,
				60, 61, -1, -1, -1, -1, -1, -1,
				-1, 0, 1, 2, 3, 4, 5, 6,
				7, 8, 9, 10, 11, 12, 13, 14,
				15, 16, 17, 18, 19, 20, 21, 22,
				23, 24, 25, -1, -1, -1, -1, -1,
				-1, 26, 27, 28, 29, 30, 31, 32,
				33, 34, 35, 36, 37, 38, 39, 40,
				41, 42, 43, 44, 45, 46, 47, 48,
				49, 50, 51, -1, -1, -1, -1, -1
			}};
		return static_cast<unsigned char>(base64CharMap[static_cast<unsigned char>(c)]);
        };

        std::vector<unsigned char> bytes;
        size_t i = 0;
        while (i < input.size()) {
            unsigned char b1 = decodeChar(input[i++]);
            unsigned char b2 = decodeChar(input[i++]);
            bytes.push_back((b1 << 2) | ((b2 & 0x30) >> 4));
            if (i < input.size() && input[i] != '=') {
                unsigned char b3 = decodeChar(input[i++]);
                bytes.push_back(((b2 & 0x0f) << 4) | ((b3 & 0x3c) >> 2));
                if (i < input.size() && input[i] != '=') {
                    unsigned char b4 = decodeChar(input[i++]);
                    bytes.push_back(((b3 & 0x03) << 6) | (b4 & 0x3f));
                }
            }
        }

        std::vector<float> result(bytes.size() / sizeof(float));
        std::memcpy(result.data(), bytes.data(), result.size() * sizeof(float));
        return result;
}
