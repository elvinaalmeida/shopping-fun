//Name: Elvina Almeida
//Class: CS 1337.012
//Date: 2/15/18
//Complier: XCODE 2018 and written on a mac
//Project: create a system that allows a user to shop for items, add them to a shopping cart, and process their order

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

//function prototypes
int readFile(double productList[145][4],string inputFileName);
int productPurchase();
bool searchProduct(double productList[145][4], double shoppingCart[145][2], int cartNum, int& rowFound);
bool quantityProduct(double productList[145][4], double shoppingCart[145][2], int cartNum, int& rowFound);
int displayInvoice(string customerInfo[5], double productList[145][4], double shoppingCart[145][2], int cartNum);

int main()
{
    string fileName= "ProductData.csv";
    double productList[145][4];     //holds the products entered from ProductData.csv
    double shoppingCart[145][2];    //holds the products and quanitity the user would like to purchase
    bool productExists;             //holds the return statement for the productExsits function
    int rowFound;                   //holds the row the product has been found in
    string customerInfo[5];         //holds the customer info entered to be printed on the invoice statement
   
    //if the data is been stored correctly into the array the program asks the user for which products they would like to purchase
    if (readFile(productList,fileName))
    {
        cout << "If you would like to continue to checkout, enter 0." << endl << endl;
        
        //the user can purchase up to 145 products
        for (int i=0; i < 145 ; i++)
        {
            shoppingCart[i][0]= productPurchase();
            
            //if the user enters 0 they have completed shooping and personal info is asked to be printed on invoice
            if (shoppingCart[i][0]==0)
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
                
                
                displayInvoice(customerInfo,productList, shoppingCart,i);
                break;
            }// end of if statement
            
            productExists= searchProduct(productList, shoppingCart, i, rowFound);
                
            if (productExists==false)
                cout << "Product does not exsist." << endl;
            
            //asks the user for a quanitity if the product exists
            else  if (productExists==true)
            {
                cout << "How many of the item would you like?" << endl;
                cin >> shoppingCart[i][1];
                   
                while (quantityProduct(productList, shoppingCart, i, rowFound)==false)
                {
                    cout << "There are not enough items of this product, please re-enter a quantity." << endl;
                    cin >> shoppingCart[i][1];
                }//end of while loop
            
            }//end of else if statement
        }//end of for loop
    }//end of if statment
}//end main

/****************************************************************************
 * Function: int readFile(double productList[145][4],string inputFileName)
 * Descr: This function reads the ProductData.csv file and stores it in an
 array. If the file cannot be opened the user is notified.
 ****************************************************************************/
int readFile(double productList[145][4],string inputFileName)
{
    fstream inFile;     //stream operator for the csv file
    int row=0;          //counter for each row
    
    inFile.open(inputFileName); //opens the file
    
    // If the input file does not exist the function returns a value of 0
    if(!inFile)
    {
        cout << "File " << "\"" << inputFileName << "\"" << " could not be opened." << endl;
        return 0;
    }
    // If the file is opened properly the contents of the file are read into the array
    else
    {
        inFile.seekg(26L, ios::beg);
        inFile >> productList[0][0];
        
        //starts at the second line and reads in product info, quanitity, and costs
        do
        {
            if (row == 0)
            {
                inFile.seekg(26L, ios::beg);
                inFile >> productList[0][0];
            }
            
            inFile.seekg(9L, ios::cur);
            inFile >> productList[row][1];
            
            inFile.seekg(1L, ios::cur);
            inFile >> productList[row][2];
            
            inFile.seekg(1L, ios::cur);
            inFile >> productList[row][3];
            
            row++;
            
            inFile.seekg(1L, ios::cur);
            inFile >> productList[row][0];
            
            
        }while (row<145 && !inFile.eof()); //end of do while
        
    }// end of if-else
    
    inFile.close(); //closes the file
    return  1;
}// end readSquare()

/****************************************************************************
 * Function: int productPurchase()
 * Descr: This functions asks the user the product number they would like to
 purchase and returns the number back to main
 ****************************************************************************/
int productPurchase()
{
    int productNumber; //holds the product number that the user enters
    
    cout << "What product would you like to purchase? ";
    cin >> productNumber;
    
    return productNumber;
}

/******************************************************************************************************************
 * Function: bool searchProduct(double productList[145][4], double shoppingCart[145][2], int cartNum, int& rowFound)
 * Descr: Searches the product in the array and ouputs the product number and cost. If the product does not exist
 it returns false to the main.
 ******************************************************************************************************************/
bool searchProduct(double productList[145][4], double shoppingCart[145][2], int cartNum, int& rowFound)
{
    bool productExists=false;
    
    //searches the productList to see if the product entered by the user exists
    for (int row=0; row<145; row++)
    {
        if (shoppingCart[cartNum][0]!=productList[row][0])
            productExists=false;
        
        else
        {
            productExists=true;
            cout << "Product Number: " << productList[row][1] << endl << "Product Cost: $" << productList[row][2] << endl;
            rowFound=row;
            return productExists;
        }
    }//end for loop
    
    return productExists;
}//end function searchProduct

/******************************************************************************************************************
 * Function: bool quantityProduct(double productList[145][4], double shoppingCart[145][2], int cartNum, int& rowFound)
 * Descr:checks to see if the quanitity of product the user wants to buy is valid and if it is, it returns true back to the main
 ******************************************************************************************************************/
bool quantityProduct(double productList[145][4], double shoppingCart[145][2], int cartNum, int& rowFound)
{
    bool quantityExists=false;  //holds the boolean value to check if the quanitity exists

    if (shoppingCart[cartNum][1]>productList[rowFound][3])
    {
        quantityExists=false;
    }
    
    else
    {
        quantityExists=true;
        cout << "Item has been added to your shopping cart." << endl;
        productList[rowFound][3]= productList[rowFound][3]-shoppingCart[cartNum][1]; //subtracts the amount of quanitity available after the user has purchased
        return quantityExists;
    }
    
    return quantityExists;
}//end function quantityProduct

/******************************************************************************************************************
 * Function: int displayInvoice(string customerInfo[5], double productList[145][4], double shoppingCart[145][2], int cartNum)
 * Descr:displays the invoice to the colsole and to a text file: "Invoice.txt"
 ******************************************************************************************************************/
int displayInvoice(string customerInfo[5], double productList[145][4], double shoppingCart[145][2], int cartNum)
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
                if (shoppingCart[i][0]==productList[row][0])
                {
                    total += shoppingCart[i][1]*productList[row][2];
                    cout << "Item Number " << productList[row][0] << setw (18) << "Product Name " << productList[row][1] << setw(7) << shoppingCart[i][1] << setw (15) << shoppingCart[i][1]*productList[row][2] << setw(17) << total << endl;
                    
                    outFile << "Item Number " << productList[row][0] << setw (18) << "Product Name " << productList[row][1] << setw(7) << shoppingCart[i][1] << setw (15) << shoppingCart[i][1]*productList[row][2] << setw(17) << total << endl;
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





