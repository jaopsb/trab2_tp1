#include "dominios.hpp";

class IUAut
{
public:
  virtual bool autenticar(string,string) = 0;
  virtual void menu() =0;
  virtual ~IUAut(){};
};

class IServAut
{
public:
  virtual bool autenticar(const Identificador &, const Senha &) = 0;
  virtual ~IServAut() {}
};
