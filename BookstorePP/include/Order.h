#pragma once
#include "Product.h"
#include "Date.h"
#include <vector>

class Order {
public:
    std::vector<Product> products;
    Date date;

    Order() {}
    Order(const std::vector<Product>& prods, const Date& dt);
};