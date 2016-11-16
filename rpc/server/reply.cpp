#include <rpc/include/precompiled.h>

namespace misc_strings 
{
const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };
} 

namespace http 
{
namespace parser 
{
namespace status_strings 
{
//=====================================================================================================================
const std::string ok =
  "HTTP/1.0 200 OK\r\n";
const std::string created =
  "HTTP/1.0 201 Created\r\n";
const std::string accepted =
  "HTTP/1.0 202 Accepted\r\n";
const std::string no_content =
  "HTTP/1.0 204 No Content\r\n";
const std::string multiple_choices =
  "HTTP/1.0 300 Multiple Choices\r\n";
const std::string moved_permanently =
  "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily =
  "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified =
  "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request =
  "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized =
  "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden =
  "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found =
  "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error =
  "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented =
  "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway =
  "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable =
  "HTTP/1.0 503 Service Unavailable\r\n";
//=====================================================================================================================
const std::string& to_buffer(reply::status_type status)
{
	switch (status)
	{
	case reply::ok:
		return ok;
	case reply::created:
		return created;
	case reply::accepted:
		return accepted;
	case reply::no_content:
		return no_content;
	case reply::multiple_choices:
		return multiple_choices;
	case reply::moved_permanently:
		return moved_permanently;
	case reply::moved_temporarily:
		return moved_temporarily;
	case reply::not_modified:
		return not_modified;
	case reply::bad_request:
		return bad_request;
	case reply::unauthorized:
		return unauthorized;
	case reply::forbidden:
		return forbidden;
	case reply::not_found:
		return not_found;
	case reply::internal_server_error:
		return internal_server_error;
	case reply::not_implemented:
		return not_implemented;
	case reply::bad_gateway:
		return bad_gateway;
	case reply::service_unavailable:
		return service_unavailable;
	default:
		return internal_server_error;
	}
}
} 
//=====================================================================================================================
namespace stock_replies 
{
const char ok[] = "";

const char created[] =
  "<html>"
  "<head><title>Created</title></head>"
  "<body><h1>201 Created</h1></body>"
  "</html>";
const char accepted[] =
  "<html>"
  "<head><title>Accepted</title></head>"
  "<body><h1>202 Accepted</h1></body>"
  "</html>";
const char non_authoritative[]=
  "<html>"
  "<head><title>Non-Authoritative Information</title></head>"
  "<body><h1>203 Non-Authoritative Information (since HTTP/1.1)</h1></body>"
  "</html>";
const char no_content[] =
  "<html>"
  "<head><title>No Content</title></head>"
  "<body><h1>204 Content</h1></body>"
  "</html>";
const char multiple_choices[] =
  "<html>"
  "<head><title>Multiple Choices</title></head>"
  "<body><h1>300 Multiple Choices</h1></body>"
  "</html>";
const char moved_permanently[] =
  "<html>"
  "<head><title>Moved Permanently</title></head>"
  "<body><h1>301 Moved Permanently</h1></body>"
  "</html>";
const char moved_temporarily[] =
  "<html>"
  "<head><title>Moved Temporarily</title></head>"
  "<body><h1>302 Moved Temporarily</h1></body>"
  "</html>";
const char not_modified[] =
  "<html>"
  "<head><title>Not Modified</title></head>"
  "<body><h1>304 Not Modified</h1></body>"
  "</html>";
const char bad_request[] =
  "<html>"
  "<head><title>Bad Request</title></head>"
  "<body><h1>400 Bad Request</h1></body>"
  "</html>";
const char unauthorized[] =
  "<html>"
  "<head><title>Unauthorized</title></head>"
  "<body><h1>401 Unauthorized</h1></body>"
  "</html>";
const char forbidden[] =
  "<html>"
  "<head><title>Forbidden</title></head>"
  "<body><h1>403 Forbidden</h1></body>"
  "</html>";
const char not_found[] =
  "<html>"
  "<head><title>Not Found</title></head>"
  "<body><h1>404 Not Found</h1></body>"
  "</html>";
const char internal_server_error[] =
  "<html>"
  "<head><title>Internal Server Error</title></head>"
  "<body><h1>500 Internal Server Error</h1></body>"
  "</html>";
const char not_implemented[] =
  "<html>"
  "<head><title>Not Implemented</title></head>"
  "<body><h1>501 Not Implemented</h1></body>"
  "</html>";
const char bad_gateway[] =
  "<html>"
  "<head><title>Bad Gateway</title></head>"
  "<body><h1>502 Bad Gateway</h1></body>"
  "</html>";
const char service_unavailable[] =
  "<html>"
  "<head><title>Service Unavailable</title></head>"
  "<body><h1>503 Service Unavailable</h1></body>"
  "</html>";
const char no_cert[] =
  "<html>"
  "<head><title>No Cert</title></head>"
  "<body><h1>496 No Cert</h1></body>"
  "</html>";
//=====================================================================================================================
std::string to_string( const reply::status_type status )
{
	switch ( status )
	{
	case reply::ok:
		return std::string(ok);
	case reply::created:
		return std::string(created);
	case reply::non_authoritative:
		return std::string(non_authoritative);
	case reply::accepted:
		return std::string(accepted);
	case reply::no_content:
		return std::string(no_content);
	case reply::multiple_choices:
		return std::string(multiple_choices);
	case reply::moved_permanently:
		return std::string(moved_permanently);
	case reply::moved_temporarily:
		return std::string(moved_temporarily);
	case reply::not_modified:
		return std::string(not_modified);
	case reply::bad_request:
		return std::string(bad_request);
	case reply::unauthorized:
		return std::string(unauthorized);
	case reply::forbidden:
		return std::string(forbidden);
	case reply::not_found:
		return std::string(not_found);
	case reply::no_cert:
		return std::string(no_cert);
	case reply::internal_server_error:
		return std::string(internal_server_error);
	case reply::not_implemented:
		return std::string(not_implemented);
	case reply::bad_gateway:
		return std::string(bad_gateway);
	case reply::service_unavailable:
		return std::string(service_unavailable);
	default:
		return std::string(internal_server_error);
	}
}
} 
//=====================================================================================================================
std::stringstream reply::ToBuffers(void) const
{
	std::stringstream stream;
	stream << status_strings::to_buffer(status);
	
	for( const auto& header : headers )
	{
		stream << header.name << misc_strings::name_value_separator << header.value << misc_strings::crlf;
	}

	stream << misc_strings::crlf << content << misc_strings::crlf << misc_strings::crlf;

	return std::move(stream);
}
//=====================================================================================================================
std::stringstream reply::PostToBuffers(void) const
{
	std::stringstream stream;
	stream << "POST rpc_client HTTP/1.0" << misc_strings::crlf;

	for (const auto& header : headers)
	{
		stream << header.name << misc_strings::name_value_separator << header.value << misc_strings::crlf;
	}

	stream << misc_strings::crlf << content << misc_strings::crlf << misc_strings::crlf;

	return std::move(stream);
}
//=====================================================================================================================
ReplyPtr reply::stock_reply( const reply::status_type status )
{
	auto rep = std::make_shared<reply>();
	rep->status = status;
	rep->content = stock_replies::to_string(status);
	rep->headers.resize(2);
	rep->headers[0].name = "Content-Length";
	rep->headers[0].value = std::to_string( rep->content.size() );
	rep->headers[1].name = "Content-Type";
	rep->headers[1].value = "text/html";
	return rep;
}
//=====================================================================================================================
}
}
