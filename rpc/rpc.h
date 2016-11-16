/// @file 
/// @brief Файл содержит список хедаров для осуществления предварительной сборки заголовков

#pragma once

//WINDOWS
#include <rpc/include/targetver.h>

#include <set>
#include <map>
#include <thread>
#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>

#pragma warning ( disable : 4005 )
#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <boost/asio.hpp>


#include <rpc/include/types.h>
#include <rpc/json/common/errors.h>
#include <rpc/json/common/exception.h>
#include <rpc/json/common/specification.h>
#include <rpc/json/value.h>
#include <rpc/json/common/specification.h>
#include <rpc/json/common/procedure.h>
#include <rpc/include/proceduremanager.h>
#include <rpc/json/requesthandlerfactory.h>
#include <rpc/server/reply.hpp>
#include <rpc/json/jsonservice.h>
#include <rpc/server/connection_manager.hpp>
#include <rpc/server/server.hpp>


#include <rpc/include/valueparser.h>
#include <rpc/include/icallbackfunction.h>
#include <rpc/include/callbackfunction.h>
#include <rpc/include/makecallback.h>

#include <rpc/include/command.h>
#include <rpc/include/connectinfo.h>
#include <rpc/include/client.h>
#include <rpc/include/clientmanager.h>

#pragma comment(lib, "libeay32MD.lib")
#pragma comment(lib, "ssleay32MD.lib")
