#pragma once

namespace http
{
namespace parser
{

/// A reply to be sent to a client.
struct reply
{
	/// The status of the reply.
	enum status_type
	{
		ok = 200,
		created = 201,
		accepted = 202,
		non_authoritative = 203, 
		no_content = 204,
		multiple_choices = 300,
		moved_permanently = 301,
		moved_temporarily = 302,
		not_modified = 304,
		bad_request = 400,
		unauthorized = 401,
		forbidden = 403,
		not_found = 404,
		no_cert = 496,
		internal_server_error = 500,
		not_implemented = 501,
		bad_gateway = 502,
		service_unavailable = 503
	} status;

	/// Convert the reply into a vector of buffers. The buffers do not own the
	/// underlying memory blocks, therefore the reply object must remain valid and
	/// not be changed until the write operation has completed.
	std::stringstream ToBuffers(void) const;
	/// @brief Формирует и возвращает post запрос в буфер
	///
	std::stringstream PostToBuffers(void) const;

	/// Get a stock reply.
	static ReplyPtr stock_reply( const status_type status);
public:
	/// The headers to be included in the reply.
	std::vector<tagHeader> headers;
	/// The content to be sent in the reply.
	std::string content;
};

} 
} 

