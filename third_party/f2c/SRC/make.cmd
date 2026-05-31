cl -c  -D__STDC__ -DWIN32 *.c
del memset.obj
del xsum.obj
del malloc.obj
link /out:f2c.exe *.obj kernel32.lib