#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;
void BuildProductList(fstream &inFile, struct product*& headRef, int& lastProductNum); //builds product list using the productData file
int length(struct product* head);   //finds the length of the product list
int productPurchase(bool &cart);    //asks the user which product they would like to purchase and returns the product number
bool searchProduct(struct product*& head, int productNum, int& productFound, bool printItemInfo); //searchs for the product number in the product linked list
bool searchQty(struct product*& head, int productNum, int& productFound, int productQty, bool& qtyOver); //checks if the quantity being purchased is available
void BuildCartList(struct cart*& headRefCart, int productNum, int itemQty); //builds the cart linked list if the product and quanity wanted exists
void PrintCartLinkedList(struct cart* head, struct product* phead); //print the current cart linked list
void displayInvoice(string customerInfo[5], struct cart* head, struct product* phead); //display invoce to console and invoice.txt
void adminOptions(string adminOption, struct product*& headRef, int &productFound, int& lastProductNum); //if the user enters admin mode they are given options to add,delete, and update a product
void addProduct(struct product*& headRef, int nItemNum, int nItemName, double nItemCost, int nItemQty);//apends new product to end of linked list
void deleteProduct(struct product*& headRef, int position); //deletes a product that's qty=0
void insertQty(struct product*& headRef, int productNum, int productQty); //inserts updated qty into the linked list
void storeCustomerInfo(string customerInfo[]);//stores info into customerInfo[]
void updateInventory(fstream &outFile, struct product* head, int length); //updates the ProductData.csv file with the inventory remaining after
void DeleteList(struct product*& headRef,struct cart*& cheadRef); //deletes the linked lists
void PrintLinkedList(struct product* head); //temp function

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

