/// @file 
/// @brief Файл содержит описание типов модуля
#pragma once

namespace rpc
{

namespace info
{
class IConnectInfo;
/// Указатель на описание подключения
typedef std::shared_ptr<IConnectInfo> IConnectInfoPtr;

class IMessageInfo;
/// Указатель на свойства запроса
typedef std::shared_ptr<IMessageInfo> IMessageInfoPtr;
}

namespace client
{

/// Указатель на список команд
class Command;
typedef std::shared_ptr<Command> CommandPtr;
/// Список команд
typedef std::vector<CommandPtr> CommandList;
}

class Value;
/// Список значений
typedef std::vector<Value> ValueList;

class Procedure;
/// Указатель на обработчик процедуры
typedef std::shared_ptr<Procedure> ProcedurePtr;
/// Список соотвествий имени процедуры с описанием обработчика процедуры
typedef std::map<std::wstring, ProcedurePtr> ProcedureList;

class IProtocol;
/// Указатель на интерфейс протокола
typedef std::shared_ptr<IProtocol> IProtocolPtr;

class Service;
/// Указатель на сервис
typedef std::shared_ptr<Service> ServicePtr;

namespace function
{
class ICallback;
/// Указатель на калбечную функцию
typedef std::shared_ptr<ICallback> ICallbackPtr;
}

namespace value
{
/// Описание структуры данных
typedef std::map<std::wstring, std::wstring> MapData;
/// Описание списка данных
typedef std::vector<MapData> MapDataList;

/// Описание структуры данных
typedef std::vector<std::wstring> StructureData;
/// Описание списка данных
typedef std::vector<StructureData> StructureDataList;

/// Описание списка id
typedef std::vector<unsigned int> IdList;
}
}

namespace http 
{
namespace server 
{
class Connection;
/// Указатель на подключение
typedef std::shared_ptr<Connection> ConnectionPtr;
class TcpStream;
class SslStream;
/// Указатель на сокет соединения
typedef std::shared_ptr<SslStream> IStreamPtr;
}

namespace parser 
{
struct reply;
/// Указатель на сообщение
typedef std::shared_ptr<reply> ReplyPtr;

/// Описание списка заголовка
struct tagHeader;
typedef std::vector<tagHeader> Headers;
}
}
