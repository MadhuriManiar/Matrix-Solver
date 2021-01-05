//Madhuri Maniar
//MVM190002
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>


using namespace std;

void getMatrix(string, float&, float&, float&, const string&);
void storeMatrix(float&, float&, float&, float *ptr2, float&);
void printMatrix(float *ptr2, int, int);
void switchRows(int, int, float *ptr2, int, float *pos, float *newptr, float *newptr2,float *pos1, float *pos2);
void multRow(int, float, float *ptr2, float *pos, int);
void scalMult(int, float, int, int, float *ptr2, float *pos);
bool solvedMatrix(float *ptr2, int, float *pos);
void printVal(float *ptr2, int, float, float, float);

int main()
{
    //define variables
    string filename, line, equation, total;
    float coefx = 0, coefy=0,coefz=0, newtot, value;
    const int SIZE = 4;
    int row=0, action, row1, row2;
    //prompt user for textfile name
    cout<<"Enter name of the text file: "<<endl;
    getline(cin, filename);
    ifstream infile;
    //open file
    infile.open(filename);
    //count how many rows are in the textfile
    while(!infile.eof()) {
        (getline(infile, line));
        row++;
    }
    infile.close();
    //define pointers
    auto *ptr = new float[SIZE*row];
    auto *newptr = new float[SIZE];
    auto *newptr2 = new float[SIZE];
    float *pos;
    pos = ptr;
    float *pos1 = newptr;
    float *pos2 = newptr2;
    infile.open(filename, ios::in);
    //open textfile
    if (!infile.fail()) {
        string str1;
        //get each line and split them up using the = sign as a delimiter
        while (!infile.eof()) {
            getline(infile, str1);
            stringstream linestream(str1);
            getline(linestream, equation, '=');
            getline(linestream, total, '=');
            int index = (int)str1.find('=');
            if (index == -1)
                newtot = 0;
            else
                newtot = stof(total.substr(0));
            getMatrix(equation, coefx, coefy, coefz, str1);
            storeMatrix(coefx, coefy, coefz, ptr, newtot);
            ptr = ptr +4;
        }
        ptr = pos;
        printMatrix(ptr, row, SIZE);
        infile.close();
        //check to make sure the equations are solved
        //if solved then print the variables
        if (solvedMatrix(ptr, row, pos))
            printVal(ptr, row, coefx, coefz, coefy);
            //if not solved, prompt user for row operations
        else {
            do {
                cout << "Select an action from the following menu. " << endl;
                cout << "1: Switch Rows. " << endl;
                cout << "2: Multiply a row by a non-zero number. " << endl;
                cout << "3: Add scalar multiple of one row to another row. " << endl;
                cout << "4: Exit the Program. " << endl;
                cin >> action;
                //if user chooses one, switch the rows also based on user input
                if (action == 1) {
                    //check to make sure the matrix is not in reduced row echelon
                    //if matrix is in reduced row echelon form, then break the while loop
                    if (solvedMatrix(ptr, row, pos)) {
                        cout << "Matrix is in reduced echelon form, cannot perform an row operations. " << endl;
                        break;
                    }
                    //prompt user for first row
                    cout << "Enter first row number. " << endl;
                    cin >> row1;
                    //check for valid row number
                    while (row1 > row) {
                        cout << "Enter valid row number. " << endl;
                        cin >> row1;
                    }
                    //prompt user for second row
                    cout << "Enter second row number. " << endl;
                    cin >> row2;
                    //check for valid row number
                    while (row2 > row) {
                        cout << "Enter valid row number. " << endl;
                        cin >> row2;
                    }
                    switchRows(row1, row2, ptr, row, pos, newptr, newptr2, pos1, pos2);
                    ptr = pos;
                }
                //if user chooses scalar row multiplication, multiply row by value that user inputs
                if (action == 2) {
                    //check to make sure the matrix is not in reduced row echelon
                    //if matrix is in reduced row echelon form, then break the while loop
                    if (solvedMatrix(ptr, row, pos)) {
                        cout << "Matrix is in reduced echelon form" << endl;
                        break;
                    }
                    //prompt user for row
                    cout << "Which row do you want to multiply? " << endl;
                    cin >> row1;
                    //check for valid row number
                    while (row1 > row) {
                        cout << "Enter valid row number. " << endl;
                        cin >> row1;
                    }
                    //prompt user for non zero value
                    cout << "Enter non zero value" << endl;
                    cin >> value;
                    //check for value in range
                    while (value == 0) {
                        cout << "Enter non zero number. " << endl;
                        cin >> value;
                    }
                    multRow(row1, value, ptr, pos, row);
                    ptr = pos;
                }
                //if user chooses 3, then multiply row and add to a different row
                if (action == 3) {
                    //check to make sure the matrix is not in reduced row echelon
                    //if matrix is in reduced row echelon form, then break the while loop
                    if (solvedMatrix(ptr, row, pos)) {
                        cout << "Matrix is in reduced echelon form" << endl;
                        break;
                    }
                    //prompt user for first row
                    cout << "Enter first row " << endl;
                    cin >> row1;
                    //check for valid row number
                    while (row1 > row) {
                        cout << "Enter valid row number. " << endl;
                        cin >> row1;
                    }
                    //prompt user for scalar multiplication of row
                    cout << "Enter non zero value. " << endl;
                    cin >> value;
                    //check for value in range
                    while (value == 0) {
                        cout << "Enter non zero value. " << endl;
                        cin >> value;
                    }
                    //prompt the user for second row
                    cout << "Enter second row. " << endl;
                    cin >> row2;
                    //check for valid row number
                    while (row2 > row) {
                        cout << "Enter valid row number. " << endl;
                        cin >> row2;
                    }
                    scalMult(row1, value, row2, row, ptr, pos);
                    ptr = pos;
                }
                //check for valid menu number
                if (action > 4 || action < 1)
                    cout << "Enter a number from the menu" << endl;

            } while (action != 4);
            printMatrix(ptr, row, SIZE);
            printVal(ptr, row, coefx, coefz, coefy);
        }
    }
    delete [] ptr;
    delete [] newptr;
    delete [] newptr2;
    return 0;
}

