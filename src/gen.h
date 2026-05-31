// -*- mode:C++ ; compile-command: "g++ -I.. -g -c gen.cc" -*-
/*
 *  Copyright (C) 2001 B. Parisse, Institut Fourier, 38402 St Martin d'Heres
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _GIAC_GEN_H
#define _GIAC_GEN_H
#include "first.h"

#include <gmp.h>
#include <iostream>
#include <string>
#include <vector>
#include "dispatch.h"
#include "vecteur.h"
#include "fraction.h"
#include <complex>
#include <stdlib.h>

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC
  extern int decimal_digits;
  extern bool is_inevalf;
  extern bool expand_re_im;
  class gen ;   // a class around gmp mpz_t to simplify usage in C++
  class identificateur; // global name
  struct symbolic; // symbolics
  class unary_function_ptr; // functions
  struct real_complex_rootof; 
  template <class T> class tensor;
  typedef tensor<gen> polynome;
  typedef Tfraction<gen> fraction;

  class gen_user ;
  // errors
  void settypeerr();
  void setsizeerr();
  void setdimerr();
  void settypeerr(const std::string & s);
  void setsizeerr(const std::string & s);
  void setdimerr(const std::string & s);
  void divisionby0err(const gen &);
  void cksignerr(const gen &);
  void invalidserieserr(const std::string &);
  void toofewargs(const std::string & s);
  void toomanyargs(const std::string & s);
  void maxordererr();

  // short integer arithmetic
  int absint(int a);
  int min(int a,int b);
  int max(int a,int b);
  int invmod(int n,int modulo);
  int powmod(int a,unsigned long n,int m);
  int gcd(int a,int b);
  int smod(int a,int b); // where b is assumed to be positive
  int simplify(int & a,int & b);

  // arbitrary precision floats (interval)
  struct real_interval {
    mpf_t * inf;
    mpf_t * sup;
    real_interval(double d) { mpf_init_set_d(*inf,d); mpf_init_set_d(*sup,d); };
    ~real_interval() { mpf_clear(*inf); mpf_clear(*sup); }
  };

  class gen {
  public:
    short int subtype;
    short int type;  // see dispatch.h
    int * ref_count;
    union {
      // atomic types
      int val; // immediate int (type _INT_)
      double _DOUBLE_val; // immediate float (type _DOUBLE_)
      mpz_t * _ZINTptr; // long int (type _ZINT)
      real_interval * _REALptr; // extended double (type _REAL)
      gen * _CPLXptr ; // complex as an gen[2] array (type _CPLX)
      identificateur * _IDNTptr; // global name identifier (type _IDNT)
      symbolic * _SYMBptr; // for symbolic objects (type _SYMB)
      gen * _MODptr;
      gen * _EXTptr; // 2 gens for alg. extension (type ext)
      // alg ext: 1st gen is a std::vector or a fraction, 2nd gen is
      // a/ a std::vector, the minimal monic polynomial (the roots are permutable)
      // b/ a real_complex_rootof given by it's min poly and 
      // c/ another type meaning that the root is expressed in terms
      //    of another rootof, in this case ext_reduce should be called
      // For 2nd order extension, X^2=d is used if d!=1 mod 4
      // X is the positive solution
      // if d=1 mod 4 the equation is X^2-X=(d-1)/4
      fraction * _FRACptr; // fraction (type _FRAC)
      polynome * _POLYptr ; // multidim. sparse polynomials (type poly)
      // _VECTosite types (std::vector<>)
      vecteur * _VECTptr ; // vecteur: std::vectors & dense_POLY1 (type _VECT)
      sparse_poly1 * _SPOL1ptr ; // std::vector<monome>: sparse 1-d poly (type _SPOL1)
      std::string * _STRNGptr;
      unary_function_ptr * _FUNCptr;
      real_complex_rootof * _ROOTptr;
      gen_user * _USERptr;
    };
    gen(): subtype(0), type(_INT_),  val(0) {};
    gen(int i): subtype(0), type(_INT_), val(i) {};
    gen(size_t i): subtype(0), type(_INT_), val(i) {};
    gen(long long i);
    gen(const mpz_t & m);
    // WARNING coerce *mptr to an int if possible, in this case delete mptr
    // Pls do not use this constructor unless you know exactly what you do!!
    gen(mpz_t * mptr);
    gen (double d): type(_DOUBLE_),_DOUBLE_val(d) {};
    gen(int a,int b);
    gen(double a,double b);
    gen(const gen & a,const gen & b);
    gen(const std::complex<double> & c);
    gen(const gen & e);
    gen (const identificateur & s);
    gen (const vecteur & v,int s=0);
    gen (const symbolic & s);
    gen (const gen_user & g);
    // Pls do not use this constructor unless you know exactly what you do
    gen(polynome * pptr);
    gen (const polynome & p);
    gen (const fraction & p);
    gen (const real_complex_rootof & r);
    gen (const std::string & s);
    gen (const std::string & s,const vecteur & l);
    gen (const sparse_poly1 & p);
    gen (const unary_function_ptr & f,int nargs=1);
    ~gen();
    gen eval(int level=100) const;
    gen evalf() const;
    gen & operator = (const gen & a);
    int to_int() const ;
    bool is_real() const ;
    bool is_cinteger() const ;
    bool is_integer() const ;
    bool is_constant() const;
    std::string print() const;
    // void modify(char * & s) { *this =gen(std::string(s)); };
    void modify(int i) { *this =gen(i); };
    void dbgprint() const; 
    void uncoerce() ;
    gen conj() const;
    gen re() const ;
    gen im() const ;
    gen squarenorm() const;
    int bindigits() const ;
    gen operator [] (int i) const ;
    gen operator [] (const gen & i) const;
    // gen & operator [] (int i) ;
    // gen & operator [] (const gen & i) ;
    gen gen::operator () (const gen & i) const;
    bool islesscomplexthan(const gen & other) const;
    int symb_size() const;
    gen change_subtype(int newsubtype) { subtype=newsubtype; return *this; }
    bool is_symb_of_sommet(const unary_function_ptr & u) const;
  };

  gen chartab2gen(char * & s);
  extern gen zero;
  extern gen plus_one;
  extern gen plus_two;
  extern gen plus_three;
  extern gen minus_one;
  extern gen cst_i;
  extern const vecteur null_vetor;
  extern double rad2deg_d;
  extern double deg2rad_d;

  bool is_zero(const gen & a);
  bool is_one(const gen & a);
  bool is_minus_one(const gen & a);
  bool is_sq_minus_one(const gen & a);
  bool is_inf(const gen & e);
  bool is_undef(const gen & e);
  bool is_zero__VECT(const vecteur & a);
  bool has_denominator(const gen & n);

  // basic arithmetic
  gen operator && (const gen & a,const gen & b);
  gen operator || (const gen & a,const gen & b);
  gen operator + (const gen & a,const gen & b);
  gen sym_add (const gen & a,const gen & b);
  gen operator - (const gen & a,const gen & b);
  gen operator - (const gen & a);
  gen sym_sub (const gen & a,const gen & b);
  gen operator * (const gen & a,const gen & b);
  gen sym_mult (const gen & a,const gen & b);
  gen pow(const gen & base,const gen & exponent);
  gen iquo(const gen & a,const gen & b); // same
  gen irem(const gen & a,const gen & b,gen & q); // same
  gen smod(const gen & a,const gen & b); // same
  gen rdiv(const gen & a,const gen & b); // rational division
  inline gen operator /(const gen & a,const gen & b){ return rdiv(a,b); };
  gen operator %(const gen & a,const gen & b); // for int only
  gen inv(const gen & a);
  gen inv_EXT(const gen & a);
  gen algebraic_EXTension(const gen & a,const gen & v);
  gen ext_reduce(const gen & a, const gen & v);

  bool operator ==(const gen & a,const gen & b);
  bool operator !=(const gen & a,const gen & b);
  gen equal(const gen & a,const gen &b);

  gen operator !(const gen & a);

  int fastsign(const gen & a);   // 0 if unknown, 1 if >0, -1 if <0
  gen sign(const gen & a);

  // Large tests if strictly not precised, if sign is unknown return false 
  bool is_greater(const gen & a,const gen &b);
  bool is_strictly_greater(const gen & a,const gen &b);
  bool is_positive(const gen & a);
  bool is_strictly_positive(const gen & a);
  // Large tests if strictly not precised, if sign is unknown make an error
  bool ck_is_greater(const gen & a,const gen &b);
  bool ck_is_strictly_greater(const gen & a,const gen &b);
  bool ck_is_positive(const gen & a);
  bool ck_is_strictly_positive(const gen & a);

  gen operator >(const gen & a,const gen & b);
  gen operator >=(const gen & a,const gen & b);
  gen operator <(const gen & a,const gen & b);
  gen operator <=(const gen & a,const gen & b);
  bool symb_size_less(const gen & a,const gen & b);

  gen min(const gen & a, const gen & b);
  gen max(const gen & a, const gen & b);
  gen factorial(unsigned long int i);
  gen comb(unsigned long int i,unsigned long j);
  gen perm(unsigned long int i,unsigned long j);
  gen pow(const gen & base, unsigned long int exponent);
  gen pow(const gen & base, int exponent);
  gen pow(unsigned long int base, unsigned long int exponent);

  // more advanced arithmetic
  gen gcd(const gen & A,const gen & B);
  gen lcm(const gen & a,const gen & b);
  gen simplify(gen & n, gen & d);
  void egcd(const gen &a,const gen &b, gen & u,gen &v,gen &d );
  gen ichinrem(const gen & a,const gen &b,const gen & amod, const gen & bmod);
  gen invmod(const gen & A,const gen & modulo);
  gen fracmod(const gen & a_orig,const gen & modulo); // -> p/q=a mod modulo
  gen powmod(const gen &base,const gen & expo,const gen & modulo);
  gen isqrt(const gen & A);
  gen re(const gen & a);
  gen im(const gen & a);
  gen conj(const gen & a);
  gen sq(const gen & a);
  gen abs(const gen & a);
  gen l1norm(const gen & a);
  gen arg(const gen & a);
  int is_perfect_square(const gen & A);
  int is_probab_prime_p(const gen & A);
  gen nextprime(const gen & a); // more precisely next probably prime
  gen prevprime(const gen & a); // more precisely prev probably prime
  int jacobi(const gen & A, const gen &B);
  int legendre(const gen & A, const gen & B);
  vecteur pascal_next_line(const vecteur & v); 
  vecteur pascal_nth_line(int n);

  // if b is a _MOD, returns a as a b _MOD 
  gen chkmod(const gen& a,const gen & b);
  // make a _MOD a%b
  gen makemod(const gen & a,const gen & b);
  // same without evaluating %
  gen makemodquoted(const gen & a,const gen & b);

  bool is_atomic(const gen & e);
  symbolic _FRAC2_SYMB(const fraction & f);
  symbolic _FRAC2_SYMB(const gen & e);
  symbolic _FRAC2_SYMB(const gen & n,const gen & d);
  gen string2gen(const std::string & ss,bool remove_ss_quotes=true);
  // by default ss is assumed to be delimited by " and "
  std::complex<double> gen2complex_d(const gen & e);
  gen eval_VECT(const vecteur & v,int subtype,int level=100);
  // functional equivalent of gen methods
  inline gen eval(const gen & e){ return e.eval(); };
  inline gen evalf(const gen & e){ return e.evalf(); };
  inline std::string print(const gen & e){ return e.print(); }
  inline bool is_real(const gen & g){ return g.is_real(); }
  inline  bool is_cinteger(const gen & g){ return g.is_cinteger();}  ;
  inline  bool is_integer(const gen & g){ return g.is_integer(); }  ;
  inline  bool is_constant(const gen & g){ return g.is_constant(); } ;

  class gen_user{
  public:
    // You must redefine memory_alloc, copy_to and memory_free
    // memory_alloc should 
    // return dynamic_cast<gen_user *> new your_class(*this);
    // copy_to should copy your data members taking care of dynamically
    // allocated data
    // memory_free shoudl
    // delete(dynamic_cast<your_class *>(ptr));
    virtual gen_user * memory_alloc() const { return new gen_user(*this); }
    virtual void copy_to (gen_user * g) const { }
    virtual void memory_free(gen_user * ptr){
      delete(ptr);
    }
    // redefine the destructor if you allocate mem dynamically for members
    virtual ~gen_user() {}; 
    gen_user operator = (const gen_user & g) { 
      g.copy_to(this);
      return *this;
    }
    // redefine operations if it makes sense. 
    // You can redefine gen_user + gen_user for speed
    virtual gen operator + (const gen &) const { setsizeerr("+ not redefined"); return *this; }
    virtual gen operator + (const gen_user & a) const { return (*this) + gen(a); }
    virtual gen operator - (const gen &) const { setsizeerr("Binary - not redefined"); return *this;}
    virtual gen operator - (const gen_user & a) const { return (*this) - gen(a); }
    virtual gen operator - () const { setsizeerr("Unary - not redefined"); return *this;}
    virtual gen operator * (const gen &) const { setsizeerr("Binary * not redefnied"); return *this;}
    virtual gen operator * (const gen_user & a) const { return (*this) * gen(a); }
    virtual gen operator / (const gen_user & a) const { setsizeerr("Binary / not redefined"); return *this; }
    virtual bool is_zero() const { setsizeerr("==0 not redefined"); return false;}
    virtual bool is_one() const { setsizeerr("==1 not redefined"); return false;}
    virtual bool is_minus_one() const { setsizeerr("==-1 not redefined"); return false;}
    virtual gen inv () const { setsizeerr("Inv not redefined"); return *this;}
    virtual gen conj() { setsizeerr("Conj not redefnied"); return *this;}
    virtual gen re() { setsizeerr("Real part not redefnied"); return *this;}
    virtual gen im() { setsizeerr("Imaginary part not redefnied"); return *this;}
    virtual gen abs() { setsizeerr("Abs not redefnied"); return *this;}
    virtual gen arg() { setsizeerr("Arg not redefnied"); return *this;}
    virtual gen operator () (const gen &) { setsizeerr("() not redefnied"); return *this;}
    virtual gen operator [] (const gen &) { setsizeerr("[] not redefnied"); return *this;}
    virtual bool operator == (const gen &) { setsizeerr("== not redefnied"); return false;}
    virtual bool operator == (const gen_user & a) const { return (*this) == gen(a); }
    // must redefine > AND <= since we do not have symetrical type arguments
    virtual gen operator > (const gen &) { setsizeerr("== not redefnied"); return *this;}
    virtual gen operator > (const gen_user & a) const { return (*this) > gen(a); }
    virtual gen operator <= (const gen &) { setsizeerr("<= not redefnied"); return *this;}
    virtual gen operator <= (const gen_user & a) const { return (*this) <= gen(a); }
    virtual gen gcd (const gen &) const { setsizeerr("gcd not redefnied"); return *this;}    
    virtual gen gcd (const gen_user & a) const { return gcd(gen(a)); }
    virtual std::string print () const { return  "Nothing_to_print";}
    void dbgprint () const { std::cerr << this->print() << std::endl;}
    virtual std::string texprint () const { return "Nothing_to_print_tex"; }
    virtual gen eval(int level) const {return *this;};
    virtual gen evalf() const {return *this;};
  };

  std::string print_the_type(int val);

  // I/O
  std::ostream & operator << (std::ostream & os,const gen & a);
  std::istream & operator >> (std::istream & is,gen & a);

  struct monome {
    gen coeff;
    gen exponent;
    monome(const gen & mycoeff) : coeff(mycoeff),exponent(zero) {};
    monome(const gen &mycoeff,const gen &myexponent) : coeff(mycoeff),exponent(myexponent) {};
    std::string print() const ;
    void dbgprint() const ;
  };
  std::ostream & operator << (std::ostream & os,const monome & m);
  std::string printi();
  std::string printinner_VECT(const vecteur & v, int subtype);
  std::string begin_VECT_string(int subtype);
  std::string end_VECT_string(int subtype);
  std::string print_VECT(const vecteur & v,int subtype=0);
  std::string print_SPOL1(const sparse_poly1 & p);
  int matchpos(const std::string & s,int pos); // find closing or opening () [] {}
  std::string cutstring(const std::string & chaine,int nchar,std::vector<int> & ligne_end) ;
  std::string calc_endlines_positions(const vecteur & history_in,const vecteur & history_out,int nchar,std::vector<int> & endlines,std::vector<int> & positions);
  bool is_operator_char(char c);
  void increase_selection(const std::string & s,int & pos1,int& pos2);
  void decrease_selection(const std::string & s,int & pos1,int& pos2);
  void move_selection_right(const std::string & s,int & pos1, int & pos2);
  void move_selection_left(const std::string & s,int & pos1, int & pos2);
  std::string remove_EXTension(const std::string & chaine);
  std::string pariprint(const gen & e);

#ifndef NO_NAMESPACE_GIAC
} // namespace giac
#endif // ndef NO_NAMESPACE_GIAC

#endif // _GIAC_GEN_H
