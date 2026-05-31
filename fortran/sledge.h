extern int sledge_(logical *job, doublereal *cons, logical *endfin, integer *invec, doublereal *tol, logical *type__, doublereal *ev, integer *numx, doublereal *xef, doublereal *ef, doublereal *pdef, doublereal *t, doublereal *rho, integer *iflag, doublereal *store);
extern int interv_(logical *first, doublereal *alpha, doublereal *beta, doublereal *cons, logical *endfin, integer *nfirst, integer *ntotal, integer *iflag, doublereal *x);
extern int aitken_(doublereal *xlim, doublereal *tol, integer *n, doublereal *x, doublereal *error);
extern doublereal asymev_(integer *nev, doublereal *qint, doublereal *rpint, doublereal *alpha1, doublereal *alpha2, doublereal *beta1, doublereal *beta2);
extern doublereal asymr_(integer *nev, doublereal *rpint, doublereal *rpata, doublereal *rpatb, doublereal *scale);
extern int brcket_(integer *n, doublereal *evlow, doublereal *evhigh, doublereal *flow, doublereal *fhigh, doublereal *abserr, doublereal *relerr, doublereal *x);
extern int class_(integer *iprint, doublereal *tol, logical *job, logical *cspec, doublereal *cev, integer *lastev, logical *lplc, doublereal *x, logical *jmesh, doublereal *hmin, logical *domesh);
extern int clsend_(doublereal *z__, doublereal *pz, doublereal *qz, doublereal *rz, integer *kmax, integer *iprint, doublereal *end, logical *endfin, integer *iend, doublereal *tol, doublereal *cev, logical *cspec, doublereal *bc, doublereal *y, logical *lplc, integer *iflag);
extern int densef_(doublereal *tol, logical *cspec, integer *iprint, integer *iter, integer *nextrp, integer *numt, doublereal *t, doublereal *rho, integer *nev, doublereal *hmin, integer *numev, doublereal *store);
extern int dscrip_(logical *lc, logical *lplc, logical *type__, logical *reg, logical *cspec, doublereal *cev, doublereal *cutoff, integer *lastev, doublereal *a1, doublereal *a1p, doublereal *a2, doublereal *a2p, doublereal *b1, doublereal *b2);
extern int extrap_(doublereal *v, doublereal *tol, integer *irow, integer *maxcol, logical *full, logical *tight, integer *mode, doublereal *vsave, integer *iprint, doublereal *error, logical *done);
extern int getef_(doublereal *ev, doublereal *efnorm, integer *iprint, doublereal *x, logical *efin);
extern int getrn_(doublereal *ev, doublereal *alpha, logical *cspec, doublereal *denom, doublereal *rsubn, doublereal *x);
extern int mesh_(logical *job, integer *nev, doublereal *x, doublereal *g, doublereal *h__, doublereal *qlnf, doublereal *z__, doublereal *tol, doublereal *hmin);
extern int power_(doublereal *x, doublereal *f, integer *n, doublereal *tol, integer *iprint, doublereal *ef, doublereal *cf, logical *osc, logical *exact, doublereal *y, integer *iflag);
extern int pqrint_(doublereal *x, doublereal *sqrtrp, doublereal *qlnf);
extern int regulr_(logical *job, logical *jobmsh, doublereal *tol, integer *nev, doublereal *ev, integer *iprint, integer *nextrp, doublereal *xef, doublereal *ef, doublereal *pdef, doublereal *hmin, doublereal *store);
extern int shoot_(doublereal *ev, doublereal *x, integer *mu, doublereal *fev);
extern int start_(logical *job, doublereal *cons, doublereal *tol, integer *nev, integer *indxev, integer *n, doublereal *xef, integer *numt, doublereal *t, integer *nextrp, doublereal *x);
extern int step_(doublereal *x, doublereal *h__, doublereal *ev, doublereal *px, doublereal *rx, doublereal *tau, doublereal *omega, doublereal *homega, doublereal *psi, doublereal *dpsi, doublereal *sclog, integer *mode);
extern int zzero_(doublereal *b, doublereal *c__, doublereal *fb, doublereal *fc, doublereal *abserr, doublereal *relerr, integer *iflag, doublereal *x);
/* comlen slclss_ 224 */
/* comlen slint_ 32 */
/* comlen sllog_ 64 */
/* comlen slreal_ 80 */
/*:ref: coeff_ 14 4 7 7 7 7 */
