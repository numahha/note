del *.obj
del *.exe
call C:\PROGRA~2\MICROS~1.0\VC\vcvarsall

cl sample_main.cpp /I include lib\ReleaseDLL\drawstuff.lib lib\ReleaseDLL\ode.lib

del *.obj
pause