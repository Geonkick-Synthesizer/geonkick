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

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <cstring>

std::string Base64EncoderDecoder::encode(const std::vector<float> &data)
{
  std::vector<unsigned char> bytes(data.size() * sizeof(float));
  std::memcpy(bytes.data(), data.data(), bytes.size());

  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  bio = BIO_new(BIO_s_mem());
  BIO_push(b64, bio);

  BIO_write(b64, bytes.data(), static_cast<int>(bytes.size()));
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bufferPtr);

  std::string encoded(bufferPtr->data, bufferPtr->length);

  BIO_free_all(b64);
  return encoded;
}

std::vector<float> Base64EncoderDecoder::decode(const std::string &input)
{
  BIO *bio, *b64;

  bio = BIO_new_mem_buf(input.c_str(), -1);
  b64 = BIO_new(BIO_f_base64());
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  BIO_push(b64, bio);

  char outputBuffer[input.length()];
  int decodedLength = BIO_read(b64, outputBuffer, input.length());

  std::vector<float> decoded(decodedLength / sizeof(float));
  std::memcpy(decoded.data(), outputBuffer, decodedLength);

  BIO_free_all(b64);
  return decoded;
}
