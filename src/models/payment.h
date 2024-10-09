#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <bsoncxx/builder/stream/document.hpp>

struct Payment {
    std::string orderId;
    std::string paymentId;
    std::string currency;
    double amount;
};

void to_bson(bsoncxx::builder::stream::document& doc, const Payment& payment);
void from_bson(const bsoncxx::document::view& view, Payment& payment);

#endif
