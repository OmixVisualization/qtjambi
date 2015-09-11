Set oShell = CreateObject ("Wscript.Shell")
Dim baseDir
Dim script
Dim cmd
baseDir = Left(WScript.ScriptFullName,InStrRev(WScript.ScriptFullName,"\"))
script = "examples.bat"
cmd = baseDir & script
oShell.Run cmd, 0, false
