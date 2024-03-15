#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    //Utworzenie obiektu klasy MainFrame, ustawienie rozmiaru g��wnego okna na 800x600 pikseli, wy�rodkowanie g��wnego okna na ekranie oraz wy�wietlenie g��wnego okna
    MainFrame* mainFrame = new MainFrame("Aplikacja bankowa");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show();

    return true;
}
