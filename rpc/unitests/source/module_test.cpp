/// @file 
/// @brief Файл используется для модульных тестов
#include <unitests/include/precompiled.h>

namespace
{
/// @britf Осуществляет формирование ответа на запрос
///
const auto callbackMapDataList = rpc::function::make_callback_ptr<int, int>
(
	L"count",
	[](const int count)
{
	
	if (count == 62){
		throw rpc::value::Exception(rpc::enums::ERROR_RPC_INVALID_PARAMS, L"Invalid parametrs");
	}
	return count + 1;
}
);

/// @brief Осуществляет обработку JSON ответа на запрос
///
const auto callbackMapDataListResult = rpc::function::make_result_callback_ptr<int>
([](int data, const rpc::info::IMessageInfoPtr& messageInfo)
{
	std::wcout << L"Result: " << data << std::endl;
},
[](const int code, const std::wstring& message, const rpc::info::IMessageInfoPtr& messageInfo)
{
	std::wcout << L"JSON error detect: " << message << std::endl;
});
}

/// @breif Тест осуществляет регистрацию функции для возврата длинного списка
///
TEST(ModuleTest, callbackMapDataList)
{
	std::string host("127.0.0.1");
	std::string port("5082");

	//Server
	rpc::ProcedureManager procedureManager;
	procedureManager.Add(rpc::Procedure(L"callbackMapDataList", callbackMapDataList));
	const auto service = std::make_shared<rpc::Service>(procedureManager);

	http::server::Server server(service, host, port);
	// Запуск сервера подключений
	server.Run();

	//Client
	rpc::ProcedureManager procedureManagerResult;
	procedureManagerResult.Add(rpc::Procedure(L"callbackMapDataList_result", callbackMapDataListResult));
	const auto serviceResult = std::make_shared<rpc::Service>(procedureManagerResult);

	rpc::client::ClientManager client(serviceResult);
	
	//отправка сообщения
	boost::property_tree::wptree params;
	params.put(L"count", 12);
	rpc::Value value(params);

	
	// Запуск сервиса клиенских подключений
	client.Run();

	auto testCommand = std::make_shared<rpc::client::Command>(L"callbackMapDataList", value, L"callbackMapDataList_result");

	rpc::client::CommandList commands;
	commands.push_back(testCommand);
	commands.push_back(testCommand);
}

