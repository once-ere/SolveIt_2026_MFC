/* A Bison parser, made from SolveIt.y
   by GNU bison 1.50.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	T_NULL	257
# define	T_IF	258
# define	T_WHILE	259
# define	T_FOR	260
# define	T_THEN	261
# define	T_ELSE	262
# define	T_RETURN	263
# define	T_READ	264
# define	T_WRITE	265
# define	T_PLOT	266
# define	T_DOT	267
# define	T_Q_STRING	268
# define	T_VAR	269
# define	T_LOCAL	270
# define	T_ARG	271
# define	T_FUNCTION	272
# define	T_NUMBER	273
# define	T_RNUMBER	274
# define	T_BLTIN_VOID	275
# define	T_BLTIN_L	276
# define	T_BLTIN_D	277
# define	T_BLTIN_DD	278
# define	T_BLTIN_DDD	279
# define	T_BLTIN_DL	280
# define	T_BLTIN_LD	281
# define	T_DEFINE_USER_FUNCTION	282
# define	SolveIt_First_System_Token	283
# define	T_NAME	284
# define	T_SPHERE	285
# define	T_BLOCK	286
# define	T_CYLINDER	287
# define	T_CONE	288
# define	T_VECTOR_ANALYSIS_OBJECT	289
# define	T_MASS	290
# define	T_IMASS	291
# define	T_CHARGE	292
# define	T_DENSITY	293
# define	T_MagneticDipoleMoment	294
# define	T_MagneticDipoleOrientation	295
# define	T_INVERSE_INERTIA_TENSOR	296
# define	T_RADIUS	297
# define	T_WIDTH	298
# define	T_HEIGHT	299
# define	T_DEPTH	300
# define	T_COLOR	301
# define	T_DRAWSTYLE	302
# define	T_SCALE	303
# define	T_DELETE	304
# define	T_POSITION	305
# define	T_MOMENTUM	306
# define	T_ANGULARMOMENTUM	307
# define	T_ORIENTATION	308
# define	T_VELOCITY	309
# define	T_DISPLACEMENT	310
# define	T_ACCELERATION	311
# define	T_ANGULARVELOCITY	312
# define	T_ENERGY	313
# define	T_KINETIC_ENERGY	314
# define	T_POTENTIAL_ENERGY	315
# define	T_POTENTIAL	316
# define	T_FORCE	317
# define	T_TORQUE	318
# define	T_CONSTRAINT_STATE	319
# define	T_contactFORCE	320
# define	T_contactTORQUE	321
# define	T_BODY_FORCE_EXTERNAL	322
# define	T_BODY_FORCE_INTERNAL	323
# define	T_COEFFICIENT_OF_STATIC_FRICTION	324
# define	T_COEFFICIENT_OF_KINETIC_FRICTION	325
# define	T_COEFFICIENT_OF_RESTITUTION	326
# define	T_DAMPING_COEFFICIENT	327
# define	T_CONTACTS	328
# define	T_DISTANCE	329
# define	T_SPEED	330
# define	T_PERIOD	331
# define	T_TRAJECTORY	332
# define	T_RECORD	333
# define	T_ANALYZE	334
# define	T_PULLEY	335
# define	T_CONNECT	336
# define	T_quadrant	337
# define	T_PLANE	338
# define	T_POINT_IN_PLANE	339
# define	T_NORMAL	340
# define	T_ANGLE	341
# define	T_LocalVectorToLab	342
# define	T_LabVectorToLocal	343
# define	T_LabPointToLocal	344
# define	T_LocalPointToLab	345
# define	T_Constraint	346
# define	T_Linear	347
# define	T_Quadratic	348
# define	T_MultiBody_Link	349
# define	T_MultiBody_Link_Joint	350
# define	T_MultiBody_SPRING_CONSTANT	351
# define	T_MultiBody_SPRING_LENGTH	352
# define	T_MultiBody_CONTROL	353
# define	T_SYSTEM_MultiBody	354
# define	T_JointType_FIXED	355
# define	T_JointType_REVOLUTE	356
# define	T_JointType_PRISMATIC	357
# define	T_JointType_SCREW	358
# define	T_JointType_CYLINDRICAL	359
# define	T_JointType_UNIVERSAL	360
# define	T_JointType_SPHERICAL	361
# define	T_JointType_FLOATING	362
# define	T_JointType_SPRING	363
# define	T_SYSTEM	364
# define	T_SYSTEM_LEFT	365
# define	T_SYSTEM_RIGHT	366
# define	T_SYSTEM_TOP	367
# define	T_SYSTEM_BOTTOM	368
# define	T_SYSTEM_NEAR	369
# define	T_SYSTEM_FAR	370
# define	T_TITLE	371
# define	T_TICMARK	372
# define	T_NODES	373
# define	T_GRAVITATIONAL_COUPLING	374
# define	T_GRAVITATIONAL_FIELD	375
# define	T_ELECTRIC_FIELD	376
# define	T_MAGNETIC_FIELD	377
# define	T_CENTRAL_FORCE	378
# define	T_LENNARD_JONES	379
# define	T_CURRENTLOOP_MAGNETICFIELD_STRENGTH	380
# define	T_SYSTEM_TIME_STEP	381
# define	T_SYSTEM_TIME	382
# define	T_SYSTEM_TIMER_INTERVAL	383
# define	T_COLLISIONDETECTION	384
# define	T_BOUNDING_BOX	385
# define	T_ENFORCE_RIGID_BOUNDARY	386
# define	T_ONRESET	387
# define	T_DEFERRED_COMMAND	388
# define	T_RUN	389
# define	T_STOP	390
# define	T_CLEAR	391
# define	T_TRANSLATE	392
# define	T_ROTATE	393
# define	T_TRUE	394
# define	T_FALSE	395
# define	T_OFF	396
# define	T__ON	397
# define	T_ONE	398
# define	T_TWO	399
# define	T_THREE	400
# define	T_FOUR	401
# define	T_DIMENSION	402
# define	T_SHOW	403
# define	T_SYSTEM_Background	404
# define	T_SYSTEM_VectorFields	405
# define	T_SYSTEM_FieldLines	406
# define	T_SYSTEM_Equipotentials	407
# define	T_STATE	408
# define	T_Stationary	409
# define	T_Scattering	410
# define	T_Superposition	411
# define	T_WaveNumber	412
# define	T_EigenNumber	413
# define	T_Method	414
# define	T_Renormalize	415
# define	T_Incident	416
# define	T_FourierTransform	417
# define	T_Nash_Chen	418
# define	T_PeriodicCayley	419
# define	T_Cayley	420
# define	T_Number	421
# define	T_Frames	422
# define	T_DOMAIN	423
# define	T_RADIAL	424
# define	T_pseudoCartesian	425
# define	T_Cartesian	426
# define	T_INFINITY	427
# define	T_MINUS_INFINITY	428
# define	T_XCOMP	429
# define	T_YCOMP	430
# define	T_ZCOMP	431
# define	T_WCOMP	432
# define	T_SCENE	433
# define	T_MAP	434
# define	T_FILE	435
# define	T_DATA	436
# define	T_BEGIN	437
# define	T_END	438
# define	T_RESET	439
# define	SolveIt_Last_System_Token	440
# define	T_ASSGNOP	441
# define	T_OR	442
# define	T_AND	443
# define	T_GT	444
# define	T_GE	445
# define	T_LT	446
# define	T_LE	447
# define	T_EQ	448
# define	T_NEQ	449
# define	T_ADD	450
# define	T_SUBTR	451
# define	T_MULT	452
# define	T_DIV	453
# define	T_MOD	454
# define	T_UNARYMINUS	455
# define	T_NOT	456
# define	T_CARET	457


/*************************************************************************
Operator precedence is determined by the line ordering of the
declarations; the higher the line number of the declaration (lower on
the page or screen), the higher the precedence.  Hence, exponentiation
***************************************************************************/
#include "stdafx.h"