int main()
{
    struct product* phead = NULL;   //creates a product structure
    struct cart* chead = NULL;      //creates a cart structure
    fstream inFile, outFile;        //creates a file stream for reading and writing a file
    int productNum, productQty;     //holds the product number and the product quanitity entered by the user
    string adminOption, adminOrBuyer, exitAdminBuyer;   //holds the outputs to exit admin, buyer, or both functions
    bool productExists, qtyExists=false;    //holds the boolean value of if the product or qty the user wants, exists
    bool enterCart=false, qtyOver;          //cart is set to true if the user enters "cart", qtyOver holds the boolean value if the qty avaliable reaches 0
    int lengthList, productFound, lastProductNum; //holds the length of the list, the product position found, and the number of the last product in the file
    string customerInfo[5];     //holds the customer info entered to be printed on the invoice statement
    
    if(!inFile) //if the file is unable to be purchased the function ends
    {
        cout << "File " << "\"" << "Product.txt" << "\"" << " could not be opened." << endl;
        return 0;
    }
    
    else
    {
        BuildProductList(inFile, phead, lastProductNum); //creates a linked list from reading the file
        lengthList=length(phead); //stores the current length of the list
        
        do
        {
            cout << "If you are an admin enter \"admin\". If you are a buyer enter \"buyer.\"" << endl;
            cin >> adminOrBuyer;
            
            if (adminOrBuyer=="admin")//if the user enters admin they are in admin status
            {
                string endAdmin; //holds the anser the user outputed, if they would like to continue or end admin mode
                
                cout << "You are currently in admin mode!" << endl;
                do
                {
                    cout << "To add a product enter \"add\"." << endl << "To delete a product enter \"delete\"." << endl << "To update a product's qty enter \"update\"." << endl;
                    cin >> adminOption; //stores the output of if the user want to add, delete, or update qty
                    
                    adminOptions(adminOption,phead,productFound,lastProductNum); //calls the adminOption function
                    
                    cout << "To exit admin mode enter \"exit\". To remain in admin mode enter \"continue\"." << endl;
                    cin >> endAdmin; //stores the output for if the user wants to "exit" admin mode
                    
                }while(endAdmin != "exit"); //continues to loop until the user enters "exit" admin mode
                
                lengthList=length(phead); //updates the length of product list once the user exists the admin option
            }//end of if statement
            
            else if (adminOrBuyer=="buyer")// if the user is a buyer they have the option to purchase products
            {
                cout << "If you would like to continue to checkout, enter 0." << endl;
                cout << "If you would like to view your cart, enter \"cart\"." << endl;
                
                for (int i=0; i < lengthList; i++)//limits the number of products you can add to your cart according to the number of products available
                {
                    productNum=productPurchase(enterCart);
                    
                    if (enterCart==true)//if the user enters cart the cart is printed
                    {
                        //check to see if a cart has been built before printing the linked list
                        if (chead==NULL)
                            cout << "There are no products in your cart." << endl;
                        else
                        {
                            PrintCartLinkedList(chead, phead);
                        }
                            productNum= productPurchase(enterCart);
                    }//end of if statement
                    
                    if (productNum==0)
                    {
                        storeCustomerInfo(customerInfo);
                        displayInvoice(customerInfo, chead, phead);
                        break;
                    }//end of if statement
                    
                    else
                    {
                        productExists= searchProduct(phead, productNum, productFound, true); //calls the function searchProduct and stores it into productExsits
                        
                        if(productExists==false) //checks if the productExists
                            cout << "Product does not exist." << endl;
                        else
                        {
                            do
                            {
                                cout << "What is the qty of the product you would like to purchase: ";
                                cin >> productQty;
                                
                                qtyExists=searchQty(phead, productNum,productFound,productQty, qtyOver); //searches the qty wanting to be purchased exists
                                
                                if (qtyOver==true)//breaks do-while loop if the qty of product reaches 0
                                {
                                    cout << "This product is out of stock." << endl;
                                    break;
                                }
                                else if(qtyExists==false)
                                    cout << "The quanitity you have entered does not exist." << endl;
                                
                            }while (qtyExists==false);//loops until the qty wanting to be purchased is put in the cart
                            
                            if(qtyExists==true)
                            {
                                cout << "Product has been added to your shopping cart." << endl;
                                BuildCartList(chead, productNum, productQty);
                            }//end of if statement qtExists
                            
                        }// end of if-else statement if product exist
                    }//end of if-else statement for the options entered in productNum
                    lengthList= length(phead); //update length of product linkedList
                }//end of for loop for loop (number of items that can be put in shopping cart)
            }//end of if-else (buyer or admin) statement
            
            cout << "If you would like to exit both modes (admin and buyer) enter \"exit\". Otherwise enter \"continue\"." << endl;
            cin >> exitAdminBuyer; //stores the output for if the user wants to "exit" admin AND buyer mode
            
        }while(exitAdminBuyer != "exit"); //continues to loop until the user enters "exit" admin AND buyer mode
        
        updateInventory(outFile, phead, lengthList); //updates the "ProductData.txt" file
        DeleteList(phead, chead); //deletes the linked list before closing the program
    }//end of if-else if file opens
    return 0;
}//end main

/****************************************************************************
 * Function:BuildProductList(fstream &inFile, struct product*& headRef, int& lastProductNum)
 * Descr: reads file from productData.csv and builds a linked list from the file
 ****************************************************************************/
void BuildProductList(fstream &inFile, struct product*& headRef, int& lastProductNum)
{
    string tempproduct; //stores the string "product"
    string titemNum, titemName, titemCost, titemQty;//stores the item number, name, cost, qty from each line of file
    int titemNum1, titemName1, titemQty1;           //stores the convereted item info (string to int)
    float titemCost1;                               //stores the convereted item info (string to int)
    
    inFile.open("ProductData.csv"); //opens the file
    
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
                newNode->previous=current;
                current = newNode;
            }
            
            lastProductNum=titemName1;
            
        }//end of while loop
        
        inFile.close(); //close the file
        headRef=head; //return the head of the file
        
    }//end if-else if file opens
}//end BuildProductList()

/****************************************************************************
 * Function:length(struct product* head)
 * Descr: finds the length of the product linked list
 ****************************************************************************/
int length(struct product* head)
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

/****************************************************************************
 * Function:productPurchase(bool &cart)
 * Descr:asks the user which product they would like to purchase and return the product number
 ****************************************************************************/
