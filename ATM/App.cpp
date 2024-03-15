#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    //Utworzenie obiektu klasy MainFrame, ustawienie rozmiaru g³ównego okna na 800x600 pikseli, wyœrodkowanie g³ównego okna na ekranie oraz wyœwietlenie g³ównego okna
    MainFrame* mainFrame = new MainFrame("Aplikacja bankowa");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show();

    return true;
}
