#!/usr/bin/awk
BEGIN{RS="";FS="\n";j=0;p=0;max=0}
{
 if($1~/Respuestas/) {
   j=NF
   for(i=2;i<=NF;i++){
     A[i]=$i
   }
 }
 else {
   p++
   B[p]=$0
   #n1=split($1,ar1,":")
   C[p]=NF
 }
 if(NF>max)
   max=NF
}
END {
   if(j==0) exit
   for(i=2;i<=j;i++){
     print A[i] > "back."ARGV[1]
   }
   for(i=1;i<=p;i++){
    # n=split(B[i],ar,":") 
     #if(n>=3 && C[i]>=3) {
       #if(ar[3]!~/Respuestas/) {
      if(C[i]==max) {
         print"" > "back."ARGV[1]
         print B[i] > "back."ARGV[1]
      }
       #}
     #}
   }
}

