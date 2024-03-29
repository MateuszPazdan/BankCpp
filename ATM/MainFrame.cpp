#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include "MainFrame.h"


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	BindHomeEventHandlers();
	BindLoginEventHandlers();
	BindRegisterEventHandlers();
	BindMenuEventHandlers();
	BindAdminMenuEventHandlers();
}

// Tworzenie kontrolek i ustawianie ich w�a�ciwo�ci
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

	loginPanelText = new wxStaticText(loginPanel, wxID_ANY, "Has�o:", wxPoint(250, 215), wxSize(300, -1));
	loginInputPassword = new wxTextCtrl(loginPanel, wxID_ANY, "", wxPoint(250, 240), wxSize(300, -1), wxTE_PASSWORD);
	
	loginBtn = new wxButton(loginPanel, wxID_ANY, "Zaloguj si�", wxPoint(250, 380), wxSize(300, 70));
	loginBackBtn = new wxButton(loginPanel, wxID_ANY, "Wr��", wxPoint(50, 500), wxSize(100, 50));

	
	//REGISTER PANEL
	registerPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));
	headlineText = new wxStaticText(registerPanel, wxID_ANY, "Rejestracja", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	registerPanel->SetFont(mainFont);
	headlineText->SetFont(headlineFont);

	registerPanelText = new wxStaticText(registerPanel, wxID_ANY, "Login:", wxPoint(250, 145), wxSize(300, -1));
	registerInputLogin = new wxTextCtrl(registerPanel, wxID_ANY, "", wxPoint(250, 170), wxSize(300, -1));

	registerPanelText = new wxStaticText(registerPanel, wxID_ANY, "Has�o:", wxPoint(250, 215), wxSize(300, -1));
	registerInputPassword = new wxTextCtrl(registerPanel, wxID_ANY, "", wxPoint(250, 240), wxSize(300, -1), wxTE_PASSWORD);
	
	registerPanelText = new wxStaticText(registerPanel, wxID_ANY, "Wp�ata:", wxPoint(250, 285), wxSize(300, -1));
	registerInputDeposit = new wxTextCtrl(registerPanel, wxID_ANY, "0", wxPoint(250, 310), wxSize(300, -1));
	
	registerBtn = new wxButton(registerPanel, wxID_ANY, "Zarejestruj si�", wxPoint(250, 380), wxSize(300, 70));
	registerBackBtn = new wxButton(registerPanel, wxID_ANY, "Wr��", wxPoint(50, 500), wxSize(100, 50));

	//MAIN USER MENU PANEL
	menuPanel = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(800, 600));
	headlineMenuText = new wxStaticText(menuPanel, wxID_ANY, "Witaj U�ytkowniku", wxPoint(0, 40), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	menuPanel->SetFont(mainFont);
	headlineMenuText->SetFont(headlineFont);

	logoutBtn = new wxButton(menuPanel, wxID_ANY, "Wyloguj", wxPoint(10, 10), wxSize(80, -1));
	moneyBalance = new wxStaticText(menuPanel, wxID_ANY, "0", wxPoint(0, 100), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	depositBtn = new wxButton(menuPanel, wxID_ANY, "Wp�a�", wxPoint(175, 150), wxSize(150, 50));
	withdrawBtn = new wxButton(menuPanel, wxID_ANY, "Wyp�a�", wxPoint(475, 150), wxSize(150, 50));

	AmountWindow = new wxTextCtrl(menuPanel, wxID_ANY, "0", wxPoint(225, 160), wxSize(150, -1));
	depositAcceptBtn = new wxButton(menuPanel, wxID_ANY, "Wp�a�", wxPoint(425, 160), wxSize(150, -1));
	AmountWindow->Hide();
	depositAcceptBtn->Hide();

	withdrawAcceptBtn = new wxButton(menuPanel, wxID_ANY, "Wyp�a�", wxPoint(425, 160), wxSize(150, -1));
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
	
	adminListPanel = new wxPanel(adminMenuPanel, wxID_ANY, wxPoint(0, 150), wxSize(800, 650));
	usersList = new wxListCtrl(adminListPanel, wxID_ANY, wxPoint(220, 00), wxSize(400, 450), wxLC_REPORT);
	usersList->InsertColumn(0, "Login", wxLIST_FORMAT_LEFT, 200);
	usersList->InsertColumn(1, "Balans", wxLIST_FORMAT_LEFT, 200);
	editBtn = new wxButton(adminListPanel, wxID_ANY, "Edytuj", wxPoint(635, 0), wxSize(150, 50));
	deleteBtn = new wxButton(adminListPanel, wxID_ANY, "Usu�", wxPoint(635, 60), wxSize(150, 50));

	userSettingsPanel = new wxPanel(adminMenuPanel, wxID_ANY, wxPoint(0, 150), wxSize(800, 650));
	userSettingsText = new wxStaticText(userSettingsPanel, wxID_ANY, "Login:", wxPoint(275, 0), wxSize(250, -1));
	userSettingsInputLogin = new wxTextCtrl(userSettingsPanel, wxID_ANY, "", wxPoint(275, 25), wxSize(250, -1));
	userSettingsText = new wxStaticText(userSettingsPanel, wxID_ANY, "Balans:", wxPoint(275, 70), wxSize(250, -1));
	userSettingsInputBalance = new wxTextCtrl(userSettingsPanel, wxID_ANY, "", wxPoint(275, 95), wxSize(250, -1));
	userSettingsConfirmBtn = new wxButton(userSettingsPanel, wxID_ANY, "Potwierd�", wxPoint(325, 150), wxSize(150, 50));
	userSettingsBackBtn = new wxButton(userSettingsPanel, wxID_ANY, "Wr��", wxPoint(325, 225), wxSize(150, 50));
	userSettingsPanel->Hide();

	//homePanel->Hide();
	loginPanel->Hide();
	registerPanel->Hide();
	menuPanel->Hide();
	adminMenuPanel->Hide();

}

// BINDS - Metody do powi�zywania zdarze� z przyciskami
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
	editBtn->Bind(wxEVT_BUTTON, &MainFrame::ShowUserSettings, this);
	deleteBtn->Bind(wxEVT_BUTTON, &MainFrame::deleteUser, this);
	userSettingsConfirmBtn->Bind(wxEVT_BUTTON, &MainFrame::editUser, this);
	userSettingsBackBtn->Bind(wxEVT_BUTTON, &MainFrame::HideUserSettings, this);
}

