#include <iostream>
#include <Testes.hpp>
#include "Controladoras.hpp"
#include "Interfaces.hpp"
#include <stdexcept>
#include <string>

using namespace std;

int main()
{
    /*
    CtrlIUAut* ctrl;
    ctrl = new CtrlIUAut();

    IServAut* ctrliserv = new CtrlServAut();

    ctrl->setCtrlServAut(ctrliserv);

    if(ctrl->autenticar()){
        cout << "SUCESSO" << endl;
    }else {
    cout << "ERRO"<< endl;
    }

    delete ctrl;
    delete ctrliserv;
    */
    CtrlIUMenu* menu;
    menu = new CtrlIUMenu();

    menu->menu();

    delete menu;
  return 0;
}
