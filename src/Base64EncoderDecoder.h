/**
 * File name: Base64EncoderDecoder.h
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

#ifndef GEONKICK_BASE64_ENCODER_DECODER
#define GEONKICK_BASE64_ENCODER_DECODER

#include "globals.h"

class Base64EncoderDecoder {
 public:
  static std::string encode(const std::vector<float> &data);
  static std::vector<float> decode(const std::string &input);
};

#endif // GEONKICK_BASE64_ENCODER_DECODER
