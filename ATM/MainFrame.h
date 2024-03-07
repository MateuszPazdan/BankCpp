#pragma once
#include "Account.h"	
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <string>
#include <unordered_map>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	std::string stdLogin = "";
	std::string stdPassword = "";
	int selectedItemIndex = -1;
	Account stdAccount = Account(stdLogin, 0.0, stdPassword);

	// BINDS
	void CreateControls();
	void BindHomeEventHandlers();
	void BindLoginEventHandlers();
	void BindRegisterEventHandlers();
	void BindMenuEventHandlers();
	void BindAdminMenuEventHandlers();
	// ACCOUNT
	void ShowRegisterPanel(wxCommandEvent& event);
	void ShowLoginPanel(wxCommandEvent& event);
	void ShowHomePanel(wxCommandEvent& event);
	void ShowDepositMenu(wxCommandEvent& event);
	void ShowWithdrawMenu(wxCommandEvent& event);
	void HideDepWitMenu(wxCommandEvent& event);
	void ClearInputs();
	void ClearDepWitInputs();
	void ShowTransactionsHisory();
	void ClearTransationsHistory();
	bool isCurrencyAmount(const std::string& input);
	bool isLoginPassword(const std::string& input);
	void createAccount(wxCommandEvent& event);
	void loginAccount(wxCommandEvent& event);
	void depositMoney(wxCommandEvent& event);
	void withdrawMoney(wxCommandEvent& event);
	void logout(wxCommandEvent& event);

	void selectUser(wxListEvent& event);
	void editUser(wxCommandEvent& event);
	void deleteUser(wxCommandEvent& event);

	void ShowUsersHisory();
	void ClearUsersHistory();

	wxStaticText* headlineText;
	
	//home
	wxPanel* homePanel;
	wxButton* chooseLoginBtn;
	wxButton* chooseRegisterBtn;
	
	//login
	wxPanel* loginPanel;
	wxStaticText* loginPanelText;
	wxTextCtrl* loginInputLogin;
	wxTextCtrl* loginInputPassword;
	wxButton* loginBtn;
	wxButton* loginBackBtn;

	//register
	wxPanel* registerPanel;
	wxStaticText* registerPanelText;
	wxTextCtrl* registerInputLogin;
	wxTextCtrl* registerInputPassword;
	wxTextCtrl* registerInputDeposit;
	wxButton* registerBtn;
	wxButton* registerBackBtn;

	//menu
	wxPanel* menuPanel;
	wxStaticText* headlineMenuText;
	wxStaticText* moneyBalance;
	wxButton* depositBtn;
	wxButton* withdrawBtn;

	wxStaticText* menuInfo;
	wxTextCtrl* AmountWindow;
	wxButton* depositAcceptBtn;
	wxTextCtrl* withdrawAmount;
	wxButton* withdrawAcceptBtn;
	wxButton* backBtn;
	wxListCtrl* transactionList;
	wxButton* logoutBtn;

	//admin menu
	wxPanel* adminMenuPanel;
	wxStaticText* headlineAdminMenuText;
	wxButton* adminLogoutBtn;

	wxListCtrl* usersList;
	wxButton* deleteBtn;
	wxButton* editBtn;
};
