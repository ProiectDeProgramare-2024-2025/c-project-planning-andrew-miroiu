#include "CartManager.h"
#include "Date.h"
#include "Product.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "StockManager.h"

CartManager::CartManager(const std::string& filename) : cartFile(filename) {}

std::vector<std::pair<std::string, int>> CartManager::load_cart() {
    std::vector<std::pair<std::string, int>> cart;
    std::ifstream fin(cartFile);
    std::string bc;
    int qty;
    while (fin >> bc >> qty) {
        cart.emplace_back(bc, qty);
    }
    fin.close();
    return cart;
}

void CartManager::save_cart(const std::vector<std::pair<std::string,int>>& cart) {
    std::ofstream fout(cartFile);
    for (const auto& item : cart) {
        fout << item.first << " " << item.second << "\n";
    }
    fout.close();
}

void CartManager::view_cart() {
    auto cart = load_cart();
    if(cart.empty()){
        std::cout << "\033[1;33mYour cart is empty.\033[0m\n";
        return;
    }
    for (const auto& item : cart) {
        std::cout << "\033[1;32m" << item.first << "\033[0m x " << item.second << "\n";
    }
}

void CartManager::add_to_cart(const std::string& barcode, int qty) {
    if(qty <= 0){
        std::cerr << "\033[1;31mError: Quantity must be positive.\033[0m\n";
        return;
    }

    // Load stock to validate barcode
    StockManager stockManager("data/stock.txt");
    stockManager.load();
    auto stock = stockManager.get_stock();

    auto it = std::find_if(stock.begin(), stock.end(), [&](const Product& p) {
        return p.barcode == barcode;
    });

    if (it == stock.end()) {
        std::cerr << "\033[1;31mError: Product with barcode " << barcode << " does not exist in stock.\033[0m\n";
        return;
    }

    auto cart = load_cart();
    bool found = false;
    for (auto& item : cart) {
        if (item.first == barcode) {
            item.second += qty;
            found = true;
            break;
        }
    }
    if (!found) cart.emplace_back(barcode, qty);
    save_cart(cart);
    std::cout << "\033[1;34mAdded to cart.\033[0m\n";
}
void CartManager::modify_cart(const std::string& barcode, int new_qty) {
    if(new_qty < 0){
        std::cerr << "\033[1;31mError: Quantity cannot be negative.\033[0m\n";
        return;
    }
    auto cart = load_cart();
    bool found = false;
    for (auto& item : cart) {
        if (item.first == barcode) {
            found = true;
            if(new_qty == 0){
                delete_from_cart(barcode);
                std::cout << "\033[1;33mProduct removed from cart.\033[0m\n";
                return;
            }
            item.second = new_qty;
            break;
        }
    }
    if(!found){
        std::cerr << "\033[1;31mError: Product not found in cart.\033[0m\n";
        return;
    }
    save_cart(cart);
    std::cout << "\033[1;34mCart updated.\033[0m\n";
}

void CartManager::delete_from_cart(const std::string& barcode) {
    auto cart = load_cart();
    size_t before = cart.size();
    cart.erase(std::remove_if(cart.begin(), cart.end(), [&](const auto& item) {
        return item.first == barcode;
    }), cart.end());
    if(cart.size() == before){
        std::cerr << "\033[1;31mError: Product not found in cart.\033[0m\n";
        return;
    }
    save_cart(cart);
    std::cout << "\033[1;33mProduct removed from cart.\033[0m\n";
}

void CartManager::purchase() {
    auto cart = load_cart();
    if (cart.empty()) {
        std::cout << "\033[1;33mCart is empty. Nothing to purchase.\033[0m\n";
        return;
    }

    // Load stock from file
    std::ifstream fin("data/stock.txt");
    if (!fin) {
        std::cerr << "\033[1;31mError: Cannot open stock file.\033[0m\n";
        return;
    }

    int n;
    fin >> n;
    std::vector<Product> stock;
    for (int i = 0; i < n; ++i) {
        std::string bc, nm, auth;
        int qty;
        double pr;
        fin >> bc >> nm >> auth >> qty >> pr;
        stock.emplace_back(bc, nm, auth, qty, pr);
    }
    fin.close();

    // Check stock availability
    for (const auto& item : cart) {
        auto it = std::find_if(stock.begin(), stock.end(), [&](const Product& p) {
            return p.barcode == item.first;
        });
        if (it == stock.end()) {
            std::cerr << "\033[1;31mError: Product with barcode " << item.first << " not found in stock.\033[0m\n";
            return;
        }
        if (it->quantity < item.second) {
            std::cerr << "\033[1;31mError: Insufficient stock for product " << it->name
                      << ". Requested: " << item.second << ", Available: " << it->quantity << "\033[0m\n";
            return;
        }
    }

    // Deduct quantities from stock
    for (auto& item : cart) {
        for (auto& p : stock) {
            if (p.barcode == item.first) {
                p.quantity -= item.second;
                break;
            }
        }
    }

    // Save updated stock back to file
    std::ofstream fout("data/stock.txt");
    if (!fout) {
        std::cerr << "\033[1;31mError: Cannot write to stock file.\033[0m\n";
        return;
    }
    fout << stock.size() << "\n";
    for (const auto& p : stock) {
        fout << p.barcode << " " << p.name << " " << p.author << " " << p.quantity << " " << p.price << "\n";
    }
    fout.close();

    // Save order details
    std::ofstream orderFile("data/orders.txt", std::ios::app);
    if (!orderFile) {
        std::cerr << "\033[1;31mError: Cannot write to orders file.\033[0m\n";
        return;
    }
    Date today;
    orderFile << today.to_string() << "\n";
    for (const auto& item : cart) {
        orderFile << item.first << " " << item.second << "\n";
    }
    orderFile << "\n";
    orderFile.close();

    // Clear cart
    cart.clear();
    save_cart(cart);

    std::cout << "\033[1;32mPurchase successful! Stock updated and order saved.\033[0m\n";
}
