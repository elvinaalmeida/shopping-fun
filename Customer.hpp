#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

class Customer
{
    
public:
    
    Customer();
    Customer(string first, string last, string add, string city, string state, int zip);
    ~Customer();
    int productPurchase(bool &cart, bool &deleteProduct);
    void BuildCartList(struct cart*& headRefCart, int productNum, int itemQty); //Shopping Cart (utilize the Linked List to create shopping cart)
    void PrintCartLinkedList(struct cart* head, struct product* phead); //Display shopping cart
    void addItem(struct cart*& headRefCart, struct cart* newNode); //add item to shopping cart
    void deleteItem(struct cart *&headRefCart, int productNum); //delete item from shopping cart
    void updateQty(struct cart*& headRefCart, int productNum, int itemQty); //updates qty of shopping cart
    bool searchQty(struct product*& head, int productNum, int& productFound, int productQty, bool& qtyOver); //checks if the quantity being purchased is available
    void storeCustomerInfo();//stores customerInfo
    void displayInvoice(struct cart* head, struct product* phead);
    
    //acessors and mutators to update private customer data
    void setFirst(string fname)
    {
        firstName = fname;
    }
    
    void setLast(string lname)
    {
        lastName = lname;
    }
    
    void setAdd(string add)
    {
        address = add;
    }
    
    void setCity(string city)
    {
        city = city;
    }
    
    void setState(string state)
    {
        state = state;
    }
    
    void setZip(int zip)
    {
        zipcode = zip;
    }
    
    string getFirst() const
    {
        return firstName;
    }
    
    string getLast() const
    {
        return lastName;
    }
    
    string getAdd() const
    {
        return address;
    }
    
    string getCity() const
    {
        return city;
    }
    
    string getState() const
    {
        return state;
    }
    
    int getZip() const
    {
        return zipcode;
    }
    
private:
    string firstName;
    string lastName;
    string address;
    string city;
    string state;
    int zipcode;
    string invoiceFileName;
    struct cart *head;
    
};

