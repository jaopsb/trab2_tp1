#include <iostream>
#include "dominios.hpp"
#include "Controladoras.hpp"

void CtrlIUAut::menu()
{
  int resp = 0;

  identificador = new Identificador();
  senha = new Senha();

  while (resp != 5)
  {

    system("cls"); //windows

    cout << "Sistema de Hospedagem 2.0" << endl;
    cout << "login     - " << LOGIN << endl;
    cout << "Registrar - " << REGISTRAR << endl;
    cout << "Sair      - 5" << endl;
    cout << "Selecione uma opcao";

    cin >> resp;

    switch (resp)
    {
    case LOGIN:
      login();
      break;
    case REGISTRAR:
      registar();
      break;
    }
  }
}

bool CtrlIUAut::autenticar(string id, string s)
{
  identificador->set_identificador(id);
  senha->set_senha(s);
}

void CtrlIUAut::login()
{
  string id, senha;
  bool fim = false;
  do
  {
    try
    {

      cout << "LOGIN" << endl;
      cout << "identificador:";
      cin >> id;
      cout << "Senha:";
      cin >> senha;

      if (CtrlIUAut::autenticar(id, senha))
        fim = true;

      system("cls"); //windows
    }
    catch (const invalid_argument &ia)
    {
      cout << ia.what() << endl;
    }
  } while (!fim);
}

void CtrlIUAut::registar()
{
  cout << "CALMAA";
}
