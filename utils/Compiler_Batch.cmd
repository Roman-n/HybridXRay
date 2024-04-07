start /wait ..\Bin\compilers_sky_x64\xrCompiler.exe -geometry -f map_test -log_name map_test_geometry -silent
start /wait ..\Bin\compilers_sky_x64\xrCompiler.exe -details -f map_test -log_name map_test_detail -silent
start /wait ..\Bin\compilers_sky_x64\xrCompiler.exe -aispawn -f map_test -log_name map_test_aimap