// Metody obs�ugi zdarze�

 // Obs�uga zdarzenia wyboru u�ytkownika na li�cie
void MainFrame::selectUser(wxListEvent& event)
{
	selectedItemIndex = event.GetIndex();
}

// Obs�uga przycisku edytowania u�ytkownika 
void MainFrame::ShowUserSettings(wxCommandEvent& event)
{
	if (selectedItemIndex != -1) {
		adminListPanel->Hide();
		userSettingsPanel->Show();
		wxString login = usersList->GetItemText(selectedItemIndex, 0);
		selectedUser = login.c_str();
		wxString balance = usersList->GetItemText(selectedItemIndex, 1);
		userSettingsInputLogin->SetValue(login);
		userSettingsInputBalance->SetValue(balance);
	}
	else {
		wxMessageBox("Wybierz u�ytkownika, aby edytowa�.");
	}
}

// Chowanie ustawie� u�ytkownika
void MainFrame::HideUserSettings(wxCommandEvent& event)
{
	adminListPanel->Show();
	userSettingsPanel->Hide();
	ClearUsersList();
	ShowUsersList();
	selectedItemIndex = -1;
	userSettingsInputLogin->SetValue("");
	userSettingsInputBalance->SetValue("");
	selectedUser = "";
}

// Obs�uga przycisku usuwanie u�ytkownika
void MainFrame::deleteUser(wxCommandEvent& event)
{
	if (selectedItemIndex != -1) {
		wxString login = usersList->GetItemText(selectedItemIndex, 0);
		stdAccount.deleteAccount(std::string(login));

		ClearUsersList();
		ShowUsersList();
		selectedItemIndex = -1;
	}
	else {
		wxMessageBox("Wybierz u�ytkownika, aby usun��.");
	}
}

