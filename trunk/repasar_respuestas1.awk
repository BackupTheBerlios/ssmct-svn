BEGIN{FS=":"; primero=1;aciertos=0;cont=0;
  #printf("%-45s%-15s%-15s\n\n","Apellidos y Nombre","Aciertos","Total");
}
{
if($0!=""&&primero) {
  resp[$1]=substr($0,index($0,":")+1)
  #printf(".........%s.....\n",resp[$1])
  #resp[$1]=$2
  cont++;
  nom=1;
}
if($0==""&&!primero) {
  #printf("%-45s%-15s%-15s\n",nombre,aciertos,cont);
  #print nombre, aciertos,"/",cont
  aciertos=0
  nom=1
}
if($0==""&&primero) {
  primero=0;
}
if($0!=""&&!primero&&!nom) {
 n_c=split(resp[$1],B,":")
 salgo=0
 for(i=1;i<=n_c;i++) {
   if(B[i]==$2) {
      aciertos++
      salgo=1
      break
   }
 }
 if(!salgo) {
  #agrego en array para luego mostrar al profe
  n_c=split(resp_mal[$1],A,":")
  for(i=1;i<=n_c;i++) {
    if(A[i]==$2)
      break
  }
  if(i>n_c)
    resp_mal[$1]=resp_mal[$1]":"$2
 }
}
if($0!=""&&!primero&&nom) {
  nombre=$3
  nom=0
  aciertos=0
}
}
END{
#  print nombre, aciertos,"/",cont
#  printf("%-45s%-15s%-15s\n",nombre,aciertos,cont);
  
  arg=substr(ARGV[2],3)
  if(resp[arg]) {
    print resp[arg]
    #print substr(resp_mal[arg],2)
    n_c=split(resp_mal[arg],A,":")
    for(i=1;i<=n_c;i++) 
      if(A[i]!="")
        print A[i]
  }
}
