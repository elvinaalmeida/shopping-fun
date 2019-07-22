#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

//struct created for Products
struct ProductList
{
    int itemNum;
    int itemName;
    double itemCost;
    int itemQty;
};

//struct created for Cart
struct ShoppingCart
{
    int itemName;
    int itemQty;
};

//function prototype
ProductList *readFile(fstream& inFile, int& size);
int productPurchase(bool &cart);
bool searchProduct(ProductList *product, ShoppingCart *cart, int cartNum, int& rowFound, int& size);
bool searchCart (ShoppingCart *cart, int size, int productNumber, int& c);
bool quantityProduct(ProductList *product, ShoppingCart *cart, int cartNum, int& rowFound, int newQty, bool productFound);
void displayCart(ShoppingCart *cart,ProductList *product, int &cartProducts);
int displayInvoice(string customerInfo[5], ProductList *product, ShoppingCart *cart, int cartNum);
void updateInventory(fstream &outFile, ProductList *product, ShoppingCart *cart, int& size, int cartNum);

int main()
{
    string fileName= "ProductData.csv";     //name of file being read
    int size;                               //size of the array being created
    fstream inFile;                         //fstream object to read file
    fstream outFile;                        //fstream object to write to file
    bool productExists;                     //holds the return statement for the productExsits function
    int rowFound;                           //holds the row the product has been found in
    bool enterCart=false;                   //holds the answer if cart was entered
    string customerInfo[5];                 //holds the customer info entered to be printed on the invoice statement
    int productNumber;                      //holds the productNumber entered
    bool productFound=false;                //holds that the product was previously found in the cart
    int newQty;                             //holds that the new qty entered
    int c;                                  //holds the value where the product entered was found again
    
    
    inFile.open(fileName); //opens the file
    // If the input file does not exist the function returns a value of 0
    if(!inFile)
    {
        cout << "File " << "\"" << fileName << "\"" << " could not be opened." << endl;
        return 0;
    }
    
    else
    {
        cout << "If you would like to continue to checkout, enter 0." << endl;
        cout << "If you would like to view your cart, enter cart." << endl << endl;
        
        ProductList *product = readFile(inFile, size); //creates a product from reading the file
        ShoppingCart *cart = new ShoppingCart[size];  //creates a shopping cart of the number of products availiable for purchase
        
        for (int i=0; i < size ; i++) 
        {
            productNumber= productPurchase(enterCart); //holds the value of product number from the function
            cart[i].itemName=productNumber;
            
            if (enterCart==true) //prints the cart
            {
                displayCart(cart,product, size);
                productNumber= productPurchase(enterCart);
                cart[i].itemName=productNumber;
            }
            
            //if the user enters 0 they have completed shooping and personal info is asked to be printed on invoice
            if (cart[i].itemName==0)
            {
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
                
                
                displayInvoice(customerInfo, product, cart ,i);
                
                updateInventory(outFile, product, cart, size, i); //updates the inventory
                delete [] cart; //deletes the array cart
    
                break;
            }// end of if statement
            
            else
            {
                productExists= searchProduct(product, cart, i, rowFound, size); //holds the value of searchProduct()
                
                if (productExists==false)
                    cout << "Product does not exist." << endl << endl;
            
                else if (productExists==true && searchCart(cart, i, productNumber, c)==false)//if the product hasnt already been added a new cart has added
                {
                    
                    cout << "How many of the item would you like? ";
                    cin >> newQty;
                    
                    cart[i].itemQty=newQty;
                    
                    while (quantityProduct(product, cart, i, rowFound, newQty, productFound)==false)
                    {
                        cout << "There are not enough items of this product, please re-enter a quantity." << endl;
                        cin >> cart[i].itemQty;
                    }//end of while loop
                    
                }//end of else if statement
                
                else if (productExists==true && searchCart(cart, i, productNumber,c)==true)//if the product has already been added a new cart
                {
                    productFound=true;
                    int oldQty=cart[c].itemQty;
                    
                    cout << "How many of the item would you like? ";
                    cin >> newQty;
                    
                    cart[c].itemQty=newQty;
                    
                    while (quantityProduct(product, cart, c, rowFound, newQty, productFound)==false)
                    {
                        oldQty=cart[c].itemQty;
                        
                        cout << "There are not enough items of this product, please re-enter a quantity." << endl;
                        cin >> newQty;
                        
                        cart[c].itemQty=newQty;
                    }//end of while loop
                }//end of else if statement
            }
            if (searchCart(cart,i, productNumber, c)==true) //if the product has already been added then the item is deleted
                i--;
        }//end of for loop
    }
}

/****************************************************************************
 * Function: ProductList *readFile(fstream &inFile, int &size)
 * Descr: This function reads file and stores it in the struct
 ****************************************************************************/
