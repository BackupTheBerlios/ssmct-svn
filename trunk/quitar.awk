BEGIN{RS=""; FS="\n"}
{
   if(NR==1) 
      desde=1
   else {
      print ""
      print $1
      desde=2
   }
   for(i=desde;i<=NF;i++) {
     n=index($i,"\"")
     b=substr($i,n+1)
     a=substr($i,1,n)
     t+=gsub("\"",",",b)
     $i=a""b 
     printf("%s\n",$i)
   }
}
END {
  print t, "sustituciones"
}
