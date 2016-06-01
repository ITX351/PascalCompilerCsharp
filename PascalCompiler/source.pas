program arugal(arugalplot);
var n,b: integer;
    dp: integer;
     p: integer;
  ans,z,r,v,a,w,i,j,k,t,m:integer;
function min(a,b:integer):integer;
begin if a>b then min:=b else min:=a end;
procedure out(x,y:integer);
begin
if x<>1 then out(x-1,p) else p := 2;
writeln(p)
end;
begin
assign(input);reset(input);
assign(output);rewrite(output);
 readln(t,v,a,w);
 i:=1;
while i<t do
 readln(n,b);
fillchar(dp,sizeof(dp),63);
r:=dp;
dp:=0;
ans:=maxinteger;
writeln(ans);
out(t,j);
close(output)
end