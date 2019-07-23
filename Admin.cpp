//
//  Admin.cpp
//  Project4
//
//  Created by Elvina Almeida on 4/25/19.
//  Copyright © 2019 Elvina Almeida. All rights reserved.
//

#include "Admin.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;


Admin::Admin()
{
    username = "admin";
    password = "admin";
}

Admin::~Admin()
{
    
}

bool Admin::validate(string checkUser, string checkPass)
{
    if ((checkUser == username) && (checkPass == password))
        return true;
    else
    {
        cout << "Wrong username/password was entered. Please try again.\n";
        cout << "Username: \n";
        cin >> checkUser;
        cout << "Password: \n";
        cin >> checkPass;
        return validate(checkUser, checkPass);
    }
    return false;
}

string Admin::adminOptions(Inventory inventory, struct product*& headRef, int &productFound, int lastProductNum)
{
    int nitemNum, nitemName, nitemQty; //stores the new item info
    float nitemCost;
    string adminOption, endAdmin;
    
    cout << "To add a product enter \"add\"." << endl << "To delete a product enter \"delete\"." << endl << "To update a product's qty enter \"qty\"." << endl << "To update a product's cost enter \"cost\"." << endl;
    cin >> adminOption; //stores the output of if the user want to add, delete, or update qty
    
    if (adminOption=="add")
    {
        cout << "What is the new product number that you would like to add: ";
        cin >> nitemNum;
        nitemName=++lastProductNum; //stores the nitemName according to last product number in product linked list
        cout << "What is the cost of the new product that you would like to add: ";
        cin >> nitemCost;
        cout << "What is the item qty of the product that you would like to add: ";
        cin >> nitemQty;
        
        addProduct(headRef, nitemNum, nitemName, nitemCost, nitemQty); //calls the function addProduct
    }//end if "add"
    if (adminOption=="delete")
    {
        cout << "What is the product number that you would like to delete: ";
        cin >> nitemNum;
        if (inventory.searchProduct(headRef, nitemNum, productFound, false)==true) //checks if the product wanting to be deleted exsists
        {
            deleteProduct(headRef,productFound); //problem with delete node
            cout << "Item has been deleted." << endl;
        }
        else
            cout << "The product you want to delete does not exist." << endl;
    }//end if "delete"
    if (adminOption=="qty")
    {
        cout << "What is the product number that you would like to update: ";
        cin >> nitemNum;
        if (inventory.searchProduct(headRef, nitemNum, productFound, false)==true) //checks if the product wanting to be deleted exsists
        {
            cout << "What is new quanitity of the product: ";
            cin >> nitemQty;
            
            insertQty(headRef, nitemNum, nitemQty); //calls the function instertQty
            cout << "Item qty of the product has been updated." << endl;
        }
        else
            cout << "The product you want to update does not exist." << endl;
    }//end if "update"
    if (adminOption=="cost")
    {
        cout << "What is the product number that you would like to update: ";
        cin >> nitemNum;
        if (inventory.searchProduct(headRef, nitemNum, productFound, false)==true) //checks if the product wanting to be deleted exsists
        {
            cout << "What is new cost of the product: ";
            cin >> nitemCost;
            
            
            insertCost(headRef, nitemNum, nitemCost); //calls the function instertCost
            cout << "Item cost of the product has been updated." << endl;
        }
        else
            cout << "The product you want to update does not exist." << endl;
    }//end if "update"
    
    cout << "To exit admin mode enter \"exit\". To remain in admin mode enter \"continue\"." << endl;
    cin >> endAdmin; //stores the output for if the user wants to "exit" admin mode
    
    return endAdmin;
}//end adminOptions

