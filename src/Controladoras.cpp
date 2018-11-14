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

/************* SCRIPTS CRIACAO DO BANCO ************/
char *SQL_STMT_CREATE_USUARIO = "CREATE TABLE IF NOT EXISTS `USUARIO` ( `ID` INTEGER, `IDENTIFICADOR` TEXT NOT NULL UNIQUE, `NOME` TEXT NOT NULL, `SENHA` TEXT NOT NULL, PRIMARY KEY(`IDENTIFICADOR`,`ID`));";
char *SQL_STMT_CREATE_ACOMODACAO = "CREATE TABLE IF NOT EXISTS `ACOMODACAO` (`id`	INTEGER,`tipo`	INTEGER NOT NULL,`capacidade`	INTEGER NOT NULL,`cidade`	TEXT NOT NULL,`estado`	INTEGER NOT NULL,`diaria`	TEXT NOT NULL,`dono`	TEXT,PRIMARY KEY(`dono`));";
char *SQL_STMT_CREATE_CARTAO = "CREATE TABLE IF NOT EXISTS 'CARTAO' ( `ID` INTEGER ,`NUMERO` TEXT NOT NULL, `DT_VALIDADE` TEXT NOT NULL, `ID_USUARIO` TEXT NOT NULL );";
char *SQL_STMT_CREATE_CONTACORRENTE = "CREATE TABLE IF NOT EXISTS 'CONTACORRENTE' ( `ID` INTEGER , `NUMERO` TEXT NOT NULL, `AGENCIA` INTEGER NOT NULL, `BANCO` INTEGER NOT NULL, `ID_USUARIO` TEXT NOT NULL );";
char *SQL_STMT_CREATE_RESERVA = "CREATE TABLE IF NOT EXISTS `RESERVA` ( `id` INTEGER , `id_usuario` INTEGER NOT NULL, `id_acomodacao` INTEGER NOT NULL, `data_incio` TEXT NOT NULL, `data_fim` TEXT NOT NULL );";
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
      system("cls"); //windows

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

/************* CONTROLADORA DE SERVICOS *************/

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

/************* CONTROLADORA DE SERVICOS *************/

/************* CONTROLADORA DE INTERFACE DE USUARIO - USUARIO ***********/
void CtrlIUUsu::setCtrlServ(IServUsu *serv)
{
  ctrl = serv;
}

