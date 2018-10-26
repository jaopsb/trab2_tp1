#include <iostream>
#include <Testes.hpp>
#include "Controladoras.hpp"
#include "Interfaces.hpp"
#include <stdexcept>
#include <string>

using namespace std;

int main()
{/*
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
    try{
    CtrlServ *serv = new CtrlServ();
    serv->init_banco();
    delete serv;

    CtrlIUMenu* menu;
    menu = new CtrlIUMenu();

    IServMenu* ctrl = new CtrlServMenu();

    menu->setCtrlServMenu(ctrl);

    menu->menu();

    delete menu;
    delete ctrl;
    }catch(const exception &ex){
    cout << ex.what() << endl;}
  return 0;
}
