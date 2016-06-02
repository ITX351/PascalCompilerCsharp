program nine(nine, nine);
var i, j: integer;
function min(a,b:integer):integer;
begin if a>b then min:=b else min:=a end;
procedure out(x, y: integer);
var z: integer;
begin
	z := x * y;
	write(x); write(y); write(z)
end;
begin
	i := 1;
	i := j[5];
	while i <= 9 do
	begin
		j := 1;
		while j <= 9 do
			out(i, j)
	end
end