/// @file 
/// @brief Файл содержит список хедаров для осуществления предварительной сборки заголовков

#pragma once

//WINDOWS
#include <rpc/include/targetver.h>

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <boost/asio.hpp>

#include <map>
#include <string>
#include <mutex> 
#include <cstdlib>
#include <sstream>
#include <exception>
#include <set>
#include <signal.h>
#include <utility>
#include <vector>
#include <thread>
#include <memory>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional/optional.hpp>
#include <boost/locale.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


#include <boost/asio/ssl.hpp>

#include <rpc/include/types.h>
#include <rpc/json/common/errors.h>
#include <rpc/json/common/exception.h>
#include <rpc/json/common/specification.h>

#include <rpc/json/common/procedure.h>
#include <rpc/include/proceduremanager.h>

#include <rpc/json/iprotocol.h>
#include <rpc/json/requesthandlerfactory.h>

#include <rpc/server/reply.hpp>
#include <rpc/server/mime_types.hpp>

#include <rpc/json/jsonservice.h>
#include <rpc/json/value.h>
#include <rpc/json/abstractprotocolhandler.h>
#include <rpc/json/rpcprotocolserverv2.h>

#include <rpc/server/reply.hpp>
#include <rpc/server/request_parser.hpp>
#include <rpc/server/connection.hpp>
#include <rpc/server/connection_manager.hpp>

#include <rpc/server/server.hpp>

#include <rpc/include/valueparser.h>
#include <rpc/include/icallbackfunction.h>

#include <rpc/include/command.h>
#include <rpc/include/connectinfo.h>
#include <rpc/include/client.h>
#include <rpc/include/clientmanager.h>

#pragma comment(lib, "libeay32MD.lib")
#pragma comment(lib, "ssleay32MD.lib")