#include "SolveIt.h"
#include "MainFrm.h"
#include "System.h"
#include "SymbolTable.h"

#include "objects.h"
#include "StackMachine.h"
#include "symbol.h"
#include "instruction.h"

#define	YYSTYPE objects
#define YY_NO_GET_LVAL 1
#include "scanner.h"

#include "parser.h" /* for token definitions and yylval */
#define YYPARSE_PARAM scanner
#define YYLEX_PARAM   scanner
int yylex ( YYSTYPE * lvalp, yyscan_t scanner );

// must be a reentrant parser:
#define	YYPURE 1
// sm is called inside sm  for 'plot' 

#define	YYDEBUG 1				/* For Debugging*/
#define	YYERROR_VERBOSE 1		/* For Debugging*/
// Last error message returned by parser
static std::string parser_error;


#define gen_code SolveIt::gen_code
#define gen_call SolveIt::gen_call

//static const objects null_ object(T_NULL);

#ifndef YYSTYPE
# define YYSTYPE int
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		407
#define	YYFLAG		-32768
#define	YYNTBASE	213

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 457 ? yytranslate[x] : 245)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     205,   206,     2,     2,   209,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   204,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   211,     2,   212,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   207,   210,   208,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     3,     5,     8,    11,    14,    17,    20,
      22,    26,    30,    34,    38,    40,    43,    50,    61,    68,
      78,    82,    85,    87,    89,    91,    93,    94,    95,    96,
      99,   103,   105,   107,   109,   111,   113,   115,   117,   119,
     121,   123,   125,   127,   129,   131,   133,   135,   137,   139,
     141,   143,   145,   147,   149,   151,   153,   157,   162,   167,
     174,   183,   190,   197,   203,   207,   211,   215,   219,   223,
     227,   231,   234,   238,   242,   246,   250,   254,   258,   262,
     266,   269,   273,   274,   275,   286,   287,   289,   293,   300,
     308,   318,   330,   344,   346,   348,   350,   352,   354,   356,
     358,   360,   362,   364,   366,   368,   370,   372,   374,   376,
     378,   380,   382,   384,   386,   388,   390,   392,   394,   396,
     398,   400,   402,   404,   406,   408,   410,   412,   414,   416,
     418,   420,   422,   424,   426,   428,   430,   432,   434,   436,
     438,   440,   442,   444,   446,   448,   450,   452,   454,   456,
     458,   460,   462,   464,   466,   468,   470,   472,   474,   476,
     478,   480,   482,   484,   486,   488,   490,   492,   494,   496,
     498,   500,   502,   504,   506,   508,   510,   512,   514,   516,
     518,   520,   522,   524,   526,   528,   530,   532,   534,   536,
     538,   540,   542,   544,   546,   548,   550,   552,   554,   556,
     558,   560,   562,   564,   566,   568,   570,   572,   574,   576,
     578,   580,   582,   584,   586,   588,   590,   592,   594,   596,
     598,   600,   602,   604,   606,   608,   610,   612,   614,   616,
     618,   620,   622,   624,   626,   629,   633,   637,   641,   643,
     645,   655,   667,   681,   697,   707,   721,   733
};
static const short yyrhs[] =
{
      -1,   214,     0,   226,     0,   217,   204,     0,   225,   204,
       0,   216,   204,     0,   230,   204,     0,   242,   204,     0,
       1,     0,    15,   187,   225,     0,    17,   187,   225,     0,
      15,   187,   236,     0,   241,   187,   225,     0,   225,     0,
       9,   225,     0,   219,   205,   218,   206,   217,   223,     0,
     220,   205,   218,   204,   218,   204,   218,   206,   217,   223,
       0,   221,   205,   218,   206,   217,   223,     0,   221,   205,
     218,   206,   217,   223,     8,   217,   223,     0,   207,   224,
     208,     0,    11,   225,     0,   225,     0,     5,     0,     6,
       0,     4,     0,     0,     0,     0,   224,   204,     0,   224,
     217,   204,     0,    19,     0,    20,     0,    15,     0,    17,
       0,    14,     0,   141,     0,   140,     0,   142,     0,   143,
       0,   173,     0,   174,     0,   164,     0,   165,     0,   166,
       0,   170,     0,   171,     0,   172,     0,   231,     0,   232,
       0,   233,     0,   234,     0,   215,     0,   241,     0,   240,
       0,   237,     0,    21,   205,   206,     0,    22,   205,   225,
     206,     0,    23,   205,   225,   206,     0,    24,   205,   225,
     209,   225,   206,     0,    25,   205,   225,   209,   225,   209,
     225,   206,     0,    26,   205,   225,   209,   225,   206,     0,
      27,   205,   225,   209,   225,   206,     0,    18,   222,   205,
     229,   206,     0,   205,   225,   206,     0,   225,   196,   225,
       0,   225,   197,   225,     0,   225,   198,   225,     0,   225,
     199,   225,     0,   225,   200,   225,     0,   225,   203,   225,
       0,   197,   225,     0,   225,   190,   225,     0,   225,   191,
     225,     0,   225,   192,   225,     0,   225,   193,   225,     0,
     225,   194,   225,     0,   225,   195,   225,     0,   225,   189,
     225,     0,   225,   188,   225,     0,   202,   225,     0,   210,
     225,   210,     0,     0,     0,    28,    15,   227,   205,   229,
     206,   228,   207,   224,   208,     0,     0,   225,     0,   229,
     209,   225,     0,    12,   205,   225,   209,   225,   206,     0,
     211,   225,   209,   225,   209,   225,   212,     0,   211,   225,
     209,   225,   209,   225,   209,   225,   212,     0,   211,   225,
     209,   225,   209,   225,   209,   225,   209,   225,   212,     0,
     211,   225,   209,   225,   209,   225,   209,   225,   209,   225,
     209,   225,   212,     0,    31,     0,    32,     0,    33,     0,
      34,     0,    35,     0,   235,     0,   101,     0,   102,     0,
     103,     0,   104,     0,   105,     0,   106,     0,   107,     0,
     108,     0,   109,     0,   175,     0,   176,     0,   177,     0,
     178,     0,    30,     0,    37,     0,    42,     0,    38,     0,
      51,     0,    52,     0,    53,     0,    54,     0,    63,     0,
      64,     0,    66,     0,    67,     0,    68,     0,    69,     0,
      55,     0,    58,     0,    57,     0,    36,     0,    39,     0,
      40,     0,    41,     0,    47,     0,    48,     0,    49,     0,
      50,     0,    43,     0,    44,     0,    45,     0,    46,     0,
      70,     0,    71,     0,    72,     0,    73,     0,    59,     0,
      60,     0,    61,     0,    62,     0,    74,     0,    75,     0,
      76,     0,    79,     0,    78,     0,    80,     0,    77,     0,
     149,     0,    65,     0,    82,     0,    81,     0,    83,     0,
     144,     0,   145,     0,   146,     0,   147,     0,    84,     0,
      85,     0,    86,     0,    87,     0,    88,     0,    89,     0,
      90,     0,    91,     0,   148,     0,    96,     0,    56,     0,
      97,     0,    98,     0,    99,     0,   100,     0,   111,     0,
     112,     0,   113,     0,   114,     0,   115,     0,   116,     0,
     117,     0,   118,     0,   119,     0,   120,     0,   121,     0,
     122,     0,   123,     0,   124,     0,   125,     0,   126,     0,
     127,     0,   128,     0,   129,     0,   130,     0,   132,     0,
     133,     0,   134,     0,   131,     0,   138,     0,   139,     0,
     150,     0,   151,     0,   152,     0,   153,     0,   154,     0,
     155,     0,   156,     0,   157,     0,   158,     0,   159,     0,
     169,     0,   160,     0,   161,     0,   162,     0,   163,     0,
     167,     0,   168,     0,   183,     0,   184,     0,   135,     0,
     136,     0,   137,     0,   179,     0,   180,     0,   181,     0,
     182,     0,   185,     0,   239,     0,   240,   238,     0,   240,
      13,   239,     0,    15,    13,   240,     0,   110,    13,   240,
       0,   244,     0,   243,     0,    15,    13,    92,    13,   225,
     194,   225,   209,   225,     0,    15,    13,    92,    13,   225,
     194,   225,   209,   225,   209,   225,     0,    15,    13,    92,
      13,   225,   194,    15,    13,    92,    13,   225,   209,   225,
       0,    15,    13,    92,    13,   225,   194,    15,    13,    92,
      13,   225,   209,   225,   209,   225,     0,    15,    13,    92,
      13,    93,   194,   225,   209,   225,     0,    15,    13,    92,
      13,    94,   194,   225,   209,   225,   209,   225,   209,   225,
       0,    15,    13,    95,    13,   225,   194,   225,   209,   225,
     209,   225,     0,    15,    13,    95,    13,   225,   194,    15,
      13,    95,    13,   225,   209,   225,   209,   225,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   250,   255,   261,   265,   269,   274,   275,   276,   277,
     282,   287,   292,   295,   305,   306,   310,   313,   318,   322,
     326,   327,   330,   333,   335,   337,   340,   344,   349,   352,
     353,   360,   361,   362,   363,   364,   366,   367,   369,   370,
     372,   373,   376,   377,   378,   379,   380,   381,   383,   384,
     385,   386,   387,   388,   393,   398,   400,   401,   402,   403,
     404,   405,   406,   408,   410,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   431,   431,   431,   452,   453,   454,   458,   462,
     465,   468,   471,   475,   476,   477,   478,   479,   482,   486,
     488,   489,   490,   491,   492,   493,   494,   495,   501,   502,
     503,   504,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   519,   520,   521,   522,   523,   524,
     525,   527,   528,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   559,   560,   561,   562,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   613,   614,   615,   616,   617,   619,   620,   625,
     631,   632,   633,   635,   636,   638,   639,   640,   641,   642,
     643,   644,   645,   649,   655,   661,   670,   679,   690,   692,
     698,   704,   709,   715,   721,   726,   739,   745
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "T_NULL", "T_IF", "T_WHILE", "T_FOR", 
  "T_THEN", "T_ELSE", "T_RETURN", "T_READ", "T_WRITE", "T_PLOT", "T_DOT", 
  "T_Q_STRING", "T_VAR", "T_LOCAL", "T_ARG", "T_FUNCTION", "T_NUMBER", 
  "T_RNUMBER", "T_BLTIN_VOID", "T_BLTIN_L", "T_BLTIN_D", "T_BLTIN_DD", 
  "T_BLTIN_DDD", "T_BLTIN_DL", "T_BLTIN_LD", "T_DEFINE_USER_FUNCTION", 
  "SolveIt_First_System_Token", "T_NAME", "T_SPHERE", "T_BLOCK", 
  "T_CYLINDER", "T_CONE", "T_VECTOR_ANALYSIS_OBJECT", "T_MASS", "T_IMASS", 
  "T_CHARGE", "T_DENSITY", "T_MagneticDipoleMoment", 
  "T_MagneticDipoleOrientation", "T_INVERSE_INERTIA_TENSOR", "T_RADIUS", 
  "T_WIDTH", "T_HEIGHT", "T_DEPTH", "T_COLOR", "T_DRAWSTYLE", "T_SCALE", 
  "T_DELETE", "T_POSITION", "T_MOMENTUM", "T_ANGULARMOMENTUM", 
  "T_ORIENTATION", "T_VELOCITY", "T_DISPLACEMENT", "T_ACCELERATION", 
  "T_ANGULARVELOCITY", "T_ENERGY", "T_KINETIC_ENERGY", 
  "T_POTENTIAL_ENERGY", "T_POTENTIAL", "T_FORCE", "T_TORQUE", 
  "T_CONSTRAINT_STATE", "T_contactFORCE", "T_contactTORQUE", 
  "T_BODY_FORCE_EXTERNAL", "T_BODY_FORCE_INTERNAL", 
  "T_COEFFICIENT_OF_STATIC_FRICTION", "T_COEFFICIENT_OF_KINETIC_FRICTION", 
  "T_COEFFICIENT_OF_RESTITUTION", "T_DAMPING_COEFFICIENT", "T_CONTACTS", 
  "T_DISTANCE", "T_SPEED", "T_PERIOD", "T_TRAJECTORY", "T_RECORD", 
  "T_ANALYZE", "T_PULLEY", "T_CONNECT", "T_quadrant", "T_PLANE", 
  "T_POINT_IN_PLANE", "T_NORMAL", "T_ANGLE", "T_LocalVectorToLab", 
  "T_LabVectorToLocal", "T_LabPointToLocal", "T_LocalPointToLab", 
  "T_Constraint", "T_Linear", "T_Quadratic", "T_MultiBody_Link", 
  "T_MultiBody_Link_Joint", "T_MultiBody_SPRING_CONSTANT", 
  "T_MultiBody_SPRING_LENGTH", "T_MultiBody_CONTROL", 
  "T_SYSTEM_MultiBody", "T_JointType_FIXED", "T_JointType_REVOLUTE", 
  "T_JointType_PRISMATIC", "T_JointType_SCREW", "T_JointType_CYLINDRICAL", 
  "T_JointType_UNIVERSAL", "T_JointType_SPHERICAL", 
  "T_JointType_FLOATING", "T_JointType_SPRING", "T_SYSTEM", 
  "T_SYSTEM_LEFT", "T_SYSTEM_RIGHT", "T_SYSTEM_TOP", "T_SYSTEM_BOTTOM", 
  "T_SYSTEM_NEAR", "T_SYSTEM_FAR", "T_TITLE", "T_TICMARK", "T_NODES", 
  "T_GRAVITATIONAL_COUPLING", "T_GRAVITATIONAL_FIELD", "T_ELECTRIC_FIELD", 
  "T_MAGNETIC_FIELD", "T_CENTRAL_FORCE", "T_LENNARD_JONES", 
  "T_CURRENTLOOP_MAGNETICFIELD_STRENGTH", "T_SYSTEM_TIME_STEP", 
  "T_SYSTEM_TIME", "T_SYSTEM_TIMER_INTERVAL", "T_COLLISIONDETECTION", 
  "T_BOUNDING_BOX", "T_ENFORCE_RIGID_BOUNDARY", "T_ONRESET", 
  "T_DEFERRED_COMMAND", "T_RUN", "T_STOP", "T_CLEAR", "T_TRANSLATE", 
  "T_ROTATE", "T_TRUE", "T_FALSE", "T_OFF", "T__ON", "T_ONE", "T_TWO", 
  "T_THREE", "T_FOUR", "T_DIMENSION", "T_SHOW", "T_SYSTEM_Background", 
  "T_SYSTEM_VectorFields", "T_SYSTEM_FieldLines", 
  "T_SYSTEM_Equipotentials", "T_STATE", "T_Stationary", "T_Scattering", 
  "T_Superposition", "T_WaveNumber", "T_EigenNumber", "T_Method", 
  "T_Renormalize", "T_Incident", "T_FourierTransform", "T_Nash_Chen", 
  "T_PeriodicCayley", "T_Cayley", "T_Number", "T_Frames", "T_DOMAIN", 
  "T_RADIAL", "T_pseudoCartesian", "T_Cartesian", "T_INFINITY", 
  "T_MINUS_INFINITY", "T_XCOMP", "T_YCOMP", "T_ZCOMP", "T_WCOMP", 
  "T_SCENE", "T_MAP", "T_FILE", "T_DATA", "T_BEGIN", "T_END", "T_RESET", 
  "SolveIt_Last_System_Token", "T_ASSGNOP", "T_OR", "T_AND", "T_GT", 
  "T_GE", "T_LT", "T_LE", "T_EQ", "T_NEQ", "T_ADD", "T_SUBTR", "T_MULT", 
  "T_DIV", "T_MOD", "T_UNARYMINUS", "T_NOT", "T_CARET", "';'", "'('", 
  "')'", "'{'", "'}'", "','", "'|'", "'['", "']'", "input", "list", 
  "asgn", "sysAsgn", "stmt", "cond", "while", "for", "if", "begin", "end", 
  "stmtlist", "expr", "functiondef", "@1", "@2", "arglist", "plot", 
  "Vector3D", "Vector4D", "Vector5D", "Vector6D", "bodyTypes", "bodyType", 
  "Joint_Type", "component", "basic_property", "property", 
  "system_property", "individual_commands", "rigid_constraint", 
  "rigid_articulated", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   213,   213,   214,   214,   214,   214,   214,   214,   214,
     215,   215,   215,   216,   217,   217,   217,   217,   217,   217,
     217,   217,   218,   219,   220,   221,   222,   223,   224,   224,
     224,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   227,   228,   226,   229,   229,   229,   230,   231,
     232,   233,   234,   235,   235,   235,   235,   235,   236,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   238,   238,
     238,   238,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   240,   240,   240,   241,   241,   242,   242,
     243,   243,   243,   243,   243,   243,   244,   244
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     1,     1,     2,     2,     2,     2,     2,     1,
       3,     3,     3,     3,     1,     2,     6,    10,     6,     9,
       3,     2,     1,     1,     1,     1,     0,     0,     0,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     4,     6,
       8,     6,     6,     5,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     0,     0,    10,     0,     1,     3,     6,     7,
       9,    11,    13,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     3,     3,     1,     1,
       9,    11,    13,    15,     9,    13,    11,    15
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     9,    25,    23,    24,     0,     0,     0,    35,    33,
      34,    26,    31,    32,     0,     0,     0,     0,     0,     0,
       0,     0,   112,   129,   113,   115,   130,   131,   132,   114,
     137,   138,   139,   140,   133,   134,   135,   136,   116,   117,
     118,   119,   126,   175,   128,   127,   145,   146,   147,   148,
     120,   121,   157,   122,   123,   124,   125,   141,   142,   143,
     144,   149,   150,   151,   155,   153,   152,   154,   159,   158,
     160,   165,   166,   167,   168,   169,   170,   171,   172,   174,
     176,   177,   178,   179,    99,   100,   101,   102,   103,   104,
     105,   106,   107,     0,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   203,   200,   201,   202,   225,   226,
     227,   204,   205,    37,    36,    38,    39,   161,   162,   163,
     164,   173,   156,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   217,   218,   219,   220,    42,    43,    44,
     221,   222,   216,    45,    46,    47,    40,    41,   228,   229,
     230,   231,   223,   224,   232,     0,     0,     0,    28,     0,
       0,     2,    52,     0,     0,     0,     0,     0,     0,     3,
       0,    48,    49,    50,    51,    55,   233,    54,    53,     0,
     239,   238,    33,    15,    53,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
      71,    80,     0,     0,     0,     0,     6,     4,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     7,     0,   108,   109,
     110,   111,   234,     0,     8,     0,     0,     0,     0,   236,
      93,    94,    95,    96,    97,    10,    98,    12,    11,    85,
      56,     0,     0,     0,     0,     0,     0,     0,   237,    64,
      29,    20,     0,    14,    81,     0,     0,    22,     0,     0,
      79,    78,    72,    73,    74,    75,    76,    77,    65,    66,
      67,    68,    69,    70,   235,    13,     0,     0,     0,    86,
       0,    57,    58,     0,     0,     0,     0,    85,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,    27,     0,    27,    88,
       0,     0,     0,     0,    87,    59,     0,    61,    62,    83,
       0,    16,     0,    18,     0,     0,    33,    76,    33,    76,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    28,     0,     0,    27,   244,     0,     0,
     240,     0,     0,     0,     0,    90,    27,    19,     0,     0,
       0,     0,     0,    84,     0,    17,     0,     0,   241,     0,
     246,     0,    91,     0,     0,     0,     0,   245,   242,     0,
      92,     0,     0,   243,   247,     0,     0,     0
};