void CtrlIUUsu::executa()
{
  int opt = 0;

  u = ctrl->buscarUsuario(identificador->get_identificador());

  while (opt != 5)
  {
    system("cls");
    cout << "|Painel Usuario - " << identificador->get_identificador() << " |" << endl;
    cout << "+-------------------------------+" << endl;
    cout << "|Cadastrar Usuario           - " << CtrlIUUsu::REGISTRAR << "|" << endl;
    cout << "|Remover Usuario             - " << CtrlIUUsu::DEL_USU << "|" << endl;
    cout << "|Editar dados                - " << CtrlIUUsu::EDIT_USU << "|" << endl;
    cout << "|Cadastrar Conta corrente    - " << CtrlIUUsu::REG_CONTAC << "|" << endl;
    cout << "|Remover Conta corrente      - " << CtrlIUUsu::DEL_CONTAC << "|" << endl;
    cout << "|Cadastrar Cartao de Credito - " << CtrlIUUsu::REG_CARTCRED << "|" << endl;
    cout << "|Remover Cartao de Credito   - " << CtrlIUUsu::DEL_CARTCRED << "|" << endl;
    cout << "|Sair                        - 5|" << endl;
    cout << "+-------------------------------+" << endl;
    cout << "|Selecione a opcao: ";
    cin >> opt;

    switch (opt)
    {
    case CtrlIUUsu::DEL_USU:
      CtrlIUUsu::deletar();
      break;
    case CtrlIUUsu::REGISTRAR:
      CtrlIUUsu::cadastrar();
      break;
    case CtrlIUUsu::EDIT_USU:
      CtrlIUUsu::editar();
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

void CtrlIUUsu::removerCdc()
{
  bool fim = false;
  char resp;
  Cartao_de_Credito *cdc;

  while (!fim)
  {
    try
    {
      cdc = ctrl->buscar_cartao(u->get_identificador());
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

      system("cls");
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

      system("cls");

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

      system("cls");

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

void CtrlIUUsu::deletar()
{
  bool fim = false;
  char resp;
  Identificador id;
  string id_del;

  while (!fim)
  {
    try
    {
      cout << "identificador do Usuario a ser deletado:";
      cin >> id_del;
      id.set_identificador(id_del);

      do
      {
        cout << "Confimar remover o usuario - " << id_del << "?" << endl
             << "(s/S/n/N):" << endl;
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        ctrl->deletarUsuario(identificador->get_identificador(), id_del);

        cout << "Usuario deletado, pressione enter para retornar ao menu" << endl;
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
      system("cls");
      cout << "+--------------------------+" << endl
           << "|Cadastrar Acomodacao   - " << CtrlIUAcom::CAD_ACOM << "|" << endl
           << "|Sair                   - 5|" << endl
           << "+--------------------------+" << endl
           << "|Opcao:";
      cin >> opt;

      switch (opt)
      {
      case CtrlIUAcom::CAD_ACOM:
        cout << "CADASTRA" << endl;

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

void CtrlIUAcom::cadastra()
{
}
/************* CONTROLADORA DE INTERFACE DE USUARIO - ACOMODACAO ***********/

/************* CONTROLADORA DE SERVICOS - ACOMODACAO *************/

void CtrlServAcom::cadastrarAcomodacao(string id, Acomodacao acom)
{
  int rc;

  //TODO: Existe Acomodacao()

  string SQL_INSERT_ACOM = "INSERT INTO ACOMODACAO (tipo,capacidade,cidade,estado,diaria,dono) VALUES(";
  SQL_INSERT_ACOM += acom.get_tipo() + ",";
  SQL_INSERT_ACOM += acom.get_capacidade() + ",";
  SQL_INSERT_ACOM += "'" + acom.get_cidade() + "',";
  SQL_INSERT_ACOM += "'" + acom.get_estado() + "',";
  SQL_INSERT_ACOM += "'" + to_string(acom.get_diaria()) + "',";
  SQL_INSERT_ACOM += "'" + id + "');";

  rc = CtrlServ::executa(SQL_INSERT_ACOM);

  trata_retorno(rc);
}

void CtrlServAcom::buscarAcomodacoes(string id)
{
  char resp;
  cout << "Procurando acomodacoes" << endl;
  cin >> resp;
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

Cartao_de_Credito *CtrlServUsu::buscar_cartao(string id)
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

  cout << SQL_EDIT_USUARIO << endl;

  rc = CtrlServ::executa(SQL_EDIT_USUARIO);
  trata_retorno(rc);
}

void CtrlServUsu::deletarUsuario(string id_usu, string id_del)
{
  int rc;
  sqlite3_stmt *stmt;

  if (id_usu.compare(id_del) == 0)
    throw runtime_error("O Usuario nao pode deletar sua propria conta");

  if (!CtrlServUsu::existeUsuario(id_del))
    throw runtime_error("O Usuario nao existe!");

  string SQL_DEL_USUARIO = "DELETE FROM Usuario WHERE ";
  SQL_DEL_USUARIO += "identificador = '" + id_del + "'";

  rc = CtrlServ::executa(SQL_DEL_USUARIO);
  trata_retorno(rc);
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
  const char *zero("0"); //valor padrao para validacao

  //comando de selecao que conta quantos usuarios existem com o identificador dado
  //deve ser retornado 0 ou 1
  string SQL_SELECT_USUARIO = "SELECT COUNT(*) FROM USUARIO WHERE ";
  SQL_SELECT_USUARIO += "identificador = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_USUARIO);
  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    //convertendo void* para cosnt char* da resposta do banco
    const char *num = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));

    if (strcmp(num, zero) == 0)
    {
      resultado = false;
    }
    else
    {
      resultado = true;
    }
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

    string id_str(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
    string nome_str(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
    string senha_str(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));

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
