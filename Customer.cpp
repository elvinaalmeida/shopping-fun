//
//  Customer.cpp
//  Project5
//
//  Created by Elvina Almeida on 5/3/19.
//  Copyright © 2019 Elvina Almeida. All rights reserved.
//

#include "Customer.hpp"
#include "LinkedList.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;


Customer::Customer()
{
    
}

Customer::Customer(string first, string last, string add, string cit, string stat, int zip)
{
    firstName = first;
    lastName = last;
    address = add;
    city = cit;
    state = stat;
    zipcode = zip;
}

Customer::~Customer()
{
    struct cart* currentNode=head;
    struct cart* nextNode=head;
    
    while (currentNode != nullptr) //Traverse the linked list until you reach the end
    {
        nextNode = currentNode->next; //Set the next node
        delete currentNode; //Delete the current node
        currentNode =nextNode; //Set current node to the next node
    }
    head= nullptr; //Return null ptr to the calling function
}

int Customer::productPurchase(bool &cart, bool &deleteProduct)
{
    int productNumber=-1; //holds the product number that the user enters
    string answer;      //holds the output answer
    
    cout << endl << "What product would you like to purchase? ";
    cin >> answer;
    
    if (answer == "cart" || answer == "Cart" || answer == "CART" ) //if cart is entered then the program will be able to print cart
        cart=true;
    else if (answer == "delete")
        deleteProduct=true;
    else
    {
        cart=false;
        productNumber=atoi(answer.c_str()); //string answer is converted to an int
    }
    
    return productNumber; //returns productNumber back to main
}//end productPurchase()


/****************************************************************************
 * Function:searchQty(struct product*& head, int productNum, int& productFound, int productQty, bool& qtyOver)
 * Descr: checks if the quantity being purchased is available
 ****************************************************************************/
bool Customer::searchQty(struct product*& head, int productNum, int& productFound, int productQty, bool& qtyOver)
{
    struct product* current = head;  //creates temp linked list and stores the head of prouduct linked list
    
    while (current != NULL)
    {
        if (current->itemNum == productNum)//if the productnum in linked list equals productNum user wants to purchase
        {
            if (current->itemQty >= productQty) //if the requested qty in product link list exists
            {
                current->itemQty -=productQty; //updates qty avalaible
                return true; //returns to main if qty is found
            }
            else if (current->itemQty == 0) //if product is out of stock
            {
                qtyOver=true;
            }
            return false;
        }
        else
            current = current->next;
    }//loops until end of linked list
    return false; //returns to main if product qty is not found
}

/****************************************************************************
 * Function:BuildCartList(struct cart*& headRefCart, int productNum, int itemQty)
 * Descr:This function reads file and stores it in the struct
 ****************************************************************************/
void Customer::BuildCartList(struct cart*& headRefCart, int productNum, int itemQty)
{
    bool productPurchased=false; //stores the boolean value if the product already exsits in the cart
    
    struct cart* current = headRefCart;  //Create a temporary node to use to transverse through the linked list
    struct cart* newNode = new struct cart; //Create a new node to add to the end of the list
    
    newNode->itemNum = productNum;  //Set the cart node to the product num and qty being purchased
    newNode->itemQty = itemQty;
    newNode->next = NULL;
    
    if (!current) //Check to see if linked list is empty
    {
        current = newNode;   //If it is append the new node onto the empty list
        headRefCart = current;
    }
    else
    {
        while (current != NULL)//checks to see if product has been purchased
        {
            if (current->itemNum == productNum) //if the productnum in linked list equals productNum is in cart
            {
                productPurchased=true;
                break;
            }
            
            else
                current = current->next; //moves to next node
            
            productPurchased=false; //if product being purchased is not already in cart
        }//end of while loop
        
        if (productPurchased==true) //if the product has been purchased it updates the quantity
        {
            updateQty(headRefCart,productNum, itemQty);
        }
        
        else //if product being purchased is not already in cart appends a new node
        {
            addItem(headRefCart,newNode);
        }//end of if-else product previously purchased
    }//end of if else if head is not NULL
}//end BuildCartList()

void Customer::addItem(struct cart*& headRefCart, struct cart* newNode)
{
    struct cart* current = headRefCart;
    
    current = headRefCart;//Set the current node to the head node
    
    while (current->next) //Traverse the linked list until you reach the end
        current = current->next;
    
    current->next = newNode;//At the end set the current to point to the new node
}

void Customer::updateQty(struct cart*& headRefCart, int productNum, int itemQty)
{
    struct cart* current = headRefCart;  //Create a temporary node to use to transverse through the linked list
    
    while (current != NULL)
    {
        if (current->itemNum == productNum)
        {
            current->itemQty +=itemQty;
            break;
        }
        else
            current = current->next;
    }
}