static const short yydefgoto[] =
{
     405,   171,   172,   173,   272,   276,   175,   176,   177,   200,
     341,   213,   273,   179,   267,   351,   300,   180,   181,   182,
     183,   184,   256,   257,   185,   242,   186,   187,   194,   189,
     190,   191
};

static const short yypact[] =
{
     399,-32768,-32768,-32768,-32768,  1617,  1617,  -192,-32768,   -12,
    -173,-32768,-32768,-32768,  -182,  -178,  -163,  -162,  -160,  -154,
    -150,    11,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    41,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1617,  1617,  1617,-32768,  1617,
    1617,-32768,-32768,  -147,  -146,  -145,  -144,  -143,  3181,-32768,
    -141,-32768,-32768,-32768,-32768,-32768,-32768,    -4,  -128,  -126,
  -32768,-32768,   -11,  3198,-32768,  3198,  1617,  2189,  1221,  1617,
    -118,  -127,  1617,  1617,  1617,  1617,  1617,  1617,-32768,  2657,
    -113,  -113,  3029,   607,    57,    90,-32768,-32768,  1617,  1617,
    1617,  1617,  1617,  1617,  1617,  1617,  1617,  1617,  1617,  1617,
    1617,  1617,  1617,  1617,  1617,-32768,-32768,  2657,-32768,-32768,
  -32768,-32768,-32768,  1617,-32768,  2657,  2655,    78,    79,    -4,
  -32768,-32768,-32768,-32768,-32768,  3198,-32768,-32768,  3198,  1617,
  -32768,  3048,  3067,  2677,  2699,  2721,  2743,  -112,    -4,-32768,
  -32768,-32768,  -109,  3198,-32768,  1617,  -110,  3198,  -107,  -108,
    3245,    -8,  -179,  -179,  -179,  -179,  -179,  -179,  -151,  -151,
    -113,  -113,  -113,  -113,-32768,  3198,  1617,  1419,  1617,  3198,
    -194,-32768,-32768,  1617,  1617,  1617,  1617,  1617,-32768,  2765,
    1023,  1617,  1023,  3086,   -95,   -94,  3214,  3230,-32768,  1617,
    3105,  2787,  3124,  3143,  -184,  1617,-32768,  -103,-32768,-32768,
    1617,  1617,  1815,  2013,  3198,-32768,  1617,-32768,-32768,-32768,
    -159,-32768,  1617,   108,  2809,  2831,    -9,  -115,    -6,   -77,
    3162,   -90,  1617,-32768,   -81,  1023,  1617,  1617,  2345,  1617,
    2501,  1617,-32768,-32768,  -123,  1023,-32768,  3198,  2853,   115,
    2875,   116,  2897,   815,  1617,-32768,-32768,-32768,  1617,  1617,
    1617,  1617,  1617,-32768,   -85,-32768,  2919,  2941,  3198,  2963,
    3198,  1617,-32768,  1617,  1617,  1617,   -54,  3198,  2985,  3007,
  -32768,  1617,  1617,  3198,  3198,   130,   131,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,     3,  -209,-32768,-32768,-32768,-32768,
    -320,  -216,     0,-32768,-32768,-32768,  -157,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   -89,  -181,   151,-32768,
  -32768,-32768
};


