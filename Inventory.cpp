//
//  Inventory.cpp
//  Project5
//
//  Created by Elvina Almeida on 5/3/19.
//  Copyright © 2019 Elvina Almeida. All rights reserved.
//

#include "Inventory.hpp"
#include "LinkedList.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;


Inventory::Inventory()
{
    
}

Inventory::Inventory(int num, int name, double cost, int qty)
{
    ItemNumber = num;
    Name = name;
    Cost = cost;
    Qty = qty;
}

Inventory::~Inventory()
{
}

void Inventory::addProduct(struct product*& headRef, int nItemNum, int nItemName, double cost, int qty)
{
    struct product* head = headRef; //Create a temporary node to keep track of the orginal head node
    nextNode = NULL; //Create a temporary node to use to transverse through the linked list
    struct product* newNode = new struct product; //Create a new node to add to the end of the list
    
    newNode->itemNum = nItemNum; //Set the Node Data
    newNode->itemName = nItemName;
    newNode->itemCost = cost;
    newNode->itemQty = qty;
    newNode->next = NULL;
    
    if (!head)  //Check to see if linked list is empty
        head = newNode;  //If it is append the new node onto the empty list
    else
    {
        nextNode = head; //Set the current node to the head node
        while (nextNode->next)  //Traverse the linked list until you reach the end
        {
            nextNode = nextNode->next;
        }
        nextNode->next = newNode; //At the end set the current to point to the new node.
    }
    headRef = head; //Return the new linked list to the calling function
}//end addProduct

void Inventory::deleteProduct(struct product*& headRef, int position) //have to search the product and return the position currently deltes the product after it
{
    nextNode=headRef;
    struct product* previousNode=headRef;
    int count=0;
    if(position==0)
    {
        headRef=nextNode->next;
        delete nextNode;
    }
    else
    {
        while (nextNode != nullptr && count !=position)
        {
            previousNode =nextNode;
            nextNode = nextNode-> next;
            count++;
        }
        
        if (count==position)
        {
            previousNode->next= nextNode->next;
            delete nextNode;
        }
    }
}

void Inventory::insertQty(struct product*& headRef, int productNum, int qty)
{
    nextNode = headRef;//Create a temporary product to keep track of the current product
    
    while (nextNode != NULL) //Traverse the product linked list until you reach the end
    {
        if (nextNode->itemNum == productNum)//if the product is found qty is updated
        {
            nextNode->itemQty=qty;
            break;
        }
        else
            nextNode = nextNode->next;
    }//end while loop
}//end insertQty

void Inventory::insertCost(struct product*& headRef, int productNum, double cost)
{
    nextNode = headRef;//Create a temporary product to keep track of the current product
    
    while (nextNode != NULL) //Traverse the product linked list until you reach the end
    {
        if (nextNode->itemNum == productNum)//if the product is found qty is updated
        {
            nextNode->itemCost=cost;
            break;
        }
        else
            nextNode = nextNode->next;
    }//end while loop
}//end insertQty

void Inventory::PrintLinkedList(struct product* head)
{
    struct product* current = head;        //Create a temporary node to keep track of the current node
    
    while (current != NULL){            //Loop through the list until we find the end
        
        cout << current->itemNum << " ";
        cout << current->itemName << " ";
        cout << fixed << setprecision(2) << current->itemCost << " ";
        cout << current->itemQty << endl;
        current = current->next;        //Set the current node to the next node
    }
}

bool Inventory::searchProduct(struct product*& head, int productNum, int& productFound, bool printItemInfo)
{
    struct product* current = head;  // creates temp product and stores the head of prouduct linked list
    int count=0;    //counter variable
    
    while (current != NULL)
    {
        if (current->itemNum == productNum)//if the productnum in linked list equals productNum user wants to purchase
        {
            if (printItemInfo == true) //if the user is in buyer mode, item name and cost are printed
            {
                cout << "Item Name: " << current->itemName << "     ";
                cout << "Item Cost: $" << current->itemCost << " " << endl;
            }
            
            productFound=count;//stores the position where the product is found
            return true; //returns to main if product is found
        }
        else
            current = current->next;
        count++;
    }//loops until end of linked list
    
    return false; //returns to main if product is not found
}


