#pragma once
#include "Account.h"	
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <string>

class MainFrame : public wxFrame
{

public:
	MainFrame(const wxString& title);
private:
	std::string stdLogin = "";
	std::string stdPassword = "";
	Account stdAccount = Account(stdLogin, 0.0, stdPassword);

	void CreateControls();
	void BindHomeEventHandlers();
	void BindLoginEventHandlers();
	void BindRegisterEventHandlers();
	void BindMenuEventHandlers();

	void ShowRegisterPanel(wxCommandEvent& event);
	void ShowLoginPanel(wxCommandEvent& event);
	void ShowHomePanel(wxCommandEvent& event);
	void ShowDepositMenu(wxCommandEvent& event);
	void ShowWithdrawMenu(wxCommandEvent& event);
	void HideDepWitMenu(wxCommandEvent& event);
	void ClearInputs();
	void ClearDepWitInputs();
	void ShowHisory();
	void ClearHistory();
	bool isCurrencyAmount(const std::string& input);
	bool isLoginPassword(const std::string& input);

	void createAccount(wxCommandEvent& event);
	void loginAccount(wxCommandEvent& event);
	void depositMoney(wxCommandEvent& event);
	void withdrawMoney(wxCommandEvent& event);
	void logout(wxCommandEvent& event);

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
	wxTextCtrl* depositAmount;
	wxButton* depositAcceptBtn;
	wxTextCtrl* withdrawAmount;
	wxButton* withdrawAcceptBtn;
	wxButton* backBtn;
	wxListCtrl* transactionList;
	wxButton* logoutBtn;
};