ProductList *readFile(fstream &inFile, int &size)
{
    int row=0;          //counter for each row
    
    //saves the size of the array needed to be created
    inFile.seekg(0L, ios::beg);
    inFile >> size;
    
    ProductList *product = new ProductList[size];
    
    //starts at the second line and reads in product info, quanitity, and costs
    do
    {
        if (row == 0)
        {
            inFile.seekg(30L, ios::beg);
            inFile >> product[0].itemNum;
        }
        
        inFile.seekg(9L, ios::cur);
        inFile >> product[row].itemName;
        
        inFile.seekg(1L, ios::cur);
        inFile >> product[row].itemCost;
        
        inFile.seekg(1L, ios::cur);
        inFile >> product[row].itemQty;
        row++;
        
        inFile.seekg(1L, ios::cur);
        inFile >> product[row].itemNum;
        
    }while (row<size && !inFile.eof()); //end of do while
    
    inFile.close(); //closes the file
    
    return product;
}// end readSquare()

/****************************************************************************
 * Function: bool searchCart (ShoppingCart *cart, int size, int productNumber, int &c)
 * Descr: This function searches the cart to see if it was previously purchased
 ****************************************************************************/
bool searchCart (ShoppingCart *cart, int size, int productNumber, int &c)
{
    
    for (int i=0; i < size; i++)
    {
        if (cart[i].itemName==productNumber)
        {
            c=i;
            return true;
        }
    }
   
    return false;
}//end of function searchCart()

/****************************************************************************
 * Function: int productPurchase()
 * Descr: This functions asks the user the product number they would like to
 purchase and returns the number back to main
 ****************************************************************************/
int productPurchase(bool &cart)
{
    int productNumber=0; //holds the product number that the user enters
    string answer;
    
    cout << "What product would you like to purchase? ";
    cin >> answer;
    
    if (answer == "cart") // if cart is entered then the program will be able to print cart
        cart=true;
    
    else
    {
        cart=false;
        productNumber=stoi(answer);
    }
    
    return productNumber;
}

/******************************************************************************************************************
 * Function: bool searchProduct(ProductList *product, ShoppingCart *cart, int cartNum, int& rowFound, int& size)
 * Descr: Searches the product in the array and ouputs the product number and cost. If the product does not exist
 it returns false to the main.
 ******************************************************************************************************************/
bool searchProduct(ProductList *product, ShoppingCart *cart, int cartNum, int& rowFound, int& size)
{
    bool productExists=false;
    
    //searches the productList to see if the product entered by the user exists
    for (int row=0; row<size; row++)
    {
        if (cart[cartNum].itemName!=product[row].itemNum)
            productExists=false;
        
        else
        {
            productExists=true;
            cout << "Product Number: " << product[row].itemName << endl << "Product Cost: $" << product[row].itemCost << endl;
            rowFound=row;
            return productExists;
        }
    }//end for loop
    
    return productExists;
}//end function searchProduct

/******************************************************************************************************************
 * Function: quantityProduct(ProductList *product, ShoppingCart *cart, int cartNum, int& rowFound, int& newQty, bool productFound)
 * Descr: checks to see if the quanitity of product the user wants to buy is valid and if it is, it returns true back to the main
 ******************************************************************************************************************/
bool quantityProduct(ProductList *product, ShoppingCart *cart, int cartNum, int& rowFound, int newQty, bool productFound)
{
    bool quantityExists=false;  //holds the boolean value to check if the quanitity exists
    
    if (cart[cartNum].itemQty > product[rowFound].itemQty)
    {
        quantityExists=false;
        return quantityExists;
    }
    
    else
    {
        quantityExists=true;
        //oldQty=cart[cartNum].itemQty;
        cout << "Item has been added to your shopping cart." << endl << endl;
        product[rowFound].itemQty=product[rowFound].itemQty-cart[cartNum].itemQty; //subtracts the amount of quanitity available after the user has purchased
        
        if (product[rowFound].itemQty<0)
            product[rowFound].itemQty=0;
        return quantityExists;
    }
    
    return quantityExists;
}//end function quantityProduct

/******************************************************************************************************************
 * Function: displayCart(ShoppingCart *cart,ProductList *product, int &cartProducts)
 * Descr: displays the current cart purchased by the user
 ******************************************************************************************************************/
