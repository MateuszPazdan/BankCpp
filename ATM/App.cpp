#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    //Utworzenie obiektu klasy MainFrame, ustawienie rozmiaru głównego okna na 800x600 pikseli, wyśrodkowanie głównego okna na ekranie oraz wyświetlenie głównego okna
    MainFrame* mainFrame = new MainFrame("Aplikacja bankowa");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show();

    return true;
}