void getMatrix(string equation, float& coefx, float& coefy, float& coefz, const string& str1)
{
    int start=0;
    string str;
    //find index of where the equations find the variables
    int index1 = (int)str1.find('x');
    int index2 = (int)str1.find('y');
    int index3 = (int)str1.find('z');
    //if variables are not found, assume coefficients are zero
    if (index1==-1)
        coefx = 0;
    if (index2==-1)
        coefy = 0;
    if (index3 ==-1)
        coefz=0;
    //go through each equation and find coefficients of each variable
    for (int i = 0; i < (int)equation.length(); i++) {
        if (equation.at(i)=='x') {
            str=equation.substr(start,i-start);
            if (str =="-")
                coefx = -1;
            else if (str=="+")
                coefx = 1;
            else if (str=="")
                coefx = 1;
            else if (str.at(0)=='+')
                coefx = stof(equation.substr(start+1, i-start));
            else if(str.at(0)=='-')
                coefx = stof(str);
            else
                coefx = stof(str);
            start = i+1;
        }
        if (equation.at(i)=='y') {
            str=equation.substr(start,i-start);
            if (str =="-")
                coefy = -1;
            else if (str=="+")
                coefy = 1;
            else if (str =="")
                coefy=1;
            else if (str.at(0)=='+')
                coefy = stof(equation.substr(start+1, i-start));
            else if(str.at(0)=='-')
                coefy = stof(str);
            else
                coefy = stof(str);
            start = i+1;
        }
        if (equation.at(i)=='z') {
            str=equation.substr(start,i-start);
            if (str =="-")
                coefz = -1;
            else if (str=="+")
                coefz = 1;
            else if (str=="")
                coefz = 1;
            else if (str.at(0)=='+')
                coefz = stof(equation.substr(start+1, i-start));
            else if(str.at(0)=='-')
                coefz = stof(str);
            else
                coefz = stof(str);
            start = i+1;
        }

    }
}