void Customer::deleteItem(struct cart *&headRefCart, int productNum)
{
    struct cart* current = headRefCart;  //Create a temporary node to use to transverse through the linked list
    struct cart* previousNode= headRefCart;
    
    int position=-1;
    
    while (current != NULL)
    {
        if (current->itemNum == productNum)
        {
            position++;
            break;
        }
        else
            current = current->next;
    }
    
    int count=0;
    if(position==0)
    {
        headRefCart=current->next;
        delete current;
    }
    else
    {
        while (current != nullptr && count !=position)
        {
            previousNode=current;
            current=current-> next;
            count++;
        }
        
        if (count==position)
        {
            previousNode->next=current->next;
            delete current;
        }
    }
    
}

void Customer::PrintCartLinkedList(struct cart* head, struct product* phead)
{
    struct product* currentp = phead;//Create a temporary node to keep track of the current node
    struct cart* currentc = head;//Create a temporary node to keep track of the currentc node
    double total=0.0;//holds the total value of all the products purchased
    
    cout << "Item Number           Item Name          Qty        Unit Cost        Total" << endl;
    cout << "__________________________________________________________________________________" << endl;
    
    while (currentc !=NULL) //Traverse the product linked list until you reach the end
    {
        while (currentp != NULL) //Traverse the cart linked list until you reach the end
        {
            if (currentp->itemNum==currentc->itemNum)
            {
                total += currentc->itemQty*currentp->itemCost;
                cout << "Item Number " << currentp->itemNum << setw (18) << "Product Name " << currentp->itemName << setw(7) << currentc->itemQty << setw (15) << currentc->itemQty* currentp->itemCost<< setw(17) << total << endl;
                break;
            }
            currentp = currentp->next;
        }
        currentc = currentc->next;
        currentp=phead;
    }//end of while loop
    
    // prints out the product purchased, quanitity, and cost of each item in the shopping cart
    cout << "__________________________________________________________________________________" << endl;
    cout << "                                                            Total: $ " << total << endl;
}//end of PrintCartLinkedList()

void Customer::storeCustomerInfo()
{
    //asks the user their customer info and it stores it in the array
    cout << "Enter first name: ";
    cin >> firstName;
    
    cout << "Enter last name: ";
    cin >> lastName;
    
    cout << "Street Address: ";
    cin.ignore(100, '\n');
    getline(cin, address);
    
    cout << "City: ";
    cin >> city;
    
    cout << "State: ";
    cin >> state;
    
    cout << "Zip Code: ";
    cin >> zipcode;
}


void Customer::displayInvoice(struct cart* head, struct product* phead)
{
    fstream outFile;                //create an invoice file after customer is finished shopping
    string i = "_Invoice.txt";
    invoiceFileName = lastName + i;
    outFile.open(invoiceFileName, ios::out);
    
    
    if (!outFile)//checks whether the file is able to be open
    {
        cout << "File " << "\"" << "Invoivce.txt" << "\"" << " could not be opened." << endl;
        return;
    }
    else
    {
        struct product* currentp = phead;//Create a temporary product to keep track of the current node
        struct cart* currentc = head;//Create a temporary cart to keep track of the current node
        double total=0.0;//holds the total value of all the products purchased
        
        //outputs customer info to console and file
        cout << "Customer Name:" << firstName << " " << lastName << endl;
        cout << "Customer Address:" << address << endl << "                 " << city << "," << state << " " << zipcode << endl;
        
        outFile << "Customer Name:" << firstName << " " << lastName << endl;
        outFile << "Customer Address:" << address << endl << "                 " << city << "," << state << " " << zipcode << endl;
        
        cout << "Item Number           Item Name          Qty        Unit Cost        Total" << endl;
        cout << "__________________________________________________________________________________" << endl;
        
        outFile << "Item Number           Item Name          Qty        Unit Cost        Total" << endl;
        outFile << "__________________________________________________________________________________" << endl;
        
        while (currentc != NULL) //Traverse the cart linked list until you reach the end
        {
            while (currentp != NULL) //Traverse the product  linked list until you find the product in cart linked list
            {
                if (currentp->itemNum==currentc->itemNum)
                {
                    total += currentc->itemQty*currentp->itemCost;
                    cout << "Item Number " << currentp->itemNum << setw (18) << "Product Name " << currentp->itemName << setw(7) << currentc->itemQty << setw (15) << currentc->itemQty* currentp->itemCost<< setw(17) << total << endl;
                    
                    outFile << "Item Number " << currentp->itemNum << setw (18) << "Product Name " << currentp->itemName << setw(7) << currentc->itemQty << setw (15) << currentc->itemQty* currentp->itemCost<< setw(17) << total << endl;
                }
                currentp = currentp->next;
            }//end of while loop
            currentc = currentc->next;
            currentp= phead;
        }//end of nested while loop
        
        // prints out the product purchased, quanitity, and cost of each item in the shopping cart to console and file
        cout << "__________________________________________________________________________________" << endl;
        cout << "                                                            Total: $ " << total << endl;
        
        outFile << "__________________________________________________________________________________" << endl;
        outFile << "                                                            Total: $ " << total << endl;
    }//end of while loop
}//end displayInvoice()



