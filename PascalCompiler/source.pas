program exProcedure(nice, nice);
var
  i, j, k: integer;
  a: array[1..5] of integer;
function min(x, y: integer): integer; 
begin
	if x < y then
	  min := x
	else
	  min := y
end;
begin
	a[1] := 3;
	a[2] := 5;
	a[3] := 4;
	a[4] := 1;
	a[5] := 2;
	k := min(a[1], a[2]);
	
	i := 1;
	while i <= 5 do
	begin
		j := i + 1;
		while j <= 5 do
		begin
			if a[j] < a[j - 1] then
			begin
				k := a[j];
				a[j] := a[j - 1];
				a[j - 1] := k
			end
			else
				k := 0
		end
	end
end