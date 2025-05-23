#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>
using namespace std;



string const FileName = "MyFile.txt";
void DisplayMenu();
void DisplayTransactionMenu();
enum enUserChoiceOption {
	ShowClientList = 1,
	AddNewClient = 2,
	DeleteClient = 3,
	UpdateClientInfo = 4,
	FindClient = 5,
	Transaction = 6,
	Exit = 7
};

enum enUserTransactionType {
	Deposite = 1,
	Withdraw = 2,
	Totalbalances = 3,
	MainMenu=4
};
struct stClientInfo {
	string AccountNumber;
	string Pincode;
	string ClientName;
	string Phone;
	double AccountBalance;
	bool isUpdated = false;
	bool isDeleted = false;
};

vector<string> SplitString(string line, string sperator = "#//#") {
	short pos = 0;
	string sWord;
	vector<string> vString;
	while ((pos = line.find(sperator)) != std::string::npos) {
		sWord = line.substr(0, pos);
		if (sWord != "") {
			vString.push_back(sWord);
		}
		line.erase(0, pos + sperator.length());
	}
	if (line != "") {
		vString.push_back(line);
	}
	return vString;
}

stClientInfo ConvertRecordToLine(string line) {
	stClientInfo Client;
	vector<string> vString = SplitString(line);
	//Now We have To Pass The Value of string to client 

	Client.AccountNumber = vString[0];
	Client.Pincode = vString[1];
	Client.ClientName = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

short ReadPostiveNumber() {
	short number;
	do {
		cout << "\nChoose What do You want to choose ? [1-7] ? ";
		cin >> number;
	} while (number <= 0 || number > 7);
	return number;
}

vector<stClientInfo> ReadDataFromFile() {
	fstream file;
	string line;
	stClientInfo Client;
	vector<stClientInfo> vClients;
	file.open(FileName, ios::in); // read mode
	if (file.is_open()) {
		while (getline(file, line)) {
			//Convert the line to record
			Client = ConvertRecordToLine(line);
			vClients.push_back(Client);
		}
		file.close();
	}
	return vClients;
}


void PrintClientRecord(stClientInfo Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.Pincode;
	cout << "| " << setw(40) << left << Client.ClientName;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowClientsInfo() {
	vector<stClientInfo> vClients = ReadDataFromFile();
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << "(Client(s)).";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (stClientInfo client : vClients) {
		PrintClientRecord(client);
		cout << "\n";
	}

}

void GoBackToMainMenu() {

	cout << "\n\n\n Press any key to go back to main menu ..";
	system("pause");
	DisplayMenu();

}




void HeaderAddNewClient() {
	cout << "\n--------------------------------------\n";
	cout << setw(10) << right << "Add New Clients Screen ";
	cout << "\n--------------------------------------\n";
	cout << "Adding New Client \n\n";
}


bool CheckNewClientIsExist(string clientAccountNumber) {
	fstream file;
	string line;
	stClientInfo client;
	vector<stClientInfo> vClients;
	file.open(FileName, ios::in);
	if (file.is_open()) {
		while (getline(file, line)) {
			client = ConvertRecordToLine(line);
			if (client.AccountNumber == clientAccountNumber) {
				file.close();
				return true;
			}
			vClients.push_back(client);
		}
		file.close();
		return false;
	}
}


stClientInfo CreateNewClient() {
	stClientInfo Client;
	cout << "\nEnter Account Number ? :";
	getline(cin >> ws, Client.AccountNumber);
	while (CheckNewClientIsExist(Client.AccountNumber)) {
		cout << "Client with [" << Client.AccountNumber << "] already exists,\n\n";
		cout << "Enter Another Account Number ? :";
		getline(cin >> ws, Client.AccountNumber);
	}
	cout << "\nEnter PinCode ? :";
	getline(cin, Client.Pincode);
	cout << "\nEnter Client Name ? :";
	getline(cin, Client.ClientName);
	cout << "\nEnter Phone ? :";
	getline(cin >> ws, Client.Phone);
	cout << "\nEnter Account Balance ? :";
	cin >> Client.AccountBalance;
	return Client;
}

string ConvertLineToRecord(stClientInfo client, string sperator = "#//#") {
	string stCompletedLine = client.AccountNumber + sperator
		+ client.Pincode + sperator
		+ client.ClientName + sperator
		+ client.Phone + sperator
		+ to_string(client.AccountBalance);
	return stCompletedLine;
}

void AddDataToFile(string data) {
	fstream file;
	file.open(FileName, ios::out | ios::app);
	if (file.is_open()) {
		file << data << "\n";
	}
	file.close();
}


void AddClients() {
	HeaderAddNewClient();
	char userChoice = 'n';
	stClientInfo client;
	do {
		client = CreateNewClient();
		AddDataToFile(ConvertLineToRecord(client));
		cout << "Do You Want To Add More New Clients ? :";
		cin >> userChoice;
	} while (tolower(userChoice) == 'y');
}


bool FindClientNumber(string userInputAccNumber, vector<stClientInfo>& vClients,stClientInfo& client) {
	for (stClientInfo& c : vClients) {
		if (c.AccountNumber == userInputAccNumber) {
			 client=c;
			return true;
		}
	}
	return false;
}


vector<stClientInfo> SaveNewDataToFile(vector<stClientInfo> vClients) {
	fstream file;
	stClientInfo client;
	string dataLine;
	file.open(FileName, ios::out);
	if (file.is_open()) {
		for (stClientInfo& c : vClients) {
			if (c.isDeleted == false) {
				dataLine = ConvertLineToRecord(c);
				file << dataLine << "\n";
			}
		}
		file.close();
	}
	return vClients;
}

void ShowClientCard(stClientInfo client) {
	cout << "The Following is the client details : \n";
	cout << "\n------------------------------------------------\n";
	cout << "Account Number :" << client.AccountNumber;
	cout << "\nAccount PinCode :" << client.Pincode;
	cout << "\nAccount Name :" << client.ClientName;
	cout << "\nAccount Phone :" << client.Phone;
	cout << "\nAccount Balance :" << client.AccountBalance;
	cout << "\n------------------------------------------------\n";

}

bool MarkDeleteClientFromFile(string AccountNumber, vector<stClientInfo>& vClients) {
	for (stClientInfo& client : vClients) {
		if (client.AccountNumber == AccountNumber) {
			client.isDeleted = true;
			return true;
		}
	}
	return false;
}

void HeaderDeleteMenu() {
	cout << "\n--------------------------------------\n";
	cout << setw(25) << right << "Delete Client Screen";
	cout << "\n--------------------------------------\n";

}

string ReadClientNumber() {
	string accountNumber;
	cout << "Please Enter The Account Number \n";
	getline(cin >> ws, accountNumber);
	return accountNumber;
}


bool DeleteAccountFromFile(vector<stClientInfo>& vClients, string userInputAccountNumber) {
	char userChoice = 'n';
	stClientInfo client;
	if (FindClientNumber(userInputAccountNumber, vClients, client)) {
		ShowClientCard(client);
		cout << "\n\n Are You Sure That Do You Want To Delete This Account ? :";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			MarkDeleteClientFromFile(userInputAccountNumber, vClients);
			SaveNewDataToFile(vClients);
			//Refresh Clients
			vClients = ReadDataFromFile();
			cout << "\nDelete Completed Successfully \n";
			return true;
		}
	}
	else {
		cout << "Sorry We Couldn't Find This Account [" << userInputAccountNumber << "]" << "\n";
		return false;
	}
}

bool MarkClientUpdate(vector<stClientInfo>& vClients, string userInputAccountNumber) {
	for (stClientInfo & client : vClients) {
		if (client.AccountNumber == userInputAccountNumber) {
			client.isUpdated = true;
			return true;
		}
	}
	return false;
}

void InputUpdatedData(stClientInfo &client) {
	cout << "Enter Pin Code : ";
	getline(cin >> ws, client.Pincode);

	cout << "Enter Name : ";
	getline(cin, client.ClientName);

	cout << "Enter Phone : ";
	getline(cin, client.Phone);


	cout << "Enter Balance : ";
	cin >> client.AccountBalance;

}


vector<stClientInfo> UpdateClient(vector<stClientInfo>& vClients) {
		for (stClientInfo& c : vClients) {
			if (c.isUpdated == true) {
				InputUpdatedData(c);
			}
		}
	return vClients;
}


void SaveUpdatedDataToFile(vector<stClientInfo>& vClients) {
	fstream file;
	string dataLine;
	file.open(FileName, ios::out);
	if (file.is_open()) {
		for (stClientInfo& client : vClients) {
			dataLine = ConvertLineToRecord(client);
			file << dataLine << "\n";
		}
	}
	file.close();
}

bool UpdateNewClient(vector<stClientInfo>& vClients, string userInputAccountNumber) {
	char userChoice = 'n';
	stClientInfo client;
	if (FindClientNumber(userInputAccountNumber, vClients, client)) {
		ShowClientCard(client);
		cout << "Are You Sure That Do You Want to update this account ? :";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			MarkClientUpdate(vClients, userInputAccountNumber);
			UpdateClient(vClients);
			SaveUpdatedDataToFile(vClients);

			//Refresh
			vClients = ReadDataFromFile();

			cout << "Successfully Updated \n";
			return true;
		}
	}
	else {
		cout << "Sorry We Couldn't Find This Account [" << userInputAccountNumber << "]" << "\n";
	}
	return false;
}


