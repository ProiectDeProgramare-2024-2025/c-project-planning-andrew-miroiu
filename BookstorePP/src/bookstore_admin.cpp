#include <iostream>
#include <fstream>
#include "StockManager.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "\033[1;31mError: No command provided.\033[0m\n";
        return 1;
    }

    StockManager manager("data/stock.txt");
    std::string cmd = argv[1];

    try {
        if (cmd == "view_stock_products") {
            auto stock = manager.get_stock();
            if(stock.empty()){
                std::cout << "\033[1;33mNo products in stock.\033[0m\n";
            }
            for (const auto& p : stock) {
                std::cout << "\033[1;36m" << p.barcode << "\033[0m | "
                          << "\033[1;34m" << p.name << "\033[0m | "
                          << "\033[1;32m" << p.author << "\033[0m | "
                          << "\033[1;33m" << p.quantity << "\033[0m | "
                          << "\033[1;31m" << p.price << "\033[0m\n";
            }
        }
        else if (cmd == "add_product" && argc == 7) {
            std::string bc = argv[2], nm = argv[3], auth = argv[4];
            int qty = std::stoi(argv[5]);
            double pr = std::stod(argv[6]);
            if(qty < 0 || pr < 0){
                throw std::invalid_argument("Quantity and price must be non-negative.");
            }
            manager.add_product(Product(bc, nm, auth, qty, pr));
            std::cout << "\033[1;32mProduct added successfully.\033[0m\n";
        }
        else if (cmd == "delete_product" && argc == 3) {
            manager.delete_product(argv[2]);
            std::cout << "\033[1;33mProduct deleted successfully.\033[0m\n";
        }
        else if (cmd == "modify_product" && argc == 5) {
            std::string field = argv[2];
            if(field != "price" && field != "quantity"){
                throw std::invalid_argument("Field must be 'price' or 'quantity'.");
            }
            manager.modify_product(field, argv[3], argv[4]);
            std::cout << "\033[1;34mProduct modified successfully.\033[0m\n";
        }
        else if (cmd == "view_orders") {
            std::ifstream fin("data/orders.txt");
            if(!fin){
                std::cout << "\033[1;33mNo orders found.\033[0m\n";
                return 0;
            }
            std::string line;
            while (getline(fin, line)) {
                std::cout << line << "\n";
            }
            fin.close();
        }
        else {
            std::cerr << "\033[1;31mError: Unknown command or incorrect arguments.\033[0m\n";
            return 1;
        }
    }
    catch(const std::exception& e){
        std::cerr << "\033[1;31mError: " << e.what() << "\033[0m\n";
        return 1;
    }
    return 0;
}
