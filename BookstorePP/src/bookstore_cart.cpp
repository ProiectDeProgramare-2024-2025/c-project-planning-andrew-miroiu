#include <iostream>
#include "CartManager.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "\033[1;31mError: No command provided.\033[0m\n";
        return 1;
    }

    CartManager cart("data/cart.txt");
    std::string cmd = argv[1];

    try {
        if (cmd == "view_cart") {
            cart.view_cart();
        }
        else if (cmd == "add_product" && argc == 4) {
            cart.add_to_cart(argv[2], std::stoi(argv[3]));
        }
        else if (cmd == "modify_product" && argc == 4) {
            cart.modify_cart(argv[2], std::stoi(argv[3]));
        }
        else if (cmd == "delete_product" && argc == 3) {
            cart.delete_from_cart(argv[2]);
        }
        else if (cmd == "purchase") {
            cart.purchase();
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
