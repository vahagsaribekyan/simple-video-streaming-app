#include "./app/app.h"                                              
#include <iostream>

int main(int, char**) {
    try {
        std::cout << "Starting the application..." << std::endl;
        Develandoo::App app;
        return app.start();
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    } catch(...) {
        std::cout << "Something went wrong" << std::endl;
        return -1;
    }                             
}