int productPurchase(bool &cart)
{
    int productNumber=0; //holds the product number that the user enters
    string answer;      //holds the output answer
    
    cout << endl << "What product would you like to purchase? ";
    cin >> answer;
    
    if (answer == "cart") //if cart is entered then the program will be able to print cart
        cart=true;
    
    else
    {
        cart=false;
        productNumber=atoi(answer.c_str()); //string answer is converted to an int
    }
    
    return productNumber; //returns productNumber back to main
}//end productPurchase()

/****************************************************************************
 * Function:searchProduct(struct product*& head, int productNum, int& productFound,  bool printItemInfo)
 * Descr: searchs for the product number in the product linked list
 ****************************************************************************/
bool searchProduct(struct product*& head, int productNum, int& productFound, bool printItemInfo)
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

/****************************************************************************
 * Function:searchQty(struct product*& head, int productNum, int& productFound, int productQty, bool& qtyOver)
 * Descr: checks if the quantity being purchased is available
 ****************************************************************************/
bool searchQty(struct product*& head, int productNum, int& productFound, int productQty, bool& qtyOver)
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
void BuildCartList(struct cart*& headRefCart, int productNum, int itemQty)
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
            current = headRefCart; //Set the current node to the head node
            
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
            
            //current->next = NULL;
        }
        
        else //if product being purchased is not already in cart appends a new node
        {
            current = headRefCart;//Set the current node to the head node
            
            while (current->next) //Traverse the linked list until you reach the end
                current = current->next;
            
            current->next = newNode;//At the end set the current to point to the new node
        }//end of if-else product previously purchased
    }//end of if else if head is not NULL
}//end BuildCartList()

/****************************************************************************
 * Function:PrintCartLinkedList(struct cart* head, struct product* phead)
 * Descr:displays the current cart linked list
 ****************************************************************************/
void PrintCartLinkedList(struct cart* head, struct product* phead)
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

void storeCustomerInfo(string customerInfo[])
{
    //asks the user their customer info and it stores it in the array
    cout << "Enter Name: ";
    cin >> customerInfo[0];
    
    cout << "Street Address: ";
    cin.ignore(100, '\n');
    getline(cin, customerInfo[1]);
    
    cout << "City: ";
    cin >> customerInfo[2];
    
    cout << "State: ";
    cin >> customerInfo[3];
    
    cout << "Zip Code: ";
    cin >> customerInfo[4];
}

/****************************************************************************
 * Function:displayInvoice(string customerInfo[5], struct cart* head, struct product* phead)
 * Descr:displays the invoice to the console and create invoice.txt
 ****************************************************************************/
void displayInvoice(string customerInfo[5], struct cart* head, struct product* phead)
{
    ofstream outFile;//defines an output stream for the data file
    
    outFile.open("Invoivce.txt"); //opens the output file
    
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
        cout << "Customer Name:" << customerInfo[0] << endl;
        cout << "Customer Address:" << customerInfo[1] << endl << "                 " << customerInfo[2] << "," << customerInfo[3] << " " << customerInfo[4] << endl;
        
        outFile << "Customer Name:" << customerInfo[0] << endl;
        outFile << "Customer Address:" << customerInfo[1] << endl << "                 " << customerInfo[2] << "," << customerInfo[3] << " " << customerInfo[4] << endl;
        
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

/****************************************************************************
 * Function:adminOptions(string adminOption, struct product*& headRef, int &productFound, int& lastProductNum)
 * Descr:if the admin enters admin status they are given the option to add product, delete product, update productsQty
 ****************************************************************************/
void adminOptions(string adminOption, struct product*& headRef, int &productFound, int& lastProductNum)
{
    int nitemNum, nitemName, nitemQty; //stores the new item info
    float nitemCost;
    
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
        if (searchProduct(headRef, nitemNum, productFound, false)==true) //checks if the product wanting to be deleted exsists
        {
            deleteProduct(headRef,productFound); //problem with delete node // fix the fact that it prints out item name and item cost
            cout << "Item has been deleted." << endl;
        }
        else
            cout << "The product you want to delete does not exist." << endl;
    }//end if "delete"
    if (adminOption=="update")
    {
        cout << "What is the product number that you would like to update: ";
        cin >> nitemNum;
        if (searchProduct(headRef, nitemNum, productFound, false)==true) //checks if the product wanting to be deleted exsists
        {
            cout << "What is new quanitity of the product: ";
            cin >> nitemQty;
            insertQty(headRef, nitemNum, nitemQty); //calls the function instertQty
            cout << "Item qty of the product has been updated." << endl;
        }
        else
            cout << "The product you want to update does not exist." << endl;
    }//end if "update"
}//end adminOptions

