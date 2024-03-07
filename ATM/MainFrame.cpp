#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include "MainFrame.h"
#include "Account.h"
#include "Transaction.h"


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	BindHomeEventHandlers();
	BindLoginEventHandlers();
	BindRegisterEventHandlers();
	BindMenuEventHandlers();
	BindAdminMenuEventHandlers();
}

void MainFrame::CreateControls()
{
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 18)));

	//START PANEL
	homePanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));
	homePanel->SetFont(mainFont);
	headlineText = new wxStaticText(homePanel, wxID_ANY, "Bank", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headlineText->SetFont(headlineFont);
	
	chooseLoginBtn = new wxButton(homePanel, wxID_ANY, "Logowanie", wxPoint(300,200), wxSize(200,70));
	chooseRegisterBtn = new wxButton(homePanel, wxID_ANY, "Rejestracja", wxPoint(300, 300), wxSize(200, 70));

	//LOGIN PANEL
	loginPanel = new wxPanel(this, wxID_ANY, wxPoint(0,0), wxSize(800,600));
	headlineText = new wxStaticText(loginPanel, wxID_ANY, "Logowanie", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	loginPanel->SetFont(mainFont);
	headlineText->SetFont(headlineFont);
	
	loginPanelText = new wxStaticText(loginPanel, wxID_ANY, "Login:", wxPoint(250, 145), wxSize(300, -1));
	loginInputLogin = new wxTextCtrl(loginPanel, wxID_ANY, "", wxPoint(250, 170), wxSize(300, -1));

	loginPanelText = new wxStaticText(loginPanel, wxID_ANY, "Has³o:", wxPoint(250, 215), wxSize(300, -1));
	loginInputPassword = new wxTextCtrl(loginPanel, wxID_ANY, "", wxPoint(250, 240), wxSize(300, -1), wxTE_PASSWORD);
	
	loginBtn = new wxButton(loginPanel, wxID_ANY, "Zaloguj siê", wxPoint(250, 380), wxSize(300, 70));
	loginBackBtn = new wxButton(loginPanel, wxID_ANY, "Wróæ", wxPoint(50, 500), wxSize(100, 50));

	
	//REGISTER PANEL
	registerPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));
	headlineText = new wxStaticText(registerPanel, wxID_ANY, "Rejestracja", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	registerPanel->SetFont(mainFont);
	headlineText->SetFont(headlineFont);

	registerPanelText = new wxStaticText(registerPanel, wxID_ANY, "Login:", wxPoint(250, 145), wxSize(300, -1));
	registerInputLogin = new wxTextCtrl(registerPanel, wxID_ANY, "", wxPoint(250, 170), wxSize(300, -1));

	registerPanelText = new wxStaticText(registerPanel, wxID_ANY, "Has³o:", wxPoint(250, 215), wxSize(300, -1));
	registerInputPassword = new wxTextCtrl(registerPanel, wxID_ANY, "", wxPoint(250, 240), wxSize(300, -1), wxTE_PASSWORD);
	
	registerPanelText = new wxStaticText(registerPanel, wxID_ANY, "Wp³ata:", wxPoint(250, 285), wxSize(300, -1));
	registerInputDeposit = new wxTextCtrl(registerPanel, wxID_ANY, "0", wxPoint(250, 310), wxSize(300, -1));
	
	registerBtn = new wxButton(registerPanel, wxID_ANY, "Zarejestruj siê", wxPoint(250, 380), wxSize(300, 70));
	registerBackBtn = new wxButton(registerPanel, wxID_ANY, "Wróæ", wxPoint(50, 500), wxSize(100, 50));

	//MAIN USER MENU PANEL
	menuPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));
	headlineMenuText = new wxStaticText(menuPanel, wxID_ANY, "Witaj U¿ytkowniku", wxPoint(0, 40), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	menuPanel->SetFont(mainFont);
	headlineMenuText->SetFont(headlineFont);

	logoutBtn = new wxButton(menuPanel, wxID_ANY, "Wyloguj", wxPoint(10, 10), wxSize(80, -1));
	moneyBalance = new wxStaticText(menuPanel, wxID_ANY, "0", wxPoint(0, 100), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	depositBtn = new wxButton(menuPanel, wxID_ANY, "Wp³aæ", wxPoint(175, 150), wxSize(150, 50));
	withdrawBtn = new wxButton(menuPanel, wxID_ANY, "Wyp³aæ", wxPoint(475, 150), wxSize(150, 50));

	AmountWindow = new wxTextCtrl(menuPanel, wxID_ANY, "0", wxPoint(225, 160), wxSize(150, -1));
	depositAcceptBtn = new wxButton(menuPanel, wxID_ANY, "Wp³aæ", wxPoint(425, 160), wxSize(150, -1));
	AmountWindow->Hide();
	depositAcceptBtn->Hide();

	withdrawAcceptBtn = new wxButton(menuPanel, wxID_ANY, "Wyp³aæ", wxPoint(425, 160), wxSize(150, -1));
	withdrawAcceptBtn->Hide();
	transactionList = new wxListCtrl(menuPanel, wxID_ANY, wxPoint(200, 300), wxSize(400, 300), wxLC_REPORT);
	transactionList->InsertColumn(0, "Typ Transakcji", wxLIST_FORMAT_LEFT, 200);
	transactionList->InsertColumn(1, "Kwota", wxLIST_FORMAT_RIGHT, 100);
	backBtn = new wxButton(menuPanel, wxID_ANY, "<", wxPoint(100, 160), wxSize(50, -1));
	backBtn->Hide();

	//ADMIN PANEL
	adminMenuPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));
	adminLogoutBtn = new wxButton(adminMenuPanel, wxID_ANY, "Wyloguj", wxPoint(10, 10), wxSize(100, -1));
	headlineAdminMenuText = new wxStaticText(adminMenuPanel, wxID_ANY, "Admin Panel", wxPoint(0, 40), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	adminMenuPanel->SetFont(mainFont);
	headlineAdminMenuText->SetFont(headlineFont);

	
	usersList = new wxListCtrl(adminMenuPanel, wxID_ANY, wxPoint(220, 150), wxSize(400, 450), wxLC_REPORT);
	usersList->InsertColumn(0, "Login", wxLIST_FORMAT_LEFT, 200);
	usersList->InsertColumn(1, "Balans", wxLIST_FORMAT_LEFT, 200);
	editBtn = new wxButton(adminMenuPanel, wxID_ANY, "Edytuj", wxPoint(635, 150), wxSize(150, 50));
	deleteBtn = new wxButton(adminMenuPanel, wxID_ANY, "Usuñ", wxPoint(635, 210), wxSize(150, 50));


	homePanel->Hide();
	loginPanel->Hide();
	registerPanel->Hide();
	menuPanel->Hide();
	//adminMenuPanel->Hide();

}