#define	YYLAST		3448


static const short yytable[] =
{
     178,   197,   245,   174,   358,   193,   195,   360,   343,   237,
     278,   279,   318,   196,   199,   319,   249,   229,   230,   231,
     232,   233,   339,   201,   234,   319,   208,   202,   268,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   203,   204,   234,   205,   377,   231,   232,   233,
     352,   206,   234,   353,   209,   207,   385,   216,   217,   243,
     218,   219,   220,   236,   249,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   244,   260,
     234,   229,   230,   231,   232,   233,   374,   259,   234,   375,
     234,   297,   298,   307,   359,   308,   310,   311,   312,   330,
     331,   342,   327,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   355,   363,   234,   229,
     230,   231,   232,   233,   391,   365,   234,   392,   379,   381,
     406,   407,   361,   354,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   373,   294,   234,
     324,   188,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,   210,   211,   212,     0,   214,
     215,   238,   239,   240,   241,   198,   198,   249,   198,   249,
       0,   198,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,     0,     0,   234,   246,     0,   255,   258,
       0,     0,   261,   262,   263,   264,   265,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   277,
     277,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,     0,     0,     0,     0,     0,
       0,     0,     0,   295,     0,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,     0,   299,
     234,     0,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,     0,     0,   234,     0,     0,   313,   316,   317,   275,
       0,     0,     0,   320,   321,   322,   323,   299,     0,     0,
       0,   277,     0,   326,     0,   328,     0,     0,     0,   334,
       0,     0,     0,     0,     0,   340,     0,     0,     0,     0,
     344,   345,   347,   349,     0,     0,   350,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   364,     0,     0,     0,   367,   368,   366,   370,
       0,   372,     0,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,   384,     0,     0,     0,   386,   387,
     388,   389,   390,     0,     0,     0,     0,     0,     0,     0,
       0,   396,     0,   397,   398,   399,     0,     0,     0,    -1,
       1,   403,   404,     2,     3,     4,     0,     0,     5,     0,
       6,     7,     0,     8,     9,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,    22,
       0,     0,     0,     0,     0,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,     0,     0,     0,     0,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,     0,     0,     0,     0,   158,   159,
     160,   161,   162,   163,   164,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   165,     0,     0,     0,
       0,   166,     0,     0,   167,     0,   168,     0,     0,   169,
     170,     2,     3,     4,     0,     0,     5,     0,     6,     0,
       0,     8,   192,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    22,     0,     0,
       0,     0,     0,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
       0,     0,     0,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,     0,     0,     0,     0,   158,   159,   160,   161,
     162,   163,   164,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     0,     0,     0,     0,   166,
       0,   270,   167,     0,   168,   271,     0,   169,   170,     2,
       3,     4,     0,     0,     5,     0,     6,     0,     0,     8,
     192,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,     0,     0,    22,     0,     0,     0,     0,
       0,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,     0,     0,     0,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
       0,     0,     0,     0,   158,   159,   160,   161,   162,   163,
     164,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,   166,     0,   270,
     167,     0,   168,   383,     0,   169,   170,     2,     3,     4,
       0,     0,     5,     0,     6,     0,     0,     8,   192,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,     0,     0,    22,     0,     0,     0,     0,     0,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,     0,     0,     0,     0,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,     0,     0,
       0,     0,   158,   159,   160,   161,   162,   163,   164,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,     0,   166,     0,     0,   167,     0,
     168,     0,     0,   169,   170,     8,   192,     0,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,     0,
       0,    22,   250,   251,   252,   253,   254,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,     0,     0,     0,     0,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,     0,     0,     0,     0,
     158,   159,   160,   161,   162,   163,   164,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   165,     0,
       0,     0,     0,   166,     0,     0,   167,     0,     0,     0,
       0,   169,   170,     8,   192,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,     0,    22,
       0,     0,     0,     0,     0,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,     0,   314,   315,     0,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,     0,     0,     0,     0,   158,   159,
     160,   161,   162,   163,   164,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   165,     0,     0,     0,
       0,   166,     0,     0,   167,     0,     0,     0,     0,   169,
     170,     8,   192,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,     0,    22,     0,     0,
       0,     0,     0,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
       0,     0,     0,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,     0,     0,     0,     0,   158,   159,   160,   161,
     162,   163,   164,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     0,     0,     0,     0,   166,
       0,     0,   167,     0,     0,     0,     0,   169,   170,     8,
     346,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,     0,     0,    22,     0,     0,     0,     0,
       0,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,     0,     0,     0,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
       0,     0,     0,     0,   158,   159,   160,   161,   162,   163,
     164,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,   166,     0,     0,
     167,     0,     0,     0,     0,   169,   170,     8,   348,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,     0,     0,    22,     0,     0,     0,     0,     0,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,     0,     0,     0,     0,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,     0,     0,
       0,     0,   158,   159,   160,   161,   162,   163,   164,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,     0,   166,     0,     0,   167,    22,
       0,     0,     0,   169,   170,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,   247,     0,     0,   248,    79,    80,    81,    82,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,     0,
       0,     0,     0,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,     0,     0,     0,   150,   151,   152,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   158,   159,
     160,   161,   162,   163,   164,    22,     0,     0,     0,     0,
       0,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,   369,     0,     0,
       0,    79,    80,    81,    82,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,     0,     0,     0,     0,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,     0,
       0,     0,   150,   151,   152,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,   159,   160,   161,   162,   163,
     164,    22,     0,     0,     0,     0,     0,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,     0,     0,     0,   371,    79,    80,    81,
      82,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,     0,     0,     0,     0,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,     0,     0,     0,   150,   151,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   159,   160,   161,   162,   163,   164,    22,     0,     0,
       0,     0,     0,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,     0,
       0,     0,     0,    79,    80,    81,    82,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,     0,     0,     0,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,     0,     0,     0,   150,   151,   152,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   158,   159,   160,   161,
     162,   163,   164,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,   234,     0,
       0,     0,     0,     0,   296,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,     0,     0,
     234,     0,     0,     0,     0,     0,   303,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
       0,     0,   234,     0,     0,     0,     0,     0,   304,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,     0,     0,   234,     0,     0,     0,     0,     0,
     305,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,     0,     0,   234,     0,     0,     0,
       0,     0,   306,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,   234,     0,
       0,     0,     0,     0,   325,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,     0,     0,
     234,     0,     0,     0,     0,     0,   336,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
       0,     0,   234,     0,     0,     0,     0,     0,   356,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,     0,     0,   234,     0,     0,     0,     0,     0,
     357,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,     0,     0,   234,     0,     0,     0,
       0,     0,   378,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,   234,     0,
       0,     0,     0,     0,   380,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,     0,     0,
     234,     0,     0,     0,     0,     0,   382,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
       0,     0,   234,     0,     0,     0,     0,     0,   393,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,     0,     0,   234,     0,     0,     0,     0,     0,
     394,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,     0,     0,   234,     0,     0,     0,
       0,     0,   395,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,   234,     0,
       0,     0,     0,     0,   401,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,     0,     0,
     234,     0,     0,     0,     0,     0,   402,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
       0,     0,   234,     0,     0,   269,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,     0,
       0,   234,     0,     0,   301,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,     0,     0,
     234,     0,     0,   302,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,     0,     0,   234,
       0,     0,   329,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,   234,     0,
       0,   335,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,     0,     0,   234,     0,     0,
     337,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,     0,     0,   234,     0,     0,   338,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,     0,     0,   234,     0,     0,   362,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,     0,     0,   234,   235,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,     0,
       0,   234,   221,   222,   223,   224,   225,   226,   332,   228,
     229,   230,   231,   232,   233,     0,     0,   234,   221,   222,
     223,   224,   225,   226,   333,   228,   229,   230,   231,   232,
     233,     0,     0,   234,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,     0,     0,   234
};

