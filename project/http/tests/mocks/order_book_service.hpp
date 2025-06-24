#ifndef SIMULATOR_HTTP_TESTS_MOCKS_ORDER_BOOK_SERVICE_HPP_
#define SIMULATOR_HTTP_TESTS_MOCKS_ORDER_BOOK_SERVICE_HPP_

#include <gmock/gmock.h>

#include "mediator/api/requests/orderbook_requests.hpp"
#include "mediator/api/services/orderbook_service.hpp"

namespace Simulator::Http::Mock {

class OrderBookService
    :   public Mediator::Communicators::OrderBookService
{
public:

    using PersistRequest = OrderBook::Request::PersistState;
    using RecoverRequest = OrderBook::Request::RecoverState;

    MOCK_METHOD(void, process, (PersistRequest &), (override));

    MOCK_METHOD(void, process, (RecoverRequest &), (override));
};

} // namespace Simulator::Http::Mock

#endif // SIMULATOR_HTTP_TESTS_MOCKS_ORDER_BOOK_SERVICE_HPP_
