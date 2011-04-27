/*
 * libfscp - C++ portable OpenSSL cryptographic wrapper library.
 * Copyright (C) 2010-2011 Julien Kauffmann <julien.kauffmann@freelan.org>
 *
 * This file is part of libfscp.
 *
 * libfscp is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * libfscp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 *
 * If you intend to use libfscp in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

/**
 * \file session_message.cpp
 * \author Julien Kauffmann <julien.kauffmann@freelan.org>
 * \brief A session message class.
 */

#include "session_message.hpp"

#include <cassert>
#include <stdexcept>

namespace fscp
{
	size_t session_message::write(void* buf, size_t buf_len, const void* ciphertext, size_t ciphertext_len, const void* ciphertext_signature, size_t ciphertext_signature_len)
	{
		const size_t payload_len = MIN_BODY_LENGTH + ciphertext_len + ciphertext_signature_len;

		if (buf_len < HEADER_LENGTH + payload_len)
		{
			throw std::runtime_error("buf_len");
		}

		buffer_tools::set<uint16_t>(buf, HEADER_LENGTH, htons(static_cast<uint16_t>(ciphertext_len)));
		memcpy(static_cast<uint8_t*>(buf) + HEADER_LENGTH + sizeof(uint16_t), ciphertext, ciphertext_len);
		buffer_tools::set<uint16_t>(buf, HEADER_LENGTH + sizeof(uint16_t) + ciphertext_len, htons(static_cast<uint16_t>(ciphertext_signature_len)));
		memcpy(static_cast<uint8_t*>(buf) + HEADER_LENGTH + 2 * sizeof(uint16_t) + ciphertext_len, ciphertext_signature, ciphertext_signature_len);

		message::write(buf, buf_len, CURRENT_PROTOCOL_VERSION, MESSAGE_TYPE_SESSION, payload_len);

		return HEADER_LENGTH + payload_len;
	}

	session_message::session_message(const void* buf, size_t buf_len) :
		message(buf, buf_len)
	{
		if (length() < MIN_BODY_LENGTH)
		{
			throw std::runtime_error("bad message length");
		}

		//TODO: Add parsing verification and make it a function check_format() (just like presentation_message)
	}

	session_message::session_message(const message& _message) :
		message(_message)
	{
		if (length() < MIN_BODY_LENGTH)
		{
			throw std::runtime_error("bad message length");
		}
	}
}
