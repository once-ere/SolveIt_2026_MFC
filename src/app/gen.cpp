// -*- mode:C++ ; compile-command: "g++ -I.. -g -c gen.cc" -*-
#include "first.h"
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
using namespace std;
#define __USE_ISOC9X 1
#include <time.h>
#include <stdexcept>
#include <ctype.h>
#include <math.h>
#include <cstdlib>
#include <list>
#include <errno.h>
#include "gen.h"
#include "gausspol.h"
#include "identificateur.h"
#include "poly.h"
#include "usual.h"
#include "input_lexer.h"
#include "sym2poly.h"
#include "vecteur.h"
#include "modpoly.h"
#include "alg_ext.h"
#include "prog.h"
#include "rpn.h"
#include "plot.h"
#include "intg.h"
#include "subst.h"

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

  bool expand_re_im=true;
  bool is_inevalf=false;

  void settypeerr(){
    throw(std::runtime_error(last_evaled_function_name+"Bad Argument Type"));
  }

  void setsizeerr(){
    throw(std::runtime_error(last_evaled_function_name+"Bad Argument Value"));
  }

  void setdimerr(){
    throw(std::runtime_error(last_evaled_function_name+"Invalid dimension"));
  }

  void settypeerr(const string & s){
    throw(std::runtime_error(s+" Error: Bad Argument Type"));
  }

  void setsizeerr(const string & s){
    throw(std::runtime_error(s+" Error: Bad Argument Value"));
  }

  void setdimerr(const string & s){
    throw(std::runtime_error(s+" Error: Invalid dimension"));
  }

  void divisionby0err(const gen & e){
    throw(std::runtime_error(last_evaled_function_name+"Division of " + e.print()+ " by 0"));
  }

  void cksignerr(const gen & e){
    throw(std::runtime_error(last_evaled_function_name+"Unable to check sign: "+e.print()));
  }

  void invalidserieserr(const string & s){
    throw(std::runtime_error(last_evaled_function_name+"Invalid series expansion: "+s));
  }

  void toofewargs(const string & s){
    throw(std::runtime_error(last_evaled_function_name+"Too few arguments: "+s));
  }

  void toomanyargs(const string & s){
    throw(std::runtime_error(last_evaled_function_name+"Too many arguments: "+s));
  }

  void maxordererr(){
    throw(std::runtime_error(last_evaled_function_name+"Max order ("+gen(max_order).print()+") exceeded or non unidirectional series"));
  }

  void swap(int & a,int & b){
      int t=a;
      a=b;
      b=t;
  }

  // void parseerror(){
  //  throw(std::runtime_error("Parse error"));
  // }

  int decimal_digits=12;
  gen zero(0);
  gen minus_one(-1);
  gen plus_one(1);
  gen plus_two(2);
  gen plus_three(3);
  gen cst_i(0,1);
  double rad2deg_d(180/M_PI);
  double deg2rad_d(M_PI/180);
  
  bool is_inf(const gen & e){
    switch (e.type){
    case _IDNT:
      return *e._IDNTptr==_IDNT_infinity;
    case _SYMB:
      return is_inf(e._SYMBptr->feuille);
    default:
      return false;
    }
  }
  bool is_undef(const gen & e){
    if (e.type==_IDNT)
      return *e._IDNTptr==_IDNT_undef;
    if ( (e.type==_POLY) && (!e._POLYptr->coord.empty()) )
      return is_undef(e._POLYptr->coord.front().value);
    else
      return false;
  }
  
  const vecteur null_vector;
  
  enum { debugtype=_CPLX };
