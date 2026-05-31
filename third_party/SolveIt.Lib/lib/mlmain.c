/*

**	Main for Mathlink

*/



#include <mathlink.h>



#if !WINDOWS_MATHLINK



int main(argc, argv)

		  int argc; char* argv[];

{

		  return MLMain(argc, argv);

}



#else



int PASCAL WinMain( HANDLE hinstCurrent, HANDLE hinstPrevious, LPSTR lpszCmdLine, int nCmdShow)

{

	char  buff[512];

	char FAR * argv[32];

	int argc;



	if( !MLInitializeIcon( hinstCurrent, nCmdShow)) return 1;

	argc = MLStringToArgv( lpszCmdLine, buff, argv, 32);

	return MLMain( argc, argv);

}

#endif
