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
const static int LOGOUT = 6;

int main()
{
  bool nlogout = false;
  int opt;
  Usuario *usu;
  bool delUsu = false;
  CtrlIUUsu *ctrlIUUsu;
  CtrlIUAcom *ctrlIUAcom;

  while (!nlogout)
  {
    try
    {
      bool fim = false;
      system("cls");
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
        cout << "|Logout                - " << LOGOUT << "|" << endl;
        cout << "+-------------------------+" << endl;
        cout << "|Opcao:";
        cin >> opt;

        switch (opt)
        {
        case USUARIO:
        {
          ctrlIUUsu = new CtrlIUUsu(usu->get_identificador(), usu->get_senha());
          IServUsu *ctrl = new CtrlServUsu();
          ctrlIUUsu->setCtrlServ(ctrl);
          ctrlIUUsu->executa();
          delUsu = ctrlIUUsu->isUsuarioLogado();
          delete ctrlIUUsu;
          delete ctrl;
          break;
        }
        case ACOMODACAO:
        {
          ctrlIUAcom = new CtrlIUAcom(usu->get_identificador());
          IServAcom *ctrl = new CtrlServAcom();
          ctrlIUAcom->setCtrlServ(ctrl);
          ctrlIUAcom->executa();
          delete ctrlIUAcom;
          delete ctrl;
          break;
        }
        case SAIR:
        {
          fim = true;
          nlogout = true;
          break;
        }
        case LOGOUT:
          fim = true;
          break;
        }
        if (delUsu)
          fim = true;
      }
    }
    catch (const exception &ex)
    {
      cout << ex.what() << endl;
    }
  }
  return 0;
}
