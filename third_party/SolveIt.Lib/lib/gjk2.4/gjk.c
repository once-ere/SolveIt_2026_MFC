#include "gjk.h"

/*
 * Implementation of the Gilbert, Johnson and Keerthi routine to compute
 * the minimum distance between two convex polyhedra.
 *
 * Version 2.4, July 1998, (c) Stephen Cameron 1996, 1997, 1998
 *
 */
/* Scrambled using scramble 1.0 (September 1996) on Fri Jul 10 09:05:36 1998
 */
#define USE_CONST_TABLES
#define TWO_TO_DIM (1<<DIM) 
#define DIM_PLUS_ONE (DIM+1)
#define TWICE_TWO_TO_DIM (TWO_TO_DIM+TWO_TO_DIM)
#define overd( ct) for ( ct=0 ; ct<DIM ; ct++ )
#define card( s) cardinality[s]
#define max_elt( s) max_element[s]
#define elts( s, i) elements[s][i]
#define non_elts( s, i) non_elements[s][i]
#define pred( s, i) predecessor[s][i]
#define succ( s, i) successor[s][i]
#define delta( s, i) delta_values[s][i]
#define prod( i, j) dot_products[i][j]
#ifdef DUMP_CONST_TABLES
#define CONSTRUCT_TABLES
#else
#ifndef USE_CONST_TABLES
#define CONSTRUCT_TABLES
#endif 
#endif 
#ifdef CONSTRUCT_TABLES
static int cardinality[TWICE_TWO_TO_DIM];
static int max_element[TWICE_TWO_TO_DIM];
static int elements[TWICE_TWO_TO_DIM][DIM_PLUS_ONE];
static int non_elements[TWICE_TWO_TO_DIM][DIM_PLUS_ONE];
static int predecessor[TWICE_TWO_TO_DIM][DIM_PLUS_ONE];
static int successor[TWICE_TWO_TO_DIM][DIM_PLUS_ONE];
#else
#define PRE_DEFINED_TABLE_DIM 3
static const int cardinality[16] = {
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
static const int max_element[16] = {
-1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
static const int elements[16][4] = {
{ 0, 0, 0, 0},
{ 0, 0, 0, 0},
{ 1, 0, 0, 0},
{ 0, 1, 0, 0},
{ 2, 0, 0, 0},
{ 0, 2, 0, 0},
{ 1, 2, 0, 0},
{ 0, 1, 2, 0},
{ 3, 0, 0, 0},
{ 0, 3, 0, 0},
{ 1, 3, 0, 0},
{ 0, 1, 3, 0},
{ 2, 3, 0, 0},
{ 0, 2, 3, 0},
{ 1, 2, 3, 0},
{ 0, 1, 2, 3} };
static const int non_elements[16][4] = {
{ 0, 1, 2, 3},
{ 1, 2, 3, 0},
{ 0, 2, 3, 0},
{ 2, 3, 0, 0},
{ 0, 1, 3, 0},
{ 1, 3, 0, 0},
{ 0, 3, 0, 0},
{ 3, 0, 0, 0},
{ 0, 1, 2, 0},
{ 1, 2, 0, 0},
{ 0, 2, 0, 0},
{ 2, 0, 0, 0},
{ 0, 1, 0, 0},
{ 1, 0, 0, 0},
{ 0, 0, 0, 0},
{ 0, 0, 0, 0} };
static const int predecessor[16][4] = {
{ 0, 0, 0, 0},
{ 0, 0, 0, 0},
{ 0, 0, 0, 0},
{ 2, 1, 0, 0},
{ 0, 0, 0, 0},
{ 4, 1, 0, 0},
{ 4, 2, 0, 0},
{ 6, 5, 3, 0},
{ 0, 0, 0, 0},
{ 8, 1, 0, 0},
{ 8, 2, 0, 0},
{ 10, 9, 3, 0},
{ 8, 4, 0, 0},
{ 12, 9, 5, 0},
{ 12, 10, 6, 0},
{ 14, 13, 11, 7} };
static const int successor[16][4] = {
{ 1, 2, 4, 8},
{ 3, 5, 9, 0},
{ 3, 6, 10, 0},
{ 7, 11, 0, 0},
{ 5, 6, 12, 0},
{ 7, 13, 0, 0},
{ 7, 14, 0, 0},
{ 15, 0, 0, 0},
{ 9, 10, 12, 0},
{ 11, 13, 0, 0},
{ 11, 14, 0, 0},
{ 15, 0, 0, 0},
{ 13, 14, 0, 0},
{ 15, 0, 0, 0},
{ 15, 0, 0, 0},
{ 0, 0, 0, 0} };
#endif 
static REAL delta_values[TWICE_TWO_TO_DIM][DIM_PLUS_ONE];
static REAL dot_products[DIM_PLUS_ONE][DIM_PLUS_ONE];
#ifdef CONSTRUCT_TABLES
static void initialise_simplex_distance( void);
#endif
static VertexID support_function( Object obj,
VertexID, REAL *, REAL *);
static VertexID support_simple( Object obj,
VertexID, REAL *, REAL *);
static VertexID support_hill_climbing( Object obj,
VertexID, REAL *, REAL *);
static int default_distance( struct simplex_point * simplex);
static void backup_distance( struct simplex_point * simplex);
static void reset_simplex( int subset, struct simplex_point * simplex);
static void compute_subterms( struct simplex_point * s);
static void compute_point( REAL pt[DIM], int len,
REAL (* vertices)[DIM], REAL lambdas[]);
static void add_simplex_vertex( struct simplex_point * s, int pos,
Object obj1, VertexID v1, Transform t1,
Object obj2, VertexID v2, Transform t2);
REAL gjk_distance(
Object obj1, Transform tr1,
Object obj2, Transform tr2,
REAL wpt1[DIM], REAL wpt2[DIM],
struct simplex_point * simplex, int use_seed
) {
VertexID v, p, maxp, minp;
REAL minus_minv, maxv, sqrd, g_val;
REAL displacementv[DIM], reverse_displacementv[DIM];
REAL local_witness1[DIM], local_witness2[DIM];
REAL local_fdisp[DIM], local_rdisp[DIM], trv[DIM];
REAL * fdisp, * rdisp;
struct simplex_point local_simplex;
int d, compute_both_witnesses, use_default, first_iteration, max_iterations;
double oldsqrd;
assert( NumVertices( obj1)>0 && NumVertices( obj2)>0 );
use_default = first_iteration = 1;
#ifdef CONSTRUCT_TABLES
initialise_simplex_distance();
#else
assert( PRE_DEFINED_TABLE_DIM >= DIM );
#endif 
compute_both_witnesses = ( wpt1!=0 ) || ( wpt2!=0 ) ||
( tr1!=0 ) || ( tr2!=0 );
if ( wpt1==0 )
wpt1 = local_witness1;
if ( wpt2==0 )
wpt2 = local_witness2;
fdisp = IdentityTransform( tr1) ? displacementv : local_fdisp;
rdisp = IdentityTransform( tr2) ? reverse_displacementv : local_rdisp;
if ( simplex==0 ) {
use_seed = 0;
simplex = & local_simplex;
}
if ( use_seed==0 ) {
simplex->simplex1[0] = 0; simplex->simplex2[0] = 0;
simplex->npts = 1; simplex->lambdas[0] = ONE;
simplex->last_best1 = 0; simplex->last_best2 = 0;
add_simplex_vertex( simplex, 0,
obj1, FirstVertex( obj1), tr1,
obj2, FirstVertex( obj2), tr2);
}
else {
for ( v=0 ; v<simplex->npts ; v++ )
add_simplex_vertex( simplex, v,
obj1, simplex->simplex1[v], tr1,
obj2, simplex->simplex2[v], tr2);
}
max_iterations = NumVertices( obj1)*NumVertices( obj2) ;
while ( max_iterations-- > 0 ) {
if ( simplex->npts==1 ) { 
simplex->lambdas[0] = ONE;
}
else { 
compute_subterms( simplex);
if ( use_default ) { 
use_default = default_distance( simplex);
}
if ( !use_default ) {
backup_distance( simplex);
}
}
if ( compute_both_witnesses ) {
compute_point( wpt1, simplex->npts, simplex->coords1,
simplex->lambdas);
compute_point( wpt2, simplex->npts, simplex->coords2,
simplex->lambdas);
overd( d) {
displacementv[ d] = wpt2[d] - wpt1[d];
reverse_displacementv[ d] = - displacementv[d];
}
}
else {
overd( d) {
displacementv[d] = 0;
for ( p=0 ; p<simplex->npts ; p++ )
displacementv[d] +=
DO_MULTIPLY( simplex->lambdas[p],
simplex->coords2[p][d] - simplex->coords1[p][d]);
reverse_displacementv[ d] = - displacementv[d];
}
}
sqrd = OTHER_DOT_PRODUCT( displacementv, displacementv);
if ( sqrd<EPSILON ) {
simplex->error = EPSILON;
return sqrd; 
}
if ( ! IdentityTransform( tr1) )
ApplyInverseRotation( tr1, displacementv, fdisp);
if ( ! IdentityTransform( tr2) )
ApplyInverseRotation( tr2, reverse_displacementv, rdisp);
maxp = support_function(
obj1,
( use_seed ? simplex->last_best1 : InvalidVertexID),
&maxv, fdisp
);
minp = support_function(
obj2,
( use_seed ? simplex->last_best2 : InvalidVertexID),
&minus_minv, rdisp
);
INCREMENT_G_TEST_COUNTER;
g_val = sqrd + maxv + minus_minv;
if ( ! IdentityTransform( tr1) ) {
ExtractTranslation( tr1, trv);
g_val += OTHER_DOT_PRODUCT( displacementv, trv);
}
if ( ! IdentityTransform( tr2) ) {
ExtractTranslation( tr2, trv);
g_val += OTHER_DOT_PRODUCT( reverse_displacementv, trv);
}
if ( g_val < 0.0 ) 
g_val = 0;
if ( g_val < EPSILON ) {
simplex->error = g_val;
return sqrd;
}
if ( (first_iteration || (sqrd < oldsqrd))
&& (simplex->npts <= DIM ) ) {
simplex->simplex1[ simplex->npts] = simplex->last_best1 = maxp;
simplex->simplex2[ simplex->npts] = simplex->last_best2 = minp;
simplex->lambdas[ simplex->npts] = ZERO;
add_simplex_vertex( simplex, simplex->npts,
obj1, maxp, tr1,
obj2, minp, tr2);
simplex->npts++;
oldsqrd = sqrd;
first_iteration = 0;
use_default = 1;
continue;
}
if ( use_default ) {
use_default = 0;
}
else { 
simplex->error = g_val;
return sqrd;
}
} 
return 0.0; 
}
int gjk_extract_point( struct simplex_point *simp,
int whichpoint, REAL vector[]) {
REAL (* coords)[DIM];
assert( whichpoint==1 || whichpoint==2 );
coords = ( whichpoint==1 ) ? simp->coords1 : simp->coords2;
compute_point( vector, simp->npts, coords, simp->lambdas);
return 1;
}
static REAL delta[TWICE_TWO_TO_DIM];
static void compute_subterms( struct simplex_point * simp) {
int i, j, ielt, jelt, s, jsubset, size = simp->npts;
REAL sum, c_space_points[DIM_PLUS_ONE][DIM];
for ( i=0 ; i<size ; i++ )
for ( j=0 ; j<DIM ; j++ )
c_space_points[i][j] =
simp->coords1[i][j] - simp->coords2[i][j];
for ( i=0 ; i<size ; i++ )
for ( j=i ; j<size ; j++ )
prod( i, j) = prod( j, i) =
OTHER_DOT_PRODUCT( c_space_points[i], c_space_points[j]);
for ( s=1 ; s<TWICE_TWO_TO_DIM && max_elt( s) < size ; s++ ) {
if ( card( s)<=1 ) { 
delta( s, elts( s, 0)) = ONE;
continue;
}
if ( card( s)==2 ) { 
delta( s, elts( s, 0)) =
prod( elts( s, 1), elts( s, 1)) -
prod( elts( s, 1), elts( s, 0));
delta( s, elts( s, 1)) =
prod( elts( s, 0), elts( s, 0)) -
prod( elts( s, 0), elts( s, 1));
continue;
}
for ( j=0 ; j<card( s) ; j++ ) {
jelt = elts( s, j);
jsubset = pred( s, j);
sum = 0;
for ( i=0 ; i < card( jsubset) ; i++ ) {
ielt = elts( jsubset, i);
sum += DO_MULTIPLY(
delta( jsubset, ielt ),
prod( ielt, elts( jsubset, 0)) - prod( ielt, jelt)
);
}
delta( s, jelt) = sum;
}
}
return;
}
static int default_distance( struct simplex_point * simplex) {
int s, j, k, ok, size;
size = simplex->npts;
INCREMENT_SIMPLICES_COUNTER;
assert( size>0 && size<=DIM_PLUS_ONE );
for ( s=1 ; s < TWICE_TWO_TO_DIM && max_elt( s) < size ; s++ ) {
delta[s] = ZERO; ok=1;
for ( j=0 ; ok && j<card( s) ; j++ ) {
if ( delta( s, elts( s, j))>ZERO )
delta[s] += delta( s, elts( s, j));
else
ok = 0;
}
for ( k=0 ; ok && k < size - card( s) ; k++ ) {
if ( delta( succ( s, k), non_elts( s, k))>0 )
ok = 0;
}
#ifdef TEST_BACKUP_PROCEDURE
ok = 0;
#endif
if ( ok && delta[s]>=TINY ) 
break;
}
if ( ok ) {
reset_simplex( s, simplex);
return 1;
}
else
return 0;
}
static void backup_distance( struct simplex_point * simplex) {
int s, i, j, k, bests;
int size = simplex->npts;
REAL distsq_num[TWICE_TWO_TO_DIM], distsq_den[TWICE_TWO_TO_DIM];
INCREMENT_BACKUP_COUNTER;
bests = 0;
for ( s=1 ; s < TWICE_TWO_TO_DIM && max_elt( s) < size ; s++ ) {
if ( delta[s] <= ZERO )
continue;
for ( i=0 ; i<card( s) ; i++ )
if ( delta( s, elts( s, i))<=ZERO )
break;
if ( i < card( s) )
continue;
distsq_num[s] = ZERO;
for ( j=0 ; j<card( s) ; j++ )
for ( k=0 ; k<card( s) ; k++ )
distsq_num[s] += DO_MULTIPLY( 
DO_MULTIPLY( delta( s, elts( s, j)), delta( s, elts( s, k))),
prod( elts( s, j), elts( s, k))
);
distsq_den[s] = DO_MULTIPLY( delta[s], delta[s]);
if ( (bests < 1) ||
( DO_MULTIPLY( distsq_num[s], distsq_den[bests]) <
DO_MULTIPLY( distsq_num[bests], distsq_den[s]) )
)
bests = s;
}
reset_simplex( bests, simplex);
return;
}
static void reset_simplex( int subset, struct simplex_point * simplex) {
int i, j, oldpos;
for ( j=0 ; j<card( subset) ; j++ ) {
oldpos = elts( subset, j);
if ( oldpos!=j ) {
simplex->simplex1[j] = simplex->simplex1[oldpos];
simplex->simplex2[j] = simplex->simplex2[oldpos];
overd(i ) {
simplex->coords1[j][i] = simplex->coords1[oldpos][i];
simplex->coords2[j][i] = simplex->coords2[oldpos][i];
}
}
simplex->lambdas[j] =
DO_DIVIDE( delta( subset, elts( subset, j)), delta[subset]);
}
simplex->npts = card( subset);
return;
}
static VertexID
support_function(
Object obj,
VertexID start, REAL * supportval, REAL * direction) {
if ( !ValidRings( obj) ) {
return support_simple( obj, start, supportval, direction);
}
else {
return support_hill_climbing( obj, start, supportval, direction);
}
}
static VertexID
support_simple(
Object obj,
VertexID start, REAL * supportval, REAL * direction) {
VertexID p, maxp;
REAL maxv, thisv;
p = maxp = FirstVertex( obj);
maxv = SupportValue( obj, maxp, direction);
for ( IncrementVertex( obj, p) ;
!LastVertex( obj, p) ;
IncrementVertex( obj, p) ) {
thisv = SupportValue( obj, p, direction);
if ( thisv>maxv ) {
maxv = thisv;
maxp = p;
}
}
*supportval = maxv;
return maxp;
}
static VertexID
support_hill_climbing(
Object obj,
VertexID start, REAL * supportval, REAL * direction) {
VertexID p, maxp, lastvisited, neighbour;
EdgeID index;
REAL maxv, thisv;
p = lastvisited = InvalidVertexID;
maxp = ( !ValidVertex( obj, start) ) ? FirstVertex( obj) : start;
maxv = SupportValue( obj, maxp, direction);
while ( p != maxp ) {
p = maxp;
for ( index = FirstEdge( obj, p) ;
ValidEdge( obj, index) ;
IncrementEdge( obj, index) ) {
neighbour = VertexOfEdge( obj, index);
if ( neighbour==lastvisited )
continue;
thisv = SupportValue( obj, neighbour, direction);
if ( thisv>maxv ) {
maxv = thisv;
maxp = neighbour;
#ifdef EAGER_HILL_CLIMBING
break;
#endif
}
}
lastvisited = p;
}
*supportval = maxv;
return p;
}
static void compute_point( REAL pt[DIM], int len, REAL (* vertices)[DIM],
REAL lambdas[]) {
int d, i;
overd( d) {
pt[d] = 0;
for ( i=0 ; i<len ; i++ )
pt[d] += DO_MULTIPLY( vertices[i][d], lambdas[i]);
}
return;
}
static void add_simplex_vertex( struct simplex_point * s, int pos,
Object obj1, VertexID v1, Transform t1, Object obj2, VertexID v2,
Transform t2) {
ApplyTransform( t1, obj1, v1, s->coords1[pos]);
ApplyTransform( t2, obj2, v2, s->coords2[pos]);
return;
}
#ifdef DUMP_CONST_TABLES
#define dump1d( array) \
printf( "static const int " #array "[%d] = {\n\t%2d", \
TWICE_TWO_TO_DIM, array[0]); \
for ( s=1 ; s<TWICE_TWO_TO_DIM ; s++ ) printf( ", %2d", array[s]); \
printf( "};\n")
#define dump2d( array) \
printf( "static const int " #array "[%d][%d] = {\n", \
TWICE_TWO_TO_DIM, DIM_PLUS_ONE); \
for ( s=0 ; s<TWICE_TWO_TO_DIM ; s++ ) { printf( "\t{ %2d", array[s][0]); \
for ( d=1 ; d<DIM_PLUS_ONE ; d++ ) printf( ", %2d", array[s][d]); \
printf( (s<TWICE_TWO_TO_DIM-1) ? "},\n" : "} };\n"); }
#endif 
#ifdef CONSTRUCT_TABLES
static int simplex_distance_initialised = 0;
static void initialise_simplex_distance( void) {
int power, d, s, e, two_to_e, next_elt, next_non_elt, pr;
int num_succ[TWICE_TWO_TO_DIM];
if ( simplex_distance_initialised )
return;
power = 1;
for ( d=0 ; d<DIM ; d++ )
power *= 2;
assert( power == TWO_TO_DIM );
for ( s=0 ; s<TWICE_TWO_TO_DIM ; s++ )
num_succ[s] = 0;
for ( s=1 ; s<TWICE_TWO_TO_DIM ; s++ ) {
two_to_e = 1;
next_elt = next_non_elt = 0;
for ( e=0 ; e<DIM_PLUS_ONE ; e++ ) {
if ( (s/two_to_e) % 2 == 1 ) {
elts( s, next_elt) = e;
pr = s - two_to_e;
pred( s, next_elt) = pr;
succ( pr, num_succ[pr]) = s;
num_succ[ pr]++;
next_elt++;
}
else
non_elts( s, next_non_elt++) = e;
two_to_e *= 2;
}
card( s) = next_elt;
max_elt( s) = elts( s, next_elt-1 );
}
card( 0) = 0; max_elt( 0) = -1;
for ( e=0 ; e<DIM_PLUS_ONE ; e++ )
non_elts( 0, e) = e;
simplex_distance_initialised = 1;
#ifdef DUMP_CONST_TABLES
printf( "\n\n");
printf( "#define PRE_DEFINED_TABLE_DIM %d\n", DIM);
dump1d( cardinality);
dump1d( max_element);
dump2d( elements);
dump2d( non_elements);
dump2d( predecessor);
dump2d( successor);
printf( "\n\n");
exit( 0);
#endif 
return;
}
#endif 

