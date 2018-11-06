#include <iostream>
#include <stdexcept>
#include <string>
#include <Testes.hpp>
#include "Controladoras.hpp"
#include "Interfaces.hpp"
#include "Stubs.h"

using namespace std;

const static int USUARIO = 1;
const static int ACOMODACAO = 2;
const static int RESERVA = 3;
const static int SAIR = 5;

int main()
{
  bool fim = false;
  int opt;
  Usuario *usu;

  CtrlIUUsu *ctrlIUUsu;

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

    while (!fim)
    {
      system("cls");
      cout << "Gerenciar Usuario     - " << USUARIO << endl;
      cout << "Gerenciar Acomodacao  - " << ACOMODACAO << endl;
      cout << "Sair                  - " << SAIR << endl;
      cin >> opt;

      switch (opt)
      {
      case 1:
      {

        //IUUser
        ctrlIUUsu = new CtrlIUUsu(usu->get_identificador(), usu->get_senha());
        CtrlServUsu *ctrl = new CtrlServUsu();
        ctrlIUUsu->setCtrlServUsu(ctrl);
        ctrlIUUsu->executa();
        break;
      }
      case 2:
      {

        cout << "Ainda nao" << endl;
        break;
      }
      case 5:
      {
        fim = true;
        break;
      }
      }
    }
  }
  catch (const exception &ex)
  {
    cout << ex.what() << endl;
  }
  return 0;
}
