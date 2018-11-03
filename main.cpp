#include <iostream>
#include <Testes.hpp>
#include "Controladoras.hpp"
#include "Interfaces.hpp"
#include <stdexcept>
#include <string>

using namespace std;

int main()
{
    Usuario  usu;
    try{
    CtrlServ *serv = new CtrlServ();
    serv->init_banco();
    delete serv;

    CtrlIUMenu* menu;
    menu = new CtrlIUMenu();

    IServMenu* ctrl = new CtrlServMenu();

    menu->setCtrlServMenu(ctrl);

    menu->menu();

    usu = menu->get_usuario_logado();

    cout << usu.get_identificador() << ', ' <<
    usu.get_nome() << ', ' << usu.get_senha() << endl;

    delete menu;
    delete ctrl;
    }catch(const exception &ex){
    cout << ex.what() << endl;}
  return 0;
}