/****************************************************************************
 * Function:addProduct(struct product*& headRef, int nItemNum, int nItemName, double nItemCost, int nItemQty)
 * Descr:updates the product linked list with the inventory remaining after products have been purchased
 ****************************************************************************/
void addProduct(struct product*& headRef, int nItemNum, int nItemName, double nItemCost, int nItemQty)
{
    struct product* head = headRef; //Create a temporary node to keep track of the orginal head node
    struct product* current = NULL; //Create a temporary node to use to transverse through the linked list
    struct product* newNode = new struct product; //Create a new node to add to the end of the list
    
    newNode->itemNum = nItemNum; //Set the Node Data
    newNode->itemName = nItemName;
    newNode->itemCost = nItemCost;
    newNode->itemQty = nItemQty;
    newNode->next = NULL;
    
    if (!head)  //Check to see if linked list is empty
        head = newNode;  //If it is append the new node onto the empty list
    else
    {
        current = head; //Set the current node to the head node
        while (current->next)  //Traverse the linked list until you reach the end
        {
            current = current->next;
        }
        current->next = newNode; //At the end set the current to point to the new node.
    }
    headRef = head; //Return the new linked list to the calling function
}//end addProduct

/****************************************************************************
 * Function:deleteProduct(struct product*& headRef, int position)
 * Descr:updates the product linked list once product has been deleted
 ****************************************************************************/
void deleteProduct(struct product*& headRef, int position) //have to search the product and return the position currently deltes the product after it
{
    struct product* currentNode=headRef;
    struct product* previousNode=headRef;
    int count=0;
    if(position==0)
    {
        headRef=currentNode->next;
        delete currentNode;
    }
    else
    {
        while (currentNode != nullptr && count !=position)
        {
            previousNode =currentNode;
            currentNode=currentNode-> next;
            count++;
        }
        
        if (count==position)
        {
            previousNode->next=currentNode->next;
            delete currentNode;
        }
    }
}

/****************************************************************************
 * Function:insertQty(struct product*& headRef, int productNum, int productQty)
 * Descr: updates the product linked list with the new qty
 ****************************************************************************/
void insertQty(struct product*& headRef, int productNum, int productQty)
{
    struct product* current = headRef;//Create a temporary product to keep track of the current product
    
    while (current != NULL) //Traverse the product linked list until you reach the end
    {
        if (current->itemNum == productNum)//if the product is found qty is updated
        {
            current->itemQty=productQty;
            break;
        }
        else
            current = current->next;
    }//end while loop
}//end insertQty

/****************************************************************************
 * Function:updateInventory(fstream &outFile, struct product* head, int length)
 * Descr:updates the ProductData.csv file with the inventory remaining after products have been purchased
 ****************************************************************************/
void updateInventory(fstream &outFile, struct product* head, int length)
{
    outFile.open("ProductData.csv", ios::in | ios::out | ios::trunc); //opens the outFile to write to
    struct product* current = head; //Create a temporary product to keep track of the current node
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
                deleteProduct(head, count);
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

/****************************************************************************
 * Function:DeleteList(struct product*& headRef, struct cart*& cheadRef)
 * Descr: deletes the product and cart linked list
 ****************************************************************************/
void DeleteList(struct product*& headRef, struct cart*& cheadRef)
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

//temp function to test if other functions work
void PrintLinkedList(struct product* head)
{
    struct product* current = head;        //Create a temporary node to keep track of the current node
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

