#ifndef Teste_H
#define Teste_H
#include <iostream>
#include <dominios.h>

using namespace std;
/****************** Classe Generica Teste *********************/
class Teste
{
public:
  void run();
  bool rodar_suite();
  void temp_teste(string nome_func, void (*f)(void));
};

/****************** Classe de Teste Executadas *********************/
class Teste_Dominios
{
public:
  void run();
};

class Teste_Entidades
{
public:
  void run();
};

/****************** TESTES DAS ENTIDADES *********************/
class Teste_Usuario : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_usuario_ok();
  static void testa_usuario_erro() throw(invalid_argument);
};

class Teste_Acomodacao : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_acomodacao_ok();
  static void testa_acomodacao_erro() throw(invalid_argument);
};

class Teste_Cartao_de_Credito : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_cartao_ok();
  static void testa_cartao_erro() throw(invalid_argument);
};

class Teste_Conta_Corrente : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_conta_corrente_ok();
  static void testa_conta_corrente_erro() throw(invalid_argument);
};

/****************** TESTES DOS DOMINIOS *********************/
class Teste_Agencia : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_agencia_ok();
  static void testa_agencia_erro() throw(invalid_argument);
};

class Teste_Banco : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_banco_ok();
  static void testa_banco_erro() throw(invalid_argument);
};

class Teste_Capacidade_Acomodacao : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_capacidade_acomodacao_ok();
  static void testa_capacidade_acomodacao_erro() throw(invalid_argument);
};

class Teste_Diaria : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_diaria_ok();
  static void testa_diaria_erro() throw(invalid_argument);
};

class Teste_Data : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_data_ok();
  static void testa_data_erro() throw(invalid_argument);
};

class Teste_Data_Validade : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_data_validade_ok();
  static void testa_data_validade_erro() throw(invalid_argument);
};

class Teste_Estado : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_estado_ok();
  static void testa_estado_erro() throw(invalid_argument);
};

class Teste_Identificador : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_identificador_ok();
  static void testa_identificador_erro() throw(invalid_argument);
};

class Teste_Nome : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_nome_ok();
  static void testa_nome_erro() throw(invalid_argument);
};

class Teste_Numero_Cartao : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_numero_cartao_ok();
  static void testa_numero_cartao_erro() throw(invalid_argument);
};

class Teste_Numero_Conta_Corrente : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_numero_conta_corrente_ok();
  static void testa_numero_conta_corrente_erro() throw(invalid_argument);
};

class Teste_Senha : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_senha_ok();
  static void testa_senha_erro() throw(invalid_argument);
};

class Teste_Tipo_Acomodacao : public Teste
{
public:
  bool rodar_suite();

private:
  bool resultado;
  static void testa_tipo_acomodacao_ok();
  static void testa_tipo_acomodacao_erro() throw(invalid_argument);
};

#endif // Teste_H