void storeMatrix(float &coefx, float &coefy, float &coefz, float *ptr2, float &newtot)
{
    //assign coefficients to new variables
    *ptr2 = coefx;
    ptr2++;
    *ptr2 = coefy;
    ptr2++;
    *ptr2 = coefz;
    ptr2++;
    *ptr2 = newtot;
    ptr2++;
}

void printMatrix(float *ptr2, int row, int SIZE)
{
    //go through each row and print the coefficients
    for (int i = 1; i <= SIZE*row; i++) {
        cout<<fixed<<setprecision(2)<<*ptr2<<" ";
        ptr2++;
        if (i%4==0)
            cout<<endl;
    }
    cout<<endl;
}

void switchRows(int rw1, int rw2, float *ptr2, int row, float *pos, float *newptr, float *newptr2, float *pos1, float *pos2)
{
    //make a temp pointer to store first row coefficients
    for (int i = 0; i < 4*row; i++) {
        if (i == 4 * (rw1 - 1)) {
            *newptr = *ptr2;
            newptr++;
            ptr2++;
            *newptr = *ptr2;
            newptr++;
            ptr2++;
            *newptr = *ptr2;
            newptr++;
            ptr2++;
            *newptr = *ptr2;
            newptr++;
            ptr2++;
        }
        else{
            ptr2++;
        }
    }
    // assign temp pointer to second row
    ptr2 = pos;
    for (int i = 0; i < 4*row; ++i) {
        if (i == 4 * (rw2 - 1)) {
            *newptr2 = *ptr2;
            newptr2++;
            ptr2++;
            *newptr2 = *ptr2;
            newptr2++;
            ptr2++;
            *newptr2 = *ptr2;
            newptr2++;
            ptr2++;
            *newptr2 = *ptr2;
            newptr2++;
            ptr2++;
        }
        else{
            ptr2++;
            //newptr2++;
        }
    }
    //assign the second row coefficients to first row
    ptr2= pos;
    newptr = pos1;
    newptr2 = pos2;
    for (int i = 0; i < 4*row; i++) {
        if (i == 4 * (rw1 - 1)) {
            *ptr2 = *newptr2;
            ptr2++;
            newptr2++;
            *ptr2 = *newptr2;
            ptr2++;
            newptr2++;
            *ptr2 = *newptr2;
            ptr2++;
            newptr2++;
            *ptr2 = *newptr2;
            ptr2++;
            newptr2++;
        }
        else{
            ptr2++;
        }
    }
    ptr2 = pos;
    for (int i = 0; i < 4*row; ++i) {
        if (i == 4 * (rw2 - 1)) {
            *ptr2 = *newptr;
            ptr2++;
            newptr++;
            *ptr2 = *newptr;
            ptr2++;
            newptr++;
            *ptr2 = *newptr;
            ptr2++;
            newptr++;
            *ptr2 = *newptr;
            ptr2++;
            newptr++;
        } else {
            ptr2++;
        }
    }
    //print new matrix out
    ptr2 = pos;
    for (int i = 1; i <= 4*row; i++)
    {
        cout<<fixed<<setprecision(2)<<*ptr2<<" ";
        ptr2++;
        if (i%4==0)
            cout<<endl;
    }
    cout<<endl;
    newptr = pos1;
    newptr2 = pos2;
    ptr2 = pos;
}

void multRow(int rw1, float val, float *ptr2, float *pos, int row)
{
    //going through row and multiplying row by value
    for (int i = 0; i < 4*row; ++i) {
        if (i == 4*(rw1-1)) {
            *ptr2 = *ptr2 * val;
            if (*ptr2 == -0)
                *ptr2 = 0;
            ptr2++;
            *ptr2 = *ptr2 * val;
            if (*ptr2 == -0)
                *ptr2 = 0;
            ptr2++;
            *ptr2 = *ptr2 * val;
            if (*ptr2 == -0)
                *ptr2 = 0;
            ptr2++;
            *ptr2 = *ptr2 * val;
            if (*ptr2 == -0)
                *ptr2 = 0;
            ptr2++;
        }
        ptr2++;
    }
    ptr2 = pos;
    //prints out new matrix
    for (int i = 1; i <= 4*row; i++)
    {
        cout<<fixed<<setprecision(2)<<*ptr2<<" ";
        ptr2++;
        if (i%4==0)
            cout<<endl;
    }
    cout<<endl;
}

