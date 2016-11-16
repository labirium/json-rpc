#pragma once

namespace http 
{
namespace parser 
{

typedef struct tagHeader
{
	std::string name;
	std::string value;
} Header;

/// A request received from a client.
struct request
{
	std::string method;
	std::string uri;
	int http_version_major;
	int http_version_minor;
	Headers headers;

	std::string GetHeaderValue( const std::string& key ) const
	{
		const auto iterator = std::find_if
		( 
			headers.begin(), 
			headers.end(), 
			[key]( const Header& data )
			{
				return data.name == key;
			} );
		if( headers.end() == iterator ){
			return std::string();
		}

		return iterator->value;		
	}

};
/// Result of parse.
enum result_type 
{ 
	good, 
	bad, 
	indeterminate 
};

/// Parser for incoming requests.
class request_parser
{
	 /// The current state of the parser.
	typedef enum State
	{
		method_start,
		method,
		uri,
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3
	};
public:
	/// @brief Конструктор
	///
	request_parser(const bool client);
	/// @brief Деструктор
	///
	~request_parser();
public:
    /// Reset to initial parser state.
    void reset( void );


	/// Parse some data. The enum return value is good when a complete request has
	/// been parsed, bad if the data is invalid, indeterminate when more data is
	/// required. The InputIterator return value indicates how much of the input
	/// has been consumed.
	template <typename InputIterator>
	std::tuple<result_type, InputIterator> parse( request& req, InputIterator begin, InputIterator end )
	{
		while (begin != end)
		{
			const result_type result = consume( req, *begin++ );
			if ( good == result || bad == result )
			{
				return std::make_tuple(result, begin);
			}
		}
    
		return std::make_tuple(indeterminate, begin);
	}

	template <typename InputIterator>
	std::tuple<result_type, InputIterator> parse_client(request& req, InputIterator begin, InputIterator end)
	{
		while (begin != end)
		{
			const result_type result = consume(req, *begin++);
			if (good == result || bad == result)
			{
				return std::make_tuple(result, begin);
			}
		}

		return std::make_tuple(indeterminate, begin);
	}

private:
	/// Handle the next character of input.
	result_type consume(request& req, char input);
private:
	State state_;
	bool client_;
};

template <typename InputIterator>
std::tuple<result_type, InputIterator> Parse( request& req, InputIterator begin, InputIterator end, bool client = false )
{
	return request_parser(client).parse( req, begin, end );
}

}
} 
