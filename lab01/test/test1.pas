PROGRAM
BEGIN
END
IF
ELSE
THEN
WHILE
REPEAT
UNTIL
FOR
DO
TO
DOWNTO
VAR
TYPE
<=
<>
>=
:=
OR DIV MOD AND NOT
ARRAY OF
RECORD
char
integer
real
thisisatest
12345isatest
123.45
+-*/< > =^
&%$#@!
{ this is a comment }
more stuff

.()  	{}[] ,;
array
if
else
while
print
int
float
char
struct
return
123
123.456
stuff
===&&||  	+-/*%==
{ this is a comment }
::@#$			{ should get JUNK for each char }
iff			{ should be IDENTIFIER (not 'if' followed by 'f') }
123WRITE		{ should be INTEGER followed by WRITE }
123.			{ should be INTEGER followed by . }
123.xyz			{ should be INTEGER . IDENTIFIER }
_this_Is_An_IdentifIER_
_
this_Is_An_IdentifIER_


