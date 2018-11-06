#include <iostream>
#include <stdexcept>
#include <string>
#include <Testes.hpp>
#include "Controladoras.hpp"
#include "Interfaces.hpp"
#include "Stubs.h"

using namespace std;

int main()
{
  Usuario *usu;
  try
  {
    //inciializacao do banco
    CtrlServ *serv = new CtrlServ();
    serv->init_banco();
    delete serv; //servico feito

    //iniciando sistema
    CtrlIUAut *iuAut = new CtrlIUAut();
    IServAut *ctrlAut = new CtrlServAut();

    iuAut->setCtrlServAut(ctrlAut);

    cout << "autenticacao" << endl;
    //autenticando acesso
    RetornoLogin retorno = iuAut->autenticar();

    if (!retorno.get_resultado())
      throw runtime_error("Programa finalizado");

    usu = retorno.get_usuario();

    //IUUser
    CtrlIUUsu *ctrUsu = new CtrlIUUsu(usu->get_identificador(), usu->get_senha());
    CtrlServUsu *ctrl = new CtrlServUsuStub();
    ctrUsu->setCtrlServUsu(ctrl);
    ctrUsu->executa();
  }
  catch (const exception &ex)
  {
    cout << ex.what() << endl;
  }
  return 0;
}
