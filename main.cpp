#include <iostream>
#include <stdexcept>
#include <string>
#include <Testes.hpp>
#include "Controladoras.hpp"
#include "Interfaces.hpp"

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
  CtrlIUAcom *ctrlIUAcom;
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

    //autenticando acesso
    RetornoLogin retorno = iuAut->autenticar();

    if (!retorno.get_resultado())
      throw runtime_error("Programa finalizado");

    delete iuAut;
    delete ctrlAut;

    usu = retorno.get_usuario();

    while (!fim)
    {
      system("cls");
      cout << "+-------------------------+" << endl;
      cout << "|Gerenciar Usuario     - " << USUARIO << "|" << endl;
      cout << "|Gerenciar Acomodacao  - " << ACOMODACAO << "|" << endl;
      cout << "|Sair                  - " << SAIR << "|" << endl;
      cout << "+-------------------------+" << endl;
      cout << "|Opcao:";
      cin >> opt;

      switch (opt)
      {
      case 1:
      {
        ctrlIUUsu = new CtrlIUUsu(usu->get_identificador(), usu->get_senha());
        IServUsu *ctrl = new CtrlServUsu();
        ctrlIUUsu->setCtrlServ(ctrl);
        ctrlIUUsu->executa();
        delete ctrlIUUsu;
        delete ctrl;
        break;
      }
      case 2:
      {
        ctrlIUAcom = new CtrlIUAcom(usu->get_identificador());
        IServAcom *ctrl = new CtrlServAcom();
        ctrlIUAcom->setCtrlServ(ctrl);
        ctrlIUAcom->executa();
        delete ctrlIUAcom;
        delete ctrl;
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
