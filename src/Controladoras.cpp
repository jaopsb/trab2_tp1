#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <typeinfo>
#include "dominios.hpp"
#include "Entidades.hpp"
#include "Controladoras.hpp"

//definindo tipo de limpeza de tela
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

/************* SCRIPTS CRIACAO DO BANCO ************/
char *SQL_STMT_CREATE_USUARIO = "CREATE TABLE IF NOT EXISTS `USUARIO` ( `ID` INTEGER, `IDENTIFICADOR` TEXT NOT NULL UNIQUE, `NOME` TEXT NOT NULL, `SENHA` TEXT NOT NULL, PRIMARY KEY(`IDENTIFICADOR`,`ID`));";
char *SQL_STMT_CREATE_ACOMODACAO = "CREATE TABLE IF NOT EXISTS `ACOMODACAO` (`titulo` TEXT NOT NULL,`id` INTEGER PRIMARY KEY AUTOINCREMENT ,`tipo`	INTEGER NOT NULL,`capacidade`	INTEGER NOT NULL,`cidade`	TEXT NOT NULL,`estado`	INTEGER NOT NULL,`diaria`	TEXT NOT NULL,`dono`	TEXT,PRIMARY KEY(`dono`),dt_dis_in TEXT NOT NULL, dt_dis_fim TEXT NOT NULL);";
char *SQL_STMT_CREATE_CARTAO = "CREATE TABLE IF NOT EXISTS 'CARTAO' ( `ID` INTEGER ,`NUMERO` TEXT NOT NULL, `DT_VALIDADE` TEXT NOT NULL, `ID_USUARIO` TEXT NOT NULL );";
char *SQL_STMT_CREATE_CONTACORRENTE = "CREATE TABLE IF NOT EXISTS 'CONTACORRENTE' ( `ID` INTEGER , `NUMERO` TEXT NOT NULL, `AGENCIA` INTEGER NOT NULL, `BANCO` INTEGER NOT NULL, `ID_USUARIO` TEXT NOT NULL );";
char *SQL_STMT_CREATE_RESERVA = "CREATE TABLE IF NOT EXISTS `RESERVA` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `id_usuario` TEXT NOT NULL, `id_acomodacao` INTEGER NOT NULL, `data_inicio` TEXT NOT NULL, `data_fim` TEXT NOT NULL );";
char *SQL_INSERT_USUARIO_ADMIN = "insert into USUARIO (identificador,nome,senha) values ('admin','Administrador','Admin123!');";
/************ SCRIPTS CRIACAO DO BANCO *************/

/***************FUNCOES AUXILIARES************/
bool trata_retorno(int rc)
{
  switch (rc)
  {
  case SQLITE_DONE:
  case SQLITE_ROW:
  case SQLITE_OK:
    return true;
    break;
  case SQLITE_CONSTRAINT:
    throw invalid_argument("Identificador ja utilizado!");
    break;
  case SQLITE_ERROR:
    throw invalid_argument("Erro no servico!");
    break;

  default:
    throw invalid_argument(to_string(rc));
    break;
  }
}

int formata_mes(string mes)
{
  if (mes.compare("jan") == 0)
    return 1;
  if (mes.compare("fev") == 0)
    return 2;
  if (mes.compare("mar") == 0)
    return 3;
  if (mes.compare("abr") == 0)
    return 4;
  if (mes.compare("mai") == 0)
    return 5;
  if (mes.compare("jun") == 0)
    return 6;
  if (mes.compare("jul") == 0)
    return 7;
  if (mes.compare("ago") == 0)
    return 8;
  if (mes.compare("set") == 0)
    return 9;
  if (mes.compare("out") == 0)
    return 10;
  if (mes.compare("nov") == 0)
    return 11;
  if (mes.compare("dez") == 0)
    return 12;
}

vector<int> formata_data(string data)
{
  string buff{""};
  vector<string> vaux;
  vector<int> retorno;

  for (auto n : data)
  {
    if (n != '/')
    {
      buff += n;
    }
    else if ((n == '/') && buff != "")
    {

      vaux.push_back(buff);

      buff = "";
    }
  }
  if (buff != "")
  {
    vaux.push_back(buff);
  }

  retorno.push_back(stoi(vaux.at(0)));
  retorno.push_back(formata_mes(vaux.at(1)));
  retorno.push_back(stoi(vaux.at(2)));

  return retorno;
}

string get_tipo_acomodacao(int tipo)
{
  switch (tipo)
  {
  case 1:
    return "Apartamento";
    break;
  case 2:
    return "Casa";
    break;
  case 3:
    return "Flat";
    break;
  }
}

string formata_cartao(string numero)
{
  string resultado;
  for (int i = 0; i < numero.length(); i++)
  {
    if (i >= 12)
    {
      resultado += numero[i];
    }
    else
    {
      resultado += '*';
    }
    if ((i + 1) % 4 == 0 && i < 16 && i > 0)
    {
      resultado += ' ';
    }
  }
  return resultado;
}

void periodo_valido(string dt_inicio, string dt_fim)
{
  int resultado;

  vector<int> vdt1 = formata_data(dt_inicio);
  vector<int> vdt2 = formata_data(dt_fim);

  if (vdt1.at(2) > vdt2.at(2))
    throw runtime_error("O ano da data de inicio nao pode ser maior que o da data de fim");

  if (vdt1.at(2) == vdt2.at(2))
  {
    if (vdt1.at(1) > vdt2.at(1))
    {
      throw runtime_error("O mes da data de inicio nao pode ser maior que o da data de fim");
    }
    else if (vdt1.at(1) == vdt2.at(1))
    {
      if (vdt1.at(0) > vdt2.at(0))
        throw runtime_error("O dia da data de inicio nao pode ser maior que o da data de fim");
    }
  }
}

void periodo_valido(string dt_inicio, string dt_fim, string dt_dis_inicio, string dt_dis_fim)
{
  int resultado;

  vector<int> vdt1 = formata_data(dt_inicio);
  vector<int> vdt2 = formata_data(dt_fim);
  vector<int> vdt_dis1 = formata_data(dt_dis_inicio);
  vector<int> vdt_dis2 = formata_data(dt_dis_fim);

  if (vdt1.at(2) > vdt2.at(2))
    throw runtime_error("O ano da data de inicio nao pode ser maior que o da data de fim");

  if (vdt1.at(2) == vdt2.at(2))
  {
    if (vdt1.at(1) > vdt2.at(1))
    {
      throw runtime_error("O mes da data de inicio nao pode ser maior que o da data de fim");
    }
    else if (vdt1.at(1) == vdt2.at(1))
    {
      if (vdt1.at(0) > vdt2.at(0))
        throw runtime_error("O dia da data de inicio nao pode ser maior que o da data de fim");
    }
  }

  //validando intervalo de ano da reserva com o intervalo da disponibilidade
  if ((vdt1.at(2) < vdt_dis1.at(2)) ||
      (vdt1.at(2) > vdt_dis2.at(2)) ||
      (vdt2.at(2) < vdt_dis1.at(2)) ||
      (vdt2.at(2) > vdt_dis2.at(2)))
    throw runtime_error("Periodo fora da disponibilidade da acomodacao");

  if (vdt1.at(2) == vdt_dis1.at(2))
  {
    if (vdt1.at(1) < vdt_dis1.at(1))
      throw runtime_error("Periodo fora da disponibilidade da acomodacao");

    if (vdt1.at(1) == vdt_dis1.at(1))
    {
      if (vdt1.at(0) < vdt_dis1.at(0))
        throw runtime_error("Periodo fora da disponibilidade da acomodacao");
    }
  }

  if (vdt2.at(2) == vdt_dis2.at(2))
  {
    if (vdt2.at(1) > vdt_dis2.at(1))
      throw runtime_error("Perioodo fora da disponibilidade da acomodacao");

    if (vdt2.at(1) == vdt_dis2.at(1))
    {
      if (vdt2.at(0) > vdt_dis2.at(0))
        throw runtime_error("Periodo fora da disponibilidade da acomodacao");
    }
  }
}