void DisplayUpdateHeader() {
	cout << "\n\n-------------------------------------------------\n";
	cout << setw(35) << right << "Update Client Screen \n";
	cout << "-------------------------------------------------\n";
}

void ShowUpdateMenu() {
	DisplayUpdateHeader();
	vector<stClientInfo> vClients = ReadDataFromFile();
	string userInputAccountNumber = ReadClientNumber();
	UpdateNewClient(vClients, userInputAccountNumber);
}


void ShowDeleteMenu() {
	HeaderDeleteMenu();
	vector<stClientInfo> vClients = ReadDataFromFile();
	string userInputAccountNumber = ReadClientNumber();
	DeleteAccountFromFile(vClients, userInputAccountNumber);
}

void DisplayFindClientHeader() {
	cout << "\n\n------------------------------------\n";
	cout << setw(30) << right << "Find Client Screen \n";
	cout << "------------------------------------\n";
}

void ShowFindClientMenu() {
	DisplayFindClientHeader();
	stClientInfo client;
	vector<stClientInfo> vClients = ReadDataFromFile();
	string userInputAccountNumber = ReadClientNumber();
	if (FindClientNumber(userInputAccountNumber, vClients, client)) {
		ShowClientCard(client);
	}
	else {
		cout << "Sorry ,We Couldn't Find this account [" << userInputAccountNumber << "] \n";
	}
}

