#pragma once
#include <string>
#include <vector>

class CartManager {
private:
    std::string cartFile;

public:
    CartManager(const std::string& filename);
    std::vector<std::pair<std::string, int>> load_cart();
    void save_cart(const std::vector<std::pair<std::string,int>>& cart);
    void view_cart();
    void add_to_cart(const std::string& barcode, int qty);
    void modify_cart(const std::string& barcode, int new_qty);
    void delete_from_cart(const std::string& barcode);
    void purchase();
};
