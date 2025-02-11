//#include <iostream>
//#include <fstream>
//#include <filesystem>
//#include <string>
//#include <vector>
//#include <algorithm>
//#include <windows.h>
//#include <cctype>
//#include <locale>
//#include <cwchar>
//
//using namespace std;
//namespace fs = std::filesystem;
//using namespace fs;
//
//class Printer
//{
//public:
//    string addressId;
//    string model;
//    string address;
//    string id;
//    string shopId;
//    int num = 0;
//
//    Printer(const string& addressId, const string& shopId, const string& address, const string& id, const string& model, int num)
//        : addressId(addressId), shopId(shopId), address(address), id(id), model(model), num(num) {}
//};
//
//
////находим первую(-ые) буквы в коде населенного пункта (первыую берем, а потом пока не появится цифра)
//string findAddressId(const string& address) {
//    string result;
//    wstring wide_address(address.begin(), address.end());
//    result += wide_address[0];
//
//    for (int i = 1; i < wide_address.length(); i++) {
//        if (iswdigit(wide_address[i]) || iswspace(wide_address[i])) {
//            break; 
//        }
//        result += wide_address[i]; 
//    }
//
//    return result;
//}
//
//vector<string> split(const string& str, char delimiter) {
//    vector<string> tokens;
//    size_t start = 0;
//    size_t end = str.find(delimiter);
//
//    if (str.empty()) {
//        return tokens; 
//    }
//
//    while (end != string::npos) {
//        tokens.push_back(str.substr(start, end - start)); 
//        start = end + 1;  
//        end = str.find(delimiter, start);  
//    }
//    tokens.push_back(str.substr(start));
//
//    return tokens;
//}
//
//int findPrinter(vector<Printer> data, string id, string address) {
//    for (int i = 0; i < data.size(); i++) {
//        if (data[i].id == id && data[i].address == address) {
//            return i;
//        }
//    }
//    return -1;
//}
//
//void readFile(string filename, vector<Printer>& data) {
//    ifstream file(filename);
//    vector<Printer> printers;
//    if (file.is_open()) {
//        string line;
//        while (getline(file, line)) {
//            vector<string> splitLine = split(line, '*');
//            if (splitLine.size() == 4) {
//                int printerNum = stoi(splitLine[splitLine.size() - 1]);
//                string searchId = splitLine[1];
//                int index = findPrinter(data, searchId, splitLine[0]);
//                if (index != -1 && data[index].num < printerNum) {
//                    data[index].num = printerNum;
//                }
//                else if (index == -1) {
//                    Printer newPrinter(findAddressId(splitLine[0]), split(splitLine[0], ' ')[0], splitLine[0], searchId, splitLine[2], printerNum);
//                    data.push_back(newPrinter);
//                }
//            }
//        }
//        file.close();
//    }
//    else {
//        cerr << "Unable to open file: " << filename << endl;
//    }
//}
//
//void writeFile(vector<Printer> data) {
//    ofstream outFile("result.csv");
//
//    if (!outFile.is_open()) {
//        cerr << "Error opening file for writing." << endl;
//        return; 
//    }
//    
//    outFile << "Код региона;Номер магазина;Адрес;СН принтера;Модель принтера;Количество копий" << endl;
//
//    for (Printer p : data) { 
//        outFile << p.addressId << ';' << p.shopId << ';' << p.address << ';' << p.id << ';' << p.model << ';' << p.num << endl;
//    }
//    outFile.close();
//    cout << "CSV file created successfully: result.csv " << endl;
//
//}
//
//int main() {
//    wcout.imbue(locale("ru_RU.UTF-8"));
//
//    path current_dir = current_path();
//    vector<Printer> data;
//    bool filesFound = false;
//    cout << "Files are being processed..." << endl;
//
//    for (const auto& entry : directory_iterator(current_dir)) {
//        if (is_regular_file(entry.status()) && entry.path().filename().string().find("Printers") == string::npos && entry.path().filename().string().find("result") == string::npos) {
//            string filename = entry.path().string();
//            readFile(filename, data);
//            filesFound = true;
//        }
//    }
//    if (!filesFound) {
//        cout << "No files were found for processing." << endl;
//    }
//    else {
//        writeFile(data);
//    }
//
//    return 0;
//}


#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <windows.h>
#include <cctype>
#include <locale>
#include <cwchar>

using namespace std;
namespace fs = std::filesystem;
using namespace fs;

class Printer
{
public:
    string addressId;
    string model;
    string address;
    string id;
    string shopId;
    int num = 0;

    Printer(const string& addressId, const string& shopId, const string& address, const string& id, const string& model, int num)
        : addressId(addressId), shopId(shopId), address(address), id(id), model(model), num(num) {}
};

//находим первую(-ые) буквы в коде населенного пункта (первыую берем, а потом пока не появится цифра)
string findAddressId(const string& address) {
    string result;
    wstring wide_address(address.begin(), address.end());
    result += wide_address[0];

    for (int i = 1; i < wide_address.length(); i++) {
        if (iswdigit(wide_address[i]) || iswspace(wide_address[i])) {
            break;
        }
        result += wide_address[i];
    }

    return result;
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    if (str.empty()) {
        return tokens;
    }

    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));

    return tokens;
}

void readFile(string filename, unordered_map<string, vector<Printer>>& data) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> splitLine = split(line, '*');
            if (splitLine.size() == 4) {
                int printerNum = stoi(splitLine[splitLine.size() - 1]);
                vector<Printer> printers = data[splitLine[1]];
                if (printers.size() == 0) {
                    Printer newPrinter(findAddressId(splitLine[0]), split(splitLine[0], ' ')[0], splitLine[0], splitLine[1], splitLine[2], printerNum);
                    data[splitLine[1]].push_back(newPrinter);
                }
                else {
                    bool found = false;
                    for (Printer p : printers) {
                        if (p.address == splitLine[0]) {
                            if (p.num < printerNum) {
                                p.num = printerNum;
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        Printer newPrinter(findAddressId(splitLine[0]), split(splitLine[0], ' ')[0], splitLine[0], splitLine[1], splitLine[2], printerNum);
                        data[splitLine[1]].push_back(newPrinter);
                    }
                }
            }
        }
        file.close();
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

void writeFile(unordered_map<string, vector<Printer>> data) {
    ofstream outFile("result.csv");

    if (!outFile.is_open()) {
        cerr << "Error opening file for writing." << endl;
        return; 
    }
    
    outFile << "Код региона;Номер магазина;Адрес;СН принтера;Модель принтера;Количество копий" << endl;

    for (const auto& pair: data) { 
        for (Printer p : pair.second) {
            outFile << p.addressId << ';' << p.shopId << ';' << p.address << ';' << p.id << ';' << p.model << ';' << p.num << endl;
        }
    }
    outFile.close();
    cout << "CSV file created successfully: result.csv " << endl;

}

int main() {
    wcout.imbue(locale("ru_RU.UTF-8"));

    path current_dir = current_path();
    unordered_map<string, vector<Printer>> data;
    bool filesFound = false;
    cout << "Files are being processed..." << endl;

    for (const auto& entry : directory_iterator(current_dir)) {
        if (is_regular_file(entry.status()) && entry.path().filename().string().find("Printers") == string::npos && entry.path().filename().string().find("result") == string::npos) {
            string filename = entry.path().string();
            readFile(filename, data);
            filesFound = true;
        }
    }
    if (!filesFound) {
        cout << "No files were found for processing." << endl;
    }
    else {
        writeFile(data);
    }

    return 0;
}