static const short yycheck[] =
{
       0,    13,    13,     0,    13,     5,     6,    13,   328,    13,
     219,   220,   206,   205,   187,   209,   197,   196,   197,   198,
     199,   200,   206,   205,   203,   209,    15,   205,   209,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   205,   205,   203,   205,   366,   198,   199,   200,
     209,   205,   203,   212,    13,   205,   376,   204,   204,   187,
     205,   205,   205,   204,   245,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   204,   206,
     203,   196,   197,   198,   199,   200,   209,   205,   203,   212,
     203,    13,    13,   205,   209,   204,   206,   204,   206,   194,
     194,   204,   311,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,     8,   207,   203,   196,
     197,   198,   199,   200,   209,   206,   203,   212,    13,    13,
       0,     0,   209,   342,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   363,   237,   203,
     307,     0,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,
      -1,    -1,    -1,    -1,    -1,   165,   166,   167,    -1,   169,
     170,   175,   176,   177,   178,   187,   187,   358,   187,   360,
      -1,   187,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,   203,   196,    -1,   198,   199,
      -1,    -1,   202,   203,   204,   205,   206,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   243,    -1,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,   259,
     203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   275,    -1,    -1,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,   203,    -1,    -1,   296,   297,   298,   209,
      -1,    -1,    -1,   303,   304,   305,   306,   307,    -1,    -1,
      -1,   311,    -1,   310,    -1,   312,    -1,    -1,    -1,   319,
      -1,    -1,    -1,    -1,    -1,   325,    -1,    -1,    -1,    -1,
     330,   331,   332,   333,    -1,    -1,   336,    -1,    -1,    -1,
      -1,    -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   352,    -1,    -1,    -1,   356,   357,   355,   359,
      -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,   365,    -1,
      -1,    -1,    -1,    -1,   374,    -1,    -1,    -1,   378,   379,
     380,   381,   382,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   391,    -1,   393,   394,   395,    -1,    -1,    -1,     0,
       1,   401,   402,     4,     5,     6,    -1,    -1,     9,    -1,
      11,    12,    -1,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,    -1,    -1,    -1,    -1,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,   202,    -1,    -1,   205,    -1,   207,    -1,    -1,   210,
     211,     4,     5,     6,    -1,    -1,     9,    -1,    11,    -1,
      -1,    14,    15,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,    -1,    -1,    -1,    -1,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,   202,
      -1,   204,   205,    -1,   207,   208,    -1,   210,   211,     4,
       5,     6,    -1,    -1,     9,    -1,    11,    -1,    -1,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
      -1,    -1,    -1,    -1,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,    -1,    -1,    -1,    -1,   202,    -1,   204,
     205,    -1,   207,   208,    -1,   210,   211,     4,     5,     6,
      -1,    -1,     9,    -1,    11,    -1,    -1,    14,    15,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,    -1,    -1,
      -1,    -1,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,   202,    -1,    -1,   205,    -1,
     207,    -1,    -1,   210,   211,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,    -1,    -1,    -1,    -1,
     179,   180,   181,   182,   183,   184,   185,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,
      -1,    -1,    -1,   202,    -1,    -1,   205,    -1,    -1,    -1,
      -1,   210,   211,    14,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    -1,    93,    94,    -1,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,    -1,    -1,    -1,    -1,   179,   180,
     181,   182,   183,   184,   185,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,   202,    -1,    -1,   205,    -1,    -1,    -1,    -1,   210,
     211,    14,    15,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,    -1,    -1,    -1,    -1,   179,   180,   181,   182,
     183,   184,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,   202,
      -1,    -1,   205,    -1,    -1,    -1,    -1,   210,   211,    14,
      15,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
      -1,    -1,    -1,    -1,   179,   180,   181,   182,   183,   184,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   197,    -1,    -1,    -1,    -1,   202,    -1,    -1,
     205,    -1,    -1,    -1,    -1,   210,   211,    14,    15,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,    -1,    -1,
      -1,    -1,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,   202,    -1,    -1,   205,    30,
      -1,    -1,    -1,   210,   211,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,   180,
     181,   182,   183,   184,   185,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,   180,   181,   182,   183,   184,
     185,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,   180,   181,   182,   183,   184,   185,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,    -1,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,   180,   181,   182,
     183,   184,   185,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,   209,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,   209,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,   209,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
     209,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,   209,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,   209,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,   209,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,   209,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
     209,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,   209,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,   209,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,   209,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,   209,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,   203,    -1,    -1,    -1,    -1,    -1,
     209,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,   209,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,   203,    -1,
      -1,    -1,    -1,    -1,   209,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
     203,    -1,    -1,    -1,    -1,    -1,   209,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
      -1,    -1,   203,    -1,    -1,   206,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,   203,    -1,    -1,   206,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,    -1,    -1,
     203,    -1,    -1,   206,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,   203,
      -1,    -1,   206,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,   203,    -1,
      -1,   206,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,    -1,   203,    -1,    -1,
     206,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,    -1,    -1,   203,    -1,    -1,   206,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,    -1,    -1,   203,    -1,    -1,   206,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,   203,   204,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    -1,
      -1,   203,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,    -1,    -1,   203,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,   203,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,   203
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 1:
{
//terminate the last fetch_execute_cycle(progbase) called for the trailing 'garbage' bytes
				gen_code( instruction::HALT, 0 );
				YYACCEPT;
			;
    break;}
case 2:
{
				gen_code( instruction::HALT, 0 );//terminate OnCompile.fetch_execute_cycle(progbase);
				YYACCEPT;
			;
    break;}
case 3:
{
						gen_code( instruction::HALT, 0 );//terminate OnCompile.fetch_execute_cycle(progbase);
					//	YYACCEPT;
					;
    break;}
case 4:
{
						gen_code( instruction::HALT, 0 );
					//	YYACCEPT;
					;
    break;}
case 5:
{
						gen_code( instruction::WRITE_Object, 0 );
						gen_code( instruction::HALT, 0 );
					//	YYACCEPT;
					;
    break;}
case 6:
{ gen_code( instruction::HALT, 0 ); ;
    break;}
case 7:
{ gen_code( instruction::HALT, 0 ); YYACCEPT;;
    break;}
case 8:
{ gen_code( instruction::HALT, 0 ); ;
    break;}
case 9:
{
						yyclearin; yyerrok;
					;
    break;}
case 10:
{
							gen_code( instruction::STORE, yyvsp[-2] );	//	gen_code( instruction::LD_VAR,	$1 );
// 'cond' should not just return code to 'push' 'var' , but rather to calc var then push
							yyval = yyvsp[0];// 'cond' must return code to assign new val to var
						;
    break;}
case 11:
{
							gen_code( instruction::STORE_ARG, yyvsp[-2] );	//	gen_code( instruction::LD_ARG,	$1 );
// 'cond' should not just return code to 'push' 'var' , but rather to calc var then push
							yyval = yyvsp[0];// 'cond' must return code to assign new val to var
						;
    break;}
case 12:
{ yyval = gen_code( instruction::Create_Body,	yyvsp[-2] ); ;
    break;}
case 13:
{
							gen_code( instruction::I_PUSH, yyvsp[-2] );//for debugging; remove later
							gen_code( instruction::SYSTEM_SET, 0 );
						;
    break;}
case 14:
{ gen_code( instruction::XPOP, 0 ); ;
    break;}
case 15:
{
					yyval = yyvsp[0];
					gen_code( instruction::DO_RETURN, 0 );
				;
    break;}
case 16:
{		/* while */
		(yyvsp[-5]).iPtr[1].obj = yyvsp[-1];		/* setjmp [no OP]; body of loop */
		(yyvsp[-5]).iPtr[2].obj = yyvsp[0]; ;
    break;}
case 17:
{		/* for */
		(yyvsp[-9]).iPtr[1].obj = yyvsp[-5];		/* setjmp [no OP]; condition */
		(yyvsp[-9]).iPtr[2].obj = yyvsp[-3];		/* setjmp [no OP]; post loop */
		(yyvsp[-9]).iPtr[3].obj = yyvsp[-1];		/* setjmp [no OP]; body of loop */
		(yyvsp[-9]).iPtr[4].obj = yyvsp[0]; ;
    break;}
case 18:
{	/* else-less if */
		(yyvsp[-5]).iPtr[1].obj = yyvsp[-1];		/* thenpart */
		(yyvsp[-5]).iPtr[2].obj = -1;		/* no elsepart */
		(yyvsp[-5]).iPtr[3].obj = yyvsp[0]; ;
    break;}
case 19:
{	/* if with else */
		(yyvsp[-8]).iPtr[1].obj = yyvsp[-4];		/* thenpart */
		(yyvsp[-8]).iPtr[2].obj = yyvsp[-1];		/* elsepart */
		(yyvsp[-8]).iPtr[3].obj = yyvsp[0]; ;
    break;}
case 20:
{ yyval = yyvsp[-1]; ;
    break;}
case 21:
{ yyval = yyvsp[0]; gen_code( instruction::WRITE_Object, 0 );;
    break;}
case 22:
{ gen_code( instruction::HALT, 0 ); ;
    break;}
case 23:
{ yyval = Reserve(3, instruction::I_WHILE); ;
    break;}
case 24:
{ yyval = Reserve(5, instruction::I_FOR); ;
    break;}
case 25:
{ yyval = Reserve(4, instruction::I_IF); ;
    break;}
case 26:
{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												yyval = stackMachine->next_free_spot_for_code_generation; ;
    break;}
case 27:
{ gen_code( instruction::HALT, 0 );
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												yyval = stackMachine->next_free_spot_for_code_generation; ;
    break;}
case 28:
{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												yyval = stackMachine->next_free_spot_for_code_generation; ;
    break;}
case 31:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 32:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 33:
{ yyval = gen_code( instruction::LD_VAR,	yyvsp[0] ); ;
    break;}
case 34:
{ yyval = gen_code( instruction::LD_ARG,	yyvsp[0] ); ;
    break;}
case 35:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 36:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 37:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 38:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 39:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 40:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 41:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 42:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 43:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 44:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 45:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 46:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 47:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 53:
{
							gen_code( instruction::I_PUSH, yyvsp[0] );//for debugging; remove later
							yyval = gen_code( instruction::SYSTEM_GET, 0 );
						;
    break;}
case 54:
{
							gen_code( instruction::I_PUSH, yyvsp[0] );//for debugging; remove later
							yyval = gen_code( instruction::SYSTEM_GET, 0 );
						;
    break;}
case 55:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0] ); ;
    break;}
