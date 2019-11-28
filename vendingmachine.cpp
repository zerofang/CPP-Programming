#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

const int CAPACITY = 40;
const int KIND_NUM = 6;

class Beverage;
class VendingMachine;

class Beverage{
    private:
        int no;
        string name;
        double price;
        int stock;
        bool isOptional;
        bool isSoldOut;

    public:
        Beverage(int _no, string _name, double _price, int _stock):no(_no), name(_name), price(_price), stock(_stock){
            isOptional = false;
            if (stock > 0) {
                isSoldOut = false;
            } else {
                isSoldOut = true;
            }
        }
        int getNo(){
            return no;
        }
        string getName(){
            return name;
        }
        void setName(string _name){
            name = _name;
        }
        double getPrice(){
            return price;
        }
        void setPrice(double _price){
            price = _price;
        }
        int getStock(){
            return stock;
        }
        void setStock(int _stock){
            stock = _stock;
        }
        bool getIsOptional(){
            return isOptional;
        }
        void setIsOptional(bool _isOptional){
            isOptional = _isOptional;
        }
        bool getIsSoldOut(){
            return isSoldOut;
        }
        void setIsSoldOut(bool _isSoldOut){
            isSoldOut = _isSoldOut;
        }
        void sold(){
            stock--;
            if (stock <= 0) {
                isSoldOut = true;
            }
        }
};

class VendingMachine{
    private:
        int capacity;
        double income;
        double coin;
        Beverage *beverages[KIND_NUM];

    public:
        static string beverageNames[KIND_NUM];
        static double initialPrices[KIND_NUM];
        static int initialStock[KIND_NUM];
        static void changeBeverageNames(){
            printf("please input 6 beverage names(split by space or enter):\n");
            for (int i = 0; i < KIND_NUM; i ++) {
                cin >> beverageNames[i];
            }
        }
        static string getLightColor(Beverage* b){
            if (b->getIsSoldOut()) {
                return "red";
            }
            if (b->getIsOptional()) {
                return "green";
            }
            return "white";
        }
        VendingMachine(int _capacity = CAPACITY):capacity(_capacity){
            income = 0;
            coin = 0;
            for (int i = 0; i < KIND_NUM; i++) {
                beverages[i] = new Beverage(i, beverageNames[i], initialPrices[i], initialStock[i]);
            }
            printCMD();
        }
        void printCMD(){
            cout<<"--------------------------------------------VendingMachine--------------------------------------------"<<endl;
            for (int i = 0; i < KIND_NUM; i++){
                cout << "|";
                cout.width(15);
                cout << beverages[i]->getNo();
                cout.width(0);
                cout << "|";
            }
            cout << endl;
            for (int i = 0; i < KIND_NUM; i++){
                cout << "|";
                cout.width(15);
                cout << "$"+to_string(beverages[i]->getPrice());
                cout.width(0);
                cout << "|";
            }
            cout << endl;
            for (int i = 0; i < KIND_NUM; i++){
                cout << "|";
                cout.width(15);
                cout << beverages[i]->getName();
                cout.width(0);
                cout << "|";
            }
            cout << endl;
            for (int i = 0; i < KIND_NUM; i++){
                cout << "|";
                cout.width(15);
                cout << getLightColor(beverages[i]);
                cout.width(0);
                cout << "|";
            }
            cout << endl;
            cout<<"--------------------------------------------VendingMachine--------------------------------------------"<<endl;
            cout << "-------------------Lights Color  red:Sold Out  green:Selectable  wihte:Not Selectable-----------------" << endl;
            cout << "--------------------INPUT  1:Drop Coin  2:Refund  3:Choose Beverage  4:Seller's Menu------------------" << endl;
            int option;
            cin >> option;
            switch (option) {
                case 1:
                    dropCoin();
                    break;
                case 2:
                    refund();
                    break;
                case 3:
                    chooseVeverage();
                    break;
                case 4:
                    enterOwnerMode();
                    break;
                default:
                    cout << "Illegal Input!" << endl;
                    printCMD();
            }
        }

        void resetLights() {
            for (int i = 0; i < KIND_NUM; i++) {
                beverages[i]->setIsOptional(false);
                if (beverages[i]->getStock() <= 0) {
                    beverages[i]->setIsSoldOut(true);
                } else {
                    beverages[i]->setIsSoldOut(false);
                }
            }
        }