void scalMult(int rw1, float mult, int rw2, int row, float *ptr2, float *pos)
{
    float *newptr;
    newptr = ptr2;
    //moves pointer to the end
    for (int i = 0; i < 4*(rw1-1); ++i)
    {
        newptr++;
    }
    //multiply row by value
    for (int i = 0; i < 4*row; i++) {
        if (i == 4 * (rw1 - 1)) {
            *newptr = *newptr * mult;
            if (*newptr < 0 && *newptr > -.000001)
                *newptr = 0;
            newptr++;
            *newptr = *newptr * mult;
            if (*newptr < 0 && *newptr > -.000001)
                *newptr = 0;
            newptr++;
            *newptr = *newptr * mult;
            if (*newptr < 0 && *newptr > -.000001)
                *newptr = 0;
            newptr++;
            *newptr = *newptr * mult;
            if (*newptr < 0 && *newptr > -.000001)
                *newptr = 0;
            newptr++;
        }
    }
    //move pointer back to the beggining
    for (int i = 0; i < 4; ++i)
    {
        newptr--;
    }
    ptr2=pos;
    //add the multiplied row to second row
    for (int i = 0; i < 4*row; ++i) {
        if (i == 4 * (rw2 - 1)) {
            *ptr2 = *ptr2 + *newptr;
            ptr2++;
            newptr++;
            *ptr2 = *ptr2 + *newptr;
            ptr2++;
            newptr++;
            *ptr2 = *ptr2 + *newptr;
            ptr2++;
            newptr++;
            *ptr2 = *ptr2 + *newptr;
            ptr2++;
            newptr++;
        }
        else {
            ptr2++;
        }
    }
    ptr2=pos;
    newptr = pos;
    //change first row back to original values
    for (int i = 0; i < 4*row; ++i)
    {
        if (i == 4*(rw1-1)) {
            *ptr2 = *newptr / mult;
            ptr2++;
            newptr++;
            *ptr2 = *newptr / mult;
            ptr2++;
            newptr++;
            *ptr2 = *newptr / mult;
            ptr2++;
            newptr++;
            *ptr2 = *newptr / mult;
            ptr2++;
            newptr++;
        }
        else {
            ptr2++;
            newptr++;
        }
    }
    ptr2 = pos;
    //print new matrix out
    for (int i = 1; i <= 4*row; i++)
    {
        cout<<fixed<<setprecision(2)<<*ptr2<<" ";
        ptr2++;
        if (i%4==0)
            cout<<endl;
    }
    cout<<endl;
}