// Obs�uga przycisku potwierdzaj�cego edycje u�ytkownika
void MainFrame::editUser(wxCommandEvent& event)
{
	std::string newLogin;
	double newBalance;
	newLogin = userSettingsInputLogin->GetValue();

	if (userSettingsInputBalance->GetValue().ToDouble(&newBalance)) {
		if (newBalance < 0) {
			wxMessageBox("Balans nie mo�e by� ujemny.");
			return;
		}
		if (!isCurrencyAmount(std::to_string(newBalance))) {
			wxMessageBox("Podaj poprawn� kwot�.");
			return;
		}
	}
	else {
		wxMessageBox("Podaj poprawn� kwot�.");
		return;
	}

	if (stdAccount.editUser(selectedUser, newLogin, newBalance)) {
	wxMessageBox("Edytowano pomy�lnie!");
	HideUserSettings(event);
	}
	else {
		wxMessageBox("B��d");
	}
}

// Wy�wietlanie listy u�ytkownik�w
void MainFrame::ShowUsersList()
{
	std::vector<Account> users = stdAccount.returnVectorOfAccounts();
	for (auto& user : users) {
		if (user.getRole() == "admin") continue;
		long index = usersList->InsertItem(usersList->GetItemCount(), user.getLogin());
		usersList->SetItem(index, 1, wxString::Format("%.2f", user.getBalance()));
	}
}

// Czyszczenie listy u�ytkownik�w
void MainFrame::ClearUsersList()
{
	usersList->DeleteAllItems();
}

// Wy�wietlanie listy transakcji
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

// Czyszczenie listy transakcji
void MainFrame::ClearTransationsHistory()
{
	transactionList->DeleteAllItems();
}


// Zarz�dzanie panelami
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


// Metody pomocnicze

// Sprawdzanie poprawno�ci formatu kwoty
bool MainFrame::isCurrencyAmount(const std::string& input){
	std::regex pattern(R"(\d+\.\d{2}[0]*)");
	return std::regex_match(input, pattern);
}

// Sprawdzanie poprawno�ci formatu loginu i has�a
bool MainFrame::isLoginPassword(const std::string& input)
{
	std::regex pattern("^[^\\s]{3,}$");
	return std::regex_match(input, pattern);
}

// Metody obs�ugi kont

// Tworzenie nowego konta
void MainFrame::createAccount(wxCommandEvent& event)
{
	wxString login = registerInputLogin->GetValue();
	wxString password = registerInputPassword->GetValue();
	std::string loginStr = std::string(login.mb_str());
	std::string passwordStr = std::string(password.mb_str());
	wxString initialDeposit = registerInputDeposit->GetValue();
	double initialDouble;
	
	if (!isLoginPassword(loginStr)) {
		wxMessageBox("Login sk�ada si� co najmniej z 3 znak�w i nie zawiera spacji.");
		return;
	}
	if (!isLoginPassword(passwordStr)) {
		wxMessageBox("Has�o sk�ada si� co najmniej z 3 znak�w i nie zawiera spacji.");
		return;
	}

	if (initialDeposit.ToDouble(&initialDouble)) {
		if (initialDouble < 0) {
			wxMessageBox("Wp�ata nie mo�e by� ujemna.");
			return;
		}
		if (!isCurrencyAmount(std::to_string(initialDouble))) {
			wxMessageBox("Podaj poprawn� kwot�.");
			return;
		}
	}
	else {
		wxMessageBox("Podaj poprawn� warto�� wp�aty.");
		return;
	}
	
	Account* newAccount = new Account(loginStr, initialDouble, passwordStr);
	if (newAccount->createAccount()) {
		wxMessageBox("Zarejestrowano pomy�lnie!");
		registerPanel->Hide();
		homePanel->Show();
		ClearInputs();
	}
	else {
		wxMessageBox("U�ytkownik ju� istnieje. Spr�buj poda� inny login.");
	}
	delete newAccount;
}

