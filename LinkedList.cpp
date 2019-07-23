//
//  LinkedList.cpp
//  Project5
//
//  Created by Elvina Almeida on 5/3/19.
//  Copyright © 2019 Elvina Almeida. All rights reserved.
//

#include "LinkedList.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;


LinkedList::LinkedList()
{
    
}

LinkedList::LinkedList(struct product*& headRef, int productNum, int name, double price, int qty)
{
    struct product* newNode = new struct product;            //Create a new node to add to the front of the list
    
    newNode->itemNum = productNum;                //Store the data inside the new node
    newNode->itemName = name;
    newNode->itemCost = price;
    newNode->itemQty = qty;
    newNode->next = headRef;                        //Set the new node next equal to the orginal head
    
    headRef = newNode;                                //Set the orginal head to point to the new node.
}

LinkedList::~LinkedList()
{
    struct product* currentNode=head;
    struct product* nextNode=head;
    
    while (currentNode != nullptr) //Traverse the linked list until you reach the end
    {
        nextNode = currentNode->next; //Set the next node
        delete currentNode; //Delete the current node
        currentNode =nextNode; //Set current node to the next node
    }
    head= nullptr; //Return null ptr to the calling function
}

LinkedList LinkedList::operator+(const LinkedList &list)
{
    LinkedList list1 = *this;
    product *tmp= list1.head;
    
    while(tmp->next!=NULL)
    {
        tmp=tmp->next;
    }
    tmp->next = list.head;
    
    return list1;
}

//build inventory link list
void LinkedList::BuildInventory(fstream & inFile, struct product*& headRef, int& lastProductNum)
{
    string tempproduct; //stores the string "product"
    string titemNum, titemName, titemCost, titemQty;//stores the item number, name, cost, qty from each line of file
    int titemNum1, titemName1, titemQty1;           //stores the convereted item info (string to int)
    float titemCost1;                               //stores the convereted item info (string to int)
    
    inFile.open("ProductData_4.csv"); //opens the file
    
    if(!inFile) //file not open
        return; //return back to main
    else
    {
        struct product* head= NULL; // created a new product head
        struct product* current = NULL; //current pointer
        
        while (!inFile.eof())//while the end of file has not been reached
        {
            struct product* newNode = new struct product; //creates a new node for the linked list
            
            // call function the get the line using getLine
            getline(inFile,titemNum,',');
            titemNum1= atoi(titemNum.c_str());
            newNode->itemNum=titemNum1;
            
            getline(inFile,tempproduct, ' ');
            getline(inFile,titemName,',');
            titemName1= atoi(titemName.c_str());
            newNode->itemName=titemName1;
            
            getline(inFile,titemCost,',');
            titemCost1= atof(titemCost.c_str());
            newNode->itemCost=titemCost1;
            
            getline(inFile,titemQty, '\r');
            titemQty1= atoi(titemQty.c_str());
            newNode->itemQty=titemQty1;
            
            if (head==NULL)//if a head does not exist it creates one and stores current node
            {
                head = newNode;
                current = head;
            }
            else//if a head does exist a new node is created and stored in current
            {
                current->next = newNode;
                current = newNode;
            }
            
            lastProductNum=titemName1;
            
        }//end of while loop
        
        inFile.close(); //close the file
        headRef=head; //return the head of the file
        
    }//end if-else if file opens
}

int LinkedList::length(struct product* head)
{
    struct product* current = head;  //Create a temporary node to keep track of the current node
    int count = 0;  // counter variable
    
    while (current != NULL) //Traverses through the list until we find the end
    {
        count++; //updates the count
        current = current->next;
    }
    return count;//returns count back to main
}//end length()

void LinkedList::PrintLinkedList(struct product* headRef)
{
    struct product* current = headRef;        //Create a temporary node to keep track of the current node
    cout << "{ ";
    while (current != NULL) {            //Loop through the list until we find the end
        cout << current->itemNum << " " ;    //Set the current node to the next node
        cout << current->itemName << " " ;    //Set the current node to the next node
        cout << current->itemCost << " " ;    //Set the current node to the next node
        cout << current->itemQty << " "  << endl;    //Set the current node to the next node
        current = current->next;
    }
    cout << "}" << endl;
}

void LinkedList::DeleteList(struct product*& headRef, struct cart*& cheadRef)
{
    struct product* pcurrentNode = headRef; //Create a temporary product to use to transverse through the linked list
    struct product* pnextNode = headRef; //Create a temporary product to keep track of the next node
    
    struct cart* ccurrentNode = cheadRef;//Create a temporary cart to use to transverse through the linked list
    struct cart* cnextNode = cheadRef; //Create a temporary cart to keep track of the next node
    
    while (pcurrentNode != nullptr) //Traverse the linked list until you reach the end
    {
        pnextNode = pcurrentNode->next; //Set the next node
        delete pcurrentNode; //Delete the current node
        pcurrentNode =pnextNode; //Set current node to the next node
    }
    headRef = nullptr; //Return null ptr to the calling function
    
    while (ccurrentNode != nullptr) //Traverse the linked list until you reach the end
    {
        cnextNode = ccurrentNode->next; //Set the next node
        delete ccurrentNode; //Delete the current node
        ccurrentNode =cnextNode; //Set current node to the next node
    }
    cheadRef = nullptr;//Return null ptr to the calling function
}//end DeleteList()

