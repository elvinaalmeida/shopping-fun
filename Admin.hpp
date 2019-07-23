//
//  Admin.hpp
//  Project4
//
//  Created by Elvina Almeida on 4/25/19.
//  Copyright © 2019 Elvina Almeida. All rights reserved.
//


#pragma once
#include <iostream>
#include <string>
#include "Inventory.hpp"
using namespace std;


class Admin: public Inventory
{
public:
    Admin();
    ~Admin();
    bool validate(string checkUser, string checkPass); //validate username and password
    string adminOptions(Inventory inventory, struct product*& headRef, int &productFound, int lastProductNum); //if the user enters admin mode they are given options to add,delete, and update a product
    
    //update Inventory Methods --> Inventory class
    
private:
    string username;
    string password;
    
};

