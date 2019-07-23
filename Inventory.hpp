#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;
class Inventory
{
public:
    Inventory();
    Inventory(int num, int name, double cost, int qty);
    ~Inventory();
    
    void addProduct(struct product*& headRef, int nItemNum, int nItemName, double cost, int qty);//apends new product to end of linked list
    void deleteProduct(struct product*& headRef, int position); //deletes a product that's qty=0
    void insertQty(struct product*& headRef, int productNum, int qty); //inserts updated qty into the linked list
    void insertCost(struct product*& headRef, int productNum, double cost); //inserts updated qty into the linked list
    bool searchProduct(struct product*& head, int productNum, int& productFound, bool printItemInfo); //searchs for the product number in the product linked list
    
    void PrintLinkedList(struct product* head);
    
    int getItemNumber() const
    {
        return ItemNumber;
    }
    int getName() const
    {
        return Name;
    }
    double getCost() const
    {
        return Cost;
    }
    int getQty() const
    {
        return Qty;
    }
    
    void setQty(int qty)
    {
        Qty = qty;
    }
    
    void setCost(double cost)
    {
        Cost = cost;
    }
    
private:
    int ItemNumber;
    int Name;
    double Cost;
    int Qty;
    struct product* nextNode;
};