#define debugtypeptr _CPLXptr

  int absint(int a){
    if (a<0)
      return -a;
    else
      return a;
  }

  int min(int a, int b){
    if (a<b)
      return a;
    else
      return b;
  }

  int max(int a, int b){
    if (a<b)
      return b;
    else
      return a;
  }

  int invmod(int a,int b){
    int aa(1),ab(0),ar(0);
    div_t qr;
    while (b){
      qr=div(a,b);
      ar=aa-qr.quot*ab;
      a=b;
      b=qr.rem;
      aa=ab;
      ab=ar;
    }
    if (a==1)
      return aa;
    if (a!=-1)
      setsizeerr("Not invertible");
    return -aa;
  }

  int powmod(int a,unsigned long n,int m){
    if (!n)
      return 1;
    int b=powmod(a,n/2,m);
    long long tmp=b;
    b=(tmp*b) % m;
    tmp=b;
    if (n % 2)
      return (tmp*a) % m;
    else
      return b;
  }

  int smod(int r,int m){
    if (!m)
      return r;
    if (m<0)
      m=-m;
    r = r % m;
    register int tmp= r+r;
    if (tmp>m)
      return r-m;
    if (tmp<-m)
      return r+m;
    return r;
  }

  int gcd(int a,int b){
    int r;
    while (b){
      r=a%b;
      a=b;
      b=r;
    }
    return absint(a);
  }

  int simplify(int & a,int & b){
    int d=gcd(a,b);
    a=a/d;
    b=b/d;
    return d;
  }

  // list for allocated mpz_t
  list<__mpz_struct> zint_list;

  inline mpz_t * new_mpz_t(){
    __mpz_struct tmp={0,0,0};
    zint_list.push_back(tmp);
    return (mpz_t *) &zint_list.back();
  }

  gen factorial(unsigned long int i){
    mpz_t * e =new mpz_t[1];
    //mpz_t * e=new_mpz_t();
    mpz_init(*e);
    mpz_fac_ui(*e,i);
    return e;
  }

  gen comb(unsigned long int i,unsigned long j){
    mpz_t * e =new mpz_t[1];
    mpz_init(*e);
    if (i<j)
      return e;
    mpz_set_ui(*e,1);
    for (unsigned long int k=i;k>i-j;--k)
      mpz_mul_ui(*e,*e,k);
    mpz_t tmp;
    mpz_init(tmp);
    mpz_fac_ui(tmp,j);
    mpz_div(*e,*e,tmp);
    return e;
  }

  gen perm(unsigned long int i,unsigned long j){
    mpz_t * e =new mpz_t[1];
    mpz_init(*e);
    if (i<j)
      return e;
    mpz_set_ui(*e,1);
    for (unsigned long int k=i;k>i-j;--k)
      mpz_mul_ui(*e,*e,k);
    return e;
  }

  gen::gen(long long i) { 
    val=i;
    //    long long temp=val;
    if (val==i){
      type=_INT_;
      subtype=0;
    }
    else {
      type =_ZINT;
      subtype=0;
      _ZINTptr = new mpz_t[1];
      ref_count=new int(1);
      // convert long long to mpz_t
      bool signe=(i<0);
      if (signe)
	i=-i;
      unsigned int i1=i>>32;
      unsigned int i2=i;
      mpz_init_set_ui(*_ZINTptr,i1);
      mpz_mul_2exp(*_ZINTptr,*_ZINTptr,32);
      mpz_add_ui(*_ZINTptr,*_ZINTptr,i2);
      if (signe)
	mpz_neg(*_ZINTptr,*_ZINTptr);
      /*
      long long lbase=65536;
      long base=65536;
      long long i1=i/lbase;
      long i2=i1/lbase; // i2=i/2^32
      //cout << "Initialization of " << _ZINTptr << endl ;
      mpz_init_set_si(*_ZINTptr,i2);
      mpz_mul_ui(*_ZINTptr,*_ZINTptr,base); // i/2^32 * 2^16
      long i2mod=i1 % lbase;
      if (i2mod>0)
	mpz_add_ui(*_ZINTptr,*_ZINTptr,i2mod);
      else
	mpz_sub_ui(*_ZINTptr,*_ZINTptr,-i2mod); // i/2^16
      mpz_mul_ui(*_ZINTptr,*_ZINTptr,base); // i/2^16 * 2^16
      long i1mod = i % lbase;
      if (i1mod>0)
	mpz_add_ui(*_ZINTptr,*_ZINTptr,i1mod);
      else
	mpz_sub_ui(*_ZINTptr,*_ZINTptr,-i1mod); // i
      */
    }
  }

  gen::gen(const mpz_t & m) { 
    type =_ZINT;
    subtype=0;
    _ZINTptr= new mpz_t[1] ;
    mpz_init_set(*_ZINTptr,m);
    ref_count=new int(1);
  }

  gen::gen(const identificateur & s){
    type=_IDNT;
    subtype=0;
    ref_count = new int(1);
    _IDNTptr= new identificateur(s);
  }

  gen::gen(const vecteur & v,int s=0)
  {
    /*
    if (v.empty()){
      type=_INT_;
      val=0;
      return;
    }
    */
    type=_VECT;
    subtype=s;
    ref_count = new int(1);
    _VECTptr= new vecteur(v);
  }


  gen::gen(const symbolic & s){
    type = _SYMB;
    subtype = 0;
    _SYMBptr = new symbolic(s) ;
    ref_count = new int(1);
  }

  gen::gen(const gen_user & g){
    type = _USER;
    subtype=0;
    _USERptr = g.memory_alloc() ;
    ref_count = new int(1);
  }

  gen::gen(const polynome & p){
    subtype=0;
    if (p.coord.empty()){
      type = _INT_;
      val = 0;
    }
    else {
      if (Tis_constant<gen>(p) && is_atomic(p.coord.front().value) ){
	type = _INT_;
	* this = p.coord.front().value;
      }
      else {
	type = _POLY;
	_POLYptr = new polynome(p) ;
	ref_count = new int(1);
      }
    }
    /*
    type = poly;
    polyptr = new polynome(p) ;
    ref_count = new int(1);
    */
  }

  gen::gen(const real_complex_rootof & r){
    subtype=0;
    type = _ROOT;
    _ROOTptr = new real_complex_rootof(r) ;
    ref_count = new int(1);
  }

  gen::gen(const fraction & p){
    subtype=0;
    if (is_zero(p.num)){
      type=_INT_;
      val=0;
    }
    else {
      if (is_one(p.den)){
	type=_INT_;
	*this = p.num;
      }
      else {
          if (is_minus_one(p.den)){
              type=_INT_;
              *this = -p.num;
          }
          else {
              
              type = _FRAC;
              _FRACptr = new fraction(p) ;
              ref_count = new int(1);
          }
          
      }
    }
  }

  gen::gen(polynome * pptr){
    subtype=0;
    type = _POLY;
    _POLYptr = pptr ;
    ref_count = new int(1);
  }

  // WARNING coerce *mptr to an int if possible, in this case delete mptr
  // Pls do not use this constructor unless you know exactly what you do!!
  gen::gen(mpz_t * mptr){
    subtype=0;
    int l=mpz_sizeinbase(*mptr,2);
    // if (l<17){
    if (l<32){
      type = _INT_;
      val = mpz_get_si(*mptr);
      // cout << "Destruction by mpz_t * " << *mptr << endl;
      mpz_clear(*mptr);
      delete  mptr;
    }
    else {
      type =_ZINT;
      _ZINTptr = mptr;
      ref_count=new int(1);
    }
    // cout << *this << endl;
  }

  gen::gen(const gen & e) { 
    // if (e.type==debugtype)
    // cout << e << "Construct @ " << *e.debugtypeptr << "[" << *e.ref_count <<"+1]" <<endl;
    type=e.type;
    subtype=e.subtype;
    if (!type){
      val=e.val;
      return;
    }
    switch (type ) {
    case _DOUBLE_:
      _DOUBLE_val=e._DOUBLE_val;
      break;
    case _ZINT: 
      ref_count=e.ref_count;
      (*ref_count)++;
      _ZINTptr=e._ZINTptr;
      break; 
    case _REAL: 
      ref_count=e.ref_count;
      (*ref_count)++;
      _REALptr=e._REALptr;
      break; 
    case _CPLX: 
      ref_count=e.ref_count;
      (*ref_count)++;
      _CPLXptr=e._CPLXptr;
      break; 
    case _IDNT:
      ref_count=e.ref_count;
      (*ref_count)++;
      _IDNTptr=e._IDNTptr;
      break;
    case _POLY: 
      ref_count=e.ref_count;
      (*ref_count)++;
      _POLYptr=e._POLYptr;
      break; 
    case _FRAC: 
      ref_count=e.ref_count;
      (*ref_count)++;
      _FRACptr=e._FRACptr;
      break; 
    case _VECT: 
      ref_count=e.ref_count;
      (*ref_count)++;
      _VECTptr=e._VECTptr;
      break; 
    case _SPOL1: 
      ref_count=e.ref_count;
      (*ref_count)++;
      _SPOL1ptr=e._SPOL1ptr;
      break; 
    case _SYMB:
      ref_count=e.ref_count;
      (*ref_count)++;
      _SYMBptr=e._SYMBptr;
      break; 
    case _USER:
      ref_count=e.ref_count;
      (*ref_count)++;
      _USERptr=e._USERptr;
      break; 
    case _EXT:
      ref_count=e.ref_count;
      (*ref_count)++;
      _EXTptr=e._EXTptr;
      break; 
    case _MOD:
      ref_count=e.ref_count;
      (*ref_count)++;
      _MODptr=e._MODptr;
      break; 
    case _ROOT:
      ref_count=e.ref_count;
      (*ref_count)++;
      _ROOTptr=e._ROOTptr;
      break; 
    case _STRNG:
      ref_count=e.ref_count;
      (*ref_count)++;
      _STRNGptr=e._STRNGptr;
      break; 
    case _FUNC:
      ref_count=e.ref_count;
      (*ref_count)++;
      _FUNCptr=e._FUNCptr;
      break; 
    default: 
      settypeerr("Gen constructor");
    }
  }

  gen::gen(int a,int b) {
    subtype=0;
    if (!b){
      type=_INT_;
      val=a;
    }
    else {
      type =_CPLX;
      subtype=0;
      _CPLXptr = new gen[2];
      _CPLXptr->type=_INT_;
      _CPLXptr->val=a;
      ++_CPLXptr;
      _CPLXptr->type=_INT_;
      _CPLXptr->val =b;
      --_CPLXptr;
      ref_count = new int(1);
    }
  }

  gen::gen(double a,double b){
    subtype=0;
    // cout << a << " " << b << " " << epsilon << endl;
    if (fabs(b)<epsilon){
      type=_DOUBLE_;
      _DOUBLE_val=a;
    }
    else {
      type =_CPLX;
      subtype=3;
      _CPLXptr = new gen[2];
      _CPLXptr->type=_DOUBLE_;
      _CPLXptr->_DOUBLE_val=a;
      ++_CPLXptr;
      _CPLXptr->type=_DOUBLE_;
      _CPLXptr->_DOUBLE_val =b;
      --_CPLXptr;
      ref_count = new int(1);
    }
  }
  
  gen::gen(const gen & a,const gen & b) { // a and b must be type <2!
    subtype=0;
    if ( (a.type>=_CPLX) || (b.type>=_CPLX) )
      settypeerr("complex constructor");
    if (is_zero(b)){
      type=a.type;
      switch (type ) {
      case _INT_: 
	val=a.val;
	break; 
      case _DOUBLE_: 
	_DOUBLE_val=a._DOUBLE_val;
	break; 
      case _ZINT: 
	ref_count=a.ref_count;
	(*ref_count)++;
	_ZINTptr=a._ZINTptr; // a is a _ZINT
	break; 
      case _REAL: 
	ref_count=a.ref_count;
	(*ref_count)++;
	_REALptr=a._REALptr; // a is a _ZINT
	break; 
      default: 
	settypeerr("complex constructor");
      }
    }
    else {
      type =_CPLX;
      subtype= (a.type==_DOUBLE_) + (b.type==_DOUBLE_)*2;
      _CPLXptr = new gen[2];
      * _CPLXptr = a;
      ++_CPLXptr;
      * _CPLXptr = b;
      --_CPLXptr;
      ref_count = new int(1);
    }
  }
  
  gen::gen(const complex<double> & c) {
    subtype=0;
    type=_CPLX;
    subtype=3;
    _CPLXptr = new gen[2];
    * _CPLXptr = real(c);
    ++_CPLXptr;
    * _CPLXptr = imag(c);
    --_CPLXptr;
    ref_count = new int(1);    
  }

  complex<double> gen2complex_d(const gen & e){
    if (e.type==_CPLX){
      if (e.subtype==3)
	return complex<double>((*e._CPLXptr)._DOUBLE_val,(*(e._CPLXptr+1))._DOUBLE_val);
      gen ee=e.evalf();
      return complex<double>((*e._CPLXptr)._DOUBLE_val,(*(e._CPLXptr+1))._DOUBLE_val);
    }
    if (e.type==_DOUBLE_)
      return complex<double>(e._DOUBLE_val,0);
    if (e.type==_INT_) 
      return complex<double>(e.val,0);
    if (e.type==_ZINT)
      return complex<double>(e.evalf()._DOUBLE_val,0);
    setsizeerr("gen -> complex");
  }

  gen chartab2gen(char * & s){
    gen res;
    // subtype=0;
    // initialize as a null _INT_
    // type = _INT_;
    // val = 0;
    if (!*s)
      return res;
    int base=0;
    if (s[0]=='#' || s[0]=='0') {
      if (s[1]=='x')
	s[0]='0';
      if (s[1]=='o'){
	s[0]='0';
	s[1]='0';
      }
    }
    if (s[1]=='b'){
      s[1]='0';
      base=2;
    }
    errno = 0;
    char * endchar;
    long ll=strtol(s,&endchar,base);
    if (*endchar) {// non numeric -> use parser
      double d=strtod(s,&endchar);
      if (*endchar)
	return gen(string(s));
      return gen(d);
    }
    if (!errno)
      return gen ( int(ll));
    int l =strlen(s);
    // check if a non 0-9 char is there
    for (int i=0;i<l;++i){
      if ((s[i]<'0') || (s[i]>'9')){
	return gen(string(s));
      }
    }
    int maxsize = 5 + (s[0]=='-');
    if (l<maxsize){
      res.type=_INT_;
      res.val = atoi(s);
      return res;
    }
    else {
      mpz_t * ptr= new mpz_t[1] ;
      mpz_init_set_str(*ptr,s,10);
      return gen(ptr);
    }
  }

  gen string2gen(const string & ss,bool remove_ss_quotes=true){
    gen res;
    res.type=_STRNG;
    res.ref_count = new int(1);
    if (remove_ss_quotes)
      res._STRNGptr = new string(ss.substr(1,ss.size()-2));
    else
      res._STRNGptr = new string(ss);
    return res;
  }

  int try_parse(){
      int res;
    try {
      res=giac_yyparse();
    }
    catch (std::runtime_error & error){
        cerr << "Error " << error.what() << endl;
        messages_to_print += string(error.what()) + '\n';
      return 1;
    }
    return res;
  }
  
  
  int protected_giac_yyparse(gen & parse_result){
      int s;
      s=the_lexer_string().size();
    if (!s)
      return 1;
    int res=try_parse();
    parse_result=parsed_gen;
    if (s==the_lexer_pos()){
        return res;
    }
    cerr << "In_VECTlete parse" << endl;
    return res;
  }

  gen::gen(const string & s){
    subtype=0;
    string ss(s);
    string::iterator it=ss.begin(),itend=ss.end();
    /*
    for (;it!=itend;++it)
      if (*it=='\\')
	*it=' ';
	*/
    set_lexer_string(ss);
    type=_INT_;
    if (protected_giac_yyparse(*this)){
      type=_STRNG;
      ref_count = new int(1);
      if (ss.empty())
	ss="""""";
      if (ss[0]!='"')
	ss = '"'+ss;
      if ((ss.size()==1) || (ss[ss.size()-1]!='"'))
	ss += '"';
      _STRNGptr = new string(ss.substr(1,ss.size()-2));
    }
  }

  gen::gen(const string & s,const vecteur & l){
    subtype=0;
    set_lexer_symbols(l);
    set_lexer_string(s);
      type=_INT_;
    if (protected_giac_yyparse(*this)){
      type=_STRNG;
      ref_count = new int(1);
      string ss(s);
      if (ss.empty())
	ss="""""";
      if (ss[0]!='"')
	ss = '"'+ss;
      if ((ss.size()==1) || (ss[ss.size()-1]!='"'))
	ss += '"';
      _STRNGptr = new string(ss.substr(1,ss.size()-2));
    }
  }

  gen::gen(const sparse_poly1 & p){
    subtype=0;
    type=_SPOL1;
    ref_count = new int(1);
    _SPOL1ptr= new sparse_poly1(p);
  }

  gen::gen(const unary_function_ptr & f,int nargs=1){
    subtype=0;
    type=_FUNC;
    subtype=nargs;
    ref_count = new int(1);
    _FUNCptr= new unary_function_ptr(f);
  }

  gen::~gen() {  
    if ((type>_DOUBLE_)){
      // if (type==debugtype)
      // cout << *this << " ~ Delete @ " << *debugtypeptr << "[" << *ref_count <<"-1]" << endl;
      (*ref_count)--;
      if (!*ref_count){
	delete ref_count;
	switch (type) {
	case _ZINT: 
	  mpz_clear(*_ZINTptr); 
	  delete  _ZINTptr;
	  break; 
	case _REAL:  
	  delete _REALptr;
	  break; 
	case _CPLX: 
	  delete [] _CPLXptr;
	  break; 
	case _IDNT: 
	  delete _IDNTptr;
	  break;
	case _VECT: 
	  //_VECTptr->clear();
	  delete _VECTptr;
	  break;
	case _SYMB: 
	  delete _SYMBptr;
	  break;
	case _USER:
	  _USERptr->memory_free(_USERptr);
	  break;
	case _EXT: 
	  delete [] _EXTptr;
	  break;
	case _MOD: 
	  delete [] _MODptr;
	  break;
	case _ROOT: 
	  delete _ROOTptr;
	  break;
	case _POLY:
	  //_POLYptr->coord.clear();
	  delete _POLYptr;
	  break;
	case _FRAC:
	  delete _FRACptr;
	  break;
	case _SPOL1:
	  delete _SPOL1ptr;
	  break;
	case _STRNG:
	  delete _STRNGptr;
	  break;
	case _FUNC:
	  delete _FUNCptr;
	  break;
	default: 
	  settypeerr("Gen Destructor");
	}
      }
    }
  }

  gen eval_VECT(const vecteur & v,int subtype,int level=100){
    // cerr << "Begin " << v << endl;
    vecteur w;
    vecteur::const_iterator it=v.begin(), itend=v.end();
    w.reserve(itend-it);
    for (;it!=itend;++it){
      if ((it->type==_SYMB) && (it->_SYMBptr->sommet==at_comment))
	continue;
      gen tmp=it->eval(level-1);
      if (subtype){
	if ((subtype==_SEQ__VECT)&&(tmp.type==_VECT) && (tmp.subtype==_SEQ__VECT)){
	  const_iterateur jt=tmp._VECTptr->begin(),jtend=tmp._VECTptr->end();
	  for (;jt!=jtend;++jt)
	    w.push_back(*jt);
	}
	else {
	  if ((subtype!=_SET__VECT) || (!equalposcomp(w,tmp)))
	    w.push_back(tmp);
	}
      }
      else
	w.push_back(tmp);
    }
    // cerr << "End " << v << " " << w << endl;
    return gen(w,subtype);
  }

      // evaluate _FUNCndary in RPN mode, f must be of type _FUNC
  gen rpneval_FUNC(const gen & f){
      int s=history_out.size();
      int nargs=max(f.subtype,0);
      if (!nargs){
	gen res=(*f._FUNCptr)(gen(history_out,_RPN_STACK__VECT));
	if ( (res.type!=_VECT) || (res.subtype!=_RPN_STACK__VECT))
	  res=gen(makevecteur(res),_RPN_STACK__VECT);
	history_out=*res._VECTptr;
	history_in=history_out;
	return res;
      }
      vecteur v(nargs);
      for (int i=nargs-1;i>=0;--i){
          v[i]=history_out.back();
          history_out.pop_back();
          history_in.pop_back();   
      }
      if (nargs==1)
          return (*f._FUNCptr)(v.front());
      else
          return (*f._FUNCptr)(v);
  }

  gen evalcomment(const vecteur & v,int level){
    const_iterateur it=v.begin(),itend=v.end();
    for (;it!=itend;++it){
      if ( (it->type!=_SYMB) || (it->_SYMBptr->sommet!=at_comment) )
	break;
    }
    if (it+1==itend)
      return it->eval(level);
    if (it!=itend)
      return eval_VECT(vecteur(it,itend),_SEQ__VECT,level);
    return zero;
  }

  gen check_not_assume(const gen & not_evaled,const gen & evaled, bool evalf_after){
    if ( (evaled.type==_VECT) && (evaled.subtype==_ASSUME__VECT) ){
      if ( evalf_after && (evaled._VECTptr->size()==2) && (evaled._VECTptr->back().type<=_CPLX) )
	return evaled._VECTptr->back().evalf();
      else
	return not_evaled;
    }
    else {
      if (evalf_after && (evaled.type!=_IDNT))
	return evaled.evalf();
      else
	return evaled;
    }
  }
  gen gen::eval(int level=100) const{
    control_c();
    if (!level)
      return *this;
    if ( (approx_mode) && (type<=_POLY) )
      return this->evalf();
    switch (type) {
    case _INT_: case _DOUBLE_: case _ZINT: case _REAL: case _CPLX: case _POLY: case _FRAC: case _SPOL1: case _EXT: case _STRNG:
      return *this;
    case _USER:
      return _USERptr->eval(level);
    case _IDNT:
      if (*_IDNTptr==_IDNT_pi || *_IDNTptr==e__IDNT )
	return *this;
      if (subtype==_GLOBAL__EVAL)
	return check_not_assume(*this,global_eval(*_IDNTptr,level),false);
      return check_not_assume(*this,_IDNTptr->eval(level-1),false);
    case _VECT:
      if (subtype==_SPREAD__VECT){
	makespreadsheetmatrice(*_VECTptr);
	spread_eval(*_VECTptr);
	return *this;
      }
      if (subtype==_FOLDER__VECT)
	return *this;
      if ( (subtype==_SEQ__VECT) && (!_VECTptr->empty()) && (_VECTptr->front().type==_SYMB) && (_VECTptr->front()._SYMBptr->sommet==at_comment) && (_VECTptr->back().type==_SYMB) && (_VECTptr->back()._SYMBptr->feuille==at_return ) )
	return evalcomment(*_VECTptr,level);
      return gen(eval_VECT(*_VECTptr,subtype,level));
    case _SYMB:
      if (subtype==_SPREAD__SYMB)
	return *this;
      return _SYMBptr->eval(level-1);
    case _MOD:
      return makemod(_MODptr->eval(),(_MODptr+1)->eval());
    case _FUNC:
      if (rpn_mode && (history_out.size()>=subtype))
	return rpneval_FUNC(*this);
      else {
	if (subtype)
	  return *this; 
	else
	  return (*_FUNCptr)(gen(vecteur(0),_SEQ__VECT));
      }
    default: 
      settypeerr("Eval") ;
    }
  }

  symbolic _FRAC2_SYMB(const fraction & f){
    if (is_one(f.num))
      return symb_inv(f.den);
    if (is_minus_one(f.num))
      return symb_inv(-f.den);      
    return symbolic(at_prod,makevecteur(f.num,symb_inv(f.den)));
  }

  symbolic _FRAC2_SYMB(const gen & e){
#ifdef DEBUG_SUPPORT
    assert(e.type==_FRAC);
#endif
    return _FRAC2_SYMB(*e._FRACptr);
  }

  symbolic _FRAC2_SYMB(const gen & n,const gen & d){
    return symbolic(at_prod,makevecteur(n,symb_inv(d)));
  }

  gen evalf_VECT(const vecteur & v,int subtype){
    bool save_is_inevalf=is_inevalf;
    is_inevalf=true;
    vecteur w;
    vecteur::const_iterator it=v.begin(), itend=v.end();
    w.reserve(itend-it);
    for (;it!=itend;++it){
      gen tmp=it->evalf();
      if (subtype){
	if ((subtype==_SEQ__VECT)&&(tmp.type==_VECT) && (tmp.subtype==_SEQ__VECT)){
	  const_iterateur jt=tmp._VECTptr->begin(),jtend=tmp._VECTptr->end();
	  for (;jt!=jtend;++jt)
	    w.push_back(*jt);
	}
	else {
	  if ((subtype==_SEQ__VECT) || (!equalposcomp(w,tmp)))
	    w.push_back(tmp);
	}
      }
      else
	w.push_back(tmp);
    }
    is_inevalf=save_is_inevalf;
    return gen(w,subtype);
  }

  gen gen::evalf() const{
    switch (type) {
    case _DOUBLE_: case _REAL: case _STRNG:
      return *this;
    case _INT_:
      return double(val);
    case _ZINT:
      return mpz_get_d(*_ZINTptr);
    case _CPLX: 
      return gen(_CPLXptr->evalf(),(_CPLXptr+1)->evalf());
    case _USER:
      return _USERptr->evalf();
    case _IDNT:
      if (subtype==_GLOBAL__EVAL)
	return check_not_assume(*this,global_eval(*this,100),true);
      return check_not_assume(*this,_IDNTptr->eval(),true);
    case _VECT:
      return gen(evalf_VECT(*_VECTptr,subtype));
    case _SYMB:
      if ( (_SYMBptr->sommet==at_pow) && (_SYMBptr->feuille._VECTptr->back().type==_INT_))
	return pow(_SYMBptr->feuille._VECTptr->front().evalf(),_SYMBptr->feuille._VECTptr->back());
      if (_SYMBptr->sommet==at_integrate)
	return _romberg(_SYMBptr->feuille);
      if (_SYMBptr->sommet==at_rootof)
	return approx_rootof(_SYMBptr->feuille);
      return _SYMBptr->evalf();
    case _FRAC:
      return rdiv(_FRACptr->num.evalf(),_FRACptr->den.evalf());
    case _FUNC: case _MOD: case _ROOT:
      return *this; // replace in RPN mode
    case _EXT:
      return alg_evalf(*_EXTptr,*(_EXTptr+1));
    default: 
      settypeerr("Evalf") ;
    }
  }
  
  gen & gen::operator = (const gen & a) { 
    register unsigned t=(type << _DECALAGE) | a.type;
    if (!t){
      subtype=a.subtype;
      val=a.val;
      return *this;
    }
    // Copy before deleting because the target might be embedded in a
    // with a ref_count of a equals to 1
    int * ref_count_save=ref_count;
    int type_save=type;
    void * ptr_save = _ZINTptr;
    subtype=a.subtype;
    type=a.type;
    // if (type==debugtype)
    // cout << a << "Overwriting by = " << *a.debugtypeptr << "[" << *(a.ref_count) << "+1]"<< endl ;
    switch (type) {
    case _INT_:
      val=a.val;
      break;
    case _DOUBLE_:
      _DOUBLE_val=a._DOUBLE_val;
      break;
    case _ZINT: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _ZINTptr=a._ZINTptr;
      break; 
    case _REAL: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _REALptr=a._REALptr;
      break; 
    case _CPLX: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _CPLXptr=a._CPLXptr;
      break;
    case _IDNT: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _IDNTptr=a._IDNTptr;
      break;
    case _VECT: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _VECTptr=a._VECTptr;
      break;
    case _SYMB: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _SYMBptr=a._SYMBptr;
      break;
    case _USER: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _USERptr=a._USERptr;
      break;
    case _EXT: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _EXTptr=a._EXTptr;
      break;
    case _MOD: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _MODptr=a._MODptr;
      break;
    case _ROOT: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _ROOTptr=a._ROOTptr;
      break;
    case _POLY: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _POLYptr=a._POLYptr;
      break;
    case _FRAC: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _FRACptr=a._FRACptr;
      break;
    case _SPOL1: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _SPOL1ptr=a._SPOL1ptr;
      break;
    case _STRNG: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _STRNGptr=a._STRNGptr;
      break;
    case _FUNC: 
      ref_count=a.ref_count;
      (*ref_count)++;
      _FUNCptr=a._FUNCptr;
      break;
    default: 
      break ;
    }
    // Now we delete the copied object
    if ((type_save>_DOUBLE_)){
      // if (type==debugtype)
      // cout << *this << " Destruction by = of " << *debugtypeptr << "[" << *ref_count << "-1]" << endl;
      (*ref_count_save)--;
      if (!*ref_count_save){
	delete ref_count_save;
	switch (type_save) {
	case _ZINT: 
	  mpz_clear( * (mpz_t *) ptr_save);
	  delete (mpz_t *) ptr_save;
	  break; 
	case _REAL: 
	  delete (real_interval *) ptr_save;
	  break; 
	case _CPLX: 
	  delete [] (gen *) ptr_save ;
	  break; 
	case _IDNT: 
	  delete (identificateur *) ptr_save ;
	  break;
	case _SYMB: 
	  delete (symbolic *) ptr_save;
	  break;
	case _USER: 
	  ((gen_user *) ptr_save)->memory_free((gen_user *) ptr_save);
	  break;
	case _EXT: 
	  delete [] (gen * ) ptr_save;
	  break;
	case _MOD: 
	  delete [] (gen * ) ptr_save;
	  break;
	case _ROOT:
	  delete (real_complex_rootof *) ptr_save;
	  break;
	case _VECT: 
	  //((vecteur *) ptr_save)->clear();
	  delete (vecteur *) ptr_save;
	  break;
	case _POLY:
	  //((polynome *) ptr_save)->coord.clear();
	  delete (polynome *) ptr_save;
	  break;
	case _FRAC:
	  delete (fraction *) ptr_save;
	  break;
	case _SPOL1:
	  delete (sparse_poly1 *) ptr_save;
	  break;
	case _STRNG:
	  delete (string *) ptr_save;
	  break;
	case _FUNC:
	  delete (unary_function_ptr *) ptr_save;
	  break;
	default: 
	  settypeerr("Gen Operator =");
	}
      }
    }
    return *this;
  }
  
  int gen::to_int() const {
    switch (type ) {
    case _INT_: 
      return val;
    case _ZINT: 
      return mpz_get_si(*_ZINTptr);
    case _CPLX: 
      return _CPLXptr->to_int() ;
    default:
      settypeerr("To_int");
    }
  }
  
  bool gen::is_real() const {
    switch (type) {
    case _INT_: case _DOUBLE_: case _ZINT:
      return true; 
    case _CPLX: 
      return (is_zero(*(_CPLXptr+1)));
    default: 
      return is_zero(im());
    }
  }
  
  bool gen::is_cinteger() const {
    switch (type ) {
    case _INT_: case _ZINT: 
      return true; 
    case _CPLX:
      return _CPLXptr->is_integer() && (_CPLXptr+1)->is_integer();
    default: 
      return false;
    }
  }
   
  bool gen::is_integer() const {
    switch (type ) {
    case _INT_: case _ZINT:
      return true;
    case _CPLX:
      return is_zero(*(_CPLXptr+1));
    default: 
      return false;
    }
  }

  bool _VECT_is_constant(const vecteur & v){
      const_iterateur it=v.begin(),itend=v.end();
      for (;it!=itend;++it)
          if (!(it->is_constant()))
              return false;
      return true;
  }
  
  bool gen::is_constant() const {
    switch (type ) {
    case _INT_: case _DOUBLE_: case _REAL: case _ZINT: case _CPLX:
      return true;
        case _VECT:
            return _VECT_is_constant(*this->_VECTptr);
        case _EXT:
            return _EXTptr->is_constant() && (_EXTptr+1)->is_constant();
        case _POLY:
            return Tis_constant<gen>(*_POLYptr) && _POLYptr->coord.front().value.is_constant();
    default: 
      return false;
    }
  }

  bool is_atomic(const gen & e){
    return (e.type<_POLY);
  }

  void gen::uncoerce() {
    if (type==_INT_){
      int tmp =val;
      _ZINTptr = new mpz_t[1];
      mpz_init_set_si(*_ZINTptr,tmp); 
      ref_count=new int(1);
      type=_ZINT;
    }
  }

  vecteur _VECTconj(const vecteur & a){
    vecteur res;
    vecteur::const_iterator it=a.begin(),itend=a.end();
    for (;it!=itend;++it)
      res.push_back(it->conj());
    return res;
  }

  gen gen::conj() const {
    switch (type ) {
    case _INT_: case _DOUBLE_: case _ZINT: case _REAL: case _STRNG:
      return *this;
    case _CPLX:
      return gen(*_CPLXptr,-(*(_CPLXptr+1)));
    case _VECT:
      return gen(_VECTconj(*_VECTptr),subtype);
    case _USER:
      return _USERptr->conj();
    case _IDNT: 
      if (!complex_variables)
	return *this;
      if ( (_IDNTptr->value) && (is_zero(_IDNTptr->value->im())) )
	return *this;
      return symbolic(at_conj,*this);
    case _SYMB:
      if (equalposcomp(plot_sommets,_SYMBptr->sommet) || equalposcomp(analytic_sommets,_SYMBptr->sommet))
	return symbolic(_SYMBptr->sommet,_SYMBptr->feuille.conj());
      else
	return symbolic(at_conj,*this);
    case _FRAC:
      return fraction(_FRACptr->num.conj(),_FRACptr->den.conj());
    case _MOD:
      return makemod(_MODptr->conj(),*(_MODptr+1));
    default: 
      settypeerr("Conj");
    }
  }
   
  vecteur _VECTre(const vecteur & a){
    vecteur res;
    vecteur::const_iterator it=a.begin(),itend=a.end();
    for (;it!=itend;++it)
      res.push_back(it->re());
    return res;
  }

  vecteur pascal_next_line(const vecteur & v){
    if (v.empty())
      return vecteur(1,plus_one);
    const_iterateur it=v.begin(),itend=v.end();
    gen current(*it);
    vecteur w;
    w.reserve(itend-it+1);
    w.push_back(current);
    for (++it;it!=itend;++it){
      w.push_back(*it+current);
      current=*it;
    }
    w.push_back(plus_one);
    return w;
  }

  vecteur pascal_nth_line(int n){
    n=absint(n);
    vecteur v(1,plus_one);
    for (int i=0;i<n;++i)
      v=pascal_next_line(v);
    return v;
  }

  gen symb_re(const symbolic & s){
    unary_function_ptr u=s.sommet;
    gen f=s.feuille;
    if ( (u==at_re) || (u==at_im) || (u==at_abs) )// re(re), re(im), re(abs)
      return s;
    if (u==at_conj)
      return re(f);
    if (u==at_plus)
      return _plus(re(f));
    if (u==at_neg)
      return -re(f);
    if (u==at_division)
      return re(f[0]*inv(f[1]));
    if (u==at_sqrt)
      return re(pow(f,plus_one_half));
    if (u==at_prod){
      if (f.type!=_VECT)
	return re(f);
      vecteur v(*f._VECTptr);
      if (v.empty())
	return plus_one;
      if (v.size()==1)
	return re(v.front());
      // cut v in 2 parts and recursive call
      // re(a*b)=re(a)*re(b)-im(a)*im(b)
      const_iterateur it=v.begin(),itend=v.end();
      const_iterateur itm=it+(itend-it+1)/2;
      gen a(symbolic(u,vecteur(it,itm)));
      gen b(symbolic(u,vecteur(itm,itend)));
      return re(a)*re(b)-im(a)*im(b);
    }
    if (u==at_pow){
      if ( is_zero(im(f._VECTptr->back())) && is_zero(im(f._VECTptr->front())) )
	return s;
      if (f._VECTptr->back().type==_INT_){
	gen e=f._VECTptr->front();
	gen r=re(e);
	gen i=im(e);
	int n=f._VECTptr->back().val;
	if (n==0)
	  return plus_one;
	if (is_zero(i))
	  return pow(r,n);
	if (is_zero(r)){
	  if (n%2)
	    return zero;
	  if (n%4==2)
	    return -pow(i,n);
	  else
	    return pow(i,n);
	}
	bool n_pos=(n>0);
	if (!n_pos)
	  return re(inv(pow(e,-n)));
	vecteur v=pascal_nth_line(n);
	vecteur somme;
	gen signe=plus_one; 
	const_iterateur it=v.begin(),itend=v.end();
	for (int j=0;j<=n;j+=2){
	  somme.push_back(signe*(*it)*pow(r,n-j)*pow(i,j));
	  ++it;
	  ++it;
	  signe=-signe;
	}
	gen res=symbolic(at_plus,somme);
	return res;
      }
    }
    gen imf=im(f);
    if (is_zero(imf) && equalposcomp(reim_op,u))
      return s;
    if (u==at_inv){
      gen tmp=re(f);
      return tmp*inv(pow(tmp,2)+pow(imf,2));
    }
    if (u==at_exp) // re(exp(f))=exp(re(f))*cos(im(f))
      return exp(re(f))*cos(imf);
    if (u==at_ln)
      return ln(abs(f));
    if (u==at_cos)
      return cosh(imf)*cos(re(f));
    if (u==at_sin)
      return cosh(imf)*sin(re(f));
    if (u==at_tan)
      return re(rdiv(sin(f),cos(f)));
    if (u==at_cosh)
      return cos(imf)*cosh(re(f));
    if (u==at_sinh)
      return cos(imf)*sinh(re(f));
    if (u==at_tanh)
      return re(rdiv(sinh(f),cosh(f)));
    return symbolic(at_re,gen(s));
  }

  gen gen::re() const {
    switch (type ) {
    case _INT_: case _DOUBLE_: case _ZINT: case _REAL: case _STRNG:
      return *this;
    case _CPLX:
      return *_CPLXptr;
    case _VECT:
      return gen(_VECTre(*_VECTptr),subtype);
    case _IDNT: 
      if (!complex_variables)
	return *this;
      if ( (_IDNTptr->value) && (is_zero(_IDNTptr->value->im())) )
	return *this;
      return symbolic(at_re,*this);
    case _SYMB:
      if (equalposcomp(plot_sommets,_SYMBptr->sommet))
	return symbolic(_SYMBptr->sommet,_SYMBptr->feuille.re());
      if (expand_re_im)
	return symb_re(*_SYMBptr);
      else
	return symbolic(at_re,*this);
    case _USER:
      return _USERptr->re();
    case _FRAC:
      return symb_re(symbolic(at_prod,makevecteur(_FRACptr->num,symbolic(at_inv,_FRACptr->den))));
    case _MOD:
      return makemod(_MODptr->re(),*(_MODptr+1));
    default: 
      settypeerr("Re");
    }
  }
   
  vecteur _VECTim(const vecteur & a){
    vecteur res;
    vecteur::const_iterator it=a.begin(),itend=a.end();
    for (;it!=itend;++it)
      res.push_back(it->im());
    return res;
  }

  gen symb_im(const symbolic & s){
    unary_function_ptr u=s.sommet;
    gen f=s.feuille;
    if ( (u==at_re) || (u==at_im) || (u==at_abs) )// im of a real
      return zero;
    if (u==at_conj)
      return -im(f);
    if (u==at_plus)
      return _plus(im(f));
    if (u==at_neg)
      return -im(f);
    if (u==at_division)
      return im(f[0]*inv(f[1]));
    if (u==at_sqrt)
      return im(pow(f,plus_one_half));
    if (u==at_prod){
      if (f.type!=_VECT)
	return im(f);
      vecteur v(*f._VECTptr);
      if (v.size()==1)
	return im(v.front());
      // cut v in 2 parts and recursive call
      // im(a*b)=re(a)*im(b)+im(a)*re(b)
      const_iterateur it=v.begin(),itend=v.end();
      const_iterateur itm=it+(itend-it+1)/2;
      gen a(symbolic(u,vecteur(it,itm)));
      gen b(symbolic(u,vecteur(itm,itend)));
      return re(a)*im(b)+im(a)*re(b);
    }
    if (u==at_pow){
      if ( is_zero(im(f._VECTptr->back())) && is_zero(im(f._VECTptr->front())) )
	return zero;
      if (f._VECTptr->back().type==_INT_) {
	gen e=f._VECTptr->front();
	gen r=re(e);
	gen i=im(e);
	int n=f._VECTptr->back().val;
	if (n==0)
	  return zero;
	if (is_zero(i))
	  return zero;
	if (is_zero(r)){
	  if (n%2==0)
	    return zero;
	  if (n%4==1)
	    return pow(i,n);
	  else
	    return -pow(i,n);
	}
	bool n_pos=(n>0);
	if (!n_pos)
	  return im(inv(pow(f,-n)));
	vecteur v=pascal_nth_line(n);
	vecteur somme;
	gen signe=plus_one; 
	const_iterateur it=v.begin(),itend=v.end();
	++it;
	for (int j=1;j<=n;j+=2){
	  somme.push_back(signe*(*it)*pow(r,n-j)*pow(i,j));
	  ++it;
	  ++it;
	  signe=-signe;
	}
	gen res=symbolic(at_plus,somme);
	return res;
      }
    }
    gen imf=im(f);
    if (is_zero(imf) && equalposcomp(reim_op,u))
      return zero;
    if (u==at_inv)
      return -imf*inv(pow(re(f),2)+pow(imf,2));
    if (u==at_exp) // im(exp(f))=exp(re(f))*sin(im(f))
      return exp(re(f))*sin(imf);
    if (u==at_ln)
      return arg(f);
    if (u==at_cos)
      return -sinh(imf)*sin(re(f));
    if (u==at_sin)
      return sinh(imf)*cos(re(f));
    if (u==at_tan)
      return im(rdiv(sin(f),cos(f)));
    if (u==at_cosh)
      return sin(imf)*sinh(re(f));
    if (u==at_sinh)
      return sin(imf)*cosh(re(f));
    if (u==at_tanh)
      return im(rdiv(sinh(f),cosh(f)));
    return symbolic(at_im,gen(s));
  }

  gen gen::im() const {
    switch (type) {
    case _INT_: case _DOUBLE_: case _ZINT: case _REAL: case _STRNG:
      return 0;
    case _CPLX:
      return *(_CPLXptr+1);
    case _VECT:
      return gen(_VECTim(*_VECTptr),subtype);
    case _IDNT: 
      if (!complex_variables)
	return zero;
      if ( (_IDNTptr->value) && (is_zero(_IDNTptr->value->im())) )
	return zero;
      return symbolic(at_im,*this);
    case _SYMB:      
      if (equalposcomp(plot_sommets,_SYMBptr->sommet))
	return symbolic(_SYMBptr->sommet,_SYMBptr->feuille.im());
      if (expand_re_im)
	return symb_im(*_SYMBptr);
      else
	return symbolic(at_im,*this);
    case _USER:
      return _USERptr->im();
    case _FRAC:
      return symb_im(symbolic(at_prod,makevecteur(_FRACptr->num,symbolic(at_inv,_FRACptr->den))));
    case _MOD:
      return makemod(_MODptr->im(),*(_MODptr+1));
    default: 
      settypeerr("Im");
    }
  }

  gen _VECTabs(const vecteur & a){
    gen res(0);
    vecteur::const_iterator it=a.begin(), itend=a.end();
    for (;it!=itend;++it){
      res=max(res,abs(*it));
    }
    return res;
  }

  gen sq(const gen & a){
    return a*a;
  }

  gen real_abs(const gen & s){
    int j=sturmsign(s);
    if (!j)
      return symbolic(at_abs,gen(s));
    return j*s;
  }

  gen idnt_abs(const gen & s){
    if (is_inf(s))
      return plus_inf;
    gen i=im(s);
    if (is_zero(i))
      return real_abs(s);
    else
      return sqrt(pow(re(s),2)+pow(i,2));
  }

  gen symb_abs(const symbolic & s){
    unary_function_ptr u=s.sommet;
    gen f=s.feuille;
    if (u==at_abs) // abs(abs)
      return s;
    if (u==at_neg)
      return abs(f);
    if (!complex_mode){ 
      if (u==at_ln)
	return real_abs(s);
      if (u==at_exp || u==at_sqrt)
	return s;
    }
    if ( (u==at_pow) && (is_zero(im(f._VECTptr->back()))))
      return symbolic(u,makevecteur(abs(f._VECTptr->front()),f._VECTptr->back()));
    if (u==at_inv)
      return inv(abs(f));
    if (u==at_prod)
      return symbolic(u,apply(f,abs));
    return idnt_abs(s);
  }

  gen abs(const gen & a){ 
    if (is_equal(a))
      return apply_to_equal(a,abs);
    switch (a.type ) {
    case _INT_: 
      return(absint(a.val));
    case _ZINT: 
      if (mpz_sgn(*a._ZINTptr)<0)
	return(-a);
      else
	return(a);
    case _CPLX: 
      return sqrt(sq(*a._CPLXptr)+sq(*(a._CPLXptr+1))) ;
    case _DOUBLE_:
      return fabs(a._DOUBLE_val);
    case _VECT:
      return _VECTabs(*a._VECTptr);
    case _IDNT:
      return idnt_abs(a);
    case _SYMB:
      return symb_abs(*a._SYMBptr);
    case _USER:
      return a._USERptr->abs();
    case _FRAC:
      return fraction(abs(a._FRACptr->num),abs(a._FRACptr->den));
    default:
      settypeerr("Abs");
    }
  }

  gen l1norm(const gen & a){ // L^1 norm: |re|+|im| for a complex
    switch (a.type ) {
    case _INT_: 
      return(absint(a.val));
    case _ZINT: 
      if (mpz_sgn(*a._ZINTptr)<0)
	return(-a);
      else
	return(a);
    case _CPLX: 
      return(abs(*a._CPLXptr)+abs(*(a._CPLXptr+1))) ;
    case _DOUBLE_:
      return fabs(a._DOUBLE_val);
    case _VECT:
      return _VECTabs(*a._VECTptr);
    case _USER:
      return a._USERptr->abs();
    case _IDNT: case _SYMB:
      return symbolic(at_abs,a);
    default:
      settypeerr("L1norm");
    }
  }

  gen arg_CPLX(const gen & a){
    gen realpart=a.re(),imaginaire=a.im();
    if (is_zero(realpart)){
      if (is_zero(imaginaire))
	return undef;
      return cst_pi_over_2*sign(imaginaire);
    }
    if (is_zero(imaginaire))
      return (1-sign(realpart))*cst_pi_over_2;
    if ( (realpart.type==_DOUBLE_) || (imaginaire.type==_DOUBLE_) )
        return eval(atan(rdiv(imaginaire,realpart))+(1-sign(realpart))*sign(imaginaire)*evalf(cst_pi_over_2));
    else
        return eval(atan(rdiv(imaginaire,realpart))+(1-sign(realpart))*sign(imaginaire)*cst_pi_over_2);
  }
  
  gen _VECTarg(const vecteur & a){
    vecteur res;
    vecteur::const_iterator it=a.begin(), itend=a.end();
    for (;it!=itend;++it){
      res.push_back(arg(*it));
    }
    return res;
  }

  gen arg(const gen & a){ 
    if (is_equal(a))
      return apply_to_equal(a,arg);
    checkanglemode();
    switch (a.type ) {
    case _INT_: case _ZINT: case _DOUBLE_: case _REAL:
      if (is_positive(a))
	return 0;
      else
	return cst_pi;
    case _CPLX:
      return arg_CPLX(a);
    case _VECT:
      return _VECTarg(*a._VECTptr);
    case _IDNT: 
    case _SYMB:
      if ( is_zero(im(a)) || (evalf(a).type==_CPLX) )
	return arg_CPLX(a);
      else
	return symbolic(at_arg,a);
    case _USER:
      return a._USERptr->arg();
    case _FRAC:
      return arg(a._FRACptr->num)-arg(a._FRACptr->den);
    default:
      settypeerr("Arg");
    }
  }

  gen gen::squarenorm() const {
    switch (type ) {
    case _INT_: case _DOUBLE_: case _ZINT: case _REAL:
      return (*this) * (*this);
    case _CPLX:
      return ( (*_CPLXptr)*(*_CPLXptr)+(*(_CPLXptr+1)*(*(_CPLXptr+1))) );      
    default: 
      return( (*this) * this->conj());
    }    
  }

  int gen::bindigits() const{
    int res,valeur;
    switch (type ) {
    case _INT_: 
      res=0;
      valeur=val;
      for (;valeur;res++)
	valeur = valeur >> 1;
      return res; 
    case _ZINT:
      return mpz_sizeinbase(*_ZINTptr,2)+1;
    case _CPLX: 
      return max(_CPLXptr->bindigits(),(_CPLXptr+1)->bindigits() ) ;
    default:
      settypeerr("Bindigits");
    }
  }

  gen gen::operator [] (int i) const{
      if (type==_SYMB){
          if (!i)
              return _SYMBptr->sommet;
          if (_SYMBptr->feuille.type!=_VECT){
              if (i==1)
                  return _SYMBptr->feuille;
              else
                  setdimerr();
          }
          if (i>_SYMBptr->feuille._VECTptr->size())
              setdimerr();
          return (*(_SYMBptr->feuille._VECTptr))[i-1];
          
      }
    if (this->type!=_VECT)
      settypeerr("Gen [int]");
    if (i>=_VECTptr->size())
      setdimerr();
    return (*(this->_VECTptr))[i];
  }

  gen gen::operator [] (const gen & i) const{
    if ((type==_STRNG) && (i.type==_INT_)){
      int s=_STRNGptr->size();
      if ( (i.val<s) && (i.val>=0))
	return string2gen(string()+'"'+(*_STRNGptr)[i.val]+'"');
    }
    if (type==_USER)
      return (*_USERptr)[i];
    if (i.type==_DOUBLE_)
      return (*this)[(int) i._DOUBLE_val];
    if (i.type==_SYMB){
      if (i._SYMBptr->sommet==at_interval) {
	if ((i._SYMBptr->feuille._VECTptr->front().type==_INT_) && (i._SYMBptr->feuille._VECTptr->back().type==_INT_) ){
	  int debut=i._SYMBptr->feuille._VECTptr->front().val,fin=i._SYMBptr->feuille._VECTptr->back().val;
	  if (fin<debut)
	    swap(debut,fin);
	  if (type==_STRNG)
	    return string2gen('"'+_STRNGptr->substr(debut,fin-debut+1)+'"');
	  if (type==_VECT){
	    debut=max(debut,0);
	    fin=min(fin,_VECTptr->size()-1);
	    return gen(vecteur(_VECTptr->begin()+debut,_VECTptr->begin()+fin+1),subtype);
	  }
	}
      }
    }
    if (i.type==_VECT){
      const_iterateur it=i._VECTptr->begin(),itend=i._VECTptr->end();
      gen res (*this);
      for (;it!=itend;++it){
	if ( (it->type==_SYMB) && (it->_SYMBptr->sommet==at_interval) && (it+1!=itend) ){
	  // submatrix extraction
	  if ((it->_SYMBptr->feuille._VECTptr->front().type==_INT_) && (it->_SYMBptr->feuille._VECTptr->back().type==_INT_) ){
	    int debut=it->_SYMBptr->feuille._VECTptr->front().val,fin=it->_SYMBptr->feuille._VECTptr->back().val;
	    if (fin<debut)
	      swap(debut,fin);
	    if (res.type==_VECT){
	      debut=max(debut,0);
	      fin=min(fin,res._VECTptr->size()-1);
	      iterateur jt=res._VECTptr->begin()+debut,jtend=_VECTptr->begin()+fin+1;
	      gen fin_it(vecteur(it+1,itend),_SEQ__VECT);
	      vecteur v;
	      v.reserve(jtend-jt);
	      for (;jt!=jtend;++jt)
		v.push_back((*jt)[fin_it]);
	      return gen(v,res.subtype);
	    }
	  }
	}
	res = res[*it];
      }
      return res;
    }
    if (i.type!=_INT_)
      settypeerr("Gen [gen]");
    return (*this)[i.val];
  }

  /*
  gen & gen::operator [](int i){
    if (this->type!=_VECT)
      settypeerr("Gen [int]");
    if (i>=_VECTptr->size())
      setdimerr();
    return (*(this->_VECTptr))[i];
  }
  
  gen & gen::operator [] (const gen & i) {
    if (i.type==_DOUBLE_)
      return (*this)[(int) i._DOUBLE_val];
    if (i.type!=_INT_)
      settypeerr("Gen [gen]");
    if (this->type!=_VECT)
      settypeerr("Gen [gen]");
    if (i.val>=_VECTptr->size())
      setdimerr("Gen [_VECT]");
    return (*(this->_VECTptr))[i.val];
  }
  */

  gen gen::operator () (const gen & i) const{
    if (type==_SYMB){
        if (_SYMBptr->sommet==at_program) {
            gen tmp=_SYMBptr->feuille;
            if (tmp.type!=_VECT)
                setsizeerr();
            (*tmp._VECTptr)[1]=i;
            return _program(tmp,undef);
        }
	if (_SYMBptr->sommet==at_rpn_prog){
	  vecteur pile;
	  if (rpn_mode)
	    pile=history_out;
	  if ( (i.type!=_VECT) || (i.subtype!=_SEQ__VECT))
	    pile.push_back(i);
	  else 
	    pile=mergevecteur(pile,*i._VECTptr);
	  vecteur prog;
	  if (_SYMBptr->feuille.type==_VECT)
	    prog=*_SYMBptr->feuille._VECTptr;
	  else
	    prog=vecteur(1,_SYMBptr->feuille);
	  return gen(rpn_eval(prog,pile),_RPN_STACK__VECT);
	}
        if (_SYMBptr->sommet==at_compose){
            gen tmp=_SYMBptr->feuille;
            if (tmp.type!=_VECT)
                return tmp(i);
            gen res=i;
            const_iterateur it=tmp._VECTptr->begin(),itend=tmp._VECTptr->end();
            for (;itend!=it;){
                --itend;
                res=(*itend)(res);
            }
            return res;
        }
        if (_SYMBptr->feuille==makevecteur(zero)){
            return _SYMBptr->sommet(i);
        }
    }
    if (type==_FUNC){
      if ( (i.type==_VECT) && (i.subtype==_SEQ__VECT) && (i._VECTptr->size()==1))
	return (*_FUNCptr)(i._VECTptr->front());
      else
	return (*_FUNCptr)(i);
    }
    if (i.type==_DOUBLE_)
      return (*this)((int) i._DOUBLE_val);
    if (type<_IDNT)
      return *this;
    if (type==_USER)
      return (*_USERptr)(i);
    if (type==_VECT)
      return (*this)[i];
    else
      return symb_of(*this,i);
  }

  // return true if *this is "strictly less complex" than other
  bool gen::islesscomplexthan (const gen & other ) const {
    if (is_zero(*this))
      return false;
    if (is_zero(other))
      return true;
    if (type != other.type)
      return type < other.type;
    switch ( type) {
    case _INT_:
      return absint(val)<absint(other.val);
    case _ZINT:
      return is_greater(abs(other),abs(*this));
    case _DOUBLE_: case _CPLX:
      return is_greater(abs(*this),abs(other));
    case _IDNT:
      return _IDNTptr->name < other._IDNTptr->name;
    case _POLY:
      if (_POLYptr->coord.size()!=other._POLYptr->coord.size())
	return _POLYptr->coord.size()<other._POLYptr->coord.size();
      return _POLYptr->coord.front().value.islesscomplexthan(other._POLYptr->coord.front().value);
    case _MOD:
      if (*(_MODptr+1)!=*(other._MODptr+1))
	setsizeerr();
      return _MODptr->islesscomplexthan(*other._MODptr);
    case _SYMB:
      return false; // &_SYMBptr->sommet < &other._SYMBptr->sommet ?
    case _EXT:
      if (*(_EXTptr+1)!=*(other._EXTptr+1))
	return (_EXTptr+1)->islesscomplexthan(*(other._EXTptr+1));
      return _EXTptr->islesscomplexthan(*(other._EXTptr));
    default:
      return is_greater(abs(*this),abs(other));
    }
  }

  bool islesscomplexthan(const gen & a,const gen & b){
    return a.islesscomplexthan(b);
  }

  int gen::symb_size () const {
    if (type==_SYMB)
      return _SYMBptr->size();
    else
      return 1;
  }

  bool symb_size_less(const gen & a,const gen & b){
    return a.symb_size() < b.symb_size();
  }

  bool gen::is_symb_of_sommet(const unary_function_ptr & u) const {
    return type==_SYMB && _SYMBptr->sommet==u;
  }

  gen operator && (const gen & a,const gen & b){
    if (is_zero(a)){
      if (b.type==_DOUBLE_)
	return 0.0;
      return a;
    }
    if (is_zero(b)){
      if (a.type==_DOUBLE_)
	return 0.0;
      return b;
    }
    if (a.is_symb_of_sommet(at_and)){
      if (b.is_symb_of_sommet(at_and))
	return symbolic(at_and,mergevecteur(*a._SYMBptr->feuille._VECTptr,*b._SYMBptr->feuille._VECTptr));
      vecteur v=*a._SYMBptr->feuille._VECTptr;
      v.push_back(b);
      return symbolic(at_and,v);
    }
    if (b.is_symb_of_sommet(at_and)){
      vecteur v=*b._SYMBptr->feuille._VECTptr;
      v.push_back(a);
      return symbolic(at_and,v);
    }
    if ( ((a.type==_IDNT) || (a.type==_SYMB)) || ((a.type==_IDNT) || (a.type==_SYMB)) )
      return symb_and(a,b);
    if ( (a.type==_DOUBLE_) || (b.type==_DOUBLE_) )
      return 1.0;
    return plus_one;
  }

  gen operator || (const gen & a,const gen & b){
    if (is_zero(a))
      return b;
    if (is_zero(b))
      return a;
    if (a.is_symb_of_sommet(at_ou)){
      if (b.is_symb_of_sommet(at_ou))
	return symbolic(at_ou,mergevecteur(*a._SYMBptr->feuille._VECTptr,*b._SYMBptr->feuille._VECTptr));
      vecteur v=*a._SYMBptr->feuille._VECTptr;
      v.push_back(b);
      return symbolic(at_ou,v);
    }
    if (b.is_symb_of_sommet(at_ou)){
      vecteur v=*b._SYMBptr->feuille._VECTptr;
      v.push_back(a);
      return symbolic(at_ou,v);
    }
    if ( ((a.type==_IDNT) || (a.type==_SYMB)) || ((a.type==_IDNT) || (a.type==_SYMB)) )
      return symb_ou(a,b);
    if ( (a.type==_DOUBLE_) || (b.type==_DOUBLE_) )
      return 1.0;
    return plus_one;
  }

  gen addpoly(const gen & th, const gen & other){
    if ((th.type!=_POLY) || (other.type!=_POLY))
      settypeerr("addpoly");
    // Tensor addition
    vector< monomial<gen> >::const_iterator a=th._POLYptr->coord.begin();
    vector< monomial<gen> >::const_iterator a_end=th._POLYptr->coord.end();
    if (a == a_end) {
      return other;
    }
    vector< monomial<gen> >::const_iterator b=other._POLYptr->coord.begin();
    vector< monomial<gen> >::const_iterator b_end=other._POLYptr->coord.end();
    if (b==b_end){
      return th;
    }
    polynome * resptr=new polynome(th._POLYptr->dim);
    Add<gen>(a,a_end,b,b_end,resptr->coord,th._POLYptr->is_strictly_greater);
    return resptr;
  }

  polynome addpoly(const polynome & p,const gen & c){
    if (is_zero(c))
      return p;
    polynome pcopy(p);
    if ( (!p.coord.empty()) && is_zero(*(p.coord.back().index.iptr)) ) {
      pcopy.coord.back().value = pcopy.coord.back().value + c;
      if (is_zero(pcopy.coord.back().value))
	pcopy.coord.pop_back();
    }
    else
      pcopy.coord.push_back(monomial<gen>(c,pcopy.dim));
    return pcopy;
  }

  gen chkmod(const gen& a,const gen & b){
    if  ( (b.type!=_MOD) || ((a.type==_MOD) && (*(a._MODptr+1)==*(b._MODptr+1))) )
      return a;
    return makemodquoted(a,*(b._MODptr+1));
  }
  gen makemod(const gen & a,const gen & b){
    if (a.type==_VECT)
      return apply1st(a,b,makemod);
    if (a.type==_POLY){
      polynome res(a._POLYptr->dim);
      vector< monomial<gen> >::const_iterator it=a._POLYptr->coord.begin(),itend=a._POLYptr->coord.end();
      res.coord.reserve(itend-it);
      for (;it!=itend;++it){
	gen tmp=makemod(it->value,b);
	if (!is_zero(tmp))
	    res.coord.push_back(monomial<gen>(tmp,it->index));
      }
      return res;
    }
    gen res;
    res.type=_MOD;
    res._MODptr=new gen[2];
    if ( (b.type==_INT_) || (b.type==_ZINT) )
      *res._MODptr=smod(a,b);
    else {
      if (b.type!=_VECT){
	res.type=0;
	delete [] res._MODptr;
	setsizeerr("Bad mod:"+b.print());
      }
      if (a.type==_VECT)
	*res._MODptr=(*a._VECTptr)%(*b._VECTptr);
      else
	*res._MODptr=a;
    }
    *(res._MODptr+1)=b;
    res.ref_count=new int(1);
    return res;
  }

  gen makemodquoted(const gen & a,const gen & b){
    gen res;
    res.type=_MOD;
    res._MODptr=new gen[2];
    *res._MODptr=a;
    *(res._MODptr+1)=b;
    res.ref_count=new int(1);
    return res;
  }

  gen modadd(const gen * a,const gen *b){
    if (*(a+1)!=*(b+1))
      setsizeerr("Mod are different");
    return makemod(*a+*b,*(a+1));
  }

  gen modsub(const gen * a,const gen *b){
    if (*(a+1)!=*(b+1))
      setsizeerr("Mod are different");
    return makemod(*a-*b,*(a+1));
  }

  gen modmul(const gen * a,const gen *b){
    if (*(a+1)!=*(b+1))
      setsizeerr("Mod are different");
    return makemod(*a*(*b),*(a+1));
  }

  gen modinv(const gen & a){
    gen modu=*(a._MODptr+1);
    if ( ( (modu.type==_INT_) || (modu.type==_ZINT) ) && 
	 ( (a._MODptr->type==_INT_) || (a._MODptr->type==_ZINT) ) )
      return makemod(invmod(*a._MODptr,modu),modu);
    if (modu.type==_VECT){
      modpoly polya,u,v,d;
      if (a._MODptr->type!=_VECT)
	polya.push_back(*a._MODptr);
      else
	polya=*a._MODptr->_VECTptr;
      egcd(polya,*modu._VECTptr,u,v,d);
      if (d.size()!=1)
	setsizeerr("Non invertible");
      return makemod(u/d.front(),modu);
    }
    return fraction(makemod(plus_one,*(a._MODptr+1)),a);
  }

  gen operator + (const gen & a,const gen & b){
    register unsigned t=(a.type<< _DECALAGE) | b.type;
    if (!t)
      return((long long) a.val+b.val);
    register mpz_t * e;
    switch ( t ) {
    case _ZINT__ZINT:
      e =new mpz_t[1];
      mpz_init(*e);
      mpz_add(*e,*a._ZINTptr,*b._ZINTptr);
      return(e);
    case _DOUBLE___DOUBLE_:
      return a._DOUBLE_val+b._DOUBLE_val;
    case _VECT__VECT:
      if (a.subtype==_POLY1__VECT)
	return gen(*a._VECTptr+*b._VECTptr,_POLY1__VECT);
      return gen(addvecteur(*a._VECTptr,*b._VECTptr),a.subtype);
    case _INT___ZINT: 
      e =new mpz_t[1];
      mpz_init(*e);
      if (a.val<0)
	mpz_sub_ui(*e,*b._ZINTptr,-a.val);
      else
	mpz_add_ui(*e,*b._ZINTptr,a.val);
      return(e);
    case _ZINT__INT_:
      e =new mpz_t[1];
      mpz_init(*e);
      if (b.val<0)
	mpz_sub_ui(*e,*a._ZINTptr,-b.val);
      else
	mpz_add_ui(*e,*a._ZINTptr,b.val);
      return(e);
    case _DOUBLE___INT_:
      return a._DOUBLE_val+b.val;
    case _INT___DOUBLE_:
      return a.val+b._DOUBLE_val;
    case _DOUBLE___ZINT:
      return a._DOUBLE_val+mpz_get_d(*b._ZINTptr);
    case _ZINT__DOUBLE_:
      return b._DOUBLE_val+mpz_get_d(*a._ZINTptr);
    case _CPLX__INT_: case _CPLX__ZINT: case _CPLX__DOUBLE_:
      return gen(*a._CPLXptr+b,*(a._CPLXptr+1));
    case _INT___CPLX: case _ZINT__CPLX: case _DOUBLE___CPLX:
      return gen(a+*b._CPLXptr,*(b._CPLXptr+1));
    case _CPLX__CPLX:
      return gen(*a._CPLXptr + *b._CPLXptr, *(a._CPLXptr+1) + *(b._CPLXptr+1));
    case _POLY__POLY:
      return addpoly(a,b);
    case _FRAC__FRAC:
      return (*a._FRACptr)+(*b._FRACptr);
    case _SPOL1__SPOL1:
      return spadd(*a._SPOL1ptr,*b._SPOL1ptr);
    case _EXT__EXT:
      return ext_add(a,b);
    case _STRNG__STRNG:
      return string2gen('"'+(*a._STRNGptr)+(*b._STRNGptr)+'"');
    case _POLY__INT_: case _POLY__ZINT: case _POLY__DOUBLE_: case _POLY__CPLX: case _POLY__MOD:
      return addpoly(*a._POLYptr,b);
    case _INT___POLY: case _ZINT__POLY: case _DOUBLE___POLY: case _CPLX__POLY: case _MOD__POLY:
      return addpoly(*b._POLYptr,a);
    case _MOD__MOD:
      return modadd(a._MODptr,b._MODptr);
    default:
      if (a.type==_USER)
	return (*a._USERptr)+b;
      if (b.type==_USER)
	return (*b._USERptr)+a;
      return sym_add(a,b);
    }
  }

  symbolic symbolic_plot_makevecteur(const unary_function_ptr & u,const gen & e){
    if ( (u!=at_pnt) || (e.type!=_VECT) || (e.subtype!=_PNT__VECT) )
      return symbolic(u,e);
    // e is a curve or a pnt
    vecteur w(*e._VECTptr);
    if ( (w.size()!=2) && (w.size()!=3))
      return symbolic(u,e);
    gen a0(w[0]);
    gen a1(w[1]);
    if ( (a1.type==_VECT) && (a1._VECTptr->size()==2) ){
      // we must project a0
      gen param=a1._VECTptr->back(); // v= [ pnt() t ]
      assert(param.type==_VECT);
      vecteur v=*param._VECTptr;
      v[1]=projection(v[0],a0);
      a0=remove_at_pnt(parameter2point(v));
      a1=makevecteur(a1._VECTptr->front(),v);
    }
    return symbolic(u,gen(makevecteur(a0,a1),_PNT__VECT));
  }

  gen sym_add (const gen & a,const gen & b){
    if ( !is_inevalf && ( (a.type==_DOUBLE_) || ( (a.type==_CPLX) && (a.subtype==3) ) ) ){
      gen b1=b.evalf();
      if (b1!=b)
	return a+b1;
    }
    if ( !is_inevalf && ( (b.type==_DOUBLE_) || ( (b.type==_CPLX) && (b.subtype==3) ) ) ){
      gen a1=a.evalf();
      if (a1!=a)
	return a1+b;
    }
    if (a.type==_VECT){
      vecteur res=*a._VECTptr;
      if (res.empty())
	return b;
      if (equalposcomp(_GROUP__VECT_subtype,a.subtype)){ // add to each element
	iterateur it=res.begin(),itend=res.end();
	for (;it!=itend;++it)
	  *it=*it+b;
	return gen(res,a.subtype);
      }
      if (a.subtype==_PNT__VECT){
	res.front()=res.front()+b;
	return gen(res,_PNT__VECT);
      }
      if (a._VECTptr->front().type==_VECT){ // matrix+cst
	int s=res.size();
	if (s!=res.front()._VECTptr->size())
	  setsizeerr();
	for (int i=0;i<s;i++)
	  (*(res[i]._VECTptr))[i] = (*(res[i]._VECTptr))[i] + b;
	return res;
      }
      // polynomial+cst
      res.back()=res.back()+b;
      if ( (res.size()==1) && is_zero(res.back()))
          return zero;
      else
          return gen(res,_POLY1__VECT);
    }
    if (b.type==_VECT)
      return sym_add(b,a);
    if ((a==undef) || (b==undef))
      return undef;
    if (is_inf(a)){
      if (is_inf(b)){
	if ((a==b) && (a!=unsigned_inf))
	  return a;
	else
	  return undef;
      }
      else
	return a;
    }
    if (is_inf(b))
      return b;
    if (is_zero(a))
      return b;
    if (is_zero(b))
      return a;
    if (a.type==_STRNG)
      return string2gen(*a._STRNGptr+b.print(),false);
    if (b.type==_STRNG)
      return string2gen(a.print()+*b._STRNGptr,false);
    if (a.type==_FRAC){
      if ( (b.type!=_SYMB) && (b.type!=_IDNT) )
	return (*a._FRACptr)+b;
      return sym_add(_FRAC2_SYMB(a),b);
    }
    if (b.type==_FRAC){
      if ( (a.type!=_SYMB) && (a.type!=_IDNT) )
	return a+(*b._FRACptr);
      return sym_add(a,_FRAC2_SYMB(b));
    }
    if (a.type==_EXT){
        if (a.is_constant() && (b.type==_POLY))
            return addpoly(*b._POLYptr,a);
        else
            return algebraic_EXTension(*a._EXTptr+b,*(a._EXTptr+1));
    }
    if (b.type==_EXT){
        if (b.is_constant() && (a.type==_POLY))
            return addpoly(*a._POLYptr,b);
        else
            return algebraic_EXTension(a+*b._EXTptr,*(b._EXTptr+1));
    }
    if ( (a.type==_SYMB) && equalposcomp(plot_sommets,a._SYMBptr->sommet) ){
      if ( (b.type==_SYMB) && equalposcomp(plot_sommets,b._SYMBptr->sommet) )
	return a._SYMBptr->feuille._VECTptr->front()+b._SYMBptr->feuille._VECTptr->front();
      else
	return symbolic_plot_makevecteur( a._SYMBptr->sommet,a._SYMBptr->feuille+b );
    }
    if ( (b.type==_SYMB) && equalposcomp(plot_sommets,b._SYMBptr->sommet) )
      return symbolic_plot_makevecteur(b._SYMBptr->sommet,b._SYMBptr->feuille+a);
    if (a.type==_SYMB && a._SYMBptr->sommet==at_equal){
      vecteur & va=*a._SYMBptr->feuille._VECTptr;
      if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
	vecteur & vb=*b._SYMBptr->feuille._VECTptr;
	return symbolic(at_equal,makevecteur(va.front()+vb.front(),va.back()+vb.back()));
      }
      else
	return symbolic(at_equal,makevecteur(va.front()+b,va.back()+b));
    }
    if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
      vecteur & vb=*b._SYMBptr->feuille._VECTptr;
      return symbolic(at_equal,makevecteur(a+vb.front(),a+vb.back()));
    }
    if ((a.type==_SYMB) && (b.type==_SYMB)){
      if (a._SYMBptr->sommet==at_plus) {
	if (b._SYMBptr->sommet==at_plus)
	  return symbolic(at_plus,mergevecteur(*(a._SYMBptr->feuille._VECTptr),*(b._SYMBptr->feuille._VECTptr)));
	else
	  return symbolic(*a._SYMBptr,b);
      }
      else { 
	if (b._SYMBptr->sommet==at_plus)
	  return symbolic(*(b._SYMBptr),a);
	else
	  return symbolic(at_plus,gen(makevecteur(a,b)));
      }
    }
    if (b.type==_SYMB){
      if (b._SYMBptr->sommet==at_plus)
	return symbolic(*b._SYMBptr,a);
      else
	return symbolic(at_plus,gen(makevecteur(a,b)));
    }
    if (a.type==_SYMB){
      if (a._SYMBptr->sommet==at_plus)
	return symbolic(*a._SYMBptr,b);
      else
	return symbolic(at_plus,gen(makevecteur(a,b)));
    }
    if ( (a.type==_IDNT) || (b.type==_IDNT))
      return symbolic(at_plus,gen(makevecteur(a,b)));
    if (a.type==_MOD)
      return a+makemod(b,*(a._MODptr+1));
    if (b.type==_MOD)
      return makemod(a,*(b._MODptr+1))+b;
    settypeerr("sym_add");
  }

  gen subpoly(const gen & th, const gen & other){
    if ((th.type!=_POLY) || (other.type!=_POLY))
      settypeerr("subpoly");
    vector< monomial<gen> >::const_iterator a=th._POLYptr->coord.begin();
    vector< monomial<gen> >::const_iterator a_end=th._POLYptr->coord.end();
    vector< monomial<gen> >::const_iterator b=other._POLYptr->coord.begin();
    vector< monomial<gen> >::const_iterator b_end=other._POLYptr->coord.end();
    if (b==b_end){
      return th;
    }
    polynome * resptr=new polynome(th._POLYptr->dim);
    Sub<gen>(a,a_end,b,b_end,resptr->coord,th._POLYptr->is_strictly_greater);
    return resptr;
  }

  polynome subpoly(const polynome & p,const gen & c){
    if (is_zero(c))
      return p;
    polynome pcopy(p);
    if ( (!p.coord.empty()) && is_zero(*(p.coord.back().index.iptr)) ) {
      pcopy.coord.back().value = pcopy.coord.back().value - c;
      if (is_zero(pcopy.coord.back().value))
	pcopy.coord.pop_back();
    }
    else
      pcopy.coord.push_back(monomial<gen>(-c,pcopy.dim));
    return pcopy;
  }

  polynome subpoly(const gen & c,const polynome & p){
    if (is_zero(c))
      return -p;
    polynome pcopy(-p);
    if ( (!p.coord.empty()) && is_zero(*(p.coord.back().index.iptr)) ) {
      pcopy.coord.back().value = pcopy.coord.back().value + c;
      if (is_zero(pcopy.coord.back().value))
	pcopy.coord.pop_back();
    }
    else
      pcopy.coord.push_back(monomial<gen>(c,pcopy.dim));
    return pcopy;
  }

  gen operator - (const gen & a,const gen & b){
    register unsigned t=(a.type<< _DECALAGE) | b.type;
    if (!t)
      return((long long) a.val-b.val);
    register mpz_t * e;
    switch ( t) {
    case _ZINT__ZINT:
      e =new mpz_t[1];
      mpz_init(*e);
      mpz_sub(*e,*a._ZINTptr,*b._ZINTptr);
      return e;
    case _DOUBLE___DOUBLE_:
      return a._DOUBLE_val-b._DOUBLE_val;
    case _VECT__VECT:
      if (a.subtype==_POLY1__VECT)
	return gen(*a._VECTptr-*b._VECTptr,_POLY1__VECT);
      return gen(subvecteur(*a._VECTptr,*b._VECTptr),a.subtype);
    case _INT___ZINT: 
      e =new mpz_t[1];
      mpz_init(*e);
      if (a.val<0)
	mpz_add_ui(*e,*b._ZINTptr,-a.val);
      else
	mpz_sub_ui(*e,*b._ZINTptr,a.val);
      mpz_neg(*e,*e);
      return(e);
    case _ZINT__INT_:
      e =new mpz_t[1];
      mpz_init(*e);
      if (b.val<0)
	mpz_add_ui(*e,*a._ZINTptr,-b.val);
      else
	mpz_sub_ui(*e,*a._ZINTptr,b.val);
      return(e);
    case _INT___DOUBLE_:
      return a.val-b._DOUBLE_val;
    case _DOUBLE___INT_:
      return a._DOUBLE_val-b.val;
    case _ZINT__DOUBLE_:
      return mpz_get_d(*a._ZINTptr)-b._DOUBLE_val;
    case _DOUBLE___ZINT:
      return a._DOUBLE_val-mpz_get_d(*b._ZINTptr);
    case _CPLX__INT_: case _CPLX__ZINT: case _CPLX__DOUBLE_:
      return gen(*a._CPLXptr-b,*(a._CPLXptr+1));
    case _INT___CPLX: case _ZINT__CPLX: case _DOUBLE___CPLX:
      return gen(a-*b._CPLXptr,-*(b._CPLXptr+1));
    case _CPLX__CPLX:
      return gen(*a._CPLXptr - *b._CPLXptr, *(a._CPLXptr+1) - *(b._CPLXptr+1));
    case _POLY__POLY:
      return subpoly(a,b);
    case _FRAC__FRAC:
        return (*a._FRACptr)-(*b._FRACptr);
    case _SPOL1__SPOL1:
      return spsub(*a._SPOL1ptr,*b._SPOL1ptr);
    case _EXT__EXT:
      return ext_sub(a,b);
    case _POLY__INT_: case _POLY__ZINT: case _POLY__DOUBLE_: case _POLY__CPLX: case _POLY__MOD:
      return subpoly(*a._POLYptr,b);
    case _INT___POLY: case _ZINT__POLY: case _DOUBLE___POLY: case _CPLX__POLY: case _MOD__POLY:
      return subpoly(a,*b._POLYptr);        
    case _MOD__MOD:
      return modsub(a._MODptr,b._MODptr);
    default:
      if (a.type==_USER)
	return (*a._USERptr)-b;
      if (b.type==_USER)
	return (-b)+a;
      return sym_sub(a,b);
    }
  }

  gen sym_sub(const gen & a,const gen & b){
    if ( !is_inevalf && ( (a.type==_DOUBLE_) || ( (a.type==_CPLX) && (a.subtype==3) ) ) ){
      gen b1=b.evalf();
      if (b1!=b)
	return a-b1;
    }
    if ( !is_inevalf && ( (b.type==_DOUBLE_) || ( (b.type==_CPLX) && (b.subtype==3) ) ) ){
      gen a1=a.evalf();
      if (a1!=a)
	return a1-b;
    }
    if (a.type==_VECT)
      return sym_add(a,-b);
    if (b.type==_VECT)
      return sym_add(-b,a);
    if ((a==undef) || (b==undef))
      return undef;
    if (is_inf(a)){
      if (is_inf(b)){
	if ((a==plus_inf) && (b==minus_inf))
	  return a;
	if ((a==minus_inf) && (b==plus_inf))	
	  return a;
	return undef;
      }
      else
	return a;
    }
    if (a.type==_FRAC){
      if ( (b.type!=_SYMB) && (b.type!=_IDNT) )
        return (*a._FRACptr)-b;
      return sym_sub(_FRAC2_SYMB(a),b);
    }
    if (b.type==_FRAC){
      if ( (a.type!=_SYMB) && (a.type!=_IDNT) )
        return a-(*b._FRACptr);
      return sym_sub(a,_FRAC2_SYMB(b));
    }
    if (a.type==_EXT){
        if (a.is_constant() && (b.type==_POLY))
            return subpoly(a,*b._POLYptr);
        else
            return algebraic_EXTension(*a._EXTptr-b,*(a._EXTptr+1));
    }
    if (b.type==_EXT){
        if (b.is_constant() && (a.type==_POLY))
            return subpoly(*a._POLYptr,b);
        else
            return algebraic_EXTension(a-*b._EXTptr,*(b._EXTptr+1));
    }
    if (a==b)
      return chkmod(zero,a);
    if (is_inf(b))
      return -b;
    if (is_zero(b))
      return a;
    if (is_zero(a))
      return -b;
    if ( (a.type==_SYMB) && equalposcomp(plot_sommets,a._SYMBptr->sommet) ){
      if ( (b.type==_SYMB) && equalposcomp(plot_sommets,b._SYMBptr->sommet) )
	return a._SYMBptr->feuille._VECTptr->front()-b._SYMBptr->feuille._VECTptr->front();
      else
	return symbolic_plot_makevecteur(a._SYMBptr->sommet,a._SYMBptr->feuille-b);
    }
    if ( (b.type==_SYMB) && equalposcomp(plot_sommets,b._SYMBptr->sommet) ){
      return sym_add(-b,a);
    }
    if (a.type==_SYMB && a._SYMBptr->sommet==at_equal){
      vecteur & va=*a._SYMBptr->feuille._VECTptr;
      if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
	vecteur & vb=*b._SYMBptr->feuille._VECTptr;
	return symbolic(at_equal,makevecteur(va.front()-vb.front(),va.back()-vb.back()));
      }
      else
	return symbolic(at_equal,makevecteur(va.front()-b,va.back()-b));
    }
    if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
      vecteur & vb=*b._SYMBptr->feuille._VECTptr;
      return symbolic(at_equal,makevecteur(a-vb.front(),a-vb.back()));
    }
    if ((a.type==_SYMB) && (b.type==_SYMB)){
      if (a._SYMBptr->sommet==at_plus) {
	if (b._SYMBptr->sommet==at_plus)
	  return symbolic(at_plus,mergevecteur(*(a._SYMBptr->feuille._VECTptr),negvecteur(*(b._SYMBptr->feuille._VECTptr))));
	else
	  return symbolic(*a._SYMBptr,-b);
      }
      else { 
	if (b._SYMBptr->sommet==at_plus)
	  return symbolic(*(-b)._SYMBptr,a);
	else
	  return symbolic(at_plus,gen(makevecteur(a,-b)));
      }
    }
    if (b.type==_SYMB){
      if (b._SYMBptr->sommet==at_plus)
	return symbolic(*(-b)._SYMBptr,a);
      else
	return symbolic(at_plus,gen(makevecteur(a,-b)));
    }
    if (a.type==_SYMB){
      if (a._SYMBptr->sommet==at_plus)
	return symbolic(*a._SYMBptr,-b);
      else
	return symbolic(at_plus,gen(makevecteur(a,-b)));
    }
    if ((a.type==_IDNT) || (b.type==_IDNT))
      return symbolic(at_plus,gen(makevecteur(a,-b)));
    if (a.type==_MOD)
      return a-makemod(b,*(a._MODptr+1));
    if (b.type==_MOD)
      return makemod(a,*(b._MODptr+1))-b;
    settypeerr("sym_sub");
  }

  vecteur negfirst(const vecteur & v){
    vecteur w(v);
    if (!w.empty())
      w.front()=-w.front();
    return w;
  }

  gen operator -(const gen & a){
    mpz_t *e ;
    switch (a.type ) {
    case _INT_: 
      return(-a.val);
    case _ZINT: 
      e=new mpz_t[1];
      mpz_init(*e);
      mpz_neg(*e,*a._ZINTptr);
      return(e);
    case _DOUBLE_:
      return -(a._DOUBLE_val);
    case _CPLX:
      return gen(-*a._CPLXptr,-*(a._CPLXptr+1));
    case _IDNT:
      if ((a==undef) || (a==unsigned_inf))
	return a;
      return symbolic(at_neg,a);
    case _SYMB:
      if (a==plus_inf)
	return minus_inf;
      if (a==minus_inf)
	return plus_inf;
      if (a._SYMBptr->sommet==at_neg)
	return a._SYMBptr->feuille;
      if (a._SYMBptr->sommet==at_plus)
	return symbolic(at_plus,negvecteur(*a._SYMBptr->feuille._VECTptr));
      if (equalposcomp(plot_sommets,a._SYMBptr->sommet))
	return symbolic_plot_makevecteur(a._SYMBptr->sommet,-a._SYMBptr->feuille);
      if (a._SYMBptr->sommet==at_equal)
	return symbolic(a._SYMBptr->sommet,makevecteur(-a._SYMBptr->feuille._VECTptr->front(),-a._SYMBptr->feuille._VECTptr->back()));
      return symbolic(at_neg,a);
    case _VECT:
      if (a.subtype==_PNT__VECT)
	return gen(negfirst(*a._VECTptr),a.subtype);
      else
	return gen(negvecteur(*a._VECTptr),a.subtype);
    case _POLY:
      return -(*a._POLYptr);
    case _EXT:
      return algebraic_EXTension(-(*a._EXTptr),*(a._EXTptr+1));
    case _USER:
      return -(*a._USERptr);
    case _MOD:
      return makemod(-*a._MODptr,*(a._MODptr+1));
    case _FRAC:
      return fraction(-(a._FRACptr->num),a._FRACptr->den);
    case _SPOL1:
      return spneg(*a._SPOL1ptr);
    case _STRNG:
      return string2gen("-"+(*a._STRNGptr),false);
    default: 
      settypeerr("Neg");
    }
  }

  gen mulpoly(const gen & th,const gen & other){
    if ((th.type!=_POLY) || (other.type!=_POLY))
      settypeerr("mulpoly");
    vector< monomial<gen> >::const_iterator ita = th._POLYptr->coord.begin();
    vector< monomial<gen> >::const_iterator ita_end = th._POLYptr->coord.end();
    vector< monomial<gen> >::const_iterator itb = other._POLYptr->coord.begin();
    vector< monomial<gen> >::const_iterator itb_end = other._POLYptr->coord.end();
    // first some trivial cases
    if (ita==ita_end)
      return(th);
    if (itb==itb_end)
      return(other);
    if (is_one(*th._POLYptr))
      return other;
    if (is_one(*other._POLYptr))
      return th;
    // Now look if length a=1 or length b=1, happens frequently
    // think of x^3*y^2*z translated to internal form
    int c1=th._POLYptr->coord.size();
    if (c1==1)
      return other._POLYptr->shift(th._POLYptr->coord.front().index,th._POLYptr->coord.front().value);
    int c2=other._POLYptr->coord.size();
    if (c2==1)
      return th._POLYptr->shift(other._POLYptr->coord.front().index,other._POLYptr->coord.front().value);
    polynome * resptr = new polynome(th._POLYptr->dim);
    Mul<gen>(ita,ita_end,itb,itb_end,resptr->coord,th._POLYptr->is_strictly_greater);
    return resptr;
  }

  vecteur multfirst(const gen & a,const vecteur & v){
    vecteur w(v);
    if (!w.empty())
      w.front()=v.front()*a;
    return w;
  }

  gen operator * (const gen & a,const gen & b){
    // cout << a << "*" << b << endl;
    register unsigned t=(a.type<< _DECALAGE) | b.type;
    if (!t)
      return gen((long long) a.val*b.val);
    register  mpz_t * e;
    switch (t) {
    case _ZINT__ZINT:
      e=new mpz_t[1];
      mpz_init(*e);
      mpz_mul(*e,*a._ZINTptr,*b._ZINTptr);
      return e;
    case _DOUBLE___DOUBLE_:
      return a._DOUBLE_val*b._DOUBLE_val;
    case _INT___ZINT: 
      e=new mpz_t[1];
      mpz_init(*e);
      if (a.val<0){
	mpz_mul_ui(*e,*b._ZINTptr,-a.val);
	mpz_neg(*e,*e);
      }
      else
	mpz_mul_ui(*e,*b._ZINTptr,a.val);
      return gen(e);
    case _ZINT__INT_:
      e=new mpz_t[1];
      mpz_init(*e);
      if (b.val<0){
	mpz_mul_ui(*e,*a._ZINTptr,-b.val);
	mpz_neg(*e,*e);
      }
      else
	mpz_mul_ui(*e,*a._ZINTptr,b.val);
      return gen(e);
    case _INT___DOUBLE_:
      return a.val*b._DOUBLE_val;
    case _DOUBLE___INT_:
      return a._DOUBLE_val*b.val;
    case _DOUBLE___ZINT:
      return a._DOUBLE_val*mpz_get_d(*b._ZINTptr);
    case _ZINT__DOUBLE_:
      return mpz_get_d(*a._ZINTptr)*b._DOUBLE_val;
    case _CPLX__INT_: case _CPLX__ZINT: case _CPLX__DOUBLE_:
      return gen(*a._CPLXptr*b,*(a._CPLXptr+1)*b);
    case _INT___CPLX: case _ZINT__CPLX: case _DOUBLE___CPLX:
      return gen(a*(*b._CPLXptr),a*(*(b._CPLXptr+1)));
    case _CPLX__CPLX:
      return gen(*a._CPLXptr * (*b._CPLXptr) - *(a._CPLXptr+1)* (*(b._CPLXptr+1)), (*b._CPLXptr) * (*(a._CPLXptr+1)) + *(b._CPLXptr+1) * (*a._CPLXptr));
    case _VECT__INT_: case _VECT__ZINT: case _VECT__DOUBLE_: case _VECT__CPLX: case _VECT__SYMB: case _VECT__IDNT: case _VECT__POLY: case _VECT__EXT: case _VECT__MOD: case _VECT__FRAC:
      if (a.subtype==_PNT__VECT)
	return gen(multfirst(b,*a._VECTptr),_PNT__VECT);
      else
	return gen(multvecteur(b,*a._VECTptr),a.subtype);
    case _INT___VECT: case _ZINT__VECT: case _DOUBLE___VECT: case _CPLX__VECT: case _SYMB__VECT: case _IDNT__VECT: case _POLY__VECT: case _EXT__VECT: case _MOD__VECT: case _FRAC__VECT:
      if (b.subtype==_PNT__VECT)
	return gen(multfirst(a,*b._VECTptr),_PNT__VECT);
      else
	return gen(multvecteur(a,*b._VECTptr),b.subtype);
    case _VECT__VECT:
      if ( (a.subtype==_POLY1__VECT) || (b.subtype==_POLY1__VECT) )
	return gen( (*a._VECTptr) * (*b._VECTptr), _POLY1__VECT);
      return ckmultmatvecteur(*a._VECTptr,*b._VECTptr);
    case _POLY__POLY:
      return mulpoly(a,b);
    case _FRAC__FRAC:
      return (*a._FRACptr)*(*b._FRACptr);
    case _SPOL1__SPOL1:
      return spmul(*a._SPOL1ptr,*b._SPOL1ptr);
    case _EXT__EXT:
      return ext_mul(a,b);
    case _POLY__INT_: case _POLY__ZINT: case _POLY__DOUBLE_: case _POLY__CPLX: 
      if (is_one(b))
	return a;
      return (*a._POLYptr) * b;
    case _POLY__MOD:
      return (*a._POLYptr) * b;
    case _INT___POLY: case _ZINT__POLY: case _DOUBLE___POLY: case _CPLX__POLY: 
      if (is_one(a))
	return b;
      return a * (*b._POLYptr);        
    case _MOD__POLY:
      return a * (*b._POLYptr);        
    case _MOD__MOD:
      return modmul(a._MODptr,b._MODptr);
    case _MOD__INT_: case _MOD__ZINT:
      return makemod(*a._MODptr*b,*(a._MODptr+1));
    case _INT___MOD: case _ZINT__MOD:
      return makemod(*b._MODptr*a,*(b._MODptr+1));
    default:
      if (a.type==_USER)
	return (*a._USERptr)*b;
      if (b.type==_USER)
	return (*b._USERptr)*a;      
      return sym_mult(a,b);
    }
  }

  gen giac_pow(const gen & base,const gen & exponent){
    return pow(base,exponent);
  }
  gen pow(const gen & base,const gen & exponent){
    if (base.type==_VECT && !ckmatrix(base)){
      if (exponent.type==_VECT)
	return apply(base,exponent,giac::giac_pow);
      return apply1st(base,exponent,&giac::giac_pow); 
    }
    if (exponent.type==_VECT)
      return apply2nd(base,exponent,&giac::giac_pow);
    if (exponent.type==_FRAC)
      return pow(base,symbolic(at_prod,makevecteur(exponent._FRACptr->num,symb_inv(exponent._FRACptr->den))));
    if (is_inf(base)){ 
      if (is_strictly_positive(-exponent) )
	return 0;
      if (base==plus_inf){
	if (is_strictly_positive(exponent))
	  return plus_inf;
	else
	  return undef;
      }
      if ( (exponent.type==_INT_) ){
	if (exponent.val % 2)
	  return base;
	else
	  return plus_inf; // for unsigned_inf in _DOUBLE_ mode only!!
	}
      if (is_strictly_positive(exponent))
	return base;
      return undef;
    }
    if (base.type==_SYMB){ 
      if (base._SYMBptr->sommet==at_exp && exponent.type!=_INT_)
        return exp(base._SYMBptr->feuille*exponent);  
      if (base._SYMBptr->sommet==at_pow){
	vecteur & v=*base._SYMBptr->feuille._VECTptr;
	gen new_exp=normal(v[1]*exponent);
	if (!complex_mode && (v[1].type==_INT_)) {
	  if ( (v[1].val%2) || (new_exp.type!=_INT_) || (new_exp.val %2==0) )
	    return pow(v[0],new_exp);
	  else
	    return pow(abs(v[0]),new_exp);	
	}
	else
	  return pow(v[0],new_exp);
      }
      if (base._SYMBptr->sommet==at_equal){
	vecteur & vb=*base._SYMBptr->feuille._VECTptr;
	return symbolic(at_equal,makevecteur(pow(vb.front(),exponent),pow(vb.back(),exponent)));
      }
    }
    switch ( (base.type<< _DECALAGE) | exponent.type ) {
    case _INT___INT_: case _ZINT__INT_: case _CPLX__INT_: case _IDNT__INT_: case _SYMB__INT_:
      return pow(base,exponent.val);
    case _DOUBLE___DOUBLE_:
      return std::pow(base._DOUBLE_val,exponent._DOUBLE_val);
    case _DOUBLE___INT_:
      return std::pow(base._DOUBLE_val,exponent.val);
    case _INT___DOUBLE_:
      return std::pow(base.val,exponent._DOUBLE_val);
    case _ZINT__DOUBLE_:
      return std::pow(mpz_get_d(*base._ZINTptr),exponent._DOUBLE_val);
    case _DOUBLE___ZINT:
      return std::pow(base._DOUBLE_val,mpz_get_d(*exponent._ZINTptr));
    case _POLY__INT_:
      if (exponent.val<0)
	return fraction(1,pow(*base._POLYptr,-exponent.val));
      else
	return pow(*base._POLYptr,exponent.val);
    case _FRAC__INT_:
      return pow(*base._FRACptr,exponent.val);
    case _EXT__INT_: case _MOD__INT_: case _VECT__INT_:
      return pow(base,exponent.val);
    default:
      if ((base==undef) || (exponent==undef))
	return undef;
      if (is_one(base) && !is_inf(exponent))
	return base;
      if (is_minus_one(base) && (exponent.type==_SYMB) && (exponent._SYMBptr->sommet==at_neg) )
	return pow(base,exponent._SYMBptr->feuille);
      if ((base.type==_INT_) && (base.val<0)){
	if (exponent==plus_one_half)
	  return cst_i*sqrt(-base.val);
	// if (exponent==-one_half)
	//  return rdiv(cst_i,sqrt(-base.val));
      }
      if (is_zero(base)){
	if (is_positive(exponent))
	  return base;
	if (is_positive(-exponent))
	  return unsigned_inf;
      }
      if (is_inf(exponent))
	return exp(exponent*ln(base));
      // extract integral powers in a product exponent
      if ((exponent.type==_SYMB) && (exponent._SYMBptr->sommet==at_prod)){
	gen subexponent_num(1),subexponent_deno(1);
	gen superexponent(1);
	const_iterateur it=exponent._SYMBptr->feuille._VECTptr->begin(),itend=exponent._SYMBptr->feuille._VECTptr->end();
	for (;it!=itend;++it){
	  if (it->type==_INT_){
	    superexponent = superexponent * (*it);
	    continue;
	  }
	  if ( (it->type==_SYMB) && (it->_SYMBptr->sommet==at_inv))
	    subexponent_deno = subexponent_deno * (it->_SYMBptr->feuille);
	  else
	    subexponent_num = subexponent_num * (*it);
	}
	if (superexponent.type!=_INT_)
	  return symbolic(at_pow,makevecteur(base,exponent));
	if (subexponent_deno.type!=_INT_)
	  return symbolic(at_pow,makevecteur(symbolic(at_pow,makevecteur(base,_FRAC2_SYMB(subexponent_num,subexponent_deno))),superexponent));
	int q=superexponent.val / subexponent_deno.val;
	int r=superexponent.val % subexponent_deno.val;
	gen res(1);
	if (r){
	  res=symbolic(at_pow,makevecteur(pow(base,subexponent_num),inv(subexponent_deno)));
	  if (r!=1)
	    res=symbolic(at_pow,makevecteur(res,r));
	}
	if (!q)
	  return res;
	if (q==1)
	  return res*symbolic(at_pow,makevecteur(base,subexponent_num));
	if (q==-1)
	  return res*inv(pow(base,subexponent_num));
	return res*symbolic(at_pow,makevecteur(pow(base,subexponent_num),q));
      }
      return symbolic(at_pow,makevecteur(base,exponent));
    }  
  }

  // 0 if unknown, 1 if >0, -1 if <0
  int fastsign(const gen & a){
    gen test=a>0;
    if (test.type==_INT_){
      if (test.val)
	return test.val;
      test=a<0;
      if (test.type==_INT_)
	return -test.val;
      else
	return 0;
    }
    else
      return 0;
  }

  bool is_greater(const gen & a,const gen &b){
    gen test=a>=b;
    if ((test.type==_INT_) && (test.val==1))
      return true;
    else
      return false;    
  }

  bool is_strictly_greater(const gen & a,const gen &b){
    gen test=a>b;
    if ((test.type==_INT_) && (test.val==1))
      return true;
    else
      return false;    
  }

  bool is_positive(const gen & a){
    switch (a.type){
    case _INT_:
      return a.val>=0;
    case _ZINT:
      if (mpz_sgn(*a._ZINTptr)==-1)
	return false;
      else
	return true;
    case _POLY:
      return is_positive(a._POLYptr->coord.front());
    case _FRAC:
      return (is_positive(a._FRACptr->num) && is_positive(a._FRACptr->den)) || (is_positive(-a._FRACptr->num) && is_positive(-a._FRACptr->den));
    case _EXT:
      return false;
    case _SYMB:
      if (a==plus_inf)
	return true;
      if (a==minus_inf)
	return false;      
      if (a._SYMBptr->sommet==at_exp)
	return true;
      if (a._SYMBptr->sommet==at_ln)
	return is_positive(a._SYMBptr->feuille-1);
      return is_greater(a,0);
    default:
      return is_greater(a,0);
    }
  }

  bool is_strictly_positive(const gen & a){
    if (is_zero(a))
      return false;
    return is_positive(a);
  }

  bool ck_is_greater(const gen & a,const gen &b){
    if (a==b)
      return true;
    gen test=a>b;
    if (test.type!=_INT_)
      cksignerr(test);
    if (test.val==1)
      return true;
    else
      return false;    
  }

  bool ck_is_strictly_greater(const gen & a,const gen &b){
    gen test=a>b;
    if (test.type!=_INT_)
      cksignerr(test);
    if (test.val==1)
      return true;
    else
      return false;    
  }

  bool ck_is_positive(const gen & a){
    switch (a.type){
    case _INT_:
      return a.val>=0;
    case _ZINT:
      if (mpz_sgn(*a._ZINTptr)==-1)
	return false;
      else
	return true;
    case _SYMB:
      if (a==plus_inf)
	return true;
      if (a==minus_inf)
	return false;
      if (a._SYMBptr->sommet==at_exp)
	return true;
      if (a._SYMBptr->sommet==at_ln)
	return ck_is_positive(a._SYMBptr->feuille-1);    
      return ck_is_greater(a,0);
    default:
      return ck_is_greater(a,0);
    }
  }

  bool ck_is_strictly_positive(const gen & a){
    if (is_zero(a))
      return false;
    return ck_is_positive(a);
  }

  gen min(const gen & a, const gen & b){
    if (a==b)
      return a;
    gen test=a>b;
    if (test.type==_INT_){
      if (test.val==1)
	return b;
      else
	return a;
    }
    return symbolic(at_min,makevecteur(a,b));
  }

  gen max(const gen & a, const gen & b){
    if (a==b)
      return a;
    gen test=a>b;
    if (test.type==_INT_){
      if (test.val==1)
	return a;
      else
	return b;
    }
    return symbolic(at_max,makevecteur(a,b));
  }

  gen sym_mult (const gen & a,const gen & b){
    if ( !is_inevalf && ( (a.type==_DOUBLE_) || ( (a.type==_CPLX) && (a.subtype==3) ) ) ){
      gen b1=b.evalf();
      if (b1!=b)
	return a*b1;
    }
    if ( !is_inevalf && ( (b.type==_DOUBLE_) || ( (b.type==_CPLX) && (b.subtype==3) ) ) ){
      gen a1=a.evalf();
      if (a1!=a)
	return a1*b;
    }
    if ((a==undef) || (b==undef))
      return undef;
    if (is_inf(a)){
      if (is_zero(normal(b)))
	return undef;
      int s=fastsign(a)*fastsign(b);
      if (s==1)
	return plus_inf;
      if (s)
	return minus_inf;
      return unsigned_inf;
    }
    if (is_inf(b)){
      if (is_zero(normal(a)))
	return undef;
      int s=fastsign(a)*fastsign(b);
      if (s==1)
	return plus_inf;
      if (s)
	return minus_inf;
      return unsigned_inf;
    }
    if (is_zero(a) )
      return a;
    if (is_zero(b))
      return b;
    if (is_one(a) && ((a.type!=_MOD) || (b.type==_MOD) ))
      return b;
    if (is_one(b) && ((b.type!=_MOD) || (a.type==_MOD) ))
      return a;
    if (a.type==_FRAC){
      if ( (b.type!=_SYMB) && (b.type!=_IDNT) )      
        return (*a._FRACptr)*b;
      return sym_mult(_FRAC2_SYMB(a),b);
    }
    if (b.type==_FRAC){
      if ( (a.type!=_SYMB) && (a.type!=_IDNT) )
        return a*(*b._FRACptr);
      return sym_mult(a,_FRAC2_SYMB(b));
    }
    if (a.type==_EXT){
        if (a.is_constant() && (b.type==_POLY))
            return a*(*b._POLYptr);
        else
            return algebraic_EXTension(*a._EXTptr*b,*(a._EXTptr+1));
    }
    if (b.type==_EXT){
        if (b.is_constant() && (a.type==_POLY))
            return (*a._POLYptr)*b;
        else
            return algebraic_EXTension(a*(*b._EXTptr),*(b._EXTptr+1));
    }
    if ( (a.type==_INT_) && (a.val<0)){
      if (b.is_symb_of_sommet(at_inv))
	return sym_mult(-a,inv(-b._SYMBptr->feuille));
      else
	return -sym_mult(-a,b);
    }
    if ( (b.type==_INT_) && (b.val<0) ){
      if (a.is_symb_of_sommet(at_inv))
	return sym_mult(-b,inv(-a._SYMBptr->feuille));
      else
	return -sym_mult(-b,a);
    }
    if ((a.type==_SYMB) && equalposcomp(plot_sommets,a._SYMBptr->sommet))
      return symbolic_plot_makevecteur(a._SYMBptr->sommet,a._SYMBptr->feuille*b);
    if ((b.type==_SYMB) && equalposcomp(plot_sommets,b._SYMBptr->sommet))
      return symbolic_plot_makevecteur(b._SYMBptr->sommet,b._SYMBptr->feuille*a);
    if (a.type==_SYMB && a._SYMBptr->sommet==at_equal){
      vecteur & va=*a._SYMBptr->feuille._VECTptr;
      if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
	vecteur & vb=*b._SYMBptr->feuille._VECTptr;
	return symbolic(at_equal,makevecteur(va.front()*vb.front(),va.back()*vb.back()));
      }
      else
	return symbolic(at_equal,makevecteur(va.front()*b,va.back()*b));
    }
    if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
      vecteur & vb=*b._SYMBptr->feuille._VECTptr;
      return symbolic(at_equal,makevecteur(a*vb.front(),a*vb.back()));
    }
    if ((a.type==_SYMB)&& (b.type==_SYMB)){
      if ((a._SYMBptr->sommet==at_prod) && (b._SYMBptr->sommet==at_prod))
	return symbolic(at_prod,mergevecteur(*(a._SYMBptr->feuille._VECTptr),*(b._SYMBptr->feuille._VECTptr)));
      else {
	if (a._SYMBptr->sommet==at_prod)
	  return symbolic(*a._SYMBptr,b);
	else {
	  if (b._SYMBptr->sommet==at_prod)
	    return symbolic(a,b._SYMBptr->sommet,b._SYMBptr->feuille);
	  else
	    return symbolic(at_prod,gen(makevecteur(a,b)));
	}
      }
    }
    if (b.type==_SYMB){
      if (b._SYMBptr->sommet==at_prod)
	return symbolic(a,b._SYMBptr->sommet,b._SYMBptr->feuille);
      else
	return symbolic(at_prod,gen(makevecteur(a,b)));
    }
    if (a.type==_SYMB){
      if (a._SYMBptr->sommet==at_prod)
	return symbolic(*a._SYMBptr,b);
      else
	return symbolic(at_prod,gen(makevecteur(a,b)));
    }
    if ((a.type==_IDNT) || (b.type==_IDNT))
      return symbolic(at_prod,gen(makevecteur(a,b)));
    if (a.type==_MOD)
      return a*makemod(b,*(a._MODptr+1));
    if (b.type==_MOD)
      return b*makemod(a,*(b._MODptr+1));
    settypeerr("sym_mult");
  }

  vecteur inv__VECT(const vecteur & v){
    vecteur w;
    if (is_squarematrix(v))
      minv(v,w);
    else {
      vecteur::const_iterator it=v.begin(),itend=v.end();
      for (;it!=itend;++it)
	w.push_back(inv(*it));
    }
    return w;
  }

  vecteur invfirst(const vecteur & v){
    vecteur w(v);
    if (!w.empty())
      w.front()=inv(w.front());
    return w;
  }

  gen inv(const gen & a){
    if (is_zero(a))
      return unsigned_inf;
    switch (a.type ) {
    case _INT_: case _ZINT:
      if (is_one(a) || (is_minus_one(a)) )
	return a;
      else
	return fraction(1,a);
    case _DOUBLE_:
      return 1/a._DOUBLE_val;
    case _CPLX:
      if (is_zero(*a._CPLXptr)){
	if (is_one(abs(*(a._CPLXptr+1))))
	  return -a;
      }
      if ( (a._CPLXptr->type==_DOUBLE_) || ((a._CPLXptr+1)->type==_DOUBLE_) )
	return gen(rdiv(a.re().evalf(),a.squarenorm().evalf()),rdiv(-a.im().evalf(),a.squarenorm().evalf()));
      return fraction(1,a);
    case _IDNT:
      if (a==undef)
	return undef;
      if (a==unsigned_inf)
	return 0;
      return symbolic(at_inv,a);
    case _SYMB:
      if ((a==plus_inf) || (a==minus_inf))
	return 0;
      if (equalposcomp(plot_sommets,a._SYMBptr->sommet))
	return symbolic_plot_makevecteur( a._SYMBptr->sommet,inv(a._SYMBptr->feuille) );
      if (a._SYMBptr->sommet==at_inv)
	return a._SYMBptr->feuille;
      if (a._SYMBptr->sommet==at_neg)
	return -inv(a._SYMBptr->feuille);      
      else {
	if (a._SYMBptr->sommet==at_prod)
	  return symbolic(at_prod,inv__VECT(*(a._SYMBptr->feuille._VECTptr)));
	else
	  return symbolic(at_inv,a);
      }
    case _VECT:
      if (a.subtype==_PNT__VECT)
	return gen(invfirst(*a._VECTptr),a.subtype);
      if (a.subtype==_POLY1__VECT)
	return fraction(gen(vecteur(1,plus_one),_POLY1__VECT),a);
      return gen(inv__VECT(*a._VECTptr),a.subtype);
    case _EXT:
      return inv_EXT(a);
    case _USER:
      return a._USERptr->inv();
    case _MOD:
      return modinv(a);
    case _FRAC:
      return fraction(a._FRACptr->den,a._FRACptr->num);
    default: 
      settypeerr("Inv");
    }
    
  }
  
  void inpow(const gen & base,unsigned long int exponent,gen & res){
    if (exponent==1)
      res=base;
    else {
      inpow(base,exponent/2,res);
      res=res*res;
      if (exponent %2)
	res=res*base;
    }
  }

  gen pow(const gen & base, unsigned long int exponent){
    mpz_t * e;
    gen res;
    switch (base.type ) {
    case _INT_: 
      if (base.val<0 && (exponent % 2))
	return(-pow(-base.val,exponent));
      else
	return(pow(absint(base.val),exponent));
    case _DOUBLE_:
      return std::pow(base._DOUBLE_val,double(exponent));
    case _ZINT: 
      e=new mpz_t[1];
      mpz_init(*e);
      mpz_pow_ui(*e,*base._ZINTptr,exponent);
      return(e);
    case _CPLX: case _EXT: case _VECT: case _MOD:
      // gauss integer power
      if (!exponent)
	return 1;
      inpow(base,exponent,res);
      return(res);
    case _IDNT:
      if (!exponent)
	return 1;
      if (exponent==1)
	return base;
      return symbolic(at_pow,makevecteur(base,(long long int)exponent));
    case _SYMB:
      if (!exponent)
	return 1;
      if (exponent==1)
	return base;
      if (base._SYMBptr->sommet==at_pow){
	res= (*((base._SYMBptr->feuille)._VECTptr))[1];
	return pow( (base._SYMBptr->feuille)._VECTptr->front(),gen((long long int) (exponent)) * res) ;
      }
      return symbolic(at_pow,makevecteur(base,(long long int) exponent));
    case _POLY:
      return pow(*base._POLYptr,(int) exponent);
    case _FRAC:
      return pow(*base._FRACptr,(int) exponent);
    default: 
      settypeerr("Pow") ;
    }
  }

  gen pow(const gen & base, int exponent){
    if (is_zero(base)){
      if (exponent>0)
	return base;
      if (!exponent)
	return undef;
      if (exponent %2)
	return unsigned_inf;
      return plus_inf;
    }
    if (exponent<0)
      return inv(pow(base,-exponent));
    unsigned long int expo=exponent;
    return(pow(base,expo));
  }

  gen pow(unsigned long int base, unsigned long int exponent){
    mpz_t *e=new mpz_t[1];
    mpz_init(*e);
    mpz_ui_pow_ui(*e,base,exponent);
    return(e);
  }

  void _ZINTdiv (const gen & a,const gen & b,mpz_t * & quo){
    // at least one is not an int, uncoerce remaining int
    mpz_t *aptr,*bptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    if (b.type!=_INT_)
      bptr=b._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,b.val);
    }
    quo=new mpz_t[1];
    mpz_init(*quo);
    mpz_tdiv_q(*quo,*aptr,*bptr);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (b.type==_INT_){
      mpz_clear(*bptr);
      delete  bptr;
    }
  }

  gen iquobest(const gen & a,const gen & b){
    if (is_strictly_positive(-a))
      return -iquobest(-a,b);
    return iquo(a+iquo(b,2),b);
  }

  gen iquocmplx(const gen & a,const gen & b){
    gen b2=b.squarenorm();
    gen ab=a*b.conj();
    gen res(iquobest(re(ab),b2),iquobest(im(ab),b2));
    return res;
  }
      // integer quotient, use rdiv for symbolic division 
  gen iquo(const gen & a,const gen & b){
    if ((b.type==_INT_)){
      switch (b.val){
      case 1:
	return a;
      case -1:
	return -a;
      }
    }
    mpz_t * quo;
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: 
      return(a.val/b.val);
    case _ZINT__ZINT: case _INT___ZINT: case _ZINT__INT_:
      _ZINTdiv(a,b,quo);
      return(quo);
    case _CPLX__INT_:  case _CPLX__ZINT:
      return gen(iquo(*a._CPLXptr,b),iquo(*(a._CPLXptr+1),b));
    case _INT___CPLX: case _ZINT__CPLX: case _CPLX__CPLX:
      return iquocmplx(a,b);
    default:
      settypeerr("iquo");
    }
  }

  gen rdivsimp(const gen & a,const gen & b){
    if (is_positive(-b))
      return rdivsimp(-a,-b);
    gen c(gcd(a,b));
    if (c.type==_CPLX)
      c=gcd(c.re(),c.im());
    return fraction(iquo(a,c),iquo(b,c));
  }

  gen divpoly(const polynome & p, const gen & e){
    if (p.coord.empty())
      return zero;
    gen d=gcd(Tcontent<gen>(p),e);
    if (is_one(d))
      return fraction(p,e);
    gen den(rdiv(e,d));
    gen iden(inv(den));
    if ( (iden.type!=_SYMB) && (iden.type!=_FRAC))
      return (p/d)*iden;
    else
      return fraction(p/d,den);
  }

  gen divpoly(const gen & e,const polynome & p){
    if (Tis_constant<gen>(p))
      return rdiv(e,p.coord.front().value);
    gen d=gcd(Tcontent<gen>(p),e);
    return fraction(rdiv(e,d),p/d);
  }
  
  gen divpolypoly(const gen & a,const gen &b){
    polynome ap(*a._POLYptr),bp(*b._POLYptr);
    polynome q(ap.dim),r(ap.dim);
    if (ap.TDivRem1(bp,q,r) && r.coord.empty())
      return q;
    return normal(fraction(a,b));
  }

  gen rdiv(const gen &a,const gen &b){
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: case _ZINT__INT_: case _ZINT__ZINT:
      if (is_zero(b))
	return unsigned_inf;
      if (is_zero(a%b))
	return iquo(a,b);
      else
	return rdivsimp(a,b);
    case _CPLX__INT_: case _CPLX__ZINT:
      if ( (a._CPLXptr->type==_DOUBLE_) || ((a._CPLXptr+1)->type==_DOUBLE_))
	return rdiv(a.evalf(),b.evalf());
      if (is_zero(b))
	return unsigned_inf;
      if (is_zero(a%b))
	return iquo(a,b);
      else
	return rdivsimp(a,b);
    case _CPLX__CPLX: case _DOUBLE___CPLX: case _INT___CPLX: case _ZINT__CPLX:
      return rdiv(a*conj(b),b.squarenorm());
    case _DOUBLE___DOUBLE_:
      return a._DOUBLE_val/b._DOUBLE_val;
    case _DOUBLE___INT_:
      return a._DOUBLE_val/b.val;
    case _INT___DOUBLE_:
      return a.val/b._DOUBLE_val;
    case _ZINT__DOUBLE_:
      return mpz_get_d(*a._ZINTptr)/b._DOUBLE_val;
    case _CPLX__DOUBLE_:
      return gen(rdiv(*a._CPLXptr,b)._DOUBLE_val,rdiv(*(a._CPLXptr+1),b)._DOUBLE_val);
    case _DOUBLE___ZINT:
      return a._DOUBLE_val/mpz_get_d(*b._ZINTptr);
      // _CPLX__DOUBLE_, _DOUBLE___CPLX, _CPLX__CPLX, _ZINT__CPLX, _INT___CPLX
    case _VECT__INT_: case _VECT__ZINT: case _VECT__DOUBLE_: case _VECT__CPLX: 
    case _VECT__SYMB: case _VECT__IDNT: case _VECT__POLY: case _VECT__EXT: case _VECT__MOD:
      return gen(divvecteur(*a._VECTptr,b),a.subtype);
    case _VECT__VECT:
       if (a.subtype==_POLY1__VECT || b.subtype==_POLY1__VECT)
	return fraction(a,b).normal();
      else
	return apply(a,b,rdiv);
    case _POLY__POLY:
      return divpolypoly(a,b);
    case _FRAC__FRAC:
      return (*a._FRACptr)/(*b._FRACptr);
    case _SPOL1__SPOL1:
      return spdiv(*a._SPOL1ptr,*b._SPOL1ptr);
    case _POLY__DOUBLE_:
      return (*a._POLYptr)/b;
    case _POLY__INT_: case _POLY__ZINT: case _POLY__CPLX: case _POLY__MOD:
      return divpoly(*a._POLYptr,b);
    case _INT___POLY: case _ZINT__POLY: case _CPLX__POLY:
      return divpoly(a,*b._POLYptr);
    case _INT___VECT: case _ZINT__VECT: case _CPLX__VECT: case _SYMB__VECT:
      if (ckmatrix(b))
	return a*inv(b);
      else
	setsizeerr();
    default:
      if ((a==undef) || (b==undef))
	return undef;
      if (is_one(b))
	return chkmod(a,b);
      if (is_minus_one(b))
	return chkmod(-a,b);
      if (is_zero(a)){
	if (!is_zero(normal(b)))
	  return a;
	else
	  return undef;
      }
      if (a==b) {
	if (is_inf(a))
	  return undef;
	else
	  return chkmod(plus_one,a);
      }
      if (a.type==_FRAC){
	if ( (b.type!=_SYMB) && (b.type!=_IDNT) )
	  return (*a._FRACptr)/b;
	return rdiv(_FRAC2_SYMB(a),b);
      }
      if (b.type==_FRAC){
	if ( (a.type!=_SYMB) && (a.type!=_IDNT) )
	  return a/(*b._FRACptr);
	return rdiv(a,_FRAC2_SYMB(b));
      }
      if (a.type==_SYMB && a._SYMBptr->sommet==at_equal){
	vecteur & va=*a._SYMBptr->feuille._VECTptr;
	if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
	  vecteur & vb=*b._SYMBptr->feuille._VECTptr;
	  return symbolic(at_equal,makevecteur(rdiv(va.front(),vb.front()),rdiv(va.back(),vb.back())));
	}
	else
	  return symbolic(at_equal,makevecteur(rdiv(va.front(),b),rdiv(va.back(),b)));
      }
      if (b.type==_SYMB && b._SYMBptr->sommet==at_equal){
	vecteur & vb=*b._SYMBptr->feuille._VECTptr;
	return symbolic(at_equal,makevecteur(rdiv(a,vb.front()),rdiv(a,vb.back())));
      }
      if ( (a.type==_SYMB) || (a.type==_IDNT) || (b.type==_SYMB) || (b.type==_IDNT) )
	return a*symb_inv(b);
      else
	return fraction(a,b).normal();
    }
  }

  void _ZINTmod (const gen & a,const gen & b,mpz_t * & rem){
    // at least one is not an int, uncoerce remaining int
    mpz_t *aptr,*bptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    if (b.type!=_INT_)
      bptr=b._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,b.val);
    }
    rem=new mpz_t[1];
    mpz_init(*rem);
    mpz_tdiv_r(*rem,*aptr,*bptr);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (b.type==_INT_){
      mpz_clear(*bptr);
      delete  bptr;
    }
  }

  gen operator %(const gen & a,const gen & b){
    mpz_t * rem;
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: 
      if (b.val)
	return(a.val % b.val);
      else
	return a.val;
    case _ZINT__ZINT: case _INT___ZINT: case _ZINT__INT_:
      _ZINTmod(a,b,rem);
      return(rem);
    case _CPLX__INT_: case _CPLX__ZINT:
      return gen(smod((*a._CPLXptr), b), smod(*(a._CPLXptr+1), b) );
    case _INT___CPLX: case _ZINT__CPLX: case _CPLX__CPLX:   
      return(a-b*iquo(a,b));
    case _VECT__VECT:
      return (*a._VECTptr)%(*b._VECTptr);
    default:
      settypeerr("%");
    }
  }

  void _ZINTrem(const gen & a,const gen &b,gen & q,mpz_t * & rem){
    // cout << a << " irem " << b << endl;
    mpz_t *aptr,*bptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    if (b.type!=_INT_)
      bptr=b._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,b.val);
    }
    rem=new mpz_t[1];
    mpz_init(*rem);
    q.uncoerce();
    mpz_tdiv_qr(*q._ZINTptr,*rem,*aptr,*bptr);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (b.type==_INT_){
      mpz_clear(*bptr);
      delete  bptr;
    }
  }

  gen irem(const gen & a,const gen & b,gen & q){
    mpz_t * rem;
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: 
      if (!b.val)
	return a;
      q=gen(a.val/b.val);
      return(a.val % b.val);
    case _ZINT__ZINT: case _INT___ZINT: case _ZINT__INT_:
      _ZINTrem(a,b,q,rem);
      return(rem);
    case _INT___CPLX: case _ZINT__CPLX: case _CPLX__CPLX:   
      q=iquo(a,b);
      return(a-b*q);
    default:
      settypeerr("irem");
    }
  }

  void _ZINTsmod(const gen & a, const gen & b, mpz_t * & rem){
    // at least one is not an int, uncoerce remaining int
    mpz_t *aptr,*bptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    if (b.type!=_INT_)
      bptr=b._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,b.val);
    }
    rem=new mpz_t[1];
    mpz_t rem1,rem2,rem3;
    mpz_init(rem1); mpz_init(rem2); mpz_init(rem3);
    mpz_mod(rem1,*aptr,*bptr); // rem1 positive remainder
    if (mpz_sgn(*bptr)>0)
      mpz_sub(rem2,rem1,*bptr); // negative remainder
    else
      mpz_add(rem2,rem1,*bptr);
    // choose smallest one in abs value
    mpz_neg(rem3,rem2);
    if (mpz_cmp(rem1,rem3)>0)
      mpz_init_set(*rem,rem2);
    else
      mpz_init_set(*rem,rem1);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (b.type==_INT_){
      mpz_clear(*bptr);
      delete  bptr;
    }
    mpz_clear(rem1); mpz_clear(rem2); mpz_clear(rem3); 
  }

  gen smod(const gen & a,const gen & b){
    mpz_t * rem;
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: 
      return smod(a.val,b.val);
    case _ZINT__INT_: case _INT___ZINT: case _ZINT__ZINT: 
      _ZINTsmod(a,b,rem);
      return(rem);
    case _CPLX__INT_: case _CPLX__ZINT:
      return gen(smod(*a._CPLXptr,b),smod(*(a._CPLXptr+1),b));
    default: 
      if ( (b.type==_INT_) || (b.type==_ZINT) )
	return a;
      // error, b must be _DOUBLE_
      throw(std::runtime_error("smod 2nd argument must be _DOUBLE_"));
      return a;
    }
  }

  string print_DOUBLE_(double d){
    if (isnan(d))
      return "undef";
    if (isinf(d))
      return "infinity";
    char s[256];
    sprintf(s,("%."+print_INT_(decimal_digits)+"g").c_str(),d);
    if (d!=floor(d) || d>RAND_MAX || d<-RAND_MAX)
      return s;
    else
      return string(s)+".0";
  }

  string print_ZINT(const mpz_t & a){
    size_t l=mpz_sizeinbase (a, 10) + 2;
    char s[l];
    return mpz_get_str (s, 10,a) ;
  }

  string printinner_VECT(const vecteur & v, int subtype){
    string s;
    vecteur::const_iterator it=v.begin(), itend=v.end();
    if (it==itend)
      return "";
    for(;;){
      if ( (subtype==_RPN_FUNC__VECT) && (it->type==_SYMB) && (it->_SYMBptr->sommet==at_quote))
	s += "'"+it->_SYMBptr->feuille.print()+"'";
      else {
	if ( (it->type==_SYMB) && (it->_SYMBptr->sommet==at_sto) )
	  s += "("+it->print()+")";
	else
	  s += it->print();
      }
      ++it;
      if (it==itend){
	return s;
      }
      if ( (subtype!=_RPN_FUNC__VECT) && 
	   //	   (subtype || (!rpn_mode) ) &&
	   ( ((it-1)->type!=_SYMB) || ((it-1)->_SYMBptr->sommet!=at_comment) )
	   )
	s += ',';
      else
	s += ' ';
    }
  }

  string begin_VECT_string(int subtype){
    string s;
    switch (subtype){
    case _SEQ__VECT:
      break;
    case _SET__VECT:
      if (maple_mode>0)
	s="{";
      else
	s="set[";
      break;
    case _RPN_STACK__VECT:
      s="stack(";
      break;
    case _RPN_FUNC__VECT:
      s="<< ";
      break;
    case _GROUP__VECT:
      s="group[";
      break;
    case _LINE__VECT:
      s="line[";
      break;
    case _VECTOR__VECT:
      s="vector[";
      break;
    case _PNT__VECT:
      s="pnt[";
      break;
    case _MATRIX__VECT:
      s="matrix[";
      break;
    case _POLY1__VECT:
      s="poly1[";
      break;
    case _ASSUME__VECT:
      s = "assume[";
      break;
    case _FOLDER__VECT:
      s = "folder[";
      break;
    default:
      s="[";
    }
    return s;
  }

  string end_VECT_string(int subtype){
    string s;
    switch (subtype){
    case _SEQ__VECT:
      return s;
    case _SET__VECT:
      if (maple_mode>0)
	return "}";
      else
	return "]";
    case _RPN_STACK__VECT:
      return ")";
    case _RPN_FUNC__VECT:
      return " >>";
    default:
      return "]";
    }    
  }

  /* Old delimiters
  string begin_VECT_string(int subtype){
    string s;
    switch (subtype){
    case 0:
      s="[";
      break;
    case _SET__VECT:
      if (maple_mode>0)
	s="{";
      else
	s="%{";
      break;
    case _RPN_STACK__VECT:
      s="stack(";
      break;
    case _RPN_FUNC__VECT:
      s="<< ";
      break;
    case _GROUP__VECT:
      s="%( ";
      break;
    case _LINE__VECT:
      s="%%(";
      break;
    case _VECTOR__VECT:
      s="%%%(";
      break;
    case _PNT__VECT:
      s="%%%%(";
      break;
    case _MATRIX__VECT:
      s="%%[";
      break;
    case _POLY1__VECT:
      s="%[";
      break;
    case _ASSUME__VECT:
      s = "%%%[";
      break;
    }
    return s;
  }

  string end_VECT_string(int subtype){
    string s;
    switch (subtype){
    case 0:
      return "]";
    case _SET__VECT:
      if (maple_mode>0)
	return "}";
      else
	return "%}";
    case _RPN_STACK__VECT:
      return ")";
    case _RPN_FUNC__VECT:
      return " >>";
    case _GROUP__VECT:
      return " %)";
    case _LINE__VECT:
      return " %%)";
    case _VECTOR__VECT:
      return " %%%)";
    case _PNT__VECT:
      return " %%%%)";
    case _MATRIX__VECT:
      return "%%]";
    case _POLY1__VECT:
      return "%]";
    case _ASSUME__VECT:
      return "%%%]";
    default:
      return s;
    }    
  }
  */

  string print_VECT(const vecteur & v,int subtype=0){
    if (v.empty()){
      switch (subtype){
      case _SEQ__VECT:
	return "NULL";
      case _SET__VECT:
	if (maple_mode>0)
	  return "{ }";
	else
	  return "%{ %}";
      case _RPN_FUNC__VECT:
        return "<< >>";
      case _RPN_STACK__VECT:
        return "stack()";
      }
    }
    string s;
    if (subtype==_SPREAD__VECT){
      s = "spreadsheet[";
      int nr=v.size();
      int nc=v.front()._VECTptr->size();
      for (int i=0;;){
	int save_r,save_c;
	vecteur & w=*v[i]._VECTptr;
	s +='[';
	for (int j=0;;){
	  save_r=printcell_current_row;
	  save_c=printcell_current_col;
	  printcell_current_row=i;
	  printcell_current_col=j;
	  s += w[j].print();
	  printcell_current_row=save_r;
	  printcell_current_col=save_c;
	  ++j;
	  if (j==nc)
	    break;
	  else
	    s+=',';
	}
	s+=']';
	++i;
	if (i==nr)
	  break;
	else
	  s+=',';
      }
      return s+']';
    }
    if ( ( subtype==_SEQ__VECT) && (v.size()==1) )
      return "seq["+v.front().print()+"]";
    else
      s=begin_VECT_string(subtype);
    s += printinner_VECT(v,subtype);
    return s+end_VECT_string(subtype);
  }

  string print_SPOL1(const sparse_poly1 & p){
    if (p.empty())
      return "0";
    string s;
    vector<monome>::const_iterator it=p.begin(), itend=p.end();
    for(;;){
      s += it->print();
      ++it;
      if (it==itend)
          return s;
      s += '+';
    }
  }

  string print_the_type(int val){
    switch(val){
    case _INT_:
      return "DOM_int";
    case _DOUBLE_:
      return "DOM_FLOAT";
    case _ZINT:
      return "DOM_INT";
    case _CPLX:
      return "DOM_COMPLEX";
    case _VECT:
      return "DOM_LIST";
    case _IDNT:
      return "DOM_IDENT";
    case _SYMB:
      return "DOM_SYMBOLIC";
    case _FRAC:
      return "DOM_RAT";
    case _STRNG:
      return "DOM_STRING";
    }
    return print_INT_(val);
  }

  string print_STRNG(const string & s){
    string res("\"");
    int l=s.size();
    for (int i=0;i<l;++i){
      res += s[i];
      if (s[i]=='"')
	res += '"';
    }
    return res+'"';
  }

  string printi(){
    if (maple_mode==3)
      return "\xa1";
    if (maple_mode>0)
      return "I";
    else
      return "i";
  }
  
  string gen::print() const{
    switch (type ) {
    case _INT_: 
      if (subtype==_INT_SOLVER){
	switch (val){
	case _BISECTION_SOLVER:
	  return "bisection_solver";
	case _FALSEPOS_SOLVER:
	  return "falsepos_solver";  
	case _BRENT_SOLVER:
	  return "brent_solver";
	case _NEWTON_SOLVER:
	  return "newton_solver";
	case _DNEWTON_SOLVER:
	  return "dnewton_solver";
	case _NEWTONJ_SOLVER:
	  return "newtonj_solver";
	case _SECANT_SOLVER:
	  return "secant_solver";
	case _STEFFENSON_SOLVER:
	  return "steffenson_solver";
	case _HYBRIDSJ_SOLVER:
	  return "hybridsj_solver";
	case _HYBRIDJ_SOLVER:
	  return "hybridj_solver";
	case _HYBRIDS_SOLVER:
	  return "hybrids_solver";
	case _HYBRID_SOLVER:
	  return "hybrid_solver";
	case _GOLUB_REINSCH_DECOMP:
	  return "golub_reinsch_decomp";
	case _GOLUB_REINSCH_MOD_DECOMP:
	  return "golub_reinsch_mode_decomp";
	case _JACOBI_DECOMP:
	  return "jacobi_decomp";
	case _MINOR_DET:
	  return "minor_det";
	case _HESSENBERG_PCAR:
	  return "hessenberg_pcar";
	case _RATIONAL_DET:
	  return "rational_det";
	default:
	  return print_INT_(val);
	}
      }
      if (subtype!=_INT_TYPE)
	return print_INT_(val);
      return print_the_type(val);
    case _DOUBLE_:
      return print_DOUBLE_(_DOUBLE_val);
    case _ZINT: 
      return print_ZINT(*_ZINTptr);
    case _CPLX:
      if (is_zero(*(_CPLXptr+1)))
	return _CPLXptr->print();
      if (is_zero(*_CPLXptr)){
	if (is_one(*(_CPLXptr+1)))
	  return printi();
	if (is_minus_one(*(_CPLXptr+1)))
	  return "-"+printi();
	return (_CPLXptr+1)->print() + "*"+printi();
      }
      if (is_one(*(_CPLXptr+1)))
	return _CPLXptr->print() + "+"+printi();
      if (is_minus_one(*(_CPLXptr+1)))
	return _CPLXptr->print() + "-"+printi();
      if (is_positive(-(*(_CPLXptr+1))))
	return _CPLXptr->print() + string("-") + (-(*(_CPLXptr+1))).print() + "*"+printi();
      return _CPLXptr->print() + string("+") + (_CPLXptr+1)->print() + "*"+printi();
    case _IDNT:
      return _IDNTptr->print();
    case _SYMB:
      if (subtype==_SPREAD__SYMB)
	return "="+_SYMBptr->print();
      else
	return _SYMBptr->print();
    case _VECT:
      return print_VECT(*_VECTptr,subtype);
    case _POLY:
      return _POLYptr->print() ;
    case _SPOL1:
      return print_SPOL1(*_SPOL1ptr);
    case _EXT:
      return "%%{"+_EXTptr->print()+':'+(*(_EXTptr+1)).print()+"%%}";
    case _USER:
      return _USERptr->print();
    case _MOD:
      if ( (_MODptr->type==_SYMB) && (_MODptr->_SYMBptr->sommet!=at_pow) )
	return "("+_MODptr->print()+") % "+(*(_MODptr+1)).print();
      return _MODptr->print()+" % "+(*(_MODptr+1)).print();
    case _ROOT:
      return _ROOTptr->print();
    case _FRAC:
      return _FRAC2_SYMB(*this).print();
    case _STRNG:
      return print_STRNG(*_STRNGptr);
    case _FUNC:
      if (*this==at_return){
	if (maple_mode==3)
	  return "Return";
	else
	  return "return ;";
      }
      if (rpn_mode || _FUNCptr->ptr->printsommet==&printastifunction) 
	return _FUNCptr->ptr->print();
      else
	return "'"+_FUNCptr->ptr->print()+"'";
    default:
      settypeerr("print");
    }
  }

  void gen::dbgprint() const{    
    if (this->type==_POLY)
      _POLYptr->dbgprint();
    else
      cout << this->print() << endl; 
  }

  ostream & operator << (ostream & os,const gen & a) { return os << a.print(); }

  string monome::print() const {
    return '<' + coeff.print() + "," + exponent.print() + '>' ;
  }

  void monome::dbgprint() const {
    cout << this->print();
  }

  ostream & operator << (ostream & os,const monome & m){
    return os << m.print() ;
  }

  /*
  gen string2_ZINT(string s,int l,int & pos){
    char ss[l+1];
    int neg=1;
    if (s[pos]=='-'){
      pos++;
      neg=-1;
    }
    int i=0;
    for (;(pos<l) && (s[pos]>='0') && (s[pos]<='9');pos++,i++)
      ss[i]=s[pos];
    if ((!i) && (s[pos]=='i') || (s[pos]=='I')){
      return(neg);
    }
    assert(i);
    ss[i]=char(0);
    mpz_t *mpzin = new mpz_t[1];
    mpz_init(*mpzin);
    mpz_set_str (*mpzin, ss, 10);
    if (neg>0)
      return(gen(mpzin));
    else
      return(-gen(mpzin));
  }

  istream & operator >> (istream & is,gen & a){
    string s;
    is >> s;
    int l=s.size();
    int pos=0;
    a=gen(0);
    while (pos<l){
      if ((s[pos]=='i') || (s[pos]=='I')){
	a=a+gen(0,1);
	pos++;
      }
      else {
	if (s[pos]=='+')
	  pos++;
	else {
	  gen tmp(string2_ZINT(s,l,pos));
	  if (s[pos]=='*'){
	    pos++;
	    assert( (s[pos]=='i') || (s[pos]=='I') );
	    pos++; // skip *I
	    a=a+tmp*cst_i;
	  }
	  else {
	    if ((s[pos]=='i') || (s[pos]=='I')){
	      pos++; // skip I
	      a=a+tmp*cst_i;	
	    }
	    else
	      a=a+tmp;
	  }
	}
      }
    }
    return is;
  }
  */

  istream & operator >> (istream & is,gen & a){
    string s;
    is >> s;
    a = gen(s,list_one_letter__IDNT);
    return is;
  }

  gen operator !(const gen & a){
    switch (a.type){
    case _INT_: case _ZINT: case _CPLX: case _DOUBLE_:
      return (is_zero(a));
    default:
      return symb_not(a);
    }
  }

  // equality of vecteurs representing geometrical lines
  bool geo_equal(const vecteur &v,const vecteur & w,int subtype){
    int vs=v.size(),ws=w.size();
    if (vs!=ws)
      return false;
    if (v==w)
      return true;
    if ( (subtype==_LINE__VECT)  && (vs==2)){
      if (v[1]==v[0])
	return v==w;
      // v[1]!=v[0]
      if (!is_zero(im(rdiv(w[0]-v[0],v[1]-v[0]))))
	return false;
      if (!is_zero(im(rdiv(w[1]-v[0],v[1]-v[0]))))
	return false;
      return true;
    }
    if (subtype==_SET__VECT){
      vecteur w1(w),v1(v);
      sort(w1.begin(),w1.end(),islesscomplexthan);
      sort(v1.begin(),v1.end(),islesscomplexthan);
      return w1==v1;
    }
    return false;
  }

  bool operator ==(const gen & a,const gen & b){
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: 
      return (a.val==b.val);
    case _INT___ZINT: 
      return (mpz_cmp_si(*b._ZINTptr,a.val)==0);
    case _ZINT__INT_:
      return (mpz_cmp_si(*a._ZINTptr,b.val)==0);
    case _ZINT__ZINT:
      return (mpz_cmp(*a._ZINTptr,*b._ZINTptr)==0);
    case _INT___CPLX: case _ZINT__CPLX:
      return ( (a==re(b)) && is_zero(im(b)));
    case _CPLX__ZINT: case _CPLX__INT_:
      return ( (re(a)==b) && is_zero(im(a))) ;
    case _CPLX__CPLX:
      return( (*a._CPLXptr==*b._CPLXptr) && (*(a._CPLXptr+1)==*(b._CPLXptr+1)) );
    case _DOUBLE___DOUBLE_:
      return std::abs(a._DOUBLE_val-b._DOUBLE_val)<epsilon;
    case _IDNT__IDNT:
      return (a._IDNTptr->name==b._IDNTptr->name) || (*a._IDNTptr->name==*b._IDNTptr->name);
    case _SYMB__SYMB:
      if (a._SYMBptr==b._SYMBptr)
	return true;
      return ( (a._SYMBptr->sommet==b._SYMBptr->sommet) && (a._SYMBptr->feuille==b._SYMBptr->feuille));
    case _VECT__VECT:
      if (a._VECTptr==b._VECTptr)
	return true;
      if (a.subtype!=b.subtype)
	return false;
      if (a.subtype)
	return geo_equal(*a._VECTptr,*b._VECTptr,a.subtype);
      return *a._VECTptr==*b._VECTptr;
    case _POLY__POLY:
      if (a._POLYptr==b._POLYptr)
	return true;
      return (a._POLYptr->dim==b._POLYptr->dim) && (a._POLYptr->coord==b._POLYptr->coord);
    case _FRAC__FRAC:
      return (a._FRACptr->num==b._FRACptr->num) && (a._FRACptr->den==b._FRACptr->den);
    case _STRNG__STRNG:
      return (a._STRNGptr==b._STRNGptr) || (*a._STRNGptr==*b._STRNGptr);
    case _FUNC__FUNC:
      return (a._FUNCptr==b._FUNCptr) || (*a._FUNCptr==*b._FUNCptr);
    case _MOD__MOD: case _EXT__EXT:
      return ( (*a._EXTptr==*b._EXTptr) && (*(a._EXTptr+1)==*(b._EXTptr+1)) );
    default: // Check pointers, type subtype
      if ((a.type==b.type) && (a.subtype==b.subtype) && (a.val==b.val))
	return true;
      if (a.type==_USER)
	return *a._USERptr==b;
      if (b.type==_USER)
	return *b._USERptr==a;
      return false;
    }
  }

  bool operator !=(const gen & a,const gen & b){
    return !(a==b);
  }

  gen equal(const gen & a,const gen &b){
    return symbolic(at_equal,makevecteur(a,b));
  }

  gen sign(const gen & a){
    if (is_equal(a))
      return apply_to_equal(a,sign);
    if (is_zero(a)){
      if (a.type==_DOUBLE_)
	return 0.0;
      else
	return 0;
    }
    switch (a.type){
    case _INT_: case _ZINT: 
      if (is_positive(a))
	return 1;
      else
	return -1;
    case _DOUBLE_:
      if (a._DOUBLE_val>0)
	return 1.0;
      else
	return -1.0;
    }
    gen b=evalf(a);
    if (b.type==_DOUBLE_){
      if (b._DOUBLE_val>0)
	return plus_one;
      if (b._DOUBLE_val==0)
	return zero;
      return minus_one;
    }
    if (is_zero(im(a))){
      int s=sturmsign(a);
      if (s)
	return s;
    }
    return symbolic(at_sign,a);
  }
  
  gen sym_is_greater(const gen & a,const gen & b){
    if (is_undef(a) || (a==unsigned_inf) || is_undef(b) || (b==unsigned_inf))
      return undef;
    if (a==b)
      return false;
    if ( (b==plus_inf) || (a==minus_inf) )
      return false;
    if ( (b==minus_inf) || (a==plus_inf) )
      return true;
    if (a.type==_USER)
      return (*a._USERptr>b);
    if (b.type==_USER)
      return (*b._USERptr<=a);
    gen approx=(a-b).evalf();
    if (approx.type==_DOUBLE_)
      return (approx>0);
    gen g=sign(a-b);
    if (is_one(g))
      return plus_one;
    if (is_minus_one(g))
      return false;
    return symb_superieur(a,b);
  }

  gen operator >(const gen & a,const gen & b){
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: 
      return (a.val>b.val);
    case _INT___ZINT: 
      return (mpz_cmp_si(*b._ZINTptr,a.val)<0);
    case _ZINT__INT_:
      return (mpz_cmp_si(*a._ZINTptr,b.val)>0);
    case _ZINT__ZINT:
      return (mpz_cmp(*a._ZINTptr,*b._ZINTptr)>0);
    case _DOUBLE___DOUBLE_:
      return a._DOUBLE_val>b._DOUBLE_val;
    case _DOUBLE___INT_:
      return a._DOUBLE_val>b.val;
    case _INT___DOUBLE_:
      return a.val>b._DOUBLE_val;
    case _DOUBLE___ZINT:
      return a._DOUBLE_val>mpz_get_d(*b._ZINTptr);
    case _ZINT__DOUBLE_:
      return b._DOUBLE_val>mpz_get_d(*a._ZINTptr);
    default:
      return sym_is_greater(a,b);
    }
  }

  gen operator <(const gen & a,const gen & b){
    return (b>a);
  }

  gen operator >=(const gen & a,const gen & b){
    gen g=!(b>a);
    if (g.type==_INT_)
      return g;
    return symb_sup_ou_egal(a,b);
  }

  gen operator <=(const gen & a,const gen & b){
    return b>=a;
  }

  bool is_zero__VECT(const vecteur & v){
    vecteur::const_iterator it=v.begin(),itend=v.end();
    for (;it!=itend;++it){
      if (!is_zero(*it))
	return false;
    }
    return true;
  }

  bool is_zero(const gen & a){
    switch (a.type ) {
    case _INT_: 
      return !a.val; 
    case _ZINT: 
      return (!mpz_sgn(*a._ZINTptr));
    case _CPLX:
      return (is_zero(*a._CPLXptr) && is_zero(*(a._CPLXptr+1)));
    case _DOUBLE_:
      return (fabs(a._DOUBLE_val)<=epsilon);
    case _VECT:
      return is_zero__VECT(*a._VECTptr);
    case _POLY:
      return a._POLYptr->coord.empty();
    case _FRAC:
      return is_zero(a._FRACptr->num);
    case _MOD:
      return is_zero(*a._MODptr);
    case _USER:
      return a._USERptr->is_zero();
    default: 
      return false;
    }
  }

  bool is_one(const gen & a){
    switch (a.type ) {
    case _INT_: 
      return a.val==1; 
    case _ZINT: 
      return (a==gen(1));
    case _CPLX:
      return (is_one(*a._CPLXptr) && is_zero(*(a._CPLXptr+1)));
    case _DOUBLE_:
      return a._DOUBLE_val==1;
    case _VECT:
      return a._VECTptr->size()==1 && is_one(a._VECTptr->front());
    case _POLY:
      return Tis_constant(*a._POLYptr) && (is_one(a._POLYptr->coord.front().value));
    case _FRAC:
      return a._FRACptr->num == a._FRACptr->den;
    case _MOD:
      return is_one(*a._MODptr);
    case _USER:
      return a._USERptr->is_one();
    default: 
      return false;
    }
  }

  bool is_minus_one(const gen & a){
    switch (a.type ) {
    case _INT_: 
      return a.val==-1; 
    case _ZINT: 
      return (a==gen(-1));
    case _CPLX:
      return (is_minus_one(*a._CPLXptr) && is_zero(*(a._CPLXptr+1)));
    case _DOUBLE_:
      return a._DOUBLE_val==-1;
    case _VECT:
      return a._VECTptr->size()==1 && is_minus_one(a._VECTptr->front());
    case _POLY:
      return Tis_constant(*a._POLYptr) && (is_minus_one(a._POLYptr->coord.front().value));
    case _FRAC:
      return a._FRACptr->num == -a._FRACptr->den;
    case _MOD:
      if (*(a._MODptr+1)==plus_two)
	return is_one(*a._MODptr);
      else
	return is_minus_one(*a._MODptr);
    case _USER:
      return a._USERptr->is_minus_one();
    default: 
      return false;
    }
  }

  bool is_sq_minus_one(const gen & a){
    switch (a.type ) {
    case _CPLX: case _MOD: case _USER:
      return is_minus_one(a*a);
    case _VECT:
      return a._VECTptr->size()==1 && is_sq_minus_one(a._VECTptr->front());
    case _POLY:
      return Tis_constant(*a._POLYptr) && (is_sq_minus_one(a._POLYptr->coord.front().value));
    default: 
      return false;
    }
  }

  gen _CPLXgcd(const gen & a,const gen & b){ // a & b must be gen
    if (!is_cinteger(a) || !is_cinteger(b) )
      return plus_one;
    gen acopy(a),bcopy(b),r;
    for (;;){
      if (is_zero(bcopy)){
	complex<double> c=gen2complex_d(acopy);
	double d=arg(c);
	int quadrant=int((2*d)/M_PI);
	switch (quadrant){
	case 0:
	  return acopy;
	case 1:
	  return acopy*(-cst_i);
	case -1:
	  return acopy*cst_i;
	case 2: case -2:
	  return -acopy;
	default:
	  return acopy;
	}
      }
      r=acopy%bcopy;
      acopy=bcopy;
      bcopy=r;
    }
  }

  gen rationalgcd(const gen &a,const gen & b); // in sym2poly.h

  // gcd(undef,x)=x to be used inside series
  gen symgcd(const gen & a,const gen& b){
    if (is_zero(a) || is_undef(a) || (is_one(b)))
      return b;
    if (is_one(a) || is_undef(b) || (is_zero(b)))
      return a;
    if (a==b)
      return a;
    if ( (a.type==_MOD) && (b.type==_MOD) && (a._MODptr->type<=_CPLX) && (b._MODptr->type<= _CPLX) )
      return chkmod(plus_one,a);
    if ( (a.type==_POLY) && (b.type==_POLY) )
      return gcd(*a._POLYptr,*b._POLYptr);
    if ( (a.type==_EXT) && (b.type ==_EXT) ){
      if ( (*(a._EXTptr+1)!=*(b._EXTptr+1)) || (a._EXTptr->type!=_VECT) || (b._EXTptr->type!=_VECT) )
	return plus_one;
      vecteur g=gcd(*a._EXTptr->_VECTptr,*b._EXTptr->_VECTptr);
      return ext_reduce(g,*(a._EXTptr+1));
    }
    if ( (a.type==_FRAC) || (b.type==_FRAC))
        return plus_one;
    if (a.type==_EXT){
      if (a._EXTptr->type!=_VECT)
	settypeerr("symgcd");
      gen aa(lgcd(*a._EXTptr->_VECTptr));
      gen res=gcd(aa,b);
      vecteur ua((*(a._EXTptr->_VECTptr))/aa),u,uv(*((a._EXTptr+1)->_VECTptr)),v,dd;
      egcd(ua,uv,u,v,dd);
      gen dd0(dd.front()),b2(rdiv(b,res));
      simplify(b2,dd0);
      if (is_one(dd0))
	return res*algebraic_EXTension(ua,uv);
      return res;
    }
    if (b.type==_EXT)
      return symgcd(b,a);
    if (a.type==_POLY)
      return gcd(*a._POLYptr,polynome(b,a._POLYptr->dim));
    if (b.type==_POLY)
      return gcd(*b._POLYptr,polynome(a,b._POLYptr->dim));
    if ( (a.type!=_DOUBLE_) && (a.type!=_VECT) && (b.type!=_DOUBLE_) && (b.type!=_VECT) )
      return rationalgcd(a,b);
    return plus_one; // settypeerr("symgcd");
  }

  gen simplify(gen & n, gen & d){
    if ( (d.type==_DOUBLE_) || 
	 ( (d.type==_CPLX) && 
	   ((d._CPLXptr->type==_DOUBLE_) || ((d._CPLXptr+1)->type==_DOUBLE_)) )
	 ){
      gen dd=d.evalf();
      n=rdiv(n.evalf(),dd);
      d=plus_one;
      return dd;
    }
    if ( (n.type==_DOUBLE_) ||
	 ( (n.type==_CPLX) && 
	   ((n._CPLXptr->type==_DOUBLE_) || ((n._CPLXptr+1)->type==_DOUBLE_)) )
	 ){
      gen nn=n.evalf();
      n=plus_one;
      d=rdiv(d.evalf(),nn);
      return nn*simplify(n,d);
    }
    if (is_one(d))
      return d;
    if ( (n.type==_MOD) && (d.type!=_MOD) )
      d=makemod(d,*(n._MODptr+1));
    if (d.type==_MOD){
      if (d._MODptr->is_integer()){
	gen dd(d);
	n=n*inv(dd);
	d=makemodquoted(plus_one,*(d._MODptr+1));
	return dd;
      }
    }
    if (is_one(n))
      return n;
    if ((n.type==_POLY) && (d.type==_POLY)){
      polynome np(*n._POLYptr),dp(*d._POLYptr);
      polynome g(np.dim);
      g=simplify(np,dp);
      n=np;
      d=dp;
      return g;
    }
    if (n.type==_VECT){
      if (d.type==_VECT){
	vecteur g=gcd(*n._VECTptr,*d._VECTptr);
	n=gen(*n._VECTptr/g,_POLY1__VECT);
	d=gen(*d._VECTptr/g,_POLY1__VECT);
	return gen(g,_POLY1__VECT);
      }
      gen nd=_gcd(n);
      gen g=simplify(nd,d);
      n=divvecteur(*n._VECTptr,g);
      return g;
    }
    if (d.type==_VECT){
      gen dd=_gcd(d);
      gen g=simplify(n,dd);
      d=divvecteur(*d._VECTptr,g);
      return g;
    }
    if (d.type==_EXT){
      if ( (d._EXTptr->type==_INT_) || (d._EXTptr->type==_ZINT) ){
	n=n*inv(d);
	gen d_copy=d;
	d=1;
	return d_copy;
      }
      if (d._EXTptr->type==_VECT){
	vecteur u,v,dd;
	egcd(*(d._EXTptr->_VECTptr),*((d._EXTptr+1)->_VECTptr),u,v,dd);
        gen tmp=algebraic_EXTension(u,*((d._EXTptr+1)->_VECTptr));
	n=n*tmp;
	d=d*tmp;
	return simplify(n,d)*inv_EXT(tmp);
      }
      settypeerr("simplify");
    }
    if (n.type==_EXT){
      gen n_EXT=*n._EXTptr;
      gen g=simplify(n_EXT,d);
      n=algebraic_EXTension(n_EXT,*(n._EXTptr+1));
      return g;
    }
    if (n.type==_POLY) {
      polynome np(*n._POLYptr),dp(d,n._POLYptr->dim);
      polynome g(np.dim);
      g=simplify(np,dp);
      n=np;
      d=dp;
      return g;
    }
    if (d.type==_POLY){
      polynome np(n,d._POLYptr->dim),dp(*d._POLYptr);
      polynome g(np.dim);
      g=simplify(np,dp);
      n=np;
      d=dp;
      return g;
    }
    vecteur l(lvar(n));
    lvar(d,l);
    gen num=e2r(n,l),den=e2r(d,l),g=gcd(num,den);
    den=rdiv(den,g);
    if (is_positive(-den)){
      den=-den;
      g=-g;
    }
    n=r2sym(rdiv(num,g),l);
    d=r2sym(den,l);
    return r2sym(g,l);
  }

  gen gcd(const gen & a,const gen & b){
    mpz_t * res;
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: 
      return(gcd(a.val,b.val)); 
    case _INT___ZINT: 
      if (a.val)
	return(int(mpz_gcd_ui(NULL,*b._ZINTptr,absint(a.val))));
      else
	return(b);
    case _ZINT__INT_:
      if (b.val)
	return(int(mpz_gcd_ui(NULL,*a._ZINTptr,absint(b.val))));
      else
	return(a);
    case _ZINT__ZINT:
      res=new mpz_t[1];
      mpz_init(*res);
      mpz_gcd(*res,*a._ZINTptr,*b._ZINTptr);
      return(res);
    case _INT___CPLX: case _ZINT__CPLX:
    case _CPLX__INT_: case _CPLX__ZINT:
    case _CPLX__CPLX:
      return _CPLXgcd(a,b);
    case _POLY__POLY:
      return gcd(*a._POLYptr,*b._POLYptr);
    case _VECT__VECT:
      return gen(gcd(*a._VECTptr,*b._VECTptr),_POLY1__VECT);
    default:
      if (a.type==_USER)
	return a._USERptr->gcd(b);
      if (b.type==_USER)
	return b._USERptr->gcd(a);
      return symgcd(a,b);
    }
  }

  gen lcm(const gen & a,const gen & b){
    return normal(rdiv(a,gcd(a,b)))*b;
  }

  void ciegcd(const gen &a_orig,const gen &b_orig, gen & u,gen &v,gen &d ){
    gen a(a_orig),b(b_orig),au(plus_one),bu(zero),q,r,ru;
    while (!is_zero(b)){
      q=iquo(a,b);
      r=a-b*q;
      a=b;
      b=r;
      ru=au-bu*q;
      au=bu;
      bu=ru;
    }
    u=au;
    d=a;
    v=iquo(d-a_orig*u,b_orig);
  }

  void egcd(const gen &ac,const gen &bc, gen & u,gen &v,gen &d ){
    gen a(ac),b(bc);
    switch ( (a.type<< _DECALAGE) | b.type ) {
    case _INT___INT_: case _INT___ZINT: case _ZINT__INT_: case _ZINT__ZINT: 
      if (a.type==_INT_)
	a.uncoerce();
      if (b.type==_INT_)
	b.uncoerce();
      if (!u.type)
	u.uncoerce();
      if (!v.type)
	v.uncoerce();
      if (!d.type)
	d.uncoerce();
      mpz_gcdext(*d._ZINTptr,*u._ZINTptr,*v._ZINTptr,*a._ZINTptr,*b._ZINTptr);
      break;
    default: 
      ciegcd(a,b,u,v,d);
      break;
    }
  }

  void _ZINTinvmod(const gen & a,const gen & modulo, mpz_t * & res){
    mpz_t *aptr,*bptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    if (modulo.type)
      bptr=modulo._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,modulo.val);
    }
    res=new mpz_t[1];
    mpz_init(*res);
    assert(mpz_invert(*res,*aptr,*bptr));
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (!modulo.type){
      mpz_clear(*bptr);
      delete  bptr;
    }
  }

  gen invmod(const gen & a,const gen & modulo){
    mpz_t * res;
    switch ( (a.type<< _DECALAGE) | modulo.type) {
    case _INT___INT_: 
      return(invmod(a.val,modulo.val));
    case _INT___ZINT: case _ZINT__INT_: case _ZINT__ZINT:
      _ZINTinvmod(a,modulo,res);
      return(res);
    default: 
      settypeerr("invmod");
    }
  }

  gen fracmod(const gen & a_orig,const gen & modulo){
    // write a as p/q with |p| and |q|<sqrt(modulo/2)
    gen a(a_orig),m(modulo);
    if (a.type==_INT_)
      a.uncoerce();
    if (m.type==_INT_)
      m.uncoerce();
    if ( (a.type!=_ZINT) || (m.type!=_ZINT) )
      setsizeerr();
    mpz_t u,d,u1,d1,absd1,sqrtm,q,ur,r,tmp;
    mpz_init_set_si(u,1);
    mpz_init_set(d,*a._ZINTptr);
    mpz_init_set_si(u1,0);
    mpz_init_set(d1,*m._ZINTptr);
    mpz_init(absd1);
    mpz_init(sqrtm);
    mpz_init(q);
    mpz_init(ur);
    mpz_init(r);
    mpz_init(tmp);
    mpz_tdiv_q_2exp(q,*m._ZINTptr,1);
    mpz_sqrt(sqrtm,q);
    int signe;
    for (;;){
      mpz_abs(d1,absd1);
      if (mpz_cmp(absd1,sqrtm)<=0)
	break;
      mpz_fdiv_qr(q,r,d,d1);
      // u-q*u1->ur, v-q*v1->vr
      mpz_mul(tmp,q,u1);
      mpz_sub(ur,u,tmp);
      // u1 -> u, ur -> u1 ; v1 -> v, vr -> v1, d1 -> d, r -> d1
      mpz_set(u,u1);
      mpz_set(u1,ur);
      mpz_set(d,d1);
      mpz_set(d1,r);
    }
    // u1*a+v1*m=d1 -> a=d1/u1 modulo m
    gen num(d1);
    gen den(u1);
    mpz_clear(u);
    mpz_clear(d);
    mpz_clear(u1);
    mpz_clear(d1);
    mpz_clear(absd1);
    mpz_clear(sqrtm);
    mpz_clear(q);
    mpz_clear(ur);
    mpz_clear(r);
    mpz_clear(tmp);
    if (is_positive(den))
      return fraction(num,den);
    else
      return fraction(-num,-den);
  }

  void _ZINTpowmod(const gen & base,const gen & expo,const gen & modulo, mpz_t * & res){
    mpz_t *aptr,*bptr;
    if (base.type)
      aptr=base._ZINTptr;
    else { 
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,base.val);
    }
    if (modulo.type)
      bptr=modulo._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,modulo.val);
    }
    res=new mpz_t[1];
    mpz_init(*res);
    if (!expo.type)
      mpz_powm_ui(*res,*aptr,expo.val,*modulo._ZINTptr);
    else
      mpz_powm (*res,*aptr,*expo._ZINTptr,*bptr);
    if (!base.type){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (!modulo.type){
      mpz_clear(*bptr);
      delete  bptr;
    }
  }

  gen powmod(const gen &base,const gen & expo,const gen & modulo){
    if ((expo.type!=_INT_) && (expo.type!=_ZINT))
      setsizeerr(); // exponent must be a _DOUBLE_ integer
    if (!is_positive(expo))
      return(powmod(invmod(base,modulo),-expo,modulo));
    if (modulo.type==_INT_){
      // try converting base to int and expo to a long
      gen mybase(base % modulo);
      if ( (expo.type==_INT_) && (mybase.type==_INT_) ){
	unsigned long tmp=expo.val;
	return powmod(mybase.val,tmp,modulo.val);
      }
    }
    mpz_t * res;
    switch ( (base.type<< _DECALAGE) | modulo.type) {
    case _INT___INT_: case _INT___ZINT: case _ZINT__INT_: case _ZINT__ZINT:
      _ZINTpowmod(base,expo,modulo,res);
      return(res);
    default: 
      settypeerr("powmod");
    }
  }

  // assuming amod and bmod are prime together, find c such that
  // c = a mod amod  and c = b mod bmod
  // hence a + A*amod = b + B*bmod
  // or A*amod -B*bmod = b - a
  gen ichinrem(const gen & a,const gen &b,const gen & amod, const gen & bmod){
    gen A,B,d,q;
    egcd(amod,bmod,A,B,d);
    if (!is_zero(irem(b-a,d,q)))
      setsizeerr("No Integer Solution");
    A=A*q;
    return(smod(A*amod+a,amod*bmod));
  }

  gen isqrt(const gen & a){
    if ( (a.type!=_INT_) && (a.type!=_ZINT))
      settypeerr("isqrt");
    mpz_t *aptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    mpz_t *res=new mpz_t[1];
    mpz_init(*res);
    mpz_sqrt(*res,*aptr);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    return(res);
  }

  int is_perfect_square(const gen & a){
    if ( (a.type!=_INT_) && (a.type!=_ZINT))
      return false;
    mpz_t *aptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    int res= mpz_perfect_square_p(*aptr);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    return res;
  }

  int is_probab_prime_p(const gen & a){
    if ( (a.type!=_INT_) && (a.type!=_ZINT))
      settypeerr("is_probab_prime_p");
    mpz_t *aptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    int res= mpz_probab_prime_p(*aptr,TEST_PROBAB_PRIME);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    return res;
  }

  gen nextprime(const gen & a){
    if ( (a.type!=_INT_) && (a.type!=_ZINT))
      settypeerr("nextprime");
    gen res(a);
    if (is_zero(smod(res,plus_two)))
      res=res+1;
    for ( ; ; res=res+2)
      if (is_probab_prime_p(res))
	return(res);
  }

  gen prevprime(const gen & a){
    if ( (a.type!=_INT_) && (a.type!=_ZINT))
      settypeerr("prevprime");
    gen res(a);
    if (is_zero(smod(res,plus_two)))
      res=res-1;
    for ( ; ; res=res-2)
      if (is_probab_prime_p(res))
	return(res);
    return zero;
  }

  int jacobi(const gen & a, const gen &b){
    if ( (a.type!=_INT_) && (a.type!=_ZINT))
      settypeerr("jacobi");
    if ( (b.type!=_INT_) && (b.type!=_ZINT))
      settypeerr("jacobi");
    mpz_t *aptr,*bptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    if (b.type!=_INT_)
      bptr=b._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,b.val);
    }
    int res= mpz_jacobi(*aptr,*bptr);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (b.type==_INT_){
      mpz_clear(*bptr);
      delete  bptr;
    }
    return res;
  }

  int legendre(const gen & a, const gen & b){
    if ( (a.type!=_INT_) && (a.type!=_ZINT))
      settypeerr("legendre");
    if ( (b.type!=_INT_) && (b.type!=_ZINT))
      settypeerr("legendre");
    mpz_t *aptr,*bptr;
    if (a.type!=_INT_)
      aptr=a._ZINTptr;
    else {
      aptr=new mpz_t[1];
      mpz_init_set_si(*aptr,a.val);
    }
    if (b.type!=_INT_)
      bptr=b._ZINTptr;
    else {
      bptr=new mpz_t[1];
      mpz_init_set_si(*bptr,b.val);
    }
    int res=mpz_legendre(*aptr,*bptr);
    if (a.type==_INT_){
      mpz_clear(*aptr);
      delete  aptr;
    }
    if (b.type==_INT_){
      mpz_clear(*bptr);
      delete  bptr;
    }
    return res;
  }

  bool has_denominator(const gen & n){
    switch (n.type ) {
    case _INT_: case _ZINT: case _CPLX: case _DOUBLE_: case _IDNT: case _EXT: case _POLY: case _MOD:
      return false;
    case _SYMB: case _FRAC:
      return true;
    default:
      settypeerr("has_denominator");
    }
  }

  // Note that this function should be optimized for large input
  string cut_string(const string & chaine,int nchar,vector<int> & ligne_end) {
    // cerr << clock() << endl;
    int pos;
    if (ligne_end.empty())
      pos=0;
    else
      pos=ligne_end.back()+1;
    int l=chaine.size();
    string res;
    for (int i=0;i<l;){
      // look for \n between i and l
      int k=chaine.find_first_of('\n',i);
      if ( (l-i<nchar) && ((k<i)||(k>=l-1)) ){
	ligne_end.push_back(pos+l);
	// cerr << clock() << endl;
	return res+chaine.substr(i,l-i);
      }
      if ((k>=i) && (k<i+nchar+4*(i==0)) ){
	ligne_end.push_back(pos+k);
	res += chaine.substr(i,k+1-i);
	i=k+1;
      }
      else {
	int j;
	int j1=chaine.find_last_of('+',i+nchar+4*(i==0));
	int j2=chaine.find_last_of('-',i+nchar+4*(i==0));
	int j3=chaine.find_last_of(',',i+nchar+4*(i==0));
	j=max(j1,max(j2,j3));
	if ((j-i)<(nchar/2))
	  j=i+nchar+4*(i==0);
	ligne_end.push_back(pos+min(j,l));
	res += chaine.substr(i,j-i);
	i=j;
	if (i<l){
	  res +="\\\n     ";
	  pos +=7;
	}
      }
    }
    // cerr << clock() << endl;
    return res;
  }

  string calc_endlines_positions(const vecteur & history_in,const vecteur & history_out,int nchar,vector<int> & endlines,vector<int> & positions){
    string res;
    endlines.clear();
    positions.clear();
    int s_in=history_in.size(),s_out=history_out.size();
    int s=max(s_in,s_out);
    for (int i=0;i<s;++i){
      string chaine;
      if (rpn_mode)
	chaine=print_INT_(s-i)+": ";
      else
	chaine=print_INT_(i)+": ";
      if (!rpn_mode){
	if (i<s_in)
	  chaine+=history_in[i].print()+" = ";
      }
      else
	chaine +="   ";
      if (i<s_out)
	chaine += history_out[i].print();
      if (i)
	res +='\n';
      res += cut_string(chaine,nchar,endlines);
      positions.push_back(endlines.back());
    }
    return res;
  }

  bool is_operator_char(char c){
    switch(c){
    case '+': case '-': case '*': case '/': case '^': case '%':
      return true;
    }
    return false;
  }

  bool is_operator_char(char c,char op){
    switch(c){
    case '+': case '-': 
      return true;
    case '*': case '/': case '^': case '%':
      return c==op;
    }
    return false;
  }

  int matchpos(const string & s,int pos){
    char c=s[pos];
    int l=s.size();
    int counter1=0,counter2=0,counter3=0,incr;
    if ( (c==')') || (c==']') || (c=='}') )
      incr=-1;
    else
      incr=1;
    for (;(pos>=0) && (pos<l);pos+=incr){
      switch (s[pos]){
      case '(':
	counter1++;
	break;
      case ')':
	counter1--;
	break;
      case '[':
	counter2++;
	break;
      case ']':
	counter2--;
	break;
      case '{':
	counter3++;
	break;
      case '}':
	counter3--;
	break;
      }
      if ( (!counter1) && (!counter2) && (!counter3) )
	break;
    }
    return pos;
  }

  void find_left(const string & s,int & pos1,int & pos2){
    int l=s.size();
    pos1=min(max(pos1,0),l);
    int pos1orig=pos1;
    if (!pos1)
      return;
    pos2=max(min(pos2,l),0);
    int counter1=0,counter2=0;
    if (pos2==l){
      int i=pos2;
      for (;i>pos1;){
	--i;
	char ch = s[i];
	if (ch=='(')
	  ++counter1;
	if (ch==')')
	  --counter1;
	if (ch=='[')
	  ++counter2;
	if (ch==']')
	  --counter2;
      }
    }
    for (;pos1>=0;--pos1){
      char ch=s[pos1];
      if ( (!counter1) && (!counter2) && ( (ch=='(') || (ch=='[') || (ch=='+') || (ch=='-') || (ch==',')  )){
	if ( (pos1<pos1orig) && ( (ch!='(') || (s[pos2-1]!=')') ) )
	  ++pos1;
	break;
      }
      if (ch=='('){
	++counter1;
	if ( (!counter1) && (!counter2) ){
	  if (s[pos2-1]==')')
	    break;
	  if ( pos1 && isalpha(s[pos1-1])){
	    --pos1;
	    for (;pos1>=0;--pos1)
	      if (!isalpha(s[pos1]))
		break;
	    ++pos1;
	  }
	  break;
	}
      }
      if (ch==')')
	--counter1;
      if (ch=='['){
	++counter2;
	if ( (!counter1) && (!counter2) ){
	  if (s[pos2-1]==']')
	    break;
	  if ( pos1 && isalpha(s[pos1-1])){
	    --pos1;
	    for (;pos1>=0;--pos1)
	      if (!isalpha(s[pos1]))
		break;
	    ++pos1;
	  }
	  break;
	}
      }
      if (ch==']')
	--counter2;
    }
  }

  void find_right(const string & s,int & pos1,int & pos2){
    int l=s.size();
    pos1=min(max(pos1,0),l);
    pos2=max(min(pos2,l),0);
    int pos2orig=pos2;
    int i=pos1,counter1=0,counter2=0;
    for (int i=pos1;(i<pos2-1) && (i<l);++i){
      char ch=s[i];
      if (ch=='(')
	++counter1;
      if (ch==')')
	--counter1;
      if (ch=='[')
	++counter2;
      if (ch==']')
	--counter2;
      if ( (counter1<0) && (pos1) ){ // restart at an earlier position
	pos1=s.find_last_of('(',pos1-1);
	if (pos1<0)
	  pos1=0;
	i=pos1-1;
	counter1=0;
	counter2=0;
      }
    }      
    for (;pos2<=l;++pos2){
      char ch=s[pos2-1];
      if ( (!counter1) && (!counter2) && ( (ch==')') ||  (ch==']') || (ch=='+') || (ch=='-') || (ch==',')  ) && (pos2>pos2orig)){
	--pos2;
	break;
      }
      if (ch=='(')
	++counter1;
      if (ch==')'){
	--counter1;
	if ( (!counter1) && (!counter2) ){
	  if ( (pos1>0) && (s[pos1]=='(') && isalpha(s[pos1-1])){
	    --pos1;
	    for (;pos1>=0;--pos1)
	      if (!isalpha(s[pos1]))
		break;
	    ++pos1;
	  }
	  break;
	}
      }
      if (ch=='[')
	++counter2;
      if (ch==']'){
	--counter2;
	if ( (!counter1) && (!counter2) ){
	  if ( (pos1>0) && (s[pos1]=='[') && isalpha(s[pos1-1])){
	    --pos1;
	    for (;pos1>=0;--pos1)
	      if (!isalpha(s[pos1]))
		break;
	    ++pos1;
	  }
	  break;
	}
      }
    }
    if (pos2==l+1)
      find_left(s,pos1,pos2);
  }

  void increase_selection(const string & s,int & pos1,int& pos2){
    int l=s.size();
    int pos1_orig(pos1),pos2_orig(pos2);
    // adjust selection (does not change anything on a valid selection)
    find_left(s,pos1,pos2);
    find_right(s,pos1,pos2);
    if ( (pos1!=pos1_orig) || (pos2!=pos2_orig) )
      return;
    if (pos1 && (pos2<l) && ( (s[pos1-1]=='(') || (s[pos1-1]==',')) && (s[pos2]!=')') && (s[pos2]!=',')){
      ++pos2;
      find_right(s,pos1,pos2);
      return;
    }
    if (pos1>1){
      char op=s[pos1-1];
      --pos1;
      for (;pos1;--pos1){
	if (s[pos1]==',')
	  op=0;
	if (!is_operator_char(s[pos1],op))
	  break;
      }
      if (s[pos1]=='(' && pos1){
	--pos1;
	for (;pos1;--pos1){
	  if (!isalpha(s[pos1]))
	    break;
	}
	++pos1;
      }
      find_left(s,pos1,pos2);
      find_right(s,pos1,pos2);
      return;
    }
    pos1=0;
    ++pos2;
    find_right(s,pos1,pos2);
  }

  void decrease_selection(const string & s,int & pos1,int& pos2){
    int l=s.size();
    int pos1_orig(pos1),pos2_orig(pos2);
    // adjust selection (does not change anything on a valid selection)
    find_left(s,pos1,pos2);
    if (pos2!=l)
      --pos2;
    if (!pos2)
      return;
    int counter1=0,counter2=0;
    char op=' ';
    if (pos2<l-1)
      op=s[pos2+1];
    for (;pos2>pos1;--pos2){
      char ch=s[pos2];
      if (ch=='('){
	++counter1;
	if ( (!counter1) && (!counter2) && pos2_orig && (s[pos2_orig-1]==')') ){
	  pos1=pos2+1;
	  pos2=pos2_orig-1;
	  return;
	}
      }
      if (ch==')')
	--counter1;
      if (ch=='[')
	++counter2;
      if (ch==']')
	--counter2;
      if (ch==',')
	op=0;
      if ( (!counter1) && (!counter2) && ( is_operator_char(ch,op) || (ch==',')) )
	return;
    }
    for (;pos1<l;++pos1){
      char ch=s[pos1];
      if ( (ch=='(') ||  (ch=='[') || (ch=='+') || (ch==','))
	break;
    }
    ++pos1;
    pos2=pos1+1;
    find_right(s,pos1,pos2);
  }

  void move_selection_right(const string & s,int & pos1, int & pos2){
    int l=s.size();
    int pos1_orig(pos1),pos2_orig(pos2);
    // adjust selection (does not change anything on a valid selection)
    find_right(s,pos1,pos2);
    pos1=pos2;
    char op=s[pos1];
    for (;pos1<l;++pos1){
      if (s[pos1]==',')
	op=0;
      if (!is_operator_char(s[pos1],op) && (s[pos1]!=')') && (s[pos1]!=']'))
	break;
    }
    pos2=pos1+1;
    find_right(s,pos1,pos2);
  }

  void move_selection_left(const string & s,int & pos1, int & pos2){
    int l=s.size();
    int pos1_orig(pos1),pos2_orig(pos2);
    // adjust selection (does not change anything on a valid selection)
    find_left(s,pos1,pos2);
    pos2=pos1-1;
    char op=s[pos2];
    for (;pos2>0;--pos2){
      if (s[pos2-1]==',')
	op=0;
      if (!is_operator_char(s[pos2-1],op) && (s[pos2-1]!='(') && (s[pos2-1]!='[') )
	break;
    }
    if (pos2<=0){
      pos1=0;
      pos2=0;
      return;
    }
    pos1=pos2-1;
    find_left(s,pos1,pos2);
    find_right(s,pos1,pos2);
  }

  string remove_EXTension(const string & chaine){
    int s=chaine.size();
    int l=chaine.find_last_of('.',s);
    if (l>0)
      return chaine.substr(0,l);
    else
      return chaine;
  }

  string pariprint(const gen & e){
    return e.print();
  }

#ifndef NO_NAMESPACE_GIAC
} // namespace giac
#endif // ndef NO_NAMESPACE_GIAC