        void dropCoin(){
            printf("Please enter the coin amount: ");
            cin >> coin;
            for (int i = 0; i < KIND_NUM; i++) {
                if (!(beverages[i]->getIsSoldOut()) && beverages[i]->getPrice() <= coin) {
                    beverages[i]->setIsOptional(true);
                }
            }
            printCMD();
        }

        void refund(){
            if (coin==0) {
                cout << "No coin, refund is invalid" << endl;
                return;
            }
            cout << "Return you $" << coin << ", please check" << endl;
            coin = 0;
            resetLights();
            printCMD();
        }

        void chooseVeverage(){
            int no;
            cout << "Please enter the beverage number: ";
            cin >> no;
            if (getLightColor(beverages[no]) == "green") {
                cout << "You successfully purchased a bottle of " << beverages[no]->getName();
                beverages[no]->sold();
                coin -= beverages[no]->getPrice();
                income += beverages[no]->getPrice();
                if (coin > 0) {
                    cout << "Return you $" << coin << ", please check" << endl;
                    coin = 0;
                }
                resetLights();
            }
            else if(getLightColor(beverages[no]) == "red") {
                cout << "Sorry, this beverage is sold out" << endl;
            }
            else {
                cout << "Sorry, the coin amount is insufficient" << endl;
            }
            printCMD();
        }

        void enterOwnerMode(){
            cout << "INPUT  1: Adjust beverage type | 2: Adjust beverage price | 3: Adjust stock | 4: Inquiry income | 5: Return to user mode" << endl;
            int option;
            cin >> option;
            switch (option) {
                case 1:
                    changeBeverage();
                    break;
                case 2:
                    changeBeveragePrices();
                    break;
                case 3:
                    changeBeverageStock();
                    break;
                case 4:
                    inquiryIncome();
                    break;
                case 5:
                    printCMD();
                    break;
                default:
                    cout << "Illegal Input!" << endl;
                    enterOwnerMode();
            }
        }

        void changeBeverage() {
            changeBeverageNames();
            for (int i = 0; i < KIND_NUM; i++) {
                delete &beverages[i];
                Beverage b(i, beverageNames[i], initialPrices[i], initialStock[i]);
                beverages[i] = &b;
            }
            enterOwnerMode();
        }

        void changeBeveragePrices(){
            cout << "Please enter the new price of the 6 beverages in the order of the beverage number(split by space or enter): " << endl;
            for (int i = 0; i < KIND_NUM; i++){
                double p;
                cin >> p;
                beverages[i]->setPrice(p);
            }
            enterOwnerMode();
        }

        void changeBeverageStock(){
            cout << "Please enter the new stock quantity of 6 beverages in the order of beverage number(split by space or enter): " << endl;
            int s[KIND_NUM];
            int sum = 0;
            for (int i = 0; i < KIND_NUM; i++){
                cin >> s[i];
                sum += s[i];
            }
            if (sum > capacity) {
                cout << "Sorry! Total beverage stock quantity exceeds limit (" << capacity << ")" << endl;
            } else {
                for (int i = 0; i < KIND_NUM; i++) {
                    beverages[i]->setStock(s[i]);
                }
            }
            resetLights();
            enterOwnerMode();
        }

        void inquiryIncome(){
            cout << "This machine earns $ " << income << endl;
            cout << "Do you want to take the money? INPUT  1: yes  2: no" << endl;
            int option;
            cin >> option;
            switch (option) {
                case 1:
                    cout << "You got $ " << income << endl;
                    income = 0;
                    break;
                case 2:
                    break;
                default:
                    cout << "Illegal Input!" << endl;
            }
            enterOwnerMode();
        }
};

string VendingMachine::beverageNames[KIND_NUM] = {"coke", "soda", "coffee", "tea", "juice", "lemonade"};
double VendingMachine::initialPrices[KIND_NUM] = {2.5, 3, 5, 3, 5, 4};
int VendingMachine::initialStock[KIND_NUM] = {6, 7, 6, 7, 6, 6};

int main(){
    VendingMachine v;
    return 0;
}