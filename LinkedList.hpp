#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Inventory.hpp"
using namespace std;

//structure to hold all item in inventory
struct product //structure of product
{
    int itemNum;
    int itemName;
    float itemCost;
    int itemQty;
    struct product* next=NULL;
    struct product* previous=NULL;
};

struct cart //structure of cart
{
    int itemNum;
    int itemQty;
    struct cart* next=NULL;
    struct cart* previous;
};

class LinkedList
{
public:
    LinkedList(); //default constructor
    LinkedList(struct product*& headRef, int productNum, int name, double price, int qty); //overloaded construtor
    ~LinkedList();
    void BuildInventory(fstream &inFile, struct product*& headRef, int& lastProductNum); //builds linkedList by reading the file
    int length(struct product* head); //finds the length of linkedList
    void PrintLinkedList(struct product* head); //prints linkedList
    LinkedList operator+(const LinkedList &list); //overloaded operator
    void DeleteList(struct product*& headRef, struct cart*& cheadRef); //deletes linkedList
    struct product* head;
};



