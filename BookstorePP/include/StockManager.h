#pragma once
#include "Product.h"
#include <vector>
#include <string>

class StockManager {
private:
    std::string filename;
    std::vector<Product> stock;

public:
    StockManager(const std::string& file);
    void load();
    void save();
    void ensure_file_exists();
    std::vector<Product> get_stock() const;
    void add_product(const Product& p);
    void delete_product(const std::string& barcode);
    void modify_product(const std::string& field, const std::string& barcode, const std::string& new_value);
};