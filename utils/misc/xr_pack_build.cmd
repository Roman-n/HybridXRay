@echo off

if [%1]==[] (
  echo Please, specify configuration
  EXIT /B
)

if [%2]==[] (
  echo Please, specify platform
  EXIT /B
)

if [%3]==[] (
  echo Please, specify build name
  EXIT /B
)

set CONFIGURATION=%~1
set PLATFORM=%~2
set BUILD_NAME=%~3

if %PLATFORM%==x64 (
    set EDITION_NAME=%BUILD_NAME%
) else (
	echo Expected x64 only
	EXIT /B
)

rem Replace spaces with dots to avoid possible problems (e.g. with GitHub nighly builds uploading)
set EDITION_NAME=%EDITION_NAME: =.%

@echo on

rem Make archives
if %CONFIGURATION%==Release (
7z a "HybridXRay.%EDITION_NAME%.7z" Bin\%PLATFORM%\%CONFIGURATION%\*.dll Bin\%PLATFORM%\%CONFIGURATION%\*.exe -i!utils\image\Icon_HybridXRay.png -i!utils\oalinst.exe -i!utils\tool_compile_xrAI.cmd -i!utils\tool_compile_xrAI_draft.cmd -i!utils\tool_compile_xrDO.cmd -i!utils\tool_compile_xrLC.cmd -i!utils\tool_create_spawn.cmd -i!utils\tool_verify_ai_map.cmd -i!utils\Compiler_AI_map.cmd -i!utils\Compiler_Batch.cmd -i!utils\Compiler_Details.cmd -i!utils\Compiler_Geometry.cmd -i!utils\Compiler_Make_spawn.cmd -i!__start_game.cmd -i!__start_game_ShoC.cmd -i!_ActorEditor.cmd -i!_LevelEditor.cmd -i!_ParticleEditor.cmd -i!_ShaderEditor.cmd -i!_ActorEditor__SoC.cmd -i!_LevelEditor__SoC.cmd -i!_ParticleEditor__SoC.cmd -i!_ShaderEditor__SoC.cmd -i!Compiler_Menu.cmd -i!Compiler_Readme.txt -i!fs.ltx -i!fs_cs.ltx -i!fs_soc.ltx -i!fsfactory.ltx -i!fsgame.ltx -i!fsgame_cs.ltx -i!fsgame_soc.ltx -i!License.txt -r0 "Bin\compilers_sky_x64\*" -i!gamedata -i!gamedata_soc -i!rawdata -i!rawdata_soc
)
if %CONFIGURATION%==Debug (
7z a "HybridXRay.%EDITION_NAME%.7z" "Bin\%PLATFORM%\%CONFIGURATION%\*" -i!Debug_ActorEditor.cmd -i!Debug_LevelEditor.cmd -i!Debug_ParticleEditor.cmd -i!Debug_ShaderEditor.cmd -i!License.txt -xr!.*
)