// BINDS
void MainFrame::BindHomeEventHandlers()
{
	chooseRegisterBtn->Bind(wxEVT_BUTTON, &MainFrame::ShowRegisterPanel, this);
	chooseLoginBtn->Bind(wxEVT_BUTTON, &MainFrame::ShowLoginPanel, this);
}

void MainFrame::BindLoginEventHandlers()
{
	loginBtn->Bind(wxEVT_BUTTON, &MainFrame::loginAccount, this);
	loginBackBtn->Bind(wxEVT_BUTTON, &MainFrame::ShowHomePanel, this);
}

void MainFrame::BindRegisterEventHandlers() {
	registerBtn->Bind(wxEVT_BUTTON, &MainFrame::createAccount, this);
	registerBackBtn->Bind(wxEVT_BUTTON, &MainFrame::ShowHomePanel, this);
}

void MainFrame::BindMenuEventHandlers()
{
	logoutBtn->Bind(wxEVT_BUTTON, &MainFrame::logout, this);
	depositBtn->Bind(wxEVT_BUTTON, &MainFrame::ShowDepositMenu, this);
	withdrawBtn->Bind(wxEVT_BUTTON, &MainFrame::ShowWithdrawMenu, this);
	backBtn->Bind(wxEVT_BUTTON, &MainFrame::HideDepWitMenu, this);
	depositAcceptBtn->Bind(wxEVT_BUTTON, &MainFrame::depositMoney, this);
	withdrawAcceptBtn->Bind(wxEVT_BUTTON, &MainFrame::withdrawMoney, this);
}

void MainFrame::BindAdminMenuEventHandlers()
{
	adminLogoutBtn->Bind(wxEVT_BUTTON, &MainFrame::logout, this);
	usersList->Bind(wxEVT_LIST_ITEM_SELECTED, &MainFrame::selectUser, this);
	editBtn->Bind(wxEVT_BUTTON, &MainFrame::editUser, this);
	deleteBtn->Bind(wxEVT_BUTTON, &MainFrame::deleteUser, this);
}
// BINDS____END


void MainFrame::selectUser(wxListEvent& event)
{
	selectedItemIndex = event.GetIndex();
	
}

void MainFrame::editUser(wxCommandEvent& event)
{

	if (selectedItemIndex != -1) {
	
		wxMessageBox("edit" + std::to_string(selectedItemIndex));
		ClearUsersHistory();
		ShowUsersHisory();
		selectedItemIndex = -1;
	}
}

