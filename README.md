# FakeAdminStarter
start process with fake administrator rights
normally, some programs can not run without administrator rights, even their functions do not need them.
this tool can run any program with limited user account rights, system will tell the program it is already elevated but actually not.
usage:
FakeAdminStarter.exe program

if no parameter is passed, by default, it will start a command prompt windows with fake administrator rights

some programs still can not run properly, because they detects file or registry or other rights, or they actually need administrator rights.the best way to run these programs is to use sandboxie.