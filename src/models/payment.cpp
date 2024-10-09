#include "payment.h"
#include <bsoncxx/types.hpp>

void to_bson(bsoncxx::builder::stream::document& doc, const Payment& payment) {
    doc << "orderId" << payment.orderId
        << "paymentId" << payment.paymentId
        << "currency" << payment.currency
        << "amount" << payment.amount;
}

void from_bson(const bsoncxx::document::view& view, Payment& payment) {
    payment.orderId = std::string(view["orderId"].get_string().value.data());
    payment.paymentId = std::string(view["paymentId"].get_string().value.data());
    payment.currency = std::string(view["currency"].get_string().value.data());
    payment.amount = view["amount"].get_double().value;
}


