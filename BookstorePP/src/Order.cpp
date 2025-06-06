#include "Order.h"

Order::Order(const std::vector<Product>& prods, const Date& dt) : products(prods), date(dt) {}