#pragma once
#include <string>

class Product {
public:
    std::string barcode;
    std::string name;
    std::string author;
    int quantity;
    double price;

    Product() {}
    Product(std::string bc, std::string nm, std::string auth, int qty, double pr);
};