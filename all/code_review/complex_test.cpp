#include "complex.hpp"

using namespace hrd11;

void TestOutput()
{

  std::cout << "\n TestOutput\n";

  Complex c1;
  Complex c2(2,3);

  std::cout << c1 << '\n';
  std::cout << c2 << '\n';
}
void TestInput()
{
  std::cout << "\n TestInput\n";

  Complex c1;
  Complex c2;

  std::cin >> c1;
  std::cout << c1 << std::endl;

  std::cin >> c2;
  std::cout << c2 << std::endl;
}
void TestMemberPlus()
{
  std::cout << "\n TestMemberPlus\n";
  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);

  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  c1 +=c2;
  std::cout << "c1+c2 = "<< c1 << std::endl;
  c1 +=c3;
  std::cout << "c1+c3 = "<< c1 << std::endl;
}
void TestMemberMinus()
{
  std::cout << "\n TestMemberMinus\n";
  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);

  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  c1 +=c2;
  std::cout << "c1-c2 = "<< c1 << std::endl;
  c1 +=c3;
  std::cout << "c1-c3 = "<< c1 << std::endl;

}
void TestMemberMult()
{
  std::cout << "\n TestMemberMult\n";
  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);

  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  c1 *=c2;
  std::cout << "c1*c2 = "<< c1 << std::endl;
  c1 *=c3;
  std::cout << "c1*c3 = "<< c1 << std::endl;
}
void TestMemberDivied()
{
  std::cout << "\n TestMemberDivied\n";
  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);

  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  c1 /=c2;
  std::cout << "c1/c2 = "<< c1 << std::endl;
  c1 /=c3;
  std::cout << "c1/c3 = "<< c1 << std::endl;
}
void TestNonMemberPlus()
{
  std::cout << "\n TestNonMemberPlus\n";

  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);
  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  std::cout << "c1+c2 = " << c1+c2 << std::endl;
  std::cout << "c1+c2+c3 = " << c1+c2+c3 << std::endl;
}
void TestNonMemberMinus()
{
  std::cout << "\n TestNonMemberMinus\n";
  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);
  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  std::cout << "c1-c2 = " << c1-c2 << std::endl;
  std::cout << "c1-c2-c3 = " << c1-c2-c3 << std::endl;
}
void TestNonMemberMult()
{
  std::cout << "\n TestNonMemberMult\n";
  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);
  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  std::cout << "c1*c2 = " << c1*c2 << std::endl;
  std::cout << "c1*c2*c3 = " << c1*c2*c3 << std::endl;
}
void TestNonMemberDivied()
{
  std::cout << "\n TestNonMemberDivied\n";
  Complex c1(3,5);
  Complex c2(2,4.2);
  Complex c3(-2,-5.3);
  std::cout <<"c1 = "<< c1 << std::endl;
  std::cout <<"c2 = "<< c2 << std::endl;
  std::cout <<"c3 = "<< c3 << std::endl;

  std::cout << "c1/c2 = " << c1/c2 << std::endl;
  std::cout << "c1/c2/c3 = " << c1/c2/c3 << std::endl;
}

Complex Foo(Complex complex)
{
  return complex;
}

void TestImplicitCast()
{
  std::cout << "\n TestImplicitCast\n";

  Complex c1;
  c1 = Foo(4.2);

  std::cout <<"c1 = "<< c1 << std::endl;
}

void TestSeters()
{
  std::cout << "\n TestSeters\n";
  Complex c1(3,5);

  std::cout <<"c1 = "<< c1 << std::endl;
  c1.SetReal(4);
  std::cout <<"c1 = "<< c1 << std::endl;
  c1.SetImg(6);
  std::cout <<"c1 = "<< c1 << std::endl;
}
void TestEqual()
{
  std::cout << "\n TestEqual\n";
  Complex c1(3,5);
  Complex c2(3,5);
  std::cout << (c1 == c2) << std::endl;
  c1.SetReal(4);
  std::cout << (c1 == c2) << std::endl;
}

void TestAll()
{
  TestOutput();
  TestInput();
  TestMemberPlus();
  TestMemberMinus();
  TestMemberMult();
  TestMemberDivied();
  TestNonMemberPlus();
  TestNonMemberMinus();
  TestNonMemberMult();
  TestNonMemberDivied();
  TestImplicitCast();
  TestSeters();
  TestEqual();
}


int main()
{
  TestAll();

  return 0;
}
