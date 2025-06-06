cmake_minimum_required(VERSION 3.10)
project(BookstorePP)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(bookstore_admin
    src/bookstore_admin.cpp
    src/Product.cpp
    src/StockManager.cpp
)

add_executable(bookstore_cart
    src/bookstore_cart.cpp
    src/Product.cpp
    src/Date.cpp
    src/Order.cpp
    src/CartManager.cpp
)