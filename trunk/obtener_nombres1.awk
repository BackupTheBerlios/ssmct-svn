#BEGIN{FS="\v"; RS="\f";
#}
BEGIN{
    while(getline var < ARGV[1] >0) {
      if(length(var)==0) {
        no_resp=1; # si es tipo Test, no tiene las respuestas aún
        continue
      }
      n=index(var,"\v")
      if(n) {
        FS="\v"; RS="\f";
      }
      else {
        FS="\n"; RS="";
	test=1;
      }
      break;
    }
}
{
  if(test && !no_resp) {
    no_resp=1
  }
  else {
    split($1,A,":");
    print A[3];
  }
}