case 56:
{ yyval=yyvsp[0]; gen_code(instruction::CALL_VOID,	yyvsp[-2] );/*->fn*/;
    break;}
case 57:
{ yyval=yyvsp[-1]; gen_code(instruction::CALL_L,		yyvsp[-3] );/*->fnX*/;
    break;}
case 58:
{ yyval=yyvsp[-1]; gen_code(instruction::CALL_D,		yyvsp[-3] );/*->fnX*/;
    break;}
case 59:
{ yyval=yyvsp[-3]; gen_code(instruction::CALL_DD,		yyvsp[-5] );/*->fnXY*/;
    break;}
case 60:
{ yyval=yyvsp[-5]; gen_code(instruction::CALL_DDD,	yyvsp[-7] );/*->fnXYZ*/;
    break;}
case 61:
{ yyval=yyvsp[-3]; gen_code(instruction::CALL_DL,		yyvsp[-5] );/*->fnXL*/;
    break;}
case 62:
{ yyval=yyvsp[-3]; gen_code(instruction::CALL_LD,		yyvsp[-5] );/*->fnLX*/;
    break;}
case 63:
{ yyval = yyvsp[-3]; gen_call( yyvsp[-4].sym , yyvsp[-1]);	;
    break;}
case 64:
{ yyval = yyvsp[-1]; ;
    break;}
