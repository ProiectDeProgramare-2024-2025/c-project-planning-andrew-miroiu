#include "Product.h"

Product::Product(std::string bc, std::string nm, std::string auth, int qty, double pr) :
    barcode(bc), name(nm), author(auth), quantity(qty), price(pr) {}