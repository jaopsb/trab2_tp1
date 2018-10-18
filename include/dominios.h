#ifndef DOMINIOS_H
#define DOMINIOS_H
#include <string>
#include <stdexcept>

using namespace std;

class Agencia
{
public:
  void set_agencia(int);
  /**\brief Retorna o valor da agencia*/
  int get_agencia() { return agencia; }

private:
  int agencia;
  void validar(int) throw(invalid_argument);
};

class Banco
{
public:
  void set_banco(int);
  /**\brief Retorna o valor do numero do banco*/
  int get_banco() { return banco; }

private:
  int banco;
  void validar(int) throw(invalid_argument);
};

class Capacidade_Acomodacao
{
public:
  void set_capacidade_acomodacao(int);
  /**\brief Retorna o valor da capacidade de acomodacao*/
  int get_capacidade_acomodacao() { return cap_acom; };

private:
  int cap_acom;
  void validar(int) throw(invalid_argument);
};

class Diaria
{
public:
  void set_diaria(float);
  /**\brief Retorna o valor da diaria*/
  float get_diaria() { return diaria; };

private:
  float diaria;
  void validar(float) throw(invalid_argument);
};

class Data
{
public:
  void set_data(string);
  /**\brief Retorna o valor da data*/
  string get_data() { return data; };

private:
  string data;
  void validar(string) throw(invalid_argument);
};

class Data_Validade
{
public:
  void set_data_validade(string);
  /**\brief Retorna o valor da data de validade*/
  string get_data_validade() { return data_val; };

private:
  string data_val;
  void validar(string) throw(invalid_argument);
};

class Estado
{
public:
  void set_estado(string);
  /**\brief Retorna o valor do estado*/
  string get_estado() { return estado; };

private:
  string estado;
  void validar(string) throw(invalid_argument);
};

class Identificador
{
public:
  void set_identificador(string);
  /**\brief Retorna o valor do identificador*/
  string get_identificador() { return identificador; };

private:
  string identificador;
  void validar(string) throw(invalid_argument);
};

class Nome
{
public:
  void set_nome(string);
  /**\brief Retorna o valor do nome*/
  string get_nome() { return nome; };

private:
  string nome;
  void validar(string) throw(invalid_argument);
};

class Numero_Cartao
{
public:
  void set_numero_cartao(string);
  /**\brief Retorna o valor do numero do cartao*/
  string get_numero_cartao() { return numero_cartao; };

private:
  string numero_cartao;
  void validar(string) throw(invalid_argument);
};

class Numero_Conta_Corrente
{
public:
  void set_numero_conta_corrente(string);
  /**\brief Retorna o valor da numero da conta corrente*/
  string get_numero_conta_corrente() { return numero_conta_corrente; };

private:
  string numero_conta_corrente;
  void validar(string) throw(invalid_argument);
};

class Senha
{
public:
  void set_senha(string);
  /**\brief Retorna o valor da senha*/
  string get_senha() { return senha; };

private:
  string senha;
  void validar(string) throw(invalid_argument);
};

class Tipo_Acomodacao
{
public:
  void set_tipo_acomodacao(int);
  /**\brief Retorna o valor do tipo de acomodacao*/
  int get_tipo_acomodacao() { return tipo_acomodacao; };

private:
  int tipo_acomodacao;
  void validar(int) throw(invalid_argument);
};

#endif // DOMINIOS_H