// Logowanie do konta
void MainFrame::loginAccount(wxCommandEvent& event)
{
	wxString login = loginInputLogin->GetValue();
	wxString password = loginInputPassword->GetValue();
	std::string loginStr = std::string(login.mb_str());
	std::string passwordStr = std::string(password.mb_str());
	std::string greeting = "Cze�� " + loginStr;


	if (stdAccount.returnAccount(loginStr, passwordStr)) {
		if(stdAccount.getRole() == "user") {
			moneyBalance->SetLabel(wxString::Format(wxT("%.2f"), stdAccount.getBalance()));
			headlineMenuText->SetLabel(greeting);
			ShowTransactionsHisory();
			loginPanel->Hide();
			menuPanel->Show();
		}
		if (stdAccount.getRole() == "admin") {
			ShowUsersList();
			loginPanel->Hide();
			adminMenuPanel->Show();
		}
		ClearInputs();
	}
	else {
		wxMessageBox("Nieprawid�owe dane");
	}
}

// Wp�acanie �rodk�w
void MainFrame::depositMoney(wxCommandEvent& event)
{
	wxString amountToDepositStr = AmountWindow->GetValue();
	double amountToDeposit;
	if (amountToDepositStr.ToDouble(&amountToDeposit)) {
		if (amountToDeposit <= 0) {
			wxMessageBox("Wp�ata nie mo�e by� ujemna i r�wna 0.");
			return;
		}
		if (!MainFrame::isCurrencyAmount(std::to_string(amountToDeposit))) {
			wxMessageBox("Podaj poprawn� kwot�.");
			return;
		}
	}
	else {
		wxMessageBox("Podaj poprawn� warto�� wp�aty.");
		return;
	}
	double updatedBalance = stdAccount.getBalance() + amountToDeposit;
	if (stdAccount.updateBalance(updatedBalance) && stdAccount.writeTransactionToDB("wplata", amountToDeposit)) {
		wxMessageBox("Wp�acono");
	}
	else {
		wxMessageBox("B��d");
		return;
	}
	wxString updatedBalanceStr = wxString::Format(wxT("%.2f"), updatedBalance); 
	moneyBalance->SetLabel(updatedBalanceStr);
	ClearDepWitInputs();
	HideDepWitMenu(event);
	ClearTransationsHistory();
	ShowTransactionsHisory();
}

// Wyp�acanie �rodk�w
void MainFrame::withdrawMoney(wxCommandEvent& event)
{
	wxString amountToWithdrawStr = AmountWindow->GetValue();
	double amountToWithdraw;
	if (amountToWithdrawStr.ToDouble(&amountToWithdraw)) {
		if (amountToWithdraw <= 0) {
			wxMessageBox("Wp�ata nie mo�e by� ujemna i r�wna 0.");
			return;
		}
		if (!MainFrame::isCurrencyAmount(std::to_string(amountToWithdraw))) {
			wxMessageBox("Podaj poprawn� kwot�.");
			return ;
		}
	}
	else {
		wxMessageBox("Podaj poprawn� warto�� wp�aty.");
		return;
	}
	if (stdAccount.getBalance() - amountToWithdraw < 0) {
		wxMessageBox("Za ma�o �rodk�w na koncie");
		return;
	}
	double updatedBalance = stdAccount.getBalance() - amountToWithdraw;
	if (stdAccount.updateBalance(updatedBalance) && stdAccount.writeTransactionToDB("wyplata", amountToWithdraw)) {
		wxMessageBox("Wyp�acono");
	}
	else {
		wxMessageBox("B��d");
		return;
	}
	
	wxString updatedBalanceStr = wxString::Format(wxT("%.2f"), updatedBalance);
	moneyBalance->SetLabel(updatedBalanceStr);
	
	ClearDepWitInputs();
	HideDepWitMenu(event);
	ClearTransationsHistory();
	ShowTransactionsHisory();
}

// Wylogowanie u�ytkownika
void MainFrame::logout(wxCommandEvent& event)
{
	if (stdAccount.getRole() == "user") {
		ClearTransationsHistory();
		menuPanel->Hide();
	}
	if (stdAccount.getRole() == "admin") {
		adminMenuPanel->Hide();
		ClearUsersList();
	}
	stdAccount.setBalance(0);
	stdAccount.setLogin("");
	stdAccount.setPassword("");
	stdAccount.setId(0);
	stdAccount.setRole("");
	homePanel->Show();
	HideUserSettings(event);
}