case 65:
{ gen_code( instruction::ADD,	0 );	;
    break;}
case 66:
{ gen_code( instruction::SUB,	0 );	;
    break;}
case 67:
{ gen_code( instruction::MULT,	0 );	;
    break;}
case 68:
{ gen_code( instruction::DIV,	0 );	;
    break;}
case 69:
{ gen_code( instruction::MOD,	0 );	;
    break;}
case 70:
{ gen_code( instruction::PWR,	0 );		;
    break;}
case 71:
{ yyval=yyvsp[0]; gen_code( instruction::NEG,	0 );	;
    break;}
case 72:
{ gen_code( instruction::I_GT,	0 );	;
    break;}
case 73:
{ gen_code( instruction::I_GE,	0 );	;
    break;}
case 74:
{ gen_code( instruction::I_LT,	0 );	;
    break;}
case 75:
{ gen_code( instruction::I_LE,	0 );	;
    break;}
case 76:
{ gen_code( instruction::I_EQ,	0 );	;
    break;}
case 77:
{ gen_code( instruction::I_NEQ,	0 );	;
    break;}
case 78:
{ gen_code( instruction::I_AND,	0 );	;
    break;}
case 79:
{ gen_code( instruction::I_OR,	0 );	;
    break;}
case 80:
{ gen_code( instruction::I_NOT,	0 );	;
    break;}
case 81:
{ yyval=yyvsp[-1]; gen_code( instruction::I_ABS,	0 );	;
    break;}
case 82:
{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												stackMachine->indefInArgList=stackMachine->indef=1;
												stackMachine->nargs	= 0;
												CSymbolTable* symbolTable	= theApp.GetSymbolTable();
												symbolTable->ConvertToFunc( yyvsp[0].sym );
											;
    break;}
case 83:
{
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												stackMachine->nargs	= stackMachine->stack_offset-1;
												stackMachine->indefInArgList= 0;
											;
    break;}
case 84:
{
												CSymbolTable* symbolTable	= theApp.GetSymbolTable();
												symbolTable->CodeFunc( yyvsp[-8].sym );
												CStackMachine* stackMachine	= theApp.GetStackMachine();
												stackMachine->indef=0;
											;
    break;}
case 85:
{ yyval = 0;;
    break;}
case 86:
{ yyval = (long)1;;
    break;}
case 87:
{ yyval = yyvsp[-2].iVal + (long)1;;
    break;}
case 88:
{ gen_code( instruction::LD_PLOT, 0 );/*plot("func", "options [x,-1,1],...,[xLabel,an_abscissa],[color,0,255,127]");*/ ;
    break;}
case 89:
{ yyval = gen_code( instruction::LD_VECTOR3,	0 ); ;
    break;}
case 90:
{ yyval = gen_code( instruction::LD_VECTOR4,	0 ); ;
    break;}
case 91:
{ yyval = gen_code( instruction::LD_VECTOR5,	0 ); ;
    break;}
case 92:
{ yyval = gen_code( instruction::LD_VECTOR6,	0 ); ;
    break;}
case 98:
{ yyval = gen_code( instruction::I_PUSH,	yyvsp[0].iVal ); ;
    break;}
case 233:
{//objects(buf)
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << yyvsp[0].iVal << ends;
									yyval = buf;
								;
    break;}
case 234:
{//"XXX.x" etc
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << *yyvsp[-1].sPtr << ',' << yyvsp[0].iVal << ends;
									yyval = buf;
								;
    break;}
case 235:
{
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << *yyvsp[-2].sPtr << ',' << yyvsp[0].iVal << ends;
									yyval = buf;
								;
    break;}
case 236:
{
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << yyvsp[-2].sym->name << ':' << *yyvsp[0].sPtr << ends;
									yyval = buf;

							gen_code( instruction::I_PUSH, yyvsp[0] );
							gen_code( instruction::LD_VAR, yyvsp[-2] );
						;
    break;}
case 237:
{
									static char buf[256];
									ostrstream oss(buf, sizeof(buf)/sizeof(buf[0]));
									oss << yyvsp[-2].sym->name << ':' << *yyvsp[0].sPtr << ends;
									yyval = buf;

							gen_code( instruction::I_PUSH, yyvsp[0] );
							gen_code( instruction::LD_VAR, yyvsp[-2] );
						;
    break;}
case 240:
{//body1 connected at expr1 to point=expr2; separation=expr3
							gen_code( instruction::LD_VAR, yyvsp[-8] );
							gen_code( instruction::I_Constraint, objects(int(0)) );
		;
    break;}
case 241:
{//body1 connected at expr1 to point=expr2; separation=expr3; options=expr4
							gen_code( instruction::LD_VAR, yyvsp[-10] );
							gen_code( instruction::I_Constraint, objects(int(2)) );
		;
    break;}