void DisplayExitScreen() {
	cout << "\n\n-----------------------------------\n";
	cout << setw(25) << right << "Program Ends :-)\n";
	cout << "-----------------------------------\n";
}





short UserInputForTransactionType() {
	short number;
	do {
		cout << "Choose What Do You Want To Do ? [1 - 4] ?";
		cin >> number;
	} while (number < 1 || number > 4);
	return number;
}



void UploadDepositeToFile(vector<stClientInfo>& vClients) {
	fstream file;
	string dataLine;
	file.open(FileName,ios::out);
	if (file.is_open()) {
		for (stClientInfo & client : vClients) {
			dataLine=ConvertLineToRecord(client);
			file << dataLine << "\n";
		}
		file.close();
	}
}

void AddDeposite(vector<stClientInfo>& vClients,double amount,string accountNumber) {
	char userChoice='n';
	cout << "Are You sure you want perform this transaction ? Y/N?";
	cin >> userChoice;

	if (tolower(userChoice) == 'y') {
	for (stClientInfo& c : vClients) {
		if (c.AccountNumber==accountNumber) {
			c.AccountBalance += amount;
			SaveUpdatedDataToFile(vClients);
			cout << "Done Successfully with thie transaction \n";
		}
	}
}

}

void DisplayHeaderDeposite() {
	cout << "\n---------------------------------------\n";
	cout << setw(15) << right << "Deposite Screen";
	cout << "\n---------------------------------------\n";
}

void MakeDeposite() {
	DisplayHeaderDeposite();
	vector<stClientInfo> vClients = ReadDataFromFile();
	stClientInfo client;
	string userInputAccountNumber;
	double depositeAmount;
	bool isFound;
	do {
		cout << "Please Enter Account Number ? :";
		cin >> userInputAccountNumber;
		isFound = FindClientNumber(userInputAccountNumber, vClients, client);
		if (!isFound) {
			cout << "Client With  [" << userInputAccountNumber << "] doesnot exist \n";
		}
	} while (!isFound);	
	if (isFound) {
		ShowClientCard(client);
		cout << "Please Enter Deposite Number ? :";
		cin >> depositeAmount;
		AddDeposite(vClients, depositeAmount,userInputAccountNumber);
	}
}


void GoBackToMainMenuTransaction() {
	cout << "\n\n\n Press any key to go back to main menu ..\n";
	system("pause");
	DisplayTransactionMenu();
}

void PrintBalancesOptionInfo(stClientInfo client) {
	cout << "| " << setw(15) << left << client.AccountNumber;
	cout << "| " << setw(40) << left << client.ClientName;
	cout << "| " << setw(12) << left << client.AccountBalance;
}


