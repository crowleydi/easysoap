REM CommandInterpreter: $(COMSPEC)

REM Set EXPATHOME to the top directiory of the Expat sources
REM This isnt working in the IDE for the moment
REM SET EXPATHOME=..\..\..\..\Expat-1.95.2

if not exist ..\..\..\win32\nul mkdir ..\..\..\win32
if not exist ..\..\..\win32\release\nul mkdir ..\..\..\win32\release
if not exist ..\..\..\win32\release\obj\nul mkdir ..\..\..\win32\release\obj

REM if exist %EXPATHOME%\Libs\expatrel.dll copy %EXPATHOME%\Libs\expatrel.dll ..\..\..\win32\release
REM if exist %EXPATHOME%\Libs\expatrel.tds copy %EXPATHOME%\Libs\expatrel.tds ..\..\..\win32\release

if exist ..\..\..\..\Expat-1.95.2\Libs\expatrel.dll copy ..\..\..\..\Expat-1.95.2\Libs\expatrel.dll ..\..\..\win32\release
if exist ..\..\..\..\Expat-1.95.2\Libs\expatrel.tds copy ..\..\..\..\Expat-1.95.2\Libs\expatrel.tds ..\..\..\win32\release