case 242:
{//body1 connected at point1=expr1 to body2 point2=expr2; separation=expr3
							gen_code( instruction::LD_VAR, yyvsp[-6] );
							gen_code( instruction::LD_VAR, yyvsp[-12] );
							gen_code( instruction::I_Constraint, objects(int(1)) );
		;
    break;}
case 243:
{//body1 connected at point1=expr1 to body2 point2=expr2; separation=expr3; options=expr4
							gen_code( instruction::LD_VAR, yyvsp[-8] );
							gen_code( instruction::LD_VAR, yyvsp[-14] );
							gen_code( instruction::I_Constraint, objects(int(3)) );
		;
    break;}
case 244:
{//body in plane: expr1*x + expr2==0;  positive normal==expr1 [eq [-1,0,0]]
							gen_code( instruction::LD_VAR, yyvsp[-8] );
							gen_code( instruction::I_Constraint, objects(int(4)) );
		;
    break;}
case 245:
{//body in quadratic: expr1*[x.x^2, x.y^2, x.z^2] + expr2*x + expr3 ==0;
		//positive normal==  gradient expr4=true
		//positive normal== -gradient expr4=false
							gen_code( instruction::LD_VAR, yyvsp[-12] );
							gen_code( instruction::I_Constraint, objects(int(5)) );
		;
    break;}
case 246:
{
							gen_code( instruction::LD_VAR, yyvsp[-10] );
							gen_code( instruction::I_CONNECT, objects(int(0)) );
		;
    break;}
case 247:
{
							gen_code( instruction::LD_VAR, yyvsp[-8] );
							gen_code( instruction::LD_VAR, yyvsp[-14] );
							gen_code( instruction::I_CONNECT, objects(int(1)) );
		;
    break;}
}



  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}

/*
///////////////////////////////////////////////////////////////////////////////
	|	T_MultiBody_Link T_DOT T_MultiBody_SPRING_CONSTANT	{ $$=T_MultiBody_SPRING_CONSTANT; }
	|	T_MultiBody_Link T_DOT T_MultiBody_SPRING_LENGTH	{ $$=T_MultiBody_SPRING_LENGTH; }
	|	T_MultiBody_Link T_DOT T_DISPLACEMENT				{ $$=T_DISPLACEMENT; }
	|	T_MultiBody_Link T_DOT T_VELOCITY					{ $$=T_VELOCITY; }
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
component:XCOMP {$$ = XCOMP - SolveIt_First_System_Token;}
;

basic_property:NAME {$$ = NAME - SolveIt_First_System_Token;}
|POSITION {$$ = POSITION - SolveIt_First_System_Token;}
;

///////////////////////////////////////////////////////////////////////////////
this fails:
plot:	PLOT '(' Q_STRING ',' expr ',' expr ',' NUMBER ')' 	{
								gen_code( instruction::LD_PLOT, 0 );
							}
	;
///////////////////////////////////////////////////////////////////////////////
x=sphere;
y=block;
x.link.[0,0,0] == [0,0,0],prismatic,[1,0,0];
y.link.[0,0,0] == x.link.[0,0,0.5],revolute,[0,0,1];
///////////////////////////////////////////////////////////////////////////////
function Victor(x$) { y=(x)/10.;return cos(2*PI*y);}
potential.name = "Victor";
function v(x$) { y=(x)/10.;return cos(2*PI*y^2);}
plot("v", "  [x-Label,'%f*dog gy'], [x,-PI, 2*PI/.3], [y ,  -1.2, 1.3]");
plot("v", " [x,-PI, 2*PI/.3], [y ,  -1.2, 1.3],[x-Label,'%f*dog gy']");
plot("v", " [x,-PI, 2*PI/.3], [y ,  -1.2, 1.3] ");
plot("v", -PI, 2*PI , 150);
potential.name = "v";

function Q(r$)
{
L0=0.00573645;
R=L0*r;
a          = 2;
d          = 0.2;
return 4*a*R/((a+R)^2+d^2);
}
function V(r$)
{
if (r == 0.0) return (-0.00556);
L0=0.00573645;
R=L0*r;
delta      = 0.0802;
Coulomb    = -0.013;
B0         = 20;
B1         = -0.05;
a          = 2;
d          = 0.2;
OmegaB0    = 0.0575567;
q          = Q(r);
p          = sqrt(q);
Ec         = EllipticEc(q);
K          = EllipticK(q);
F1         = (4*(a + R)*a/((a + R)^2 + d^2))* sqrt(a/(R*q)) * ( -Ec + (1 - q/2)*K );
DeE        = (Ec-K)/(2*q);
DeK        = (Ec-(1-q)*K)/(2*(1-q)*q);
F2         = ((a^2 - R^2 + d^2)*a*p^3/(R^2*sqrt(R*a))) *( -DeE - 0.5*K + (1 - 0.5*q)*DeK);
return ((B1/B0)*( OmegaB0 *( 0.5 + delta ) + Coulomb )*( F1 + F2 )  / OmegaB0 );
}
plot("V", " [x,0, 1000], [y ,  -.012, .008] ");
potential.name = "V";

x=[5,0,0];
x*x;
x^x;
x-3 *x;
y=[5,-3,0];
x*y;
x^y;
x-3 *y;
[0,0,1]*[5,0,0];
[0,0,1]^[5,0,0];

function foo(x$) {
	return x;
}
foo(3.);


function foo(x$, y$) {
	if(y == 0) 	 return x;
	return x/y;
}
foo(3.,4);
foo(foo(1.,2), foo(3.,4));

function ack(x$, y$) {
	if(x == 0) return (y+1);
	if(y == 0) 	 return (ack(x - 1, 1));
	return (ack(x - 1, ack(x, y - 1)));
}
ack(0, 3);
ack(3, 3);


function Ack(x$, y$) {
	if(x == 0) return (y+1);
	return x/y;
}
Ack(0, 3);

	
n=0;
function ack(x$, y$) {
	n = n+1;
	if(x == 0) return (y+1);
	if(y == 0) 	 return (ack(x - 1, 1));
	return (ack(x - 1, ack(x, y - 1)));
}
ack(3, 3);
n;

function foo(x$) { y=(x)/100.;return cos(PI*y);}foo(9.9);

function fo(x$) { y=foo(x)/100.;return cos(PI*y);}fo(9.9);
j=2;while (j<5) { write(j);j=j+1;};
for (j=2;j<5;j=j+1) { write(j);};
if (2>10) { write(9*1.7);};
if (2>1) { write(-PI);x=9*(1.7/0.9);write(x);};
if (2>10) { x=(9*1.7);} else { x=-PI*1.5;};
if (2>10) { write(9*1.7);} else { write(-PI*1.5);};

system.boundingBox = [-0.1, 2.0, 1.9, 4.0, -4.5, 4.5];
system.rotate=true;
system.translate = [ 0.0 ,  0.0 ,  -0.45 ];
system.rotate=true;
system.boundingBox = [-0.1, 2.0, 1.9, 4.0, 2.5, 10];
system.left.right.bottom;
x=sphere;
x.position;
x.position.x;
x.position=[1.56,-.5,.3];
x.position.x/x.position.x;
x.position.w;
x.width;
m1=sphere;
function fx(x$) {return -x;}
m1.externalForce.x = "fx";
m1.position=[1,0,0];
plot("fx", "  [x-Label,'%f*dog gy'], [x,-1.2, 1.3], [y ,  1.2, -1.3]");

*/

////////////////////////////////////////////////////////////////////////

/*=========================================================================
								 YYERROR
=========================================================================*/
using namespace SolveIt;
void yyerror ( char *s )	/* Called by yyparse on error */
{
	CStackMachine* stackMachine	= theApp.GetStackMachine();
	stackMachine->errors++;
	if (stackMachine->possyyFlexLexer == NULL) return;
	stackMachine->possyyFlexLexer << s << endl;
}
/**************************** End Grammar File ***************************/