bool solvedMatrix(float *ptr2, int row, float *pos)
{
    //checks to make sure if matrix is in row reduces echelon form
    int index1 = 0;
    int index2 = 0;
    //int index3 = 0;
    //int index4 = 0;
    string x, y, z;
    //move pointer to the end
    for (int i = 0; i < 4*row; i++)
    {
        ptr2++;
    }
    //check to make sure zero rows are at the bottom
    for (int i = 4*row; i > 0; i--) {
        if (i % 4 == 0) {
            if (*ptr2 == 0) {
                ptr2--;
                if (*ptr2 == 0) {
                    ptr2--;
                    if (*ptr2 == 0) {
                        ptr2--;
                        if (*ptr2 == 0)
                            index1++;
                    }
                }
            }
        } else
            ptr2--;
    }
    ptr2 = pos;
    //check to make sure leading entries are ones
    for (int i = 0; i < 4*row; ++i) {
        if (i % 4 == 0) {
            if (*ptr2 == 1) {
                ptr2++;
                if (*ptr2 == 0) {
                    ptr2++;
                    if (*ptr2 == 0) {
                        ptr2++;
                        if (*ptr2 != 0||*ptr2==0) {
                            index2++;
                        }
                    }
                }
            }
        }
        if (i % 4 == 1) {
            if (*ptr2 == 0) {
                ptr2++;
                if (*ptr2 == 1) {
                    ptr2++;
                    if (*ptr2 == 0) {
                        ptr2++;
                        if (*ptr2 != 0||*ptr2==0) {
                            index2++;
                        }
                    }
                }
            }
        }
        if (i % 4 == 2) {
            if (*ptr2 == 0) {
                ptr2++;
                if (*ptr2 == 0) {
                    ptr2++;
                    if (*ptr2 == 1) {
                        ptr2++;
                        if (*ptr2 != 0||*ptr2==0) {
                            index2++;
                        }
                    }
                }
            }
        }
        else
            ptr2++;
    }
    //depending on if certain conditions are true, the matrix is in row reduced echelon form
    if (row == 4) {
        if (index1 == 1 && index2==3)
            return true;
        if (index1 == 1 && index2 == 2)
            return true;
    }
    else if(row==3) {
        if (index1 == 1 && index2 == 3)
            return true;
        if (index1==1 && index2 == 2)
            return true;
        if (index2==3)
            return true;
    }
    else if (row==2){
        if (index1 == 0 && index2 == 2)
            return true;
        if (index1 == 0 && index2 == 1)
            return true;
    }
    return false;
}

void printVal(float *ptr2, int row, float coex, float coey, float coez)
{
    //prints out variables if matrix is in row reduced echelon form
    int index = 0;
    if (row==2)
    {
        for (int i = 0; i < 4 * row; ++i) {
            if (i % 4 == 0) {
                if (*ptr2 == 1) {
                    ptr2++;
                    if (*ptr2 == 0) {
                        ptr2++;
                        ptr2++;
                        if (*ptr2 != 0 || *ptr2==0) {
                            coex = *ptr2;
                            index++;
                        }
                    }
                }
            }
            if (i % 4 == 1) {
                if (*ptr2 == 0) {
                    ptr2++;
                    if (*ptr2 == 1) {
                        ptr2++;
                        if (*ptr2==0||*ptr2!=0){
                            ptr2++;
                            if (*ptr2 != 0 || *ptr2==0) {
                                coey = *ptr2;
                                index++;
                            }
                        }
                    }
                }
            }
            else
                ptr2++;
        }
    }
    else {
        for (int i = 0; i < 4 * row; ++i) {
            if (i % 4 == 0) {
                if (*ptr2 == 1) {
                    ptr2++;
                    if (*ptr2 == 0) {
                        ptr2++;
                        if (*ptr2 == 0) {
                            ptr2++;
                            if (*ptr2==0||*ptr2!=0) {
                                coex = *ptr2;
                                index++;
                            }
                        }
                    }
                }
            }
            if (i % 4 == 1) {
                if (*ptr2 == 0) {
                    ptr2++;
                    if (*ptr2 == 1) {
                        ptr2++;
                        if (*ptr2 == 0) {
                            ptr2++;
                            if (*ptr2==0||*ptr2!=0) {
                                coey = *ptr2;
                                index++;
                            }
                        }
                    }
                }
            }
            if (i % 4 == 2) {
                if (*ptr2 == 0) {
                    ptr2++;
                    if (*ptr2 == 0) {
                        ptr2++;
                        if (*ptr2 == 1) {
                            ptr2++;
                            if (*ptr2==0||*ptr2!=0) {
                                coez = *ptr2;
                                index++;
                            }
                        }
                    }
                }
            } else
                ptr2++;
        }
    }
    //based on certain conditions, the number for each variable prints out
    if (row==2){
        if (index==2 || index==1){
            cout<<"x = "<<coex<<endl;
            cout<<"y = "<<coey<<endl;
        }
    }
    if (row==3 || row==4) {
        if (index == 3) {
            cout << "x = " << coex << endl;
            cout << "y = " << coey << endl;
            cout << "z = " << coez << endl;
        }
        if (index==2){
            cout<<"x = "<<coex<<endl;
            cout<<"y = "<<coey<<endl;
        }
    }
}