void MainFrame::deleteUser(wxCommandEvent& event)
{
	if (selectedItemIndex != -1) {
		wxMessageBox("delete" + std::to_string(selectedItemIndex));
		ClearUsersHistory();
		ShowUsersHisory();
		selectedItemIndex = -1;
	}
}

void MainFrame::ShowUsersHisory()
{
	std::vector<Account> users = stdAccount.returnVectorOfAccounts();
	for ( auto& user : users) {
		if (user.getRole() == "admin") continue;
		long index = usersList->InsertItem(usersList->GetItemCount(), user.getLogin());
		usersList->SetItem(index, 1, wxString::Format("%.2f", user.getBalance()));
	}
}

void MainFrame::ClearUsersHistory()
{
	usersList->DeleteAllItems();
}

void MainFrame::ClearTransationsHistory()
{
	transactionList->DeleteAllItems();
}

// ACCOUNT_MANAGEMENT
void MainFrame::ShowRegisterPanel(wxCommandEvent& event)
{
	homePanel->Hide();
	registerPanel->Show();
}

void MainFrame::ShowLoginPanel(wxCommandEvent& event)
{
	homePanel->Hide();
	loginPanel->Show();
}

void MainFrame::ShowHomePanel(wxCommandEvent& event)
{
	homePanel->Show();
	ClearInputs();
	loginPanel->Hide();
	registerPanel->Hide();
}

void MainFrame::ShowDepositMenu(wxCommandEvent& event)
{
	backBtn->Show();
	AmountWindow->Show();
	depositAcceptBtn->Show();
	depositBtn->Hide();
	withdrawBtn->Hide();
}

void MainFrame::ShowWithdrawMenu(wxCommandEvent& event)
{
	backBtn->Show();
	AmountWindow->Show();
	withdrawAcceptBtn->Show();
	depositBtn->Hide();
	withdrawBtn->Hide();
}

void MainFrame::HideDepWitMenu(wxCommandEvent& event)
{
	backBtn->Hide();
	withdrawAcceptBtn->Hide();
	AmountWindow->Hide();
	depositAcceptBtn->Hide();
	depositBtn->Show();
	withdrawBtn->Show();
	ClearDepWitInputs();
}

void MainFrame::ClearInputs()
{
	loginInputLogin->Clear();
	loginInputPassword->Clear();
	registerInputLogin->Clear();
	registerInputPassword->Clear();
	registerInputDeposit->SetValue("0");
}

void MainFrame::ClearDepWitInputs()
{
	AmountWindow->SetValue("0");
}

void MainFrame::ShowTransactionsHisory()
{
	std::vector<Transaction> transactions = stdAccount.returnVectorOfTransactions();
	for (int i = transactions.size() - 1; i >= 0; i--) {
		Transaction transaction = transactions[i];
		wxString type = wxString::Format("%s", transaction.getType().c_str());
		wxString amount = wxString::Format("%.2f", transaction.getAmount());

		long index = transactionList->InsertItem(transactionList->GetItemCount(), type);
		transactionList->SetItem(index, 1, amount);
	}
}



bool MainFrame::isCurrencyAmount(const std::string& input){
	std::regex pattern(R"(\d+\.\d{2}[0]*)");
	return std::regex_match(input, pattern);
}

bool MainFrame::isLoginPassword(const std::string& input)
{
	std::regex pattern("^[^\\s]{3,}$");
	return std::regex_match(input, pattern);
}

void MainFrame::createAccount(wxCommandEvent& event)
{
	wxString login = registerInputLogin->GetValue();
	wxString password = registerInputPassword->GetValue();
	std::string loginStr = std::string(login.mb_str());
	std::string passwordStr = std::string(password.mb_str());
	wxString initialDeposit = registerInputDeposit->GetValue();
	double initialDouble;
	
	if (!isLoginPassword(loginStr)) {
		wxMessageBox("Login sk³ada siê co najmniej z 3 znaków i nie zawiera spacji.");
		return;
	}
	if (!isLoginPassword(passwordStr)) {
		wxMessageBox("Has³o sk³ada siê co najmniej z 3 znaków i nie zawiera spacji.");
		return;
	}

	if (initialDeposit.ToDouble(&initialDouble)) {
		if (initialDouble < 0) {
			wxMessageBox("Wp³ata nie mo¿e byæ ujemna.");
			return;
		}
		if (!isCurrencyAmount(std::to_string(initialDouble))) {
			wxMessageBox("Podaj poprawn¹ kwotê.");
			return;
		}
	}
	else {
		wxMessageBox("Podaj poprawn¹ wartoœæ wp³aty.");
		return;
	}
	
	Account* newAccount = new Account(loginStr, initialDouble, passwordStr);
	if (newAccount->createAccount()) {
		delete newAccount;
		wxMessageBox("Zarejestrowano pomyœlnie!");
		registerPanel->Hide();
		homePanel->Show();
		ClearInputs();
	}
	else {
		wxMessageBox("U¿ytkownik ju¿ istnieje. Spróbuj podaæ inny login.");
	}
}