string formata_senha(string senha)
{
  string nSenha;
  for (int i = 0; i < senha.length(); i++)
  {
    nSenha += '*';
  }
  return nSenha;
}

/***************FUNCOES AUXILIARES************/

/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

CtrlIUAut::CtrlIUAut()
{
  identificador = new Identificador();
  senha = new Senha();
}

void CtrlIUAut::setCtrlServAut(IServAut *iServAut)
{
  ctrlServAut = iServAut;
}

RetornoLogin CtrlIUAut::autenticar()
{
  string id, snh;
  char resp;
  bool resultado = true;
  bool fim = false;

  while (!fim)
  {
    try
    {
      cout << "identificador:";
      cin >> id;
      identificador->set_identificador(id);
      cout << "Senha:";
      cin >> snh;
      senha->set_senha(snh);
      system(CLEAR); //windows

      //TODO retornar o usuario
      Usuario *usu = ctrlServAut->autenticar(*identificador, *senha);
      retorno.set_usuario(usu);
      fim = true;
    }
    catch (const exception &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        resultado = false;
        fim = true;
      }
    }
  }

  retorno.set_resultado(resultado);

  return retorno;
}

/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

/************* CONTROLADORA DE INTERFACE DE USUARIO - USUARIO ***********/
void CtrlIUUsu::setCtrlServ(IServUsu *serv)
{
  ctrl = serv;
}

void CtrlIUUsu::executa()
{
  bool fim = false;
  int opt = 0;

  u = ctrl->buscarUsuario(identificador->get_identificador());

  while (!fim)
  {
    system(CLEAR);
    cout << "|Painel Usuario - " << identificador->get_identificador() << " |" << endl
         << "+--------------------------------+" << endl
         << "|Cadastrar Usuario           - " << CtrlIUUsu::REGISTRAR << " |" << endl
         << "|Remover Usuario             - " << CtrlIUUsu::DEL_USU << " |" << endl
         << "|Editar dados                - " << CtrlIUUsu::EDIT_USU << " |" << endl
         << "|Ver Dados do Usuario        - " << CtrlIUUsu::BUS_USU << " |" << endl
         << "|Cadastrar Conta Corrente    - " << CtrlIUUsu::REG_CONTAC << " |" << endl
         << "|Remover Conta Corrente      - " << CtrlIUUsu::DEL_CONTAC << " |" << endl
         << "|Cadastrar Cartao de Credito - " << CtrlIUUsu::REG_CARTCRED << " |" << endl
         << "|Remover Cartao de Credito   - " << CtrlIUUsu::DEL_CARTCRED << "|" << endl
         << "|Sair                        - 5 |" << endl
         << "+--------------------------------+" << endl
         << "|Selecione a opcao: ";
    cin >> opt;

    switch (opt)
    {
    case CtrlIUUsu::DEL_USU:
      fim = CtrlIUUsu::deletar();
      break;
    case CtrlIUUsu::REGISTRAR:
      CtrlIUUsu::cadastrar();
      break;
    case CtrlIUUsu::EDIT_USU:
      CtrlIUUsu::editar();
      break;
    case CtrlIUUsu::BUS_USU:
      CtrlIUUsu::buscarUsu();
      break;
    case CtrlIUUsu::REG_CONTAC:
      CtrlIUUsu::cadastrarCC();
      break;
    case CtrlIUUsu::DEL_CONTAC:
      CtrlIUUsu::deletarCC();
      break;
    case CtrlIUUsu::REG_CARTCRED:
      CtrlIUUsu::cadastrarCdC();
      break;
    case CtrlIUUsu::DEL_CARTCRED:
      CtrlIUUsu::removerCdc();
      break;
    case 5:
      fim = true;
      break;
    default:
      break;
    }
  }
}

CtrlIUUsu::CtrlIUUsu(string id, string sn)
{
  identificador = new Identificador();
  identificador->set_identificador(id);
  senha = new Senha();
  senha->set_senha(sn);
}

void CtrlIUUsu::buscarUsu()
{
  bool fim = false;
  bool temCdc = true;
  bool temCc = true;
  char resp;
  Cartao_de_Credito *cdc = NULL;
  Conta_corrente *cc = NULL;
  Usuario *us;

  while (!fim)
  {
    try
    {
      us = ctrl->buscarUsuario(identificador->get_identificador());
    }
    catch (const runtime_error &rt)
    {
      throw rt;
    }
    try
    {
      cdc = ctrl->buscarCartao(identificador->get_identificador());
    }
    catch (const runtime_error &rt)
    {
      temCdc = false;
    }
    try
    {
      cc = ctrl->buscarContaCorrente(identificador->get_identificador());
    }
    catch (const runtime_error &rt)
    {
      temCc = false;
    }
    //system(CLEAR);

    cout << "+----------------+" << endl
         << "|Dados do Usuario|" << endl
         << "+----------------+" << endl;
    cout << "|Nome: " << us->get_nome() << endl
         << "|identificador: " << us->get_identificador() << endl;
    cout << "|Conta: ";

    if (temCc)
    {
      cout << cc->get_numero() << endl
           << "    |Agencia: " << cc->get_agencia() << endl
           << "    |Banco: " << cc->get_banco() << endl;
    }
    else
    {
      cout << "nao ha conta cadastrada" << endl;
    }
    cout << "|Cartao de credito: ";
    if (temCdc)
    {
      cout << endl
           << "    |Numero: " << formata_cartao(cdc->get_numero()) << endl
           << "    |Validade: " << cdc->get_data_validade() << endl;
    }
    else
    {
      cout << "nao ha cartao de credito cadastrado";
    }
    cout << endl
         << "pressione enter para voltar ao menu";
    getchar();
    getchar();

    delete cdc;
    delete cc;
    delete us;
    fim = true;
  }
}

