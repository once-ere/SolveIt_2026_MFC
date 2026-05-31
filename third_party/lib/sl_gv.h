
/* sl_gv.h --- header file to define the global variables for sledge.c */

int      g_print,             /*  control printing                     */
         g_level,             /*  number of mesh bisections            */
         g_maxext,            /*  maximum number of extrap. allowed    */
         g_maxint,            /*  maximum number of mesh intervals     */
         g_maxlvl,            /*  maximum number of levels allowed     */
         g_nextrp,            /*  actual number of extrapolations      */
         g_nsgnf,             /*  used in shoot function               */
         g_nxinit,            /*  number of initial mesh points        */
         kclass[2];           /*  mesh type from classify_end          */

long int g_ncoeff;            /*  total number of coeff. evaluations   */

double   *x,                  /*  vector of mesh points                */
         *xef,                /*  mesh points of eigenfunction u       */
         **ef,                /*  u(x) estimates                       */
         **pdef,              /*  (pu')(x) estimates                   */
         g_a,                 /*  left endpoint of the interval        */
         g_b,                 /*  right endpoint of the interval       */
         g_a1, g_a1p, g_a2, g_a2p, g_b1, g_b2,
                              /*  boundary condition constants         */
         g_u,                 /*  see remarks in function start        */
         g_under,             /*  see remarks in function start        */

/*       variables defined in classify, classify_end, and mesh         */

         g_cp[2],g_cr[2],g_ep[2],g_er[2],g_cutoff,g_d[2][4],g_emu[2],
         g_eqlnf[2],g_eta[2][2],g_pnu[2],

         g_hmin;

logical ctype[2][4],          /*  classification "type"                */
        g_afin,               /*  TRUE iff left of interval is finite  */
        g_bfin,               /*  TRUE iff right of interval is finite */
        g_countz,             /*  TRUE to count zeros in shoot         */
        g_lflag[6],           /*  flag to indicate different warnings  */
        g_lnf,                /*  TRUE if the problem is in LNF        */
        g_reg[2],             /*  TRUE if endpoint is regular          */
        g_lc[2],              /*  TRUE if endpoint is Limit Circle     */
        g_osc[2];             /*  TRUE if endpoint is Oscillatory      */

FILE    *output_file;         /*  output file pointer                  */