void MainFrame::loginAccount(wxCommandEvent& event)
{
	wxString login = loginInputLogin->GetValue();
	wxString password = loginInputPassword->GetValue();
	std::string loginStr = std::string(login.mb_str());
	std::string passwordStr = std::string(password.mb_str());
	std::string greeting = "Czeœæ " + loginStr;


	if (stdAccount.returnAccount(loginStr, passwordStr)) {
		
		if(stdAccount.getRole() == "user") {
			moneyBalance->SetLabel(wxString::Format(wxT("%.2f"), stdAccount.getBalance()));
			headlineMenuText->SetLabel(greeting);
			ShowTransactionsHisory();
			loginPanel->Hide();
			menuPanel->Show();
		}
		if (stdAccount.getRole() == "admin") {
			ShowUsersHisory();
			loginPanel->Hide();
			adminMenuPanel->Show();
		}

	}
	else {
		wxMessageBox("Nieprawid³owe dane");
	}
}

void MainFrame::depositMoney(wxCommandEvent& event)
{
	wxString amountToDepositStr = AmountWindow->GetValue();
	double amountToDeposit;
	if (amountToDepositStr.ToDouble(&amountToDeposit)) {
		if (amountToDeposit <= 0) {
			wxMessageBox("Wp³ata nie mo¿e byæ ujemna i równa 0.");
			return;
		}
		if (!MainFrame::isCurrencyAmount(std::to_string(amountToDeposit))) {
			wxMessageBox("Podaj poprawn¹ kwotê.");
			return;
		}
	}
	else {
		wxMessageBox("Podaj poprawn¹ wartoœæ wp³aty.");
		return;
	}
	double updatedBalance = stdAccount.getBalance() + amountToDeposit;
	if (stdAccount.updateBalance(updatedBalance) && stdAccount.writeTransactionToDB("wplata", amountToDeposit)) {
		wxMessageBox("Wp³acono");
	}
	else {
		wxMessageBox("B³¹d");
		return;
	}
	wxString updatedBalanceStr = wxString::Format(wxT("%.2f"), updatedBalance); 
	moneyBalance->SetLabel(updatedBalanceStr);
	ClearDepWitInputs();
	HideDepWitMenu(event);
	ClearTransationsHistory();
	ShowTransactionsHisory();
}

void MainFrame::withdrawMoney(wxCommandEvent& event)
{
	wxString amountToWithdrawStr = AmountWindow->GetValue();
	double amountToWithdraw;
	if (amountToWithdrawStr.ToDouble(&amountToWithdraw)) {
		if (amountToWithdraw <= 0) {
			wxMessageBox("Wp³ata nie mo¿e byæ ujemna i równa 0.");
			return;
		}
		if (!MainFrame::isCurrencyAmount(std::to_string(amountToWithdraw))) {
			wxMessageBox("Podaj poprawn¹ kwotê.");
			return ;
		}
	}
	else {
		wxMessageBox("Podaj poprawn¹ wartoœæ wp³aty.");
		return;
	}
	if (stdAccount.getBalance() - amountToWithdraw < 0) {
		wxMessageBox("Za ma³o œrodków na koncie");
		return;
	}
	double updatedBalance = stdAccount.getBalance() - amountToWithdraw;
	if (stdAccount.updateBalance(updatedBalance) && stdAccount.writeTransactionToDB("wyplata", amountToWithdraw)) {
		wxMessageBox("Wyp³acono");
	}
	else {
		wxMessageBox("B³¹d");
		return;
	}
	
	wxString updatedBalanceStr = wxString::Format(wxT("%.2f"), updatedBalance);
	moneyBalance->SetLabel(updatedBalanceStr);
	
	ClearDepWitInputs();
	HideDepWitMenu(event);
	ClearTransationsHistory();
	ShowTransactionsHisory();
}

void MainFrame::logout(wxCommandEvent& event)
{
	if (stdAccount.getRole() == "user") {
		ClearTransationsHistory();
		menuPanel->Hide();
	}
	if (stdAccount.getRole() == "admin") {
		adminMenuPanel->Hide();
		ClearUsersHistory();
	}
	stdAccount.setBalance(0);
	stdAccount.setLogin("");
	stdAccount.setPassword("");
	stdAccount.setId(0);
	stdAccount.setRole("");
	homePanel->Show();
}

// ACCOUNT_MANAGEMENT____END