void CtrlIUUsu::removerCdc()
{
  bool fim = false;
  char resp;
  Cartao_de_Credito *cdc;

  while (!fim)
  {
    try
    {
      cdc = ctrl->buscarCartao(u->get_identificador());
      cout << "###########################" << endl
           << "#Remover Cartao?          #" << endl
           << "#Numero: " << cdc->get_numero() << " #" << endl
           << "#Data Validade: " << cdc->get_data_validade() << "     #" << endl
           << "##########################"
           << "(s/S/n/N): ";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
      if (resp == 's' || resp == 'S')
      {
        ctrl->deletarCartaodeCredito(u->get_identificador());
        cout << "Cartao removido, pressione enter para retornar ao menu";
        getchar();
        getchar();
      }
      fim = true;
    }
    catch (const exception &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUUsu::cadastrarCdC()
{
  bool fim = false;
  char resp;

  string num, dtVal;
  Numero_Cartao numero;
  Data_Validade data_val;

  while (!fim)
  {
    try
    {
      cout << "|Cadastrar Cartao de Credito" << endl;
      cout << "|Numero:";
      cin >> num;
      numero.set_numero_cartao(num);
      cout << "|Data de Validade:";
      cin >> dtVal;
      data_val.set_data_validade(dtVal);

      system(CLEAR);
      cout << "###############################" << endl
           << "#Confirmar cadastro de Cartao?#" << endl
           << "#Numero: " << num << "     #" << endl
           << "#Data de Validade: " << dtVal << "      #" << endl
           << "###############################"
           << "(s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        ctrl->cadastraCartaodeCredito(u->get_identificador(), numero.get_numero_cartao(), data_val.get_data_validade());
        cout << "Cartao de Credito cadastrado com sucesso, pressione enter para retornar ao menu" << endl;
        getchar();
        getchar();
      }
      fim = true;
    }
    catch (const exception &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUUsu::cadastrarCC()
{
  bool fim = false;
  char resp;
  string nconta;
  int nagencia, nbanco;
  Conta_corrente *conta;
  Numero_Conta_Corrente num_conta;
  Agencia agencia;
  Banco banco;

  while (!fim)
  {
    try
    {

      cout << endl;
      cout << "|Cadastrar Conta Corrente" << endl;
      cout << "|Numero da conta:";
      cin >> nconta;
      num_conta.set_numero_conta_corrente(nconta);

      cout << "|Numero da agencia:";
      cin >> nagencia;
      agencia.set_agencia(nagencia);

      cout << "|Numero banco:";
      cin >> nbanco;
      banco.set_banco(nbanco);

      system(CLEAR);

      cout << "##########################" << endl
           << "#Confirmar Cadastro?     #" << endl
           << "#Numero da conta: " << num_conta.get_numero_conta_corrente() << " #" << endl
           << "#Numero da agencia: " << agencia.get_agencia() << "  #" << endl
           << "#Numero do banco: " << banco.get_banco() << "    #" << endl
           << "##########################" << endl
           << "(s/S/n/N):";

      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
      if (resp == 's' || resp == 'S')
      {
        ctrl->cadastraContaCorrente(u->get_identificador(),
                                    num_conta.get_numero_conta_corrente(),
                                    agencia.get_agencia(),
                                    banco.get_banco());

        cout << "-Conta corrente cadastrada com sucesso. presisone enter para retornar ao menu-" << endl;
        getchar();
        getchar();
        fim = true;
      }
    }
    catch (const exception &ia)
    {
      cout << "-ERRO - " << ia.what() << "-" << endl;
      cout << "-Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUUsu::deletarCC()
{
  char resp;
  bool fim = false;

  while (!fim)
  {
    try
    {

      Conta_corrente *cc = ctrl->buscarContaCorrente(u->get_identificador());
      cout << "##########################" << endl
           << "#Remover Conta Corrente? #" << endl
           << "#Conta - " << cc->get_numero() << "          #" << endl
           << "#Agencia - " << cc->get_agencia() << "           #" << endl
           << "#Banco - " << cc->get_banco() << "             #" << endl
           << "##########################" << endl;
      cout << "(s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
      if (resp == 's' || resp == 'S')
      {
        ctrl->deletarContaCorrente(u->get_identificador());
        cout << "Conta corrente removida com sucesso, pressione enter para retornar ao menu";
        getchar();
        getchar();
      }
      fim = true;
    }
    catch (const exception &e)
    {
      cout << "Erro - " << e.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUUsu::editar()
{
  int alteracoes = 0;
  char resp;
  bool fim = false;

  Nome nome;
  Identificador iden;
  Senha sen;

  string novoNome, novoId, novaSenha;

  while (!fim)
  {
    try
    {
      u = ctrl->buscarUsuario(identificador->get_identificador());
      cout << endl;
      cout << "|Editar Dados" << endl
           << "|Para alterar os dados, escreva-o nos determinados campos,para pula-lo, escreva espaco e em seguida enter|" << endl;
      cout << "|Nome (" << u->get_nome() << "):";

      getline(cin, novoNome);
      getline(cin, novoNome);

      if (novoNome.compare(" ") != 0)
      {
        nome.set_nome(novoNome);
        alteracoes++;
      }

      cout << "|Senha:";
      getline(cin, novaSenha);

      if (novaSenha.compare(" ") != 0)
      {
        sen.set_senha(novaSenha);
        alteracoes++;
      }

      if (alteracoes > 0)
      {

        cout << "#######################" << endl;
        cout << "#Confirmar alteracoes?" << endl;

        if (!nome.get_nome().empty())
        {
          cout << "#Nome: " << nome.get_nome() << endl;
        }
        if (!sen.get_senha().empty())
        {
          cout << "#Senha: " << formata_senha(sen.get_senha()) << endl;
        }
        cout << "#######################" << endl;
        cout << "(s/S/n/N):";
        do
        {
          cin >> resp;
        } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
        if (resp == 's' || resp == 'S')
        {
          ctrl->editarUsuario(u->get_identificador(), nome.get_nome(), sen.get_senha());
          cout << "-Dados editados com sucesso pressione enter para voltar ao menu-" << endl;
          getchar();
          getchar();
        }
      }
      fim = true;
    }
    catch (const invalid_argument &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUUsu::cadastrar()
{
  Identificador id;
  Senha sn;
  Nome n;
  char resp;
  string nome, senha, identificador;
  bool fim = false;
  bool resultado = true;
  while (!fim)
  {
    try
    {
      cout << "Nome:";
      getline(cin, nome);
      getline(cin, nome); //apagando enter
      n.set_nome(nome);
      cout << "Identificador:";
      cin >> identificador;
      id.set_identificador(identificador);
      cout << "Senha:";
      cin >> senha;
      sn.set_senha(senha);

      system(CLEAR);

      cout << "Confirmar cadastro?" << endl
           << "Nome: " << n.get_nome() << "\t"
           << "identificador:" << id.get_identificador() << endl
           << "Senha : " << formata_senha(sn.get_senha()) << endl
           << "(s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        ctrl->cadastrarUsuario(identificador, nome, senha);
        cout << "Usuario cadastrado, pressione enter para retornar ao menu" << endl;
        getchar();
        getchar();
      }
      fim = true;
    }
    catch (const invalid_argument &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        resultado = false;
        fim = true;
      }
    }
  }
}

bool CtrlIUUsu::deletar()
{
  bool fim = false;
  char resp;
  bool resultado = false;

  while (!fim)
  {
    try
    {
      do
      {
        cout << "Confimar remover o usuario - " << u->get_identificador() << "?" << endl
             << "(s/S/n/N):" << endl;
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        resultado = ctrl->deletarUsuario(identificador->get_identificador());

        cout << "Usuario deletado, pressione enter para retornar ao menu" << endl;
        CtrlIUUsu::setIsLogado(false);
        getchar();
        getchar();
      }

      fim = true;
    }
    catch (const exception &ex)
    {
      cout << "ERRO - " << ex.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
  return resultado;
}
/************* CONTROLADORA DE INTERFACE DE USUARIO - USUARIO ***********/

/************* CONTROLADORA DE INTERFACE DE USUARIO - ACOMODACAO ***********/

CtrlIUAcom::CtrlIUAcom(string id)
{
  identificador = new Identificador();
  identificador->set_identificador(id);
}
void CtrlIUAcom::setCtrlServ(IServAcom *c)
{
  ctrl = c;
}

void CtrlIUAcom::executa()
{
  bool fim = false;
  char resp;
  int opt;

  while (!fim)
  {
    try
    {
      system(CLEAR);
      cout << "+--------------------------------------+" << endl
           << "|Cadastrar Acomodacao               - " << CtrlIUAcom::CAD_ACOM << "|" << endl
           << "|Suas Acomodacoes                   - " << CtrlIUAcom::BUS_ACOMS << "|" << endl
           << "|Todas Acomodacoes                  - " << CtrlIUAcom::BUS_TD_ACOMS << "|" << endl
           << "|Remover Acomodacoes                - " << CtrlIUAcom::DEL_ACOM << "|" << endl
           << "|Reservar Acomodacao por um periodo - " << CtrlIUAcom::REG_RES << "|" << endl
           << "|Ver Reservas                       - " << CtrlIUAcom::BUS_RES << "|" << endl
           << "|Remover Reservas                   - " << CtrlIUAcom::DEL_RES << "|" << endl
           << "|Sair                               - 5|" << endl
           << "+--------------------------------------+" << endl
           << "|Opcao:";
      cin >> opt;

      switch (opt)
      {
      case CtrlIUAcom::CAD_ACOM:
        CtrlIUAcom::cadastra();
        break;
      case CtrlIUAcom::BUS_ACOMS:
        CtrlIUAcom::buscarAcoms();
        break;
      case CtrlIUAcom::DEL_ACOM:
        CtrlIUAcom::deletarAcom();
        break;
      case CtrlIUAcom::REG_RES:
        CtrlIUAcom::cadastrarReserva();
        break;
      case CtrlIUAcom::BUS_RES:
        CtrlIUAcom::buscarReservas();
        break;
      case CtrlIUAcom::DEL_RES:
        CtrlIUAcom::deletarReservas();
        break;
      case CtrlIUAcom::BUS_TD_ACOMS:
        CtrlIUAcom::buscarTdAcoms();
        break;
      case 5:
        fim = true;
        break;
      }
    }
    catch (const exception &ex)
    {
      cout << "ERRO - " << ex.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUAcom::buscarTdAcoms()
{
  char resp;
  bool fim = false;
  cout << "TOPZERA" << endl;

  while (!fim)
  {
    try
    {

      vector<Acomodacao> listaAcomdacoes = ctrl->buscarAcomodacoes();

      cout << "+-----------+" << endl
           << "|Acomodacoes|" << endl
           << "+-----------+" << endl
           << endl;

      for (int i = 0; i < listaAcomdacoes.size(); i++)
      {
        cout << "|Acomodacao: " << listaAcomdacoes.at(i).get_titulo() << endl
             << "|Tipo: " << listaAcomdacoes.at(i).get_tipo() << endl
             << "|Cidade: " << listaAcomdacoes.at(i).get_cidade() << "\t |Estado: " << listaAcomdacoes.at(i).get_estado() << endl
             << "|Diaria: " << listaAcomdacoes.at(i).get_diaria() << endl
             << "|Disponibilidade: " << listaAcomdacoes.at(i).get_data_disponibilidade_inicio() << " a " << listaAcomdacoes.at(i).get_data_disponibilidade_fim() << endl
             << "|Dono: " << listaAcomdacoes.at(i).get_identificador() << endl
             << endl;
      }
      getchar();
      getchar();
      fim = true;
    }
    catch (const exception &ex)
    {
      cout << "ERRO - " << ex.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUAcom::deletarReservas()
{
  bool fim = false;
  char resp;
  int opt;
  while (!fim)
  {
    try
    {
      system(CLEAR);

      vector<Reserva> listaReserva = ctrl->buscarReservas(identificador->get_identificador());

      cout << "+----------------+" << endl
           << "|Deletar Reservas|" << endl
           << "+----------------+" << endl;

      for (int i = 0; i < listaReserva.size(); i++)
      {
        cout << "|" << i << "-" << endl
             << "|Titulo: " << listaReserva.at(i).get_titulo() << endl
             << "|Estado: " << listaReserva.at(i).get_estado() << "\t|Cidade: " << listaReserva.at(i).get_cidade() << endl
             << "|Periodo: " << listaReserva.at(i).get_data_inicio() << " a " << listaReserva.at(i).get_data_fim() << endl
             << endl;
      }

      cout << "|Selecione a reserva:";
      cin >> opt;

      Reserva resv = listaReserva.at(opt);

      cout << "#Confirmar remocao de reserva?#" << endl
           << "|Titulo: " << resv.get_titulo() << endl
           << "|Estado: " << resv.get_estado() << endl
           << "|Periodo: " << resv.get_data_inicio() << " a " << resv.get_data_fim() << endl
           << "(s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        ctrl->removerReserva(resv);
        cout << "-Reserva removida com sucesso,pressione enter para retornar ao menu-" << endl;
        getchar();
        getchar();
      }
      fim = true;
    }
    catch (const exception &e)
    {
      cout << "-Erro: " << e.what() << "-" << endl
           << "Deseja tentar novamente?" << endl
           << " (s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
      if (resp == 'n' || resp == 'N')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUAcom::buscarReservas()
{
  bool fim = false;

  while (!fim)
  {
    try
    {
      system(CLEAR);

      vector<Reserva> listaReserva = ctrl->buscarReservas(identificador->get_identificador());

      cout << "+---------------------+" << endl
           << "|Suas Reservas        |" << endl
           << "+---------------------+" << endl;

      for (int i = 0; i < listaReserva.size(); i++)
      {
        cout << "|Titulo: " << listaReserva.at(i).get_titulo() << endl
             << "|Estado: " << listaReserva.at(i).get_estado() << "\t|Cidade: " << listaReserva.at(i).get_cidade() << endl
             << "|Periodo: " << listaReserva.at(i).get_data_inicio() << " a " << listaReserva.at(i).get_data_fim() << endl
             << endl;
      }
      getchar();
      getchar();
      fim = true;
    }
    catch (const exception &e)
    {
      cout << "-Erro :" << e.what() << "-" << endl;
      getchar();
      getchar();
      fim = true;
    }
  }
}

void CtrlIUAcom::cadastrarReserva()
{
  int opt;
  char resp;
  bool fim = false;
  bool fim2 = false;

  string data_inicio, data_fim;
  Acomodacao acom;

  while (!fim)
  {
    try
    {
      vector<Acomodacao> lista = ctrl->buscarAcomodacoesParaReserva(identificador->get_identificador());

      for (int i = 0; i < lista.size(); i++)
      {
        cout << endl
             << "|" << i << "- Acomodacao: " << lista.at(i).get_titulo() << endl
             << "|Tipo: " << get_tipo_acomodacao(lista.at(i).get_tipo()) << endl
             << "|Diaria: " << lista.at(i).get_diaria() << endl
             << "|Disponibilidade: " << lista.at(i).get_data_disponibilidade_inicio() << " a " << lista.at(i).get_data_disponibilidade_fim() << endl
             << "|Dono: " << lista.at(i).get_identificador() << endl
             << endl;
      }
      cout << "|Selecionar acomodacao:";

      do
      {
        cin >> opt;
      } while (opt < 0 && opt > lista.size());

      acom = lista.at(opt);

      while (!fim2)
      {
        try
        {
          cout << "|Acomodacao: " << acom.get_titulo() << endl
               << "|Data de inicio:";
          cin >> data_inicio;
          Data *dt_in = new Data();
          dt_in->set_data(data_inicio);
          delete dt_in;

          cout << "|Data de fim:";
          cin >> data_fim;
          Data *dt_fim = new Data();
          dt_fim->set_data(data_fim);
          delete dt_fim;

          periodo_valido(data_inicio,
                         data_fim,
                         acom.get_data_disponibilidade_inicio(),
                         acom.get_data_disponibilidade_fim());

          //verificar se o usuario tem cartao de credito

          cout << "|Confirmar rserva?" << endl
               << "|Acomodacao: " << acom.get_titulo() << endl
               << "|Data de inicio: " << data_inicio << "\t |Data de fim: " << data_fim << endl
               << "(s/S/n/N):";
          do
          {
            cin >> resp;
          } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
          if (resp == 's' || resp == 'S')
          {
            ctrl->cadastrarReserva(identificador->get_identificador(), acom, data_inicio, data_fim);
            cout << "Reserva feita com sucesso!" << endl;
          }
          fim2 = true;
          fim = true;
        }
        catch (const exception &e)
        {
          cout << endl
               << "-Erro " << e.what() << endl
               << "Deseja continuar a reserva?" << endl
               << "(s/S/n/N):";
          do
          {
            cin >> resp;
          } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
          if (resp == 'n' || resp == 'N')
          {
            fim2 = true;
            fim = true;
          }
        }
      }
    }
    catch (const exception &e)
    {
      cout << "-Erro: " << e.what() << "-" << endl
           << "Deseja tentar novamente?" << endl
           << " (s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
      if (resp == 'n' || resp == 'N')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUAcom::buscarAcoms()
{
  char resp;
  bool fim = false;
  while (!fim)
  {
    try
    {
      system(CLEAR);
      cout << "+-------------------+" << endl
           << "|Suas de Acomodacoes|" << endl
           << "+-------------------+" << endl;

      vector<Acomodacao>
          listaAcomdacoes = ctrl->buscarAcomodacoes(identificador->get_identificador());

      for (int i = 0; i < listaAcomdacoes.size(); i++)
      {
        cout << endl
             << "|Titulo - " << listaAcomdacoes.at(i).get_titulo() << endl
             << "|Tipo - " << listaAcomdacoes.at(i).get_tipo() << endl
             << "|Capacidade -  " << listaAcomdacoes.at(i).get_capacidade() << endl
             << "|Estado - " << listaAcomdacoes.at(i).get_estado() << endl
             << "|Cidade - " << listaAcomdacoes.at(i).get_cidade() << endl
             << "|Diaria - R$ " << listaAcomdacoes.at(i).get_diaria() << endl
             << "|Disponibilidade - " << listaAcomdacoes.at(i).get_data_disponibilidade_inicio() << " a " << listaAcomdacoes.at(i).get_data_disponibilidade_fim() << endl;
      }
      getchar();
      getchar();
      fim = true;
    }
    catch (const exception &e)
    {
      cout << "-Erro :" << e.what() << "-" << endl;
      getchar();
      getchar();
      fim = true;
    }
  }
}

void CtrlIUAcom::deletarAcom()
{
  int opt;
  char resp;
  bool fim = false;

  while (!fim)
  {
    try
    {
      system(CLEAR);

      vector<Acomodacao> listaAcomdacoes = ctrl->buscarAcomodacoes(identificador->get_identificador());

      for (int i = 0; i < listaAcomdacoes.size(); i++)
      {
        cout << "+--------------+" << endl
             << "|Acomodacao -" << i << "|" << endl
             << "+--------------+" << endl
             << "|Titulo - " << listaAcomdacoes.at(i).get_titulo() << endl;
      }
      cout << "Selecione a acomodacao pelo numero:";

      do
      {
        cin >> opt;
      } while (opt < 0 && opt > listaAcomdacoes.size());

      Acomodacao acom = listaAcomdacoes.at(opt);

      cout << "#Confirmar remocao de acomodacao?#" << endl
           << "|Acomodacao - " << opt << endl
           << "|Titulo - " << acom.get_titulo() << endl
           << "(s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        ctrl->removerAcomodacao(acom);
        cout << "-Acomodacao removida com sucesso,pressione enter para retornar ao menu-" << endl;
        getchar();
        getchar();
      }

      fim = true;
    }
    catch (const exception &e)
    {
      cout << "-Erro: " << e.what() << "-" << endl
           << "Deseja tentar novamente?" << endl
           << " (s/S/n/N):";
      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
      if (resp == 'n' || resp == 'N')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUAcom::cadastra()
{
  char resp;
  bool fim = false;

  string titulo_entrada,
      dt_in_entrada,
      dt_fim_entrada,
      cidade_entrada,
      estado_entrada;

  int tipo_entrada;

  int capacidade_entrada;
  float diaria_entrada;

  while (!fim)
  {
    try
    {
      if (!ctrl->podeCadastrarAcomodacao(identificador->get_identificador()))
        throw runtime_error("Para cadastrar uma acomodacao, primeiro cadastre uma conta corrente");

      system(CLEAR);
      cout << "+--------------------+" << endl
           << "|Cadastrar Acomodacao|" << endl
           << "+--------------------+" << endl;

      cout << "|Titulo da Acomodacao (Visibilidade eh a chave do negocio):";
      getline(cin, titulo_entrada);
      getline(cin, titulo_entrada);

      if (titulo_entrada.empty() || titulo_entrada.compare(" ") == 0)
        throw runtime_error("Titulo nao pode ser apenas um espaco vazio.");

      cout << "|Capacidade da Acomodacao:";
      cin >> capacidade_entrada;
      Capacidade_Acomodacao *cap = new Capacidade_Acomodacao();
      cap->set_capacidade_acomodacao(capacidade_entrada);
      delete cap;

      cout << "|Tipo de Acomodacao:" << endl
           << "+---------------+" << endl
           << "|Apartamento - 1|" << endl
           << "|Casa        - 2|" << endl
           << "|Flat        - 3|" << endl
           << "+---------------+" << endl
           << "|Opcao:";
      do
      {
        cin >> tipo_entrada;
      } while (tipo_entrada < 0 || tipo_entrada > 3);

      cout << "|Cidade:";
      getline(cin, cidade_entrada);
      getline(cin, cidade_entrada);
      Nome *nome = new Nome();
      nome->set_nome(cidade_entrada);
      delete nome;

      cout << "|Estado:";
      cin >> estado_entrada;
      Estado *estado = new Estado();
      estado->set_estado(estado_entrada);
      delete estado;

      cout << "|Diaria:";
      cin >> diaria_entrada;
      Diaria *diaria = new Diaria();
      diaria->set_diaria(diaria_entrada);
      delete diaria;

      cout << "|Disponibilidade:" << endl
           << "|Data de inicio:";
      cin >> dt_in_entrada;
      Data *dt_in = new Data();
      dt_in->set_data(dt_in_entrada);
      delete dt_in;

      cout << "|Data de fim:";
      cin >> dt_fim_entrada;
      Data *dt_fim = new Data();
      dt_fim->set_data(dt_fim_entrada);
      delete dt_fim;

      periodo_valido(dt_in_entrada, dt_fim_entrada);

      cout << endl
           << "#######################" << endl
           << "#Confirmar cadastro?" << endl
           << "#Titulo: " << titulo_entrada << endl
           << "#Capacidade: " << capacidade_entrada << endl
           << "#Diaria: R$" << diaria_entrada << endl
           << "#Estado: " << estado_entrada << endl
           << "#Tipo: " << tipo_entrada << endl
           << "#Disponibilidade: " << dt_in_entrada << " a " << dt_fim_entrada << endl
           << "#######################" << endl
           << "(s/S/n/N):";

      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        Acomodacao acom(titulo_entrada,
                        capacidade_entrada,
                        cidade_entrada,
                        diaria_entrada,
                        estado_entrada,
                        identificador->get_identificador(),
                        tipo_entrada,
                        dt_in_entrada,
                        dt_fim_entrada);

        ctrl->cadastrarAcomodacao(acom);
        cout << "-Cadastro de acomodacao feito com sucesso, pressione enter para retornar ao menu-" << endl;
        getchar();
        getchar();
      }
      fim = true;
    }
    catch (const exception &ex)
    {
      cout << "ERRO - " << ex.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

/************* CONTROLADORA DE INTERFACE DE USUARIO - ACOMODACAO ***********/

/************* CONTROLADORA DE SERVICOS - ACOMODACAO *************/

bool CtrlServAcom::podeReservar(string id)
{
  int rc;
  bool resultado = false;

  string SQL_SELECT_CARTAO = "SELECT * FROM CARTAO WHERE id_usuario = '" + id + "';";

  rc = CtrlServAcom::executa(SQL_SELECT_CARTAO);

  if (trata_retorno(rc))
  {
    sqlite3_stmt *stmt = CtrlServ::get_stmt();
    resultado = atoi((char *)sqlite3_column_text(stmt, 0)) > 0;
  }

  CtrlServ::finaliza();
  return resultado;
}

static int callbackAcomodacoes(void *data, int argc, char **argv, char **azColName)
{
  Acomodacao acom;

  if (argc == 0)
    throw runtime_error("Nao existem acomodacoes cadastradas");

  vector<Acomodacao> *lista = reinterpret_cast<vector<Acomodacao> *>(data);

  acom.set_id_acomodacao(atoi(argv[0]));
  acom.set_tipo(atoi(argv[1]));
  acom.set_capacidade(atoi(argv[2]));
  acom.set_cidade(argv[3]);
  acom.set_estado(argv[4]);
  acom.set_diaria(atof(argv[5]));
  acom.set_identificador(argv[6]);
  acom.set_titulo(argv[7]);
  acom.set_data_disponibilidade_inicio(argv[8]);
  acom.set_data_disponibilidade_fim(argv[9]);
  lista->push_back(acom);

  return 0;
}

vector<Acomodacao> CtrlServAcom::buscarAcomodacoesParaReserva(string id)
{
  int rc;
  sqlite3_stmt *stmt;
  const char *data;
  char *zErrMsg = 0;

  vector<Acomodacao> lista;

  if (!CtrlServAcom::podeReservar(id))
    throw runtime_error("Para fazer uma reserva eh necessario o cadastro de um cartao de credito.");

  string SQL_SELECT_ACOMODACOES = "SELECT * FROM ACOMODACAO where dono != '" + id + "';";

  sqlite3 *banco = CtrlServ::get_banco();

  sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  rc = sqlite3_exec(banco, SQL_SELECT_ACOMODACOES.c_str(), callbackAcomodacoes, &lista, &zErrMsg);

  if (lista.size() == 0)
    throw runtime_error("Nao existem Acomodacoes para reserva");

  return lista;
}

vector<Acomodacao> CtrlServAcom::buscarAcomodacoes()
{
  int rc;
  sqlite3_stmt *stmt;
  const char *data;
  char *zErrMsg = 0;

  vector<Acomodacao> lista;

  string SQL_SELECT_ACOMODACAO = "SELECT * FROM ACOMODACAO;";

  sqlite3 *banco = CtrlServ::get_banco();

  sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  rc = sqlite3_exec(banco, SQL_SELECT_ACOMODACAO.c_str(), callbackAcomodacoes, &lista, &zErrMsg);

  return lista;
}

vector<Acomodacao> CtrlServAcom::buscarAcomodacoes(string id)
{
  int rc;
  sqlite3_stmt *stmt;
  const char *data;
  char *zErrMsg = 0;

  vector<Acomodacao> lista;

  string SQL_SELECT_ACOMODACOES = "SELECT * FROM ACOMODACAO WHERE ";
  SQL_SELECT_ACOMODACOES += "dono = '" + id + "';";

  sqlite3 *banco = CtrlServ::get_banco();

  sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  rc = sqlite3_exec(banco, SQL_SELECT_ACOMODACOES.c_str(), callbackAcomodacoes, &lista, &zErrMsg);

  if (lista.size() == 0)
    throw runtime_error("Nao existem Acomodacoes");

  return lista;
}

void CtrlServAcom::removerAcomodacao(Acomodacao acom)
{
  int rc;

  string SQL_DELETE_ACOM = "DELETE FROM ACOMODACAO WHERE ";
  SQL_DELETE_ACOM += "id = " + to_string(acom.get_id_acomodacao()) + " and ";
  SQL_DELETE_ACOM += "dono = '" + acom.get_identificador() + "' and ";
  SQL_DELETE_ACOM += "titulo = '" + acom.get_titulo() + "';";

  rc = CtrlServ::executa(SQL_DELETE_ACOM);

  trata_retorno(rc);
}

bool CtrlServAcom::existeAcomodacao(string titulo, string id)
{
  int rc;
  bool resultado = false;

  string SQL_SELECT_ACOMODACAO = "SELECT COUNT(*) FROM ACOMODACAO WHERE ";
  SQL_SELECT_ACOMODACAO += "titulo = '" + titulo + "' and ";
  SQL_SELECT_ACOMODACAO += "dono = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_ACOMODACAO);

  if (trata_retorno(rc))
  {
    sqlite3_stmt *stmt = CtrlServ::get_stmt();

    int num = atoi((char *)sqlite3_column_text(stmt, 0));
    resultado = num > 0;
  }

  CtrlServ::finaliza();
  return resultado;
}

bool CtrlServAcom::podeCadastrarAcomodacao(string id)
{
  int rc;
  bool resultado = false;

  string SQL_CADASTRAR_ACOMODACAO = "SELECT count(*) from Usuario as u, Contacorrente as cc  where ";
  SQL_CADASTRAR_ACOMODACAO += "u.identificador = '" + id + "' and u.identificador = cc.id_usuario;";

  rc = CtrlServ::executa(SQL_CADASTRAR_ACOMODACAO);

  if (trata_retorno(rc))
  {
    sqlite3_stmt *stmt = CtrlServ::get_stmt();
    int num = atoi((char *)sqlite3_column_text(stmt, 0));

    resultado = num == 1;
  }

  return resultado;
}

void CtrlServAcom::cadastrarAcomodacao(Acomodacao acom)
{
  int rc;

  if (CtrlServAcom::existeAcomodacao(acom.get_titulo(), acom.get_identificador()))
    throw runtime_error("Ja existe uma acomodacao com este titulo");

  string SQL_INSERT_ACOM = "INSERT INTO ACOMODACAO (titulo,tipo,capacidade,cidade,estado,diaria,dono,dt_dis_in,dt_dis_fim) VALUES(";
  SQL_INSERT_ACOM += "'" + acom.get_titulo() + "',";
  SQL_INSERT_ACOM += to_string(acom.get_tipo()) + ",";
  SQL_INSERT_ACOM += to_string(acom.get_capacidade()) + ",";
  SQL_INSERT_ACOM += "'" + acom.get_cidade() + "',";
  SQL_INSERT_ACOM += "'" + acom.get_estado() + "',";
  SQL_INSERT_ACOM += "'" + to_string(acom.get_diaria()) + "',";
  SQL_INSERT_ACOM += "'" + acom.get_identificador() + "',";
  SQL_INSERT_ACOM += "'" + acom.get_data_disponibilidade_inicio() + "',";
  SQL_INSERT_ACOM += "'" + acom.get_data_disponibilidade_fim() + "');";

  rc = CtrlServ::executa(SQL_INSERT_ACOM);

  trata_retorno(rc);
  CtrlServ::finaliza();
}

bool CtrlServAcom::existeReserva(int id_acom, string dt_inicio, string dt_fim)
{
  int rc;
  bool resultado = false;

  string SQL_SELECT_RESERVA = "SELECT COUNT(*) FROM RESERVA WHERE ";
  SQL_SELECT_RESERVA += "id_acomodacao = " + to_string(id_acom) + " and (";
  SQL_SELECT_RESERVA += "data_inicio = '" + dt_inicio + "' or ";
  SQL_SELECT_RESERVA += "data_fim = '" + dt_fim + "');";

  rc = CtrlServ::executa(SQL_SELECT_RESERVA);

  if (trata_retorno(rc))
  {
    sqlite3_stmt *stmt = CtrlServ::get_stmt();
    resultado = atoi((char *)sqlite3_column_text(stmt, 0)) > 0;
  }
  CtrlServ::finaliza();
  return resultado;
}

void CtrlServAcom::cadastrarReserva(string id, Acomodacao acom, string data_inicio, string data_fim)
{
  int rc;

  if (CtrlServAcom::existeReserva(acom.get_id_acomodacao(), data_inicio, data_fim))
    throw runtime_error("Ja existe uma reserva para esta acomodacao no determinado periodo");

  string SQL_INSERT_RESERVA = "INSERT INTO RESERVA (id_usuario,id_acomodacao,data_inicio,data_fim) VALUES(";
  SQL_INSERT_RESERVA += "'" + id + "',";
  SQL_INSERT_RESERVA += "'" + to_string(acom.get_id_acomodacao()) + "',";
  SQL_INSERT_RESERVA += "'" + data_inicio + "',";
  SQL_INSERT_RESERVA += "'" + data_fim + "');";

  rc = CtrlServ::executa(SQL_INSERT_RESERVA);
  trata_retorno(rc);
  CtrlServ::finaliza();
}

static int callbackReservas(void *data, int argc, char **argv, char **azColName)
{
  Reserva resv;

  if (argc == 0)
    throw runtime_error("Nao existem reservas");

  vector<Reserva> *lista = reinterpret_cast<vector<Reserva> *>(data);

  resv.set_id_reserva(atoi(argv[0]));
  resv.set_titulo(argv[1]);
  resv.set_data_inicio(argv[2]);
  resv.set_data_fim(argv[3]);
  resv.set_cidade(argv[4]);
  resv.set_estado(argv[5]);

  lista->push_back(resv);

  return 0;
}

vector<Reserva> CtrlServAcom::buscarReservas(string id)
{
  int rc;
  vector<Reserva> lista;
  char *zErrMsg = 0;

  string SQL_SELECT_RESERVA = "select rer.id,acom.titulo,rer.data_inicio,rer.data_fim,acom.cidade,acom.estado from RESERVA rer,ACOMODACAO acom where";
  SQL_SELECT_RESERVA += " id_usuario = '" + id + "'";
  SQL_SELECT_RESERVA += " and rer.id_acomodacao = acom.id;";

  sqlite3 *banco = CtrlServ::get_banco();

  sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  rc = sqlite3_exec(banco, SQL_SELECT_RESERVA.c_str(), callbackReservas, &lista, &zErrMsg);

  if (lista.size() == 0)
    throw runtime_error("Nao existem reservas feitas");

  return lista;
}

void CtrlServAcom::removerReserva(Reserva resv)
{
  int rc;

  string SQL_DELETE_RESERVA = "DELETE FROM RESERVA WHERE ";
  SQL_DELETE_RESERVA += "id = " + to_string(resv.get_id_reserva()) + ";";

  rc = CtrlServ::executa(SQL_DELETE_RESERVA);
  trata_retorno(rc);
  CtrlServ::finaliza();
}

/************* CONTROLADORA DE SERVICOS - ACOMODACAO *************/

/************* CONTROLADORA DE SERVICOS - USUARIO *************/

void CtrlServUsu::deletarCartaodeCredito(string id)
{
  int rc;

  if (!CtrlServUsu::existeCartaodeCredito(id))
    throw runtime_error("Nao existe cartao cadastrado");

  string SQL_DELETE_CARTAO = "DELETE FROM CARTAO WHERE id_usuario = '" + id + "';";

  rc = CtrlServ::executa(SQL_DELETE_CARTAO);
  trata_retorno(rc);
  CtrlServ::finaliza();
}

void CtrlServUsu::cadastraCartaodeCredito(string id, string numero, string dt_validade)
{
  int rc;

  if (CtrlServUsu::existeCartaodeCredito(id))
    throw runtime_error("Ja existe um cartao cadastrado, remova-o para cadastrar outro");

  string SQL_INSERT_CARTAO = "INSERT INTO CARTAO (numero, dt_validade,id_usuario) values(";
  SQL_INSERT_CARTAO += "'" + numero + "',";
  SQL_INSERT_CARTAO += "'" + dt_validade + "',";
  SQL_INSERT_CARTAO += "'" + id + "');";

  rc = CtrlServ::executa(SQL_INSERT_CARTAO);
  trata_retorno(rc);
  CtrlServ::finaliza();
}

bool CtrlServUsu::existeCartaodeCredito(string id)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;

  string SQL_SELECT_CARTAO = "SELECT COUNT(*) FROM CARTAO WHERE ";
  SQL_SELECT_CARTAO += "id_usuario = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_CARTAO);

  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    int num = atoi((char *)sqlite3_column_text(stmt, 0));

    resultado = num > 0;
  }
  CtrlServ::finaliza();
  return resultado;
}

Cartao_de_Credito *CtrlServUsu::buscarCartao(string id)
{
  int rc;
  Cartao_de_Credito *cdc;
  sqlite3_stmt *stmt;

  if (!CtrlServUsu::existeCartaodeCredito(id))
    throw runtime_error("Nao existe cartao cadastrado");

  string SQL_SELECT_CARTAO = "SELECT * FROM CARTAO WHERE ";
  SQL_SELECT_CARTAO += "id_usuario = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_CARTAO);
  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    string numero = (char *)sqlite3_column_text(stmt, 1);
    string dt_val = (char *)sqlite3_column_text(stmt, 2);

    cdc = new Cartao_de_Credito(numero, dt_val);
  }

  return cdc;
}

Conta_corrente *CtrlServUsu::buscarContaCorrente(string id)
{
  int rc;
  Conta_corrente *cc;
  sqlite3_stmt *stmt;

  if (!CtrlServUsu::existeContaCorrente(id))
    throw runtime_error("Nao existe conta corrente cadastrada para este usuario");

  string SQL_SELECT_CC = "SELECT * FROM CONTACORRENTE WHERE ";
  SQL_SELECT_CC += "id_usuario = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_CC);
  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();

    string nnum((char *)sqlite3_column_text(stmt, 1));
    int nagencia = atoi((char *)sqlite3_column_text(stmt, 2));
    int nbanco = atoi((char *)sqlite3_column_text(stmt, 3));

    cc = new Conta_corrente(nagencia, nbanco, nnum);
  }
  CtrlServ::finaliza();
  return cc;
}

Usuario *CtrlServUsu::buscarUsuario(string id)
{
  int rc;
  Usuario *u;
  sqlite3_stmt *stmt;

  if (!CtrlServUsu::existeUsuario(id))
    throw runtime_error("Usuario nao encontrado");

  string SQL_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE ";
  SQL_SELECT_USUARIO += "identificador = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_USUARIO);

  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    string id((char *)sqlite3_column_text(stmt, 1));
    string nome((char *)sqlite3_column_text(stmt, 2));
    string senha((char *)sqlite3_column_text(stmt, 3));

    u = new Usuario(id, nome, senha);
  }
  CtrlServ::finaliza();
  return u;
}

void CtrlServUsu::editarUsuario(string identificador, string nome, string senha)
{
  int rc;

  string SQL_EDIT_USUARIO = "UPDATE Usuario set ";

  if (!nome.empty())
  {
    SQL_EDIT_USUARIO += "nome = '" + nome + "' ";
  }

  if (!nome.empty() && !senha.empty())
  {
    SQL_EDIT_USUARIO += " and ";
  }

  if (!senha.empty())
  {
    SQL_EDIT_USUARIO += "senha = '" + senha + "' ";
  }

  SQL_EDIT_USUARIO += "where identificador = '" + identificador + "';";

  rc = CtrlServ::executa(SQL_EDIT_USUARIO);
  trata_retorno(rc);
}

bool CtrlServUsu::deletarUsuario(string id_usu)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;

  if (!CtrlServUsu::existeUsuario(id_usu))
    throw runtime_error("O Usuario nao existe!");

  if (CtrlServUsu::existeReservaEmAcomodacao(id_usu))
    throw runtime_error("Existem reservas em acomodacoes do usuario");

  if (CtrlServUsu::existeContaCorrente(id_usu))
    CtrlServUsu::deletarContaCorrente(id_usu);

  if (CtrlServUsu::existeCartaodeCredito(id_usu))
    CtrlServUsu::deletarCartaodeCredito(id_usu);

  //verificar se tem: reserva, acomodacao

  string SQL_DEL_USUARIO = "DELETE FROM Usuario WHERE ";
  SQL_DEL_USUARIO += "identificador = '" + id_usu + "'";

  rc = CtrlServ::executa(SQL_DEL_USUARIO);
  resultado = trata_retorno(rc);

  return resultado;
}

void CtrlServUsu::deletarContaCorrente(string id)
{
  int rc;

  if (!CtrlServUsu::existeContaCorrente(id))
    throw runtime_error("Nao existe conta cadastrada");

  string SQL_DELETE_CC = "DELETE FROM CONTACORRENTE WHERE ";
  SQL_DELETE_CC += "id_usuario = '" + id + "';";

  rc = CtrlServ::executa(SQL_DELETE_CC);
  trata_retorno(rc);
}

bool CtrlServUsu::existeReservaEmAcomodacao(string id)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;

  string SQL_SELECT_ACOMODACAO_RESERVA = "SELECT COUNT(*) FROM ACOMODACAO as acom, RESERVA as resv WHERE ";
  SQL_SELECT_ACOMODACAO_RESERVA += "acom.dono = '" + id + "' and acom.id = resv.id_acomodacao;";

  rc = CtrlServ::executa(SQL_SELECT_ACOMODACAO_RESERVA);

  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    int num = atoi((char *)sqlite3_column_text(stmt, 0));
    resultado = num > 0;
  }
  CtrlServ::finaliza();
  return resultado;
}

bool CtrlServUsu::existeReserva(string id)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;

  string SQL_SELECT_RESERVA = "SELECT COUNT(*) FROM RESERVA WHERE ";
  SQL_SELECT_RESERVA += "id_usuario = '" + id + "'";

  rc = CtrlServ::executa(SQL_SELECT_RESERVA);

  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    int num = atoi((char *)sqlite3_column_text(stmt, 0));

    resultado = num > 0;
  }
  return resultado;
}

bool CtrlServUsu::existeContaCorrente(string id)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;

  string SQL_SELECT_CC = "SELECT COUNT(*) FROM CONTACORRENTE WHERE ";
  SQL_SELECT_CC += "id_usuario = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_CC);

  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    int num = atoi(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));

    resultado = num > 0;
  }
  CtrlServ::finaliza();
  return resultado;
}

bool CtrlServUsu::existeUsuario(string id)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;

  //comando de selecao que conta quantos usuarios existem com o identificador dado
  //deve ser retornado 0 ou 1
  string SQL_SELECT_USUARIO = "SELECT COUNT(*) FROM USUARIO WHERE ";
  SQL_SELECT_USUARIO += "identificador = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_USUARIO);

  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();

    int num = atoi((char *)sqlite3_column_text(stmt, 0));
    resultado = num > 0;
  }
  CtrlServ::finaliza();
  return resultado;
}

void CtrlServUsu::cadastrarUsuario(string id, string nome, string senha)
{
  int rc;
  sqlite3_stmt *stmt;

  string SQL_INSERT_USUARIO = "INSERT INTO USUARIO (identificador,nome,senha) VALUES (";
  SQL_INSERT_USUARIO += "'" + id + "',";
  SQL_INSERT_USUARIO += "'" + nome + "',";
  SQL_INSERT_USUARIO += "'" + senha + "');";

  if (CtrlServUsu::existeUsuario(id))
    throw invalid_argument("Usuario ja Cadastrado!");

  rc = CtrlServ::executa(SQL_INSERT_USUARIO);

  trata_retorno(rc);
  CtrlServ::finaliza();
}

void CtrlServUsu::cadastraContaCorrente(string id, string nconta, int nagencia, int nbanco)
{
  int rc;
  sqlite3_stmt *stmt;

  if (!CtrlServUsu::existeUsuario(id))
    throw runtime_error("Usuario nao existe");

  if (CtrlServUsu::existeContaCorrente(id))
    throw runtime_error("Ja existe uma conta cadastrada,remova-a para cadastrar uma nova");

  string SQL_INSERT_CC = "INSERT INTO CONTACORRENTE (ID_USUARIO,NUMERO,AGENCIA,BANCO) VALUES(";
  SQL_INSERT_CC += "'" + id + "',";
  SQL_INSERT_CC += "'" + nconta + "',";
  SQL_INSERT_CC += to_string(nagencia) + ",";
  SQL_INSERT_CC += to_string(nbanco) + ");";

  rc = CtrlServ::executa(SQL_INSERT_CC);

  trata_retorno(rc);
  CtrlServ::finaliza();
}
/************* CONTROLADORA DE SERVICOS DE USUARIO - *************/

/************* CONTROLADORA DE SERVICOS DE AUTENTICACAO - USUARIO - *************/
Usuario *CtrlServAut::autenticar(Identificador &id, Senha &senha)
{
  int rc;
  Usuario *usu;
  sqlite3 *banco = CtrlServ::get_banco();

  vector<vector<string>> result;

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  rc = sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  string SQL_STMT_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE ";
  SQL_STMT_SELECT_USUARIO += "identificador = '" + id.get_identificador() + "'";
  SQL_STMT_SELECT_USUARIO += " AND ";
  SQL_STMT_SELECT_USUARIO += "senha = '" + senha.get_senha() + "';";

  sqlite3_prepare_v2(banco, SQL_STMT_SELECT_USUARIO.c_str(), -2, &stmt, NULL);
  sqlite3_reset(stmt);

  rc = sqlite3_step(stmt);

  if (trata_retorno(rc) && sqlite3_data_count(stmt) > 0)
  {

    string id_str((char *)sqlite3_column_text(stmt, 1));
    string nome_str((char *)sqlite3_column_text(stmt, 2));
    string senha_str((char *)sqlite3_column_text(stmt, 3));

    usu = new Usuario(id_str, nome_str, senha_str);
  }
  else
  {
    throw runtime_error("Usuario nao cadastrado");
  }

  sqlite3_finalize(stmt);
  sqlite3_close(banco);
  return usu;
}
/************* CONTROLADORA DE SERVICOS DE AUTENTICACAO - USUARIO - *************/

/****CONTROLADORA DE SERVICOS BASE ********/

bool CtrlServ::banco_criado = false;

int CtrlServ::executa(string sql)
{
  int rc;
  sqlite3 *banco = CtrlServ::get_banco();

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  sqlite3_prepare_v2(banco, sql.c_str(), -2, &stmt, NULL);

  rc = sqlite3_step(stmt);

  sqlite3_close(banco);
  return rc;
}

void CtrlServ::finaliza()
{
  sqlite3_finalize(stmt);
  sqlite3_close(banco);
}

bool CtrlServ::init_banco()
{
  bool resultado = true;
  sqlite3_stmt *stmt;
  vector<char *> sql;

  if (!CtrlServ::banco_criado)
  {

    sql.push_back(SQL_STMT_CREATE_USUARIO);
    sql.push_back(SQL_STMT_CREATE_ACOMODACAO);
    sql.push_back(SQL_STMT_CREATE_CARTAO);
    sql.push_back(SQL_STMT_CREATE_CONTACORRENTE);
    sql.push_back(SQL_STMT_CREATE_RESERVA);

    //inicia o banco
    if (sqlite3_open(NOME_BD, &banco) != SQLITE_OK)
    {
      throw sqlite3_errmsg(banco);
    }

    try
    {
      //inicia o banco de dados com as tabelas
      for (int i = 0; i < sql.size(); i++)
      {
        sqlite3_prepare_v2(banco, sql.at(i), -2, &stmt, NULL);

        if (sqlite3_step(stmt) == SQLITE_ERROR)
          throw sqlite3_errmsg(banco);
      }
      //insere usuario Administrador
      sqlite3_prepare_v2(banco, SQL_INSERT_USUARIO_ADMIN, -2, &stmt, NULL);

      if (sqlite3_step(stmt) == SQLITE_ERROR)
        throw sqlite3_errmsg(banco);
    }
    catch (char *msg)
    {
      cout << msg << endl;
      sqlite3_free(msg);
      resultado = false;
    }

    sqlite3_close(banco);
    banco_criado = true;
  } //banco ja criado
  return resultado;
}
