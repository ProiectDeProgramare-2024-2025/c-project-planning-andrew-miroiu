#include "StockManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

StockManager::StockManager(const std::string& file) : filename(file) {
    ensure_file_exists();
    load();
}

void StockManager::ensure_file_exists() {
    if (!std::filesystem::exists(filename)) {
        std::vector<Product> initialStock = {
            Product("111", "C++Primer", "Lippman", 10, 45.99),
            Product("112", "EffectiveC++", "Meyers", 5, 39.99),
            Product("113", "CleanCode", "Martin", 8, 42.50)
        };
        stock = initialStock;
        save();
    }
}

void StockManager::load() {
    stock.clear();
    std::ifstream fin(filename);
    if (!fin) return;
    int n; fin >> n;
    for (int i = 0; i < n; i++) {
        std::string bc, nm, auth;
        int qty; double pr;
        fin >> bc >> nm >> auth >> qty >> pr;
        stock.emplace_back(bc, nm, auth, qty, pr);
    }
    fin.close();
}

void StockManager::save() {
    std::ofstream fout(filename);
    fout << stock.size() << "\n";
    for (const auto& p : stock) {
        fout << p.barcode << " " << p.name << " " << p.author << " " << p.quantity << " " << p.price << "\n";
    }
    fout.close();
}

std::vector<Product> StockManager::get_stock() const {
    return stock;
}

void StockManager::add_product(const Product& p) {
    stock.push_back(p);
    save();
}

void StockManager::delete_product(const std::string& barcode) {
    stock.erase(std::remove_if(stock.begin(), stock.end(), [&](const Product& p) {
        return p.barcode == barcode;
    }), stock.end());
    save();
}

void StockManager::modify_product(const std::string& field, const std::string& barcode, const std::string& new_value) {
    for (auto& p : stock) {
        if (p.barcode == barcode) {
            if (field == "price") p.price = std::stod(new_value);
            else if (field == "quantity") p.quantity = std::stoi(new_value);
        }
    }
    save();
}