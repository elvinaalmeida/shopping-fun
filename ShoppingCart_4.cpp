#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include "LinkedList.hpp"
#include "Admin.hpp"
#include "Customer.hpp"
#include "Inventory.hpp"

using namespace std;

void updateInventory(Inventory inventory, fstream &outFile, struct product*& headRef, int length); //updates the ProductData.csv file with the inventory remaining after

int main()
{
    struct product* phead = NULL;   //creates a product structure
    struct cart* chead = NULL;      //creates a cart structure
    fstream inFile, outFile;        //creates a file stream for reading and writing a file
    int productNum, productQty;     //holds the product number and the product quanitity entered by the user
    string adminOption, adminOrBuyer, exitAdminBuyer;   //holds the outputs to exit admin, buyer, or both functions
    bool productExists, qtyExists=false;    //holds the boolean value of if the product or qty the user wants, exists
    bool enterCart=false, qtyOver, productDelete=false;          //cart is set to true if the user enters "cart", qtyOver holds the boolean value if the qty avaliable reaches 0
    int lengthList, productFound, lastProductNum; //holds the length of the list, the product position found, and the number of the last product in the file
    string userAttempt, passAttempt; //stores username, password
    int modeChoice; //holds the answer to menu option
    string endAdmin; //holds the anser the user outputed, if they would like to continue or end admin mode
    
    if(!inFile) //if the file is unable to be purchased the function ends
    {
        cout << "File " << "\"" << "Product.txt" << "\"" << " could not be opened." << endl;
        return 0;
    }
    
    else
    {
        LinkedList linkList; //creates LinkedList object
        Admin admin;    //creates admin
        Inventory inventory; //creates inventory
        
        linkList.BuildInventory(inFile,phead,lastProductNum); //creates a linked list from reading the file
        lengthList= linkList.length(phead); //stores the current length of the list
        
        do
        {
            cout << "Choose an option\n";
            cout << "1. Admin" << endl << "2. Customer" << endl << "3. Exit" << endl;
            cin >> modeChoice;
            
            switch(modeChoice)
            {
                case 1: //admin
                {
                    cout << "Enter admin username: ";
                    cin >> userAttempt;
                    cout << "Enter admin password: ";
                    cin >> passAttempt;
                    
                    if (admin.validate(userAttempt, passAttempt) == true)
                    {
                        do//allow admin to edit more than once
                        {
                            endAdmin=admin.adminOptions(inventory,phead,productFound,lastProductNum); //calls the adminOption function
                        } while (endAdmin != "exit");
                        
                        lengthList=linkList.length(phead); //updates the length of product list once the user exists the admin option
                    }
                    break;
                }//end case 1
                    
                case 2: //customer
                {
                    Customer customer;
                    
                    cout << "If you would like to continue to checkout, enter 0." << endl;
                    cout << "If you would like to view your cart, enter \"cart\"." << endl;
                    cout << "If you would like to delete an item from your cart, enter \"delete\"."<< endl;
                    
                    for (int i=0; i < lengthList; i++)//limits the number of products you can add to your cart according to the number of products available
                    {
                        productNum=customer.productPurchase(enterCart, productDelete);
                        
                        if (enterCart==true)//if the user enters cart the cart is printed
                        {
                            //check to see if a cart has been built before printing the linked list
                            if (chead==NULL)
                                cout << "There are no products in your cart." << endl;
                            else
                            {
                                customer.PrintCartLinkedList(chead, phead);
                            }
                            
                            enterCart=false;
                            continue;
                            
                        }//end of if statement
                        
                        if (productDelete==true)
                        {
                            cout << "What is the product purchased you would like to delete? ";
                            cin >> productNum;
                            customer.deleteItem(chead, productNum);
                            productDelete=false;
                            
                            continue;
                        }
                        
                        if (productNum==0)
                        {
                            customer.storeCustomerInfo();
                            customer.displayInvoice(chead,phead);
                            //DeleteList(phead, chead);
                            break;
                        }//end of if statement
                        
                        if (productNum>0)
                        {
                            productExists= inventory.searchProduct(phead, productNum, productFound, true); //calls the function searchProduct and stores it into productExsits
                            
                            if(productExists==false) //checks if the productExists
                                cout << "Product does not exist." << endl;
                            else
                            {
                                do
                                {
                                    cout << "What is the qty of the product you would like to purchase: ";
                                    cin >> productQty;
                                    
                                    qtyExists= customer.searchQty(phead, productNum,productFound,productQty, qtyOver); //searches the qty wanting to be purchased exists
                                    
                                    if (qtyOver==true)//breaks do-while loop if the qty of product reaches 0
                                    {
                                        cout << "This product is out of stock." << endl;
                                        qtyOver=false;
                                        break;
                                    }
                                    else if(qtyExists==false)
                                        cout << "The quanitity you have entered does not exist." << endl;
                                    
                                }while (qtyExists==false);//loops until the qty wanting to be purchased is put in the cart
                                
                                if(qtyExists==true)
                                {
                                    cout << "Product has been added to your shopping cart." << endl;
                                    customer.BuildCartList(chead,productNum, productQty);
                                }//end of if statement qtExists
                                
                            }// end of if-else statement if product exist
                        }//end of if-else statement for the options entered in productNum
                        
                        lengthList= linkList.length(phead); //update length of product linkedList
                    }//end of for loop for loop (number of items that can be put in shopping cart)
                    
                    break;
                }//end case 2
                    
                case 3://exit
                {
                    updateInventory(inventory, outFile, phead, lengthList); //updates the "ProductData.txt" file
                    linkList.DeleteList(phead,chead);
                    modeChoice = 0;
                    break;
                }
                    
                default:
                    cout << "The option you have entered is invalid." << endl;
                    break;
            }
            
        }while(modeChoice==1 || modeChoice==2 || modeChoice==3); //continues to loop until the user enters "exit" admin AND buyer mode
        
    }//end of if-else if file opens
    
    return 0;
}//end main


/****************************************************************************
 * Function:updateInventory(fstream &outFile, struct product* head, int length)
 * Descr:updates the ProductData.csv file with the inventory remaining after products have been purchased
 ****************************************************************************/
void updateInventory(Inventory inventory, fstream &outFile, struct product*& headRef, int length)
{
    outFile.open("ProductData.csv", ios::in | ios::out | ios::trunc); //opens the outFile to write to
    struct product* current = headRef; //Create a temporary product to keep track of the current node
    int count=0; //counter variable
    
    if (!outFile) //checks whether the file is able to be open
    {
        cout << "File " << "\"" << "Invoivce.txt" << "\"" << " could not be opened." << endl;
        return;
    }
    else
    {
        while(current!=NULL) //depending on the size of the line the new product is insterted
        {
            if (current->itemQty==0)
            {
                inventory.deleteProduct(headRef, count);
                count--;
            }
            else if (current!=NULL)
            {
                outFile << current->itemNum << ",";
                outFile << "Product " << current->itemName<< ",";
                outFile << current->itemCost << ",";
                outFile << current->itemQty;
                
                if (current->next !=NULL)
                    outFile << "\r";
            }
            current=current->next;
            count++;
        } //end of do while*/
        
        outFile.close(); //closes the file
    }//end of if-else
}//end updateInventory()