void displayCart(ShoppingCart *cart,ProductList *product, int &cartProducts)
{
    double total=0.0;       //holds the total value of all the products purchased
    
    
    cout << "Item Number           Item Name          Qty        Unit Cost        Total" << endl;
    cout << "__________________________________________________________________________________" << endl;
    
    
    // prints out the product purchased, quanitity, and cost of each item in the shopping cart
    for (int i=0; i < cartProducts; i++ )
    {
        for (int row=0; row<145; row++)
        {
            if (cart[i].itemName==product[row].itemNum)
            {
                total += cart[i].itemQty*product[row].itemCost;
                    cout << "Item Number " << product[row].itemNum << setw (18) << "Product Name " << product[row].itemName << setw(7) << cart[i].itemQty << setw (15) << cart[i].itemQty*product[row].itemCost<< setw(17) << total << endl;
                
            }
        }
            
    }
        cout << "__________________________________________________________________________________" << endl;
        cout << "                                                            Total: $ " << total << endl;
}//end function displayCart

/******************************************************************************************************************
 * Function: int displayInvoice(string customerInfo[5], ProductList *product, ShoppingCart *cart, int cartNum)
 * Descr:displays the invoice to the colsole and to a text file: "Invoice.txt"
 ******************************************************************************************************************/
int displayInvoice(string customerInfo[5], ProductList *product, ShoppingCart *cart, int cartNum)
{
    ofstream outFile;       //defines an output stream for the data file
    double total=0.0;       //holds the total value of all the products purchased
    
    
    outFile.open("Invoivce.txt");   //opens the output file
    //checks whether the file is able to be open
    if (!outFile)
    {
        cout << "File " << "\"" << "Invoivce.txt" << "\"" << " could not be opened." << endl;
        return 0;
    }
    
    else
    {
        //outputs the user info to the console
        cout << "Customer Name:" << customerInfo[0] << endl;
        cout << "Customer Address:" << customerInfo[1] << endl << "                 " << customerInfo[2] << "," << customerInfo[3] << " " << customerInfo[4] << endl;
        
        cout << "Item Number           Item Name          Qty        Unit Cost        Total" << endl;
        cout << "__________________________________________________________________________________" << endl;
        
        //outputs the user info to the text file
        outFile << "Customer Name:" << customerInfo[0] << endl;
        outFile<< "Customer Address:" << customerInfo[1] << endl << "                 " << customerInfo[2] << "," << customerInfo[3] << " " << customerInfo[4] << endl;
        
        outFile << "Item Number           Item Name          Qty        Unit Cost        Total" << endl;
        outFile << "________________________________________________________________________________" << endl;
        
        // prints out the product purchased, quanitity, and cost of each item in the shopping cart
        for (int i=0; i < cartNum; i++ )
        {
            for (int row=0; row<145; row++)
            {
                if (cart[i].itemName==product[row].itemNum)
                {
                    total += cart[i].itemQty*product[row].itemCost;
                    cout << "Item Number " << product[row].itemNum << setw (18) << "Product Name " << product[row].itemName << setw(7) << cart[i].itemQty << setw (15) << cart[i].itemQty*product[row].itemCost<< setw(17) << total << endl;
                    
                    outFile << "Item Number " << product[row].itemNum << setw (18) << "Product Name " << product[row].itemName << setw(7) << cart[i].itemQty<< setw (15) << cart[i].itemQty*product[row].itemCost << setw(17) << total << endl;
                }
                
            }
            
        }
        cout << "__________________________________________________________________________________" << endl;
        cout << "                                                            Total: $ " << total << endl;
        
        outFile << "_______________________________________________________________________________" << endl;
        outFile << "                                                            Total: $ " << total << endl;
    }
    return 1;
}//end function displayInvoice

/******************************************************************************************************************
 * Function: void updateInventory(fstream &outFile, ProductList *product, ShoppingCart *cart, int& size, int cartNum)
 * Descr: updates the inventorys and keeps it ready for the next user
 ******************************************************************************************************************/
void updateInventory(fstream &outFile, ProductList *product, ShoppingCart *cart, int& size, int cartNum)
{
    // opens the outfile
    outFile.open("ProductData.csv");
    int row=0;
    
    if (!outFile)
        cout << "Error";
    
    else
    {
        do //depending on the size of the line the new product is insterted
        {
            if (row == 0)
            {
                outFile.seekp(30L, ios::beg);
            }
            
            if (product[row].itemQty < 10)
            {
                outFile.seekp(24, ios::cur);
                outFile << "00" << product[row].itemQty;
                outFile.seekp(1, ios::cur);
            }
            
            else if (product[row].itemQty < 100)
            {
                outFile.seekp(24, ios::cur);
                outFile << "0" << product[row].itemQty;
                outFile.seekp(1, ios::cur);
            }
            
            else
            {
                outFile.seekp(24, ios::cur);
                outFile << product[row].itemQty;
                outFile.seekp(1, ios::cur);
            }
            
            row++;
        }while (row<size && !outFile.eof()); //end of do while*/
        
        delete [] product;
        outFile.close(); //closes the file
    }//end of if-else
}//end function updateInventory