void TotalBalancesOption() {
	vector <stClientInfo> vClients = ReadDataFromFile();
	double totalBalances = 0;
	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "__________________________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "__________________________________________________________\n" << endl;
	if (vClients.size() == 0) {
		cout << "\t\t\t\t\t There is no clients yet";
	}
	else {
		for (stClientInfo& c : vClients) {
			PrintBalancesOptionInfo(c);
			totalBalances += c.AccountBalance;
			cout << endl;
		}
		cout << "\n_______________________________________________________";
		cout << "__________________________________________________________\n" << endl;
		cout << "\t\t\t\t\t Total Balances = " << totalBalances << "\n";
	}
	
}



void DisplayHeaderWithdraw() {
	cout << "\n---------------------------------------\n";
	cout << setw(15) << right << "Withdraw Screen";
	cout << "\n---------------------------------------\n";
}

double CheckWithdraw(vector<stClientInfo>& vClients,string userAccountNumber) {
	double withdrawAmount;
	cout << "Please enter withdraw amount ? :";
	cin >> withdrawAmount;
	for (stClientInfo& c : vClients) {
		if (userAccountNumber == c.AccountNumber) {
			while (c.AccountBalance < withdrawAmount) {
				cout << "Amount Exceeds the balance ,You can withdraw up to :" << c.AccountBalance << "\n";
				cout << "Please Enter another amount \n";
				cin >> withdrawAmount;
			}
		}
		return withdrawAmount;
	}

}

void MakeWithdraw() {
	DisplayHeaderWithdraw();
	vector<stClientInfo> vClients = ReadDataFromFile();
	stClientInfo client;
	string userInputAccountNumber;
	double withdrawAmount;
	bool isFound;
	do {
		cout << "Please Enter Account Number ? :";
		cin >> userInputAccountNumber;
		isFound = FindClientNumber(userInputAccountNumber, vClients, client);
		if (!isFound) {
			cout << "Client With  [" << userInputAccountNumber << "] doesnot exist \n";
		}
	} while (!isFound);
	if (isFound) {
		ShowClientCard(client);
		withdrawAmount= CheckWithdraw(vClients,userInputAccountNumber);
		AddDeposite(vClients,withdrawAmount*-1,userInputAccountNumber);
	}
}


void CheckUserInputTransactionType(short userInput) {
	switch ((enUserTransactionType)userInput)
	{
	case enUserTransactionType::Deposite:
		system("cls");
		MakeDeposite();
		GoBackToMainMenuTransaction();
		break;

	case enUserTransactionType::Withdraw:
		system("cls");
		MakeWithdraw();
		GoBackToMainMenuTransaction();
		break;


	case enUserTransactionType::Totalbalances:
		system("cls");
		TotalBalancesOption();
		GoBackToMainMenuTransaction();
		break;

	case enUserTransactionType::MainMenu:
		system("cls");
		DisplayMenu();
		break;


	default:
		break;
	}
}

void DisplayTransactionMenu() {
	system("cls");
	cout << "=====================================================\n";
	cout << setw(40) << right << "Transaction Menu Screen \n";
	cout << "=====================================================\n";
	cout << "\t[1] Deposite.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "===========================================\n";
	CheckUserInputTransactionType(UserInputForTransactionType());
}







void ChechUserOption(short userOption) {
	switch ((enUserChoiceOption)userOption)
	{
	case enUserChoiceOption::ShowClientList:
		system("cls");
		ShowClientsInfo();
		GoBackToMainMenu();
		break;

	case enUserChoiceOption::AddNewClient:
		system("cls");
		AddClients();
		GoBackToMainMenu();
		break;


	case enUserChoiceOption::UpdateClientInfo:
		system("cls");
		ShowUpdateMenu();
		GoBackToMainMenu();
		break;


	case enUserChoiceOption::DeleteClient:
		system("cls");
		ShowDeleteMenu();
		GoBackToMainMenu();
		break;



	case enUserChoiceOption::FindClient:
		system("cls");
		ShowFindClientMenu();
		GoBackToMainMenu();
		break;

	case enUserChoiceOption::Transaction:
		system("cls");
		DisplayTransactionMenu();
		break;

	case enUserChoiceOption::Exit:
		system("cls");
		DisplayExitScreen();
		break;

	default:
		break;
	}

}





void DisplayMenu() {
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";
	ChechUserOption((enUserChoiceOption)ReadPostiveNumber());

}





int main() {
	DisplayMenu();

	system("pause>0");